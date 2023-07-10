﻿/*
 * Copyright (c) 2016 The ZLMediaKit project authors. All Rights Reserved.
 *
 * This file is part of ZLMediaKit(https://github.com/xia-chu/ZLMediaKit).
 *
 * Use of this source code is governed by MIT license that can be found in the
 * LICENSE file in the root of the source tree. All contributing project authors
 * may be found in the AUTHORS file in the root of the source tree.
 */

#include <map>
#include <list>
#include <iostream>
#include <functional>
#include "Rtsp/RtpReceiver.h"

using namespace std;
using namespace mediakit;

void test_real() {
    //这个是一次真实的rtp seq记录
    list<uint16_t> input_list = {15125, 15126, 15127, 15128, 15129, 15130, 15131, 15132, 15133, 15134, 15135, 15136,
                                 15137, 15138, 15139, 15140, 15141, 15142, 15143, 15144, 15145, 15146, 15147, 15148,
                                 15149, 15150, 15151, 15152, 15153, 15154, 15155, 15156, 15157, 15158, 15159, 15160,
                                 15161, 15162, 15163, 15164, 15165, 15166, 15167, 15168, 15169, 15170, 15171, 15172,
                                 15173, 15174, 15175, 15289, 15290, 15291, 15292, 15293, 15294, 15295, 15296, 15297,
                                 15298, 15299, 15300, 15301, 15302, 15303, 15304, 15305, 15306, 15307, 15308, 15309,
                                 15310, 15311, 15312, 15313, 15314, 15315, 15316, 15317, 15318, 15319, 15320, 15321,
                                 15322, 15176, 15177, 15178, 15179, 15180, 15181, 15182, 15183, 15184, 15185, 15186,
                                 15190, 15191, 15192, 15193, 15194, 15195, 15196, 15197, 15198, 15199, 15200, 15201,
                                 15202, 15203, 15204, 15205, 15206, 15207, 15208, 15209, 15210, 15211, 15212, 15213,
                                 15214, 15215, 15216, 15217, 15218, 15219, 15220, 15221, 15222, 15223, 15224, 15225,
                                 15226, 15227, 15228, 15229, 15230, 15231, 15232, 15233, 15234, 15235, 15236, 15237,
                                 15238, 15239, 15323, 15324, 15325, 15326, 15327, 15328, 15329, 15330, 15331, 15332,
                                 15333, 15334, 15335, 15336, 15337, 15338, 15339, 15340, 15341, 15342, 15343, 15344,
                                 15345, 15346, 15347, 15348, 15349, 15350, 15351, 15352, 15353, 15354, 15355, 15356,
                                 15357, 15358, 15359, 15360, 15361, 15362, 15363, 15364, 15365, 15366, 15367, 15368,
                                 15369, 15370, 15371, 15372, 15373, 15374, 15375, 15376, 15377, 15378, 15379, 15380,
                                 15381, 15382, 15383, 15384, 15385, 15386, 15387, 15388, 15389, 15390, 15391, 15392,
                                 15393, 15394, 15395, 15396, 15397, 15398, 15399, 15400, 15401, 15402, 15403, 15404,
                                 15405, 15406, 15407, 15408, 15409, 15410, 15411, 15412, 15413, 15414, 15415, 15416,
                                 15417, 15418, 15419, 15420, 15421, 15422, 15423, 15424, 15425, 15426, 15427, 15428,
                                 15429, 15430, 15431, 15432, 15433, 15434, 15435, 15436, 15437, 15438, 15240, 15241,
                                 15242, 15243, 15244, 15245, 15246, 15247, 15248, 15249, 15251, 15254, 15255, 15256,
                                 15257, 15258, 15259, 15260, 15261, 15262, 15263, 15264, 15265, 15266, 15267, 15268,
                                 15269, 15270, 15271, 15272, 15273, 15274, 15275, 15276, 15277, 15278, 15279, 15280,
                                 15281, 15282, 15283, 15284, 15285, 15286, 15287, 15288, 15439, 15440, 15441, 15442,
                                 15443, 15444, 15445, 15446, 15447, 15448, 15449, 15450, 15451, 15452, 15453, 15454,
                                 15455, 15456, 15457, 15458, 15459, 15460, 15461, 15462, 15463, 15464, 15465, 15466,
                                 15467, 15468, 15469, 15470, 15471, 15472, 15473, 15474, 15475, 15476, 15477, 15478,
                                 15479, 15480, 15481, 15482, 15483, 15484, 15485, 15486, 15487, 15488, 15489, 15490,
                                 15491, 15492, 15493, 15494, 15495, 15496, 15497, 15498, 15499, 15500, 15501, 15502,
                                 15503, 15504, 15505, 15506, 15507, 15508, 15509, 15510, 15511, 15512, 15513, 15514,
                                 15515, 15516, 15517, 15518, 15519, 15520, 15521, 15522, 15523, 15524, 15525, 15526,
                                 15527, 15528, 15529, 15530, 15531, 15532, 15533, 15534, 15535, 15536, 15537, 15538,
                                 15539, 15540, 15541, 15542, 15543, 15544, 15545, 15546, 15547, 15548, 15549, 15550,
                                 15551, 15552, 15553, 15554, 15555, 15556, 15557, 15558, 15559, 15560, 15561, 15562,
                                 15563, 15564, 15565, 15566, 15567, 15568, 15569, 15570, 15571, 15572, 15573, 15574,
                                 15575, 15576, 15577, 15578, 15579, 15580, 15581, 15582, 15583, 15584, 15585, 15586,
                                 15587, 15588, 15589, 15590, 15591, 15592, 15593, 15594, 15595, 15596, 15597, 15598,
                                 15599, 15600, 15601, 15602, 15603, 15604, 15605, 15606, 15607, 15608, 15609, 15610,
                                 15611, 15612, 15613, 15614, 15615, 15616, 15617, 15618, 15619, 15620, 15621, 15622,
                                 15623, 15624, 15625, 15626, 15627, 15628, 15629, 15630, 15631, 15632, 15633, 15634,
                                 15635, 15636, 15637, 15638, 15639, 15640, 15641, 15642, 15643, 15644, 15645, 15646,
                                 15647, 15648, 15649, 15650, 15651, 15652, 15653, 15654, 15655, 15656, 15657, 15658,
                                 15659, 15660, 15661, 15662, 15663, 15664, 15665, 15666, 15667, 15668, 15669, 15670,
                                 15671, 15672, 15673, 15674, 15675, 15676, 15677, 15678, 15679, 15680, 15681, 15682,
                                 15683, 15684, 15685, 15686, 15687, 15688, 15689, 15690, 15691, 15692, 15693, 15694,
                                 15695, 15696, 15697, 15698, 15699, 15700, 15701, 15702, 15703, 15704, 15705, 15706,
                                 15707, 15708, 15709, 15710, 15711, 15712, 15713, 15714, 15715, 15716, 15717, 15718,
                                 15719, 15720, 15721, 15722, 15723, 15724, 15725, 15726, 15727, 15728, 15729, 15730,
                                 15731, 15732, 15733, 15734, 15735, 15736, 15737, 15738, 15739, 15740, 15741, 15742,
                                 15743, 15744, 15745, 15746, 15747, 15748, 15749, 15750, 15751, 15752, 15753, 15754,
                                 15755, 15756, 15757, 15758, 15759, 15760, 15761, 15762, 15763, 15764, 15765, 15766,
                                 15767, 15768, 15769, 15770, 15771, 15772, 15773, 15774, 15775, 15776, 15777, 15778,
                                 15779, 15780, 15781, 15782, 15783, 15784, 15785, 15786, 15787, 15788, 15789, 15790,
                                 15791, 15792, 15793, 15794, 15795, 15796, 15797, 15798, 15799, 15800, 15801, 15802,
                                 15803, 15804, 15805, 15806, 15807, 15808, 15809, 15810, 15811, 15812, 15813, 15814,
                                 15815, 15816, 15817, 15818, 15819, 15820, 15821, 15822, 15823, 15824, 15825, 15826,
                                 15827, 15828, 15829, 15830, 15831, 15832, 15833, 15834, 15835, 15836, 15837, 15838,
                                 15839, 15840, 15841, 15842, 15843, 15844, 15845, 15846, 15847, 15848, 15849, 15850,
                                 15851, 15852, 15853, 15854, 15855, 15856, 15857, 15858, 15859, 15860, 15861, 15862,
                                 15863, 15864, 15865, 15866, 15867, 15868, 15869, 15870, 15871, 15872, 15873, 15874,
                                 15875, 15876, 15877, 15878, 15879, 15880, 15881, 15882, 15883, 15884, 15885, 15886,
                                 15887, 15888, 15889, 15890, 15891, 15892, 15893, 15894, 15895, 15896, 15897, 15898,
                                 15899, 15900, 15901, 15902, 15903, 15904, 15905, 15906, 15907, 15908, 15909, 15910,
                                 15911, 15912, 15913, 15914, 15915, 15916, 15917, 15918, 15919, 15920, 15921, 15922,
                                 15923, 15924, 15925, 15926, 15927, 15928, 15929, 15930, 15931, 15932, 15933, 15934,
                                 15935, 15936, 15937, 15938, 15939, 15940, 15941, 15942, 15943, 15944, 15945, 15946,
                                 15947, 15948, 15949, 15950, 15951, 15952, 15953, 15954, 15955, 15956, 15957, 15958,
                                 15959, 15960, 15961, 15962, 15963, 15964, 15965, 15966, 15967, 15968, 15969, 15970,
                                 15971, 15972, 15973, 15974, 15975, 15976, 15977, 15978, 15979, 15980, 15981, 15982,
                                 15983, 15984, 15985, 15986, 15987, 15988, 15989, 15990, 15991, 15992, 15993, 15994,
                                 15995, 15996, 15997, 15998, 15999, 16000, 16001, 16002, 16003, 16004, 16005, 16006,
                                 16007, 16008, 16009, 16010, 16011, 16012, 16013, 16014, 16015, 16016, 16017, 16018,
                                 16019, 16020, 16021, 16022, 16023, 16024, 16025, 16026, 16027, 16028, 16029, 16030,
                                 16031, 16032, 16033, 16034, 16035, 16036, 16037, 16038, 16039, 16040, 16041, 16042,
                                 16043, 16044, 16045, 16046, 16047, 16048, 16049, 16050, 16051, 16052, 16053, 16054,
                                 16055, 16056, 16057, 16058, 16059, 16060, 16061, 16062, 16063, 16064, 16065, 16066,
                                 16067, 16068, 16069, 16070, 16071, 16072, 16073, 16074, 16075, 16076, 16077, 16078,
                                 16079, 16080, 16081, 16082, 16083, 16084};

    PacketSortor<uint16_t, uint16_t> sortor;
    list<uint16_t> sorted_list;
    sortor.setOnSort([&](uint16_t seq, uint16_t packet) {
        sorted_list.push_back(seq);
    });

    for (auto &seq : input_list) {
        sortor.sortPacket(seq, seq);
    }

    cout << "输入数据个数:" << input_list.size()
         << " 抖动缓冲区大小:" << sortor.getJitterSize();

    //清空缓存
    sortor.flush();

    cout << " 输出数据个数:" << sorted_list.size() << endl;

#if 1
    {
        cout << endl;
        cout << "排序前:" << endl;
        int i = 0;
        for (auto &item : input_list) {
            cout << item << " ";
            if (++i % 10 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }

    {
        cout << endl;
        cout << "排序后:" << endl;
        int i = 0;
        for (auto &item : sorted_list) {
            cout << item << " ";
            if (++i % 10 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }
#endif
}

void test_rand(){
    srand((unsigned) time(NULL));
    PacketSortor<uint16_t, uint16_t> sortor;
    list<uint16_t> input_list, sorted_list, drop_list, repeat_list;
    sortor.setOnSort([&](uint16_t seq, const uint16_t &packet) {
        sorted_list.push_back(seq);
    });

    for (int i = 0; i < 1000;) {
        //模拟乱序，count是连续倒序次数,最多连续乱序8次
        int count = 1 + rand() % 10;
        for (int j = i + count; j >= i; --j) {

#if 1
            //模拟回环
            uint16_t seq = 0xFFFF - 500 + j;
#else
            uint16_t seq = j;
#endif

#if 1
            int input = 0;
            if (seq != 0xFFFF && seq != 0) {
                //模拟回环时丢包
                if (seq % (1 + rand() % 100) == 0) {
                    //模拟重复，概率5%左右
                    sortor.sortPacket(seq, seq);
                    input_list.push_back(seq);
                    ++input;
                }
                if (seq % (1 + rand() % 100) != 0) {
                    //模拟丢包，概率5%左右
                    sortor.sortPacket(seq, seq);
                    input_list.push_back(seq);
                    ++input;
                }
            }
            switch (input) {
                //输入0次，丢包
                case 0 :
                    drop_list.push_back(seq);
                    break;
                    //输入1次，正常
                case 1 :
                    break;
                    //输入2+次，重复包
                default:
                    repeat_list.push_back(seq);
                    break;
            }
#else
            sortor.sortPacket(seq, seq);
            input_list.push_back(seq);
#endif
        }
        i += (count + 1);
    }
    cout << "输入数据个数:" << input_list.size()
         << " 抖动缓冲区大小:" << sortor.getJitterSize()
         << " 丢包个数:" << drop_list.size()
         << " 重复包个数:" << repeat_list.size();

    //清空缓存
    sortor.flush();

    cout << " 输出数据个数:" << sorted_list.size() << endl;

#if 1
    {
        cout << endl;
        cout << "丢包列表:" << endl;
        int i = 0;
        for (auto &item : drop_list) {
            cout << item << " ";
            if (++i % 10 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }

    {
        cout << endl;
        cout << "重复包列表:" << endl;
        int i = 0;
        for (auto &item : repeat_list) {
            cout << item << " ";
            if (++i % 10 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }

    {
        cout << endl;
        cout << "排序前:" << endl;
        int i = 0;
        for (auto &item : input_list) {
            cout << item << " ";
            if (++i % 10 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }

    {
        cout << endl;
        cout << "排序后:" << endl;
        int i = 0;
        for (auto &item : sorted_list) {
            cout << item << " ";
            if (++i % 10 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }
#endif
}

//该测试程序用于检验rtp排序算法的正确性
int main(int argc, char *argv[]) {
    //测试真实的rtp seq
    cout << "###### 真实的rtp seq #####" << endl;
    test_real();

    //模拟rtp乱序、回环、丢包、重复情况
    cout << "###### 模拟的rtp seq #####" << endl;
    test_rand();
    return 0;
}