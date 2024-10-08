#ifndef __DAC_DATA_H
#define __DAC_DATA_H
#include "stdlib.h"
#include "ti_msp_dl_config.h"

const uint16_t gOutputSignalSine10000[] = {1463, 1536, 1610, 1682, 1754, 1824, 1894, 1961, 2027, 2090, 2151, 2209,
                                           2264, 2316, 2365, 2410, 2451, 2488, 2522, 2551, 2576, 2596, 2612, 2624,
                                           2631, 2633, 2631, 2624, 2612, 2596, 2576, 2551, 2522, 2488, 2451, 2410,
                                           2365, 2316, 2264, 2209, 2151, 2090, 2027, 1961, 1894, 1824, 1754, 1682,
                                           1610, 1536, 1463, 1389, 1316, 1244, 1172, 1101, 1032, 965, 899, 836,
                                           775, 717, 662, 610, 561, 516, 475, 437, 404, 375, 350, 329,
                                           313, 302, 295, 293, 295, 302, 313, 329, 350, 375, 404, 437,
                                           475, 516, 561, 610, 662, 717, 775, 836, 899, 965, 1032, 1101,
                                           1172, 1244, 1316, 1389};
// 1MSPS 1kHz���Ҳ� 12bit
const uint16_t gOutputSignalSine1000[] = {1463, 1470, 1478, 1485, 1492, 1500, 1507, 1514, 1522, 1529, 1536, 1544,
                                          1551, 1558, 1566, 1573, 1580, 1588, 1595, 1602, 1610, 1617, 1624, 1631,
                                          1639, 1646, 1653, 1660, 1668, 1675, 1682, 1689, 1697, 1704, 1711, 1718,
                                          1725, 1732, 1740, 1747, 1754, 1761, 1768, 1775, 1782, 1789, 1796, 1803,
                                          1810, 1817, 1824, 1831, 1838, 1845, 1852, 1859, 1866, 1873, 1880, 1887,
                                          1894, 1900, 1907, 1914, 1921, 1928, 1934, 1941, 1948, 1954, 1961, 1968,
                                          1974, 1981, 1988, 1994, 2001, 2007, 2014, 2020, 2027, 2033, 2039, 2046,
                                          2052, 2059, 2065, 2071, 2077, 2084, 2090, 2096, 2102, 2108, 2115, 2121,
                                          2127, 2133, 2139, 2145, 2151, 2157, 2163, 2168, 2174, 2180, 2186, 2192,
                                          2197, 2203, 2209, 2214, 2220, 2226, 2231, 2237, 2242, 2248, 2253, 2259,
                                          2264, 2269, 2275, 2280, 2285, 2290, 2296, 2301, 2306, 2311, 2316, 2321,
                                          2326, 2331, 2336, 2341, 2346, 2350, 2355, 2360, 2365, 2369, 2374, 2378,
                                          2383, 2388, 2392, 2397, 2401, 2405, 2410, 2414, 2418, 2422, 2427, 2431,
                                          2435, 2439, 2443, 2447, 2451, 2455, 2459, 2463, 2466, 2470, 2474, 2478,
                                          2481, 2485, 2488, 2492, 2495, 2499, 2502, 2506, 2509, 2512, 2515, 2519,
                                          2522, 2525, 2528, 2531, 2534, 2537, 2540, 2543, 2545, 2548, 2551, 2554,
                                          2556, 2559, 2561, 2564, 2566, 2569, 2571, 2574, 2576, 2578, 2580, 2582,
                                          2585, 2587, 2589, 2591, 2593, 2595, 2596, 2598, 2600, 2602, 2603, 2605,
                                          2607, 2608, 2610, 2611, 2612, 2614, 2615, 2616, 2618, 2619, 2620, 2621,
                                          2622, 2623, 2624, 2625, 2626, 2626, 2627, 2628, 2629, 2629, 2630, 2630,
                                          2631, 2631, 2632, 2632, 2632, 2633, 2633, 2633, 2633, 2633, 2633, 2633,
                                          2633, 2633, 2633, 2633, 2632, 2632, 2632, 2631, 2631, 2630, 2630, 2629,
                                          2629, 2628, 2627, 2626, 2626, 2625, 2624, 2623, 2622, 2621, 2620, 2619,
                                          2618, 2616, 2615, 2614, 2612, 2611, 2610, 2608, 2607, 2605, 2603, 2602,
                                          2600, 2598, 2596, 2595, 2593, 2591, 2589, 2587, 2585, 2582, 2580, 2578,
                                          2576, 2574, 2571, 2569, 2566, 2564, 2561, 2559, 2556, 2554, 2551, 2548,
                                          2545, 2543, 2540, 2537, 2534, 2531, 2528, 2525, 2522, 2519, 2515, 2512,
                                          2509, 2506, 2502, 2499, 2495, 2492, 2488, 2485, 2481, 2478, 2474, 2470,
                                          2466, 2463, 2459, 2455, 2451, 2447, 2443, 2439, 2435, 2431, 2427, 2422,
                                          2418, 2414, 2410, 2405, 2401, 2397, 2392, 2388, 2383, 2378, 2374, 2369,
                                          2365, 2360, 2355, 2350, 2346, 2341, 2336, 2331, 2326, 2321, 2316, 2311,
                                          2306, 2301, 2296, 2290, 2285, 2280, 2275, 2269, 2264, 2259, 2253, 2248,
                                          2242, 2237, 2231, 2226, 2220, 2214, 2209, 2203, 2197, 2192, 2186, 2180,
                                          2174, 2168, 2163, 2157, 2151, 2145, 2139, 2133, 2127, 2121, 2115, 2108,
                                          2102, 2096, 2090, 2084, 2077, 2071, 2065, 2059, 2052, 2046, 2039, 2033,
                                          2027, 2020, 2014, 2007, 2001, 1994, 1988, 1981, 1974, 1968, 1961, 1954,
                                          1948, 1941, 1934, 1928, 1921, 1914, 1907, 1900, 1894, 1887, 1880, 1873,
                                          1866, 1859, 1852, 1845, 1838, 1831, 1824, 1817, 1810, 1803, 1796, 1789,
                                          1782, 1775, 1768, 1761, 1754, 1747, 1740, 1732, 1725, 1718, 1711, 1704,
                                          1697, 1689, 1682, 1675, 1668, 1660, 1653, 1646, 1639, 1631, 1624, 1617,
                                          1610, 1602, 1595, 1588, 1580, 1573, 1566, 1558, 1551, 1544, 1536, 1529,
                                          1522, 1514, 1507, 1500, 1492, 1485, 1478, 1470, 1463, 1456, 1448, 1441,
                                          1433, 1426, 1419, 1411, 1404, 1397, 1389, 1382, 1375, 1367, 1360, 1353,
                                          1345, 1338, 1331, 1323, 1316, 1309, 1302, 1294, 1287, 1280, 1273, 1265,
                                          1258, 1251, 1244, 1236, 1229, 1222, 1215, 1208, 1200, 1193, 1186, 1179,
                                          1172, 1165, 1158, 1151, 1143, 1136, 1129, 1122, 1115, 1108, 1101, 1094,
                                          1087, 1080, 1073, 1066, 1060, 1053, 1046, 1039, 1032, 1025, 1018, 1012,
                                          1005, 998, 991, 985, 978, 971, 965, 958, 951, 945, 938, 932,
                                          925, 918, 912, 906, 899, 893, 886, 880, 873, 867, 861, 855,
                                          848, 842, 836, 830, 823, 817, 811, 805, 799, 793, 787, 781,
                                          775, 769, 763, 757, 751, 746, 740, 734, 728, 723, 717, 711,
                                          706, 700, 694, 689, 683, 678, 673, 667, 662, 656, 651, 646,
                                          641, 635, 630, 625, 620, 615, 610, 605, 600, 595, 590, 585,
                                          580, 575, 571, 566, 561, 556, 552, 547, 543, 538, 534, 529,
                                          525, 520, 516, 512, 508, 503, 499, 495, 491, 487, 483, 479,
                                          475, 471, 467, 463, 459, 456, 452, 448, 444, 441, 437, 434,
                                          430, 427, 423, 420, 417, 414, 410, 407, 404, 401, 398, 395,
                                          392, 389, 386, 383, 380, 377, 375, 372, 369, 367, 364, 362,
                                          359, 357, 354, 352, 350, 348, 345, 343, 341, 339, 337, 335,
                                          333, 331, 329, 328, 326, 324, 322, 321, 319, 318, 316, 315,
                                          313, 312, 311, 309, 308, 307, 306, 305, 304, 303, 302, 301,
                                          300, 299, 298, 298, 297, 296, 296, 295, 295, 294, 294, 294,
                                          293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293, 293,
                                          293, 294, 294, 294, 295, 295, 296, 296, 297, 298, 298, 299,
                                          300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 311, 312,
                                          313, 315, 316, 318, 319, 321, 322, 324, 326, 328, 329, 331,
                                          333, 335, 337, 339, 341, 343, 345, 348, 350, 352, 354, 357,
                                          359, 362, 364, 367, 369, 372, 375, 377, 380, 383, 386, 389,
                                          392, 395, 398, 401, 404, 407, 410, 414, 417, 420, 423, 427,
                                          430, 434, 437, 441, 444, 448, 452, 456, 459, 463, 467, 471,
                                          475, 479, 483, 487, 491, 495, 499, 503, 508, 512, 516, 520,
                                          525, 529, 534, 538, 543, 547, 552, 556, 561, 566, 571, 575,
                                          580, 585, 590, 595, 600, 605, 610, 615, 620, 625, 630, 635,
                                          641, 646, 651, 656, 662, 667, 673, 678, 683, 689, 694, 700,
                                          706, 711, 717, 723, 728, 734, 740, 746, 751, 757, 763, 769,
                                          775, 781, 787, 793, 799, 805, 811, 817, 823, 830, 836, 842,
                                          848, 855, 861, 867, 873, 880, 886, 893, 899, 906, 912, 918,
                                          925, 932, 938, 945, 951, 958, 965, 971, 978, 985, 991, 998,
                                          1005, 1012, 1018, 1025, 1032, 1039, 1046, 1053, 1060, 1066, 1073, 1080,
                                          1087, 1094, 1101, 1108, 1115, 1122, 1129, 1136, 1143, 1151, 1158, 1165,
                                          1172, 1179, 1186, 1193, 1200, 1208, 1215, 1222, 1229, 1236, 1244, 1251,
                                          1258, 1265, 1273, 1280, 1287, 1294, 1302, 1309, 1316, 1323, 1331, 1338,
                                          1345, 1353, 1360, 1367, 1375, 1382, 1389, 1397, 1404, 1411, 1419, 1426,
                                          1433, 1441, 1448, 1456};

#endif
