/* level_data.h 
 * Author: Mips96
 * Level data header from Trogdor-Reburninated project by Mips96.
 * 
 * Stores data for all Trogdor level layouts in a huge array. There are only
 * 33 layouts, so after level 33 they begin to repeat.
 * 
 * 
 * Array contents explanation by Mips96 below:
 * 
 * Byte 1:      Background (1, 2, 3, 4) (default to 1)
 * Byte 2:      Treasure Hut Index (1, 2, 3, 4, 5, 6) (0 = no treasure hut)
 * Byte 3:      Cottage #1 Direction (1, 2, 3, 4) (0 = does not exist)
 * Bytes 4-5:   Cottage #1 X (dstrect.x = OBJ_TO_SCREEN_AT_FRACTION_X(gameWidth, cottage, (VAL+2466)/5000.0))
 * Bytes 6-7:   Cottage #1 Y (dstrect.y = OBJ_TO_SCREEN_AT_FRACTION_Y(gameHeight, cottage, (VAL+2183)/3600.0))
 * ...
 * Byte 28:     Cottage #6 Direction
 * Bytes 29-30: Cottage #6 X
 * Bytes 31-32: Cottage #6 Y
 */
// negative decimal to Sint16: https://www.mathsisfun.com/binary-decimal-hexadecimal-converter.html


#include "enums.h"

#pragma once 

namespace trog { 

constexpr auto DIR_UP = 1;
constexpr auto DIR_DOWN = 2;
constexpr auto DIR_LEFT = 3;
constexpr auto DIR_RIGHT = 4;

const short levels[33][20] = {
	/* 1 */ { 1, 0,
	DIR_LEFT, 1585, -666,
	DIR_RIGHT, -1813, 291,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0 },
	/* 2 */ { 1, 0,
	DIR_DOWN, 1775, -1096,
	DIR_DOWN, -1796, -1022,
	DIR_UP, -133, 1277,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0 },
	/* 3 */ { 1, 0,
	DIR_DOWN, 1015, -866,
	DIR_DOWN, 1664, -382,
	DIR_DOWN, -1044, -955,
	DIR_DOWN, -1854, -585,
	0, 0, 0,
	0, 0, 0 },
	/* 4 */ { 2, 0,
	DIR_RIGHT, -2125, -1046,
	DIR_UP, 1904, 538,
	DIR_DOWN, -2124, -435,
	DIR_LEFT, 1806, 1115,
	0, 0, 0,
	0, 0, 0 },
	/* 5 */ { 2, 0,
	DIR_RIGHT, -1665, -666,
	DIR_UP, 924, -82,
	DIR_DOWN, -484, -1095,
	DIR_LEFT, 1526, 975,
	DIR_UP, -1220, 830,
	0, 0, 0 },
	/* 6 */ { 3, 0,
	DIR_RIGHT, -2125, -446,
	DIR_UP, 384, 1098,
	DIR_DOWN, -204, -995,
	DIR_LEFT, 1886, -545,
	DIR_UP, -740, 1090,
	0, 0, 0 },
	/* 7 */ { 3, 3,
	DIR_RIGHT, -1236, -942,
	DIR_RIGHT, -1205, -346,
	DIR_RIGHT, -2034, -45,
	DIR_RIGHT, -1200, 270,
	DIR_RIGHT, -1204, 885,
	0, 0, 0 },
	/* 8 */ { 3, 0,
	DIR_RIGHT, -2125, -446,
	DIR_DOWN, 904, -862,
	DIR_DOWN, -204, -995,
	DIR_LEFT, 1886, -545,
	DIR_DOWN, -1180, -830,
	0, 0, 0 },
	/* 9 */ { 3, 0,
	DIR_RIGHT, -2125, -446,
	DIR_LEFT, 1886, -545,
	DIR_DOWN, 1984, 258,
	DIR_DOWN, -1044, -1015,
	DIR_DOWN, -2080, 350,
	DIR_DOWN, 896, -1035 },
	/* 10 */ { 4, 0,
	DIR_RIGHT, -2385, -486,
	DIR_DOWN, 1784, 258,
	DIR_DOWN, -1244, -1215,
	DIR_LEFT, 2086, -545,
	DIR_DOWN, -1900, 350,
	DIR_DOWN, 1096, -1235 },
	/* 11 */ { 4, 6,
	DIR_RIGHT, -2365, -486,
	DIR_LEFT, 1204, 918,
	DIR_DOWN, -1084, -1055,
	DIR_LEFT, 1526, 135,
	DIR_DOWN, -1520, -450,
	DIR_UP, 2076, 865 },
	/* 12 */ { 4, 0,
	DIR_DOWN, -1084, -1055,
	DIR_RIGHT, -2025, -346,
	DIR_LEFT, 26, -385,
	DIR_DOWN, 1760, -1070,
	DIR_UP, 1756, 1205,
	0, 0, 0 },
	/* 13 */ { 1, 0,
	DIR_UP, -2025, 1258,
	DIR_UP, 1984, 1258,
	DIR_DOWN, -24, -1275,
	DIR_UP, 6, 1258,
	DIR_DOWN, -2040, -1090,
	DIR_DOWN, 1976, -1075 },
	/* 14 */ { 1, 0,
	DIR_RIGHT, -2125, 453,
	DIR_DOWN, 224, -502,
	DIR_DOWN, -1244, -1215,
	DIR_LEFT, 1946, 1094,
	DIR_DOWN, -520, -510,
	DIR_DOWN, 1096, -1235 },
	/* 15 */ { 1, 0,
	DIR_RIGHT, -2125, 833,
	DIR_LEFT, 1204, 918,
	DIR_DOWN, -2124, -975,
	DIR_LEFT, 1526, 135,
	DIR_LEFT, 1940, -570,
	DIR_UP, 2076, 865 },
	/* 16 */ { 2, 3,
	DIR_RIGHT, -665, 705,
	DIR_UP, 1204, 1178,
	DIR_DOWN, -2020, -1010,
	DIR_UP, 1916, 1185,
	DIR_RIGHT, -633, -989,
	DIR_RIGHT, -665, -126 },
	/* 17 */ { 2, 0,
	DIR_RIGHT, -2205, -926,
	DIR_DOWN, 2076, -195,
	DIR_LEFT, 1486, 734,
	DIR_DOWN, 1360, -1070,
	DIR_UP, 796, 1225,
	0, 0, 0 },
	/* 18 */ { 3, 0,
	DIR_UP, -845, -321,
	DIR_RIGHT, -596, 699,
	DIR_DOWN, -24, -1275,
	DIR_UP, 1106, 319,
	DIR_DOWN, -2040, -1090,
	DIR_DOWN, 1976, -1075 },
	/* 19 */ { 3, 0,
	DIR_RIGHT, -2125, -1106,
	DIR_DOWN, 1024, 238,
	DIR_LEFT, 1926, -1145,
	DIR_DOWN, -1020, 270,
	0, 0, 0,
	0, 0, 0 },
	/* 20 */ { 4, 0,
	DIR_RIGHT, -1345, 853,
	DIR_LEFT, 1204, 918,
	DIR_UP, -2104, 824,
	DIR_LEFT, 1526, 135,
	DIR_RIGHT, -2140, 70,
	DIR_UP, 2076, 865 },
	/* 21 */ { 4, 0,
	DIR_RIGHT, -905, -47,
	DIR_UP, 1204, 1178,
	DIR_DOWN, 1256, -155,
	DIR_LEFT, 166, -765,
	DIR_DOWN, -2020, -1010,
	DIR_UP, 1916, 1185 },
	/* 22 */ { 4, 0,
	DIR_RIGHT, -1205, -107,
	DIR_UP, 1904, 698,
	DIR_DOWN, 1896, -815,
	DIR_LEFT, -1234, 575,
	DIR_DOWN, -1180, -770,
	0, 0, 0 },
	/* 23 */ { 1, 0,
	DIR_RIGHT, -2245, 1154,
	DIR_DOWN, -516, -1042,
	DIR_DOWN, 256, -1055,
	DIR_LEFT, 1846, 1135,
	DIR_DOWN, -1280, -1050,
	DIR_DOWN, 996, -1015 },
	/* 24 */ { 1, 0,
	DIR_RIGHT, -2125, 1074,
	DIR_LEFT, 1344, -882,
	DIR_DOWN, -1924, -815,
	DIR_UP, 1856, 1125,
	0, 0, 0,
	0, 0, 0 },
	/* 25 */ { 1, 0,
	DIR_RIGHT, -2065, -506,
	DIR_DOWN, -1084, -1055,
	DIR_LEFT, -314, -505,
	DIR_DOWN, 1760, -1070,
	DIR_UP, -1084, 885,
	0, 0, 0 },
	/* 26 */ { 2, 0,
	DIR_UP, -2025, -642,
	DIR_UP, 1984, -642,
	DIR_DOWN, -1984, -75,
	DIR_UP, 6, -642,
	DIR_DOWN, 1960, 10,
	0, 0, 0 },
	/* 27 */ { 2, 0, // and also 59 and also 91
	DIR_RIGHT, 1076, 1186,
	DIR_DOWN, 224, -502,
	DIR_DOWN, -1304, -555,
	DIR_LEFT, -1439, 1186,
	DIR_DOWN, -520, -510,
	DIR_DOWN, 1116, -475 },
	/* 28 */ { 3, 0,
	DIR_RIGHT, -1236, -942,
	DIR_RIGHT, -2225, -346,
	DIR_RIGHT, 1386, -45,
	DIR_RIGHT, -2240, 390,
	DIR_RIGHT, -1244, 1005,
	0, 0, 0 },
	/* 29 */ { 3, 0,
	DIR_RIGHT, 855, 274,
	DIR_DOWN, -1976, 238,
	DIR_DOWN, -204, -995,
	DIR_LEFT, 1806, 835,
	DIR_DOWN, -1100, -350,
	0, 0, 0 },
	/* 30 */ { 3, 0,
	DIR_LEFT, 1826, 175,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0 },
	/* 31 */ { 3, 0,
	DIR_RIGHT, -2225, -946,
	DIR_DOWN, 784, 338,
	DIR_DOWN, -1104, 345,
	DIR_LEFT, 1966, -905,
	DIR_DOWN, -1900, 350,
	DIR_DOWN, 1536, 325 },
	/* 32 */ { 4, 0,
	DIR_RIGHT, -2085, 934,
	DIR_LEFT, 1144, -242,
	DIR_DOWN, -804, 365,
	DIR_LEFT, 1466, -1025,
	DIR_DOWN, -1240, 970,
	DIR_UP, 2016, -295 },
	/* 33 */ { 4, 0,
	DIR_LEFT, 305, -886,
	DIR_RIGHT, -533, -889,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0,
	0, 0, 0 }
};

}