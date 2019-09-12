#include "WebSocketClient.h"



int mediakit::WebSocketClient::send(const string& buf)
{
    if (_sock)
    {
        if (_WSClientStatus == WORKING)
        {
            _session->send(buf);
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

void mediakit::WebSocketClient::clear()
{
    _method.clear();
    _path.clear();
    _parser.Clear();
    _recvedBodySize = 0;
    _totalBodySize = 0;
    _aliveTicker.resetTime();
    _chunkedSplitter.reset();
    HttpRequestSplitter::reset();
}

const std::string & mediakit::WebSocketClient::responseStatus() const
{
    return _parser.Url();
}

const mediakit::WebSocketClient::HttpHeader & mediakit::WebSocketClient::responseHeader() const
{
    return _parser.getValues();
}

const mediakit::Parser& mediakit::WebSocketClient::response() const
{
    return _parser;
}

const std::string & mediakit::WebSocketClient::getUrl() const
{
    return _url;
}

int64_t mediakit::WebSocketClient::onResponseHeader(const string &status, const HttpHeader &headers)
{
    DebugL << status;
    //��Content-Length�ֶ�ʱĬ�Ϻ���ȫ��content
    return -1;
}

void mediakit::WebSocketClient::onResponseBody(const char *buf, int64_t size, int64_t recvedSize, int64_t totalSize)
{
    DebugL << size << " " << recvedSize << " " << totalSize;
}

void mediakit::WebSocketClient::onResponseCompleted()
{
    DebugL;
}

int64_t mediakit::WebSocketClient::onRecvHeader(const char *data, uint64_t len)
{
    _parser.Parse(data);
    if (_parser.Url() == "101")
    {
        switch (_WSClientStatus)
        {
        case HANDSHAKING:
        {
            StrCaseMap& valueMap = _parser.getValues();
            auto key = valueMap.find("Sec-WebSocket-Accept");
            if (key != valueMap.end() && key->second.length() > 0) {
                onConnect(SockException());
            }
            break;
        }
        }
        return -1;
    }
    else
    {
        shutdown(SockException(Err_shutdown, _parser.Url().c_str()));
        return 0;
    }
    return -1;
}

void mediakit::WebSocketClient::onRecvContent(const char *data, uint64_t len)
{
    if (_chunkedSplitter) {
        _chunkedSplitter->input(data, len);
        return;
    }
    auto recvedBodySize = _recvedBodySize + len;
    if (_totalBodySize < 0) {
        //���޳��ȵ�content,���֧��INT64_MAX���ֽ�
        onResponseBody(data, len, recvedBodySize, INT64_MAX);
        _recvedBodySize = recvedBodySize;
        return;
    }

    //�̶����ȵ�content
    if (recvedBodySize < _totalBodySize) {
        //content��δ�������
        onResponseBody(data, len, recvedBodySize, _totalBodySize);
        _recvedBodySize = recvedBodySize;
        return;
    }

    //content�������
    onResponseBody(data, _totalBodySize - _recvedBodySize, _totalBodySize, _totalBodySize);
    bool biggerThanExpected = recvedBodySize > _totalBodySize;
    onResponseCompleted_l();
    if (biggerThanExpected) {
        //������content���ݱ���ʵ��С����ô����ֻ��ȡǰ�沿�ֵĲ��Ͽ�����
        shutdown(SockException(Err_shutdown, "http response content size bigger than expected"));
    }
}

void mediakit::WebSocketClient::onConnect(const SockException &ex)
{
    _aliveTicker.resetTime();
    if (ex) {
        onDisconnect(ex);
        return;
    }

    //�ȼ���http�ͻ���ֻ�����һ������ݣ�ֻ����httpͷ����ʡ�ڴ棩
    _sock->setReadBuffer(std::make_shared<BufferRaw>(1 * 1024));

    _totalBodySize = 0;
    _recvedBodySize = 0;
    HttpRequestSplitter::reset();
    _chunkedSplitter.reset();

    if (_WSClientStatus == WSCONNECT)
    {
        //Websocket����
        string random = get_random(16);
        auto Sec_WebSocket_Key = encodeBase64(SHA1::encode_bin(random));
        _key = Sec_WebSocket_Key;
        string p = generate_websocket_client_handshake(_ip.c_str(), _port, _url.c_str(), _key.c_str());
        TcpClient::send(p);
        _WSClientStatus = HANDSHAKING;
    }
    else if (_WSClientStatus == HANDSHAKING)
    {
        _WSClientStatus = WORKING;
    }

    onFlush();
}

void mediakit::WebSocketClient::onRecv(const Buffer::Ptr &pBuf)
{
    _aliveTicker.resetTime();
    if (_WSClientStatus == HANDSHAKING || _WSClientStatus == WSCONNECT)
        HttpRequestSplitter::input(pBuf->data(), pBuf->size());
    else if (_WSClientStatus == WORKING)
    {
        WebSocketSplitter::decode((uint8_t *)pBuf->data(), pBuf->size());
    }
}

void mediakit::WebSocketClient::onErr(const SockException &ex)
{
    _session->onError(ex);
    onDisconnect(ex);
}

void mediakit::WebSocketClient::onManager()
{
    if (_WSClientStatus != WORKING)
    {
        if (_fTimeOutSec > 0 && _aliveTicker.elapsedTime() > _fTimeOutSec * 1000) {
            //��ʱ
            shutdown(SockException(Err_timeout, "ws server respone timeout"));
        }
    }
    else
        _session->onManager();
}

std::string mediakit::WebSocketClient::generate_websocket_client_handshake(const char* ip, uint16_t port, const char * path, const char * key)
{
    /**
* @brief  ҵ�����ݱ���Ƭ�ĵ�Ƭ����С�� ���� 65535 - 14 - 1
*/
#define DATA_FRAME_MAX_LEN  65520
#define HANDSHAKE_SIZE      1024
    char buf[HANDSHAKE_SIZE] = { 0 };

    snprintf(buf, HANDSHAKE_SIZE,
        "GET %s HTTP/1.1\r\n"
        "Host: %s:%d\r\n"
        "Upgrade: websocket\r\n"
        "Connection: Upgrade\r\n"
        "Sec-WebSocket-Key: %s\r\n"
        "Sec-WebSocket-Version: 13\r\n"
        "\r\n",
        path, ip, port, key);
    string temBuf(buf);
    return temBuf;
}

std::string mediakit::WebSocketClient::get_random(size_t n)
{
    random_device rd;
    _StrPrinter printer;
    for (int i = 0; i < n; i++)
    {
        unsigned int rnd = rd();
        printer << rnd % 9;
    }

    return string(printer);
}

void mediakit::WebSocketClient::onWebSocketDecodeHeader(const WebSocketHeader &packet)
{
    //�°���ԭ���İ�����������յ�
    _remian_data.clear();

    if (_firstPacket) {
        //���Ǹ�WebSocket�Ự��������ͨ��Http�Ự
        _firstPacket = false;
        //�˴���ȡ���ݲ�����websocketЭ����
    }
}

void mediakit::WebSocketClient::onWebSocketDecodePlayload(const WebSocketHeader &packet, const uint8_t *ptr, uint64_t len, uint64_t recved)
{
    _remian_data.append((char *)ptr, len);
}

void mediakit::WebSocketClient::onWebSocketDecodeComplete(const WebSocketHeader &header_in)
{
    WebSocketHeader& header = const_cast<WebSocketHeader&>(header_in);
    auto  flag = header._mask_flag;
    header._mask_flag = false;

    switch (header._opcode) {
    case WebSocketHeader::CLOSE: {
        shutdown(SockException(Err_timeout, "session timeouted"));
    }
                                 break;
    case WebSocketHeader::PING: {
        const_cast<WebSocketHeader&>(header)._opcode = WebSocketHeader::PONG;
        WebSocketSplitter::encode(header, (uint8_t *)_remian_data.data(), _remian_data.size());
    }
                                break;
    case WebSocketHeader::CONTINUATION: {

    }
                                        break;
    case WebSocketHeader::TEXT:
    case WebSocketHeader::BINARY: {
        BufferString::Ptr buffer = std::make_shared<BufferString>(_remian_data);
        _session->onRecv(buffer);
    }
                                  break;
    default:
        break;
    }
    _remian_data.clear();
    header._mask_flag = flag;
}

void mediakit::WebSocketClient::onWebSocketEncodeData(const uint8_t *ptr, uint64_t len)
{
    TcpClient::send(string((char*)ptr, len));
}

void mediakit::WebSocketClient::onResponseCompleted_l()
{
    _totalBodySize = 0;
    _recvedBodySize = 0;
    onResponseCompleted();
}
