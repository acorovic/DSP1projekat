#include "tistdtypes.h"
#include "sine_table.h"

Int16 sine_table[SINE_TABLE_SIZE+1] =
{
		0,
		50, 100, 150, 201, 251, 301, 351, 402,
		452, 502, 552, 603, 653, 703, 753, 804,
		854, 904, 954, 1005, 1055, 1105, 1155, 1206,
		1256, 1306, 1356, 1406, 1457, 1507, 1557, 1607,
		1658, 1708, 1758, 1808, 1858, 1908, 1959, 2009,
		2059, 2109, 2159, 2209, 2260, 2310, 2360, 2410,
		2460, 2510, 2560, 2610, 2661, 2711, 2761, 2811,
		2861, 2911, 2961, 3011, 3061, 3111, 3161, 3211,
		3261, 3311, 3361, 3411, 3461, 3511, 3561, 3611,
		3661, 3711, 3761, 3811, 3861, 3911, 3961, 4011,
		4060, 4110, 4160, 4210, 4260, 4310, 4359, 4409,
		4459, 4509, 4559, 4608, 4658, 4708, 4758, 4807,
		4857, 4907, 4957, 5006, 5056, 5106, 5155, 5205,
		5254, 5304, 5354, 5403, 5453, 5502, 5552, 5601,
		5651, 5700, 5750, 5799, 5849, 5898, 5948, 5997,
		6047, 6096, 6145, 6195, 6244, 6293, 6343, 6392,
		6441, 6491, 6540, 6589, 6638, 6688, 6737, 6786,
		6835, 6884, 6933, 6982, 7032, 7081, 7130, 7179,
		7228, 7277, 7326, 7375, 7424, 7473, 7522, 7571,
		7619, 7668, 7717, 7766, 7815, 7864, 7912, 7961,
		8010, 8059, 8107, 8156, 8205, 8253, 8302, 8351,
		8399, 8448, 8496, 8545, 8593, 8642, 8690, 8739,
		8787, 8836, 8884, 8932, 8981, 9029, 9077, 9126,
		9174, 9222, 9270, 9319, 9367, 9415, 9463, 9511,
		9559, 9607, 9655, 9703, 9751, 9799, 9847, 9895,
		9943, 9991, 10039, 10087, 10135, 10182, 10230, 10278,
		10326, 10373, 10421, 10469, 10516, 10564, 10611, 10659,
		10706, 10754, 10801, 10849, 10896, 10944, 10991, 11038,
		11086, 11133, 11180, 11227, 11275, 11322, 11369, 11416,
		11463, 11510, 11557, 11604, 11651, 11698, 11745, 11792,
		11839, 11886, 11933, 11980, 12026, 12073, 12120, 12166,
		12213, 12260, 12306, 12353, 12399, 12446, 12492, 12539,
		12585, 12632, 12678, 12724, 12771, 12817, 12863, 12909,
		12956, 13002, 13048, 13094, 13140, 13186, 13232, 13278,
		13324, 13370, 13416, 13462, 13507, 13553, 13599, 13645,
		13690, 13736, 13782, 13827, 13873, 13918, 13964, 14009,
		14055, 14100, 14145, 14191, 14236, 14281, 14326, 14372,
		14417, 14462, 14507, 14552, 14597, 14642, 14687, 14732,
		14777, 14822, 14866, 14911, 14956, 15001, 15045, 15090,
		15135, 15179, 15224, 15268, 15313, 15357, 15401, 15446,
		15490, 15534, 15579, 15623, 15667, 15711, 15755, 15799,
		15843, 15887, 15931, 15975, 16019, 16063, 16107, 16150,
		16194, 16238, 16281, 16325, 16368, 16412, 16455, 16499,
		16542, 16586, 16629, 16672, 16716, 16759, 16802, 16845,
		16888, 16931, 16974, 17017, 17060, 17103, 17146, 17189,
		17232, 17274, 17317, 17360, 17402, 17445, 17487, 17530,
		17572, 17615, 17657, 17699, 17742, 17784, 17826, 17868,
		17910, 17952, 17994, 18036, 18078, 18120, 18162, 18204,
		18246, 18287, 18329, 18371, 18412, 18454, 18495, 18537,
		18578, 18620, 18661, 18702, 18744, 18785, 18826, 18867,
		18908, 18949, 18990, 19031, 19072, 19113, 19154, 19194,
		19235, 19276, 19316, 19357, 19397, 19438, 19478, 19519,
		19559, 19599, 19640, 19680, 19720, 19760, 19800, 19840,
		19880, 19920, 19960, 20000, 20040, 20079, 20119, 20159,
		20198, 20238, 20277, 20317, 20356, 20396, 20435, 20474,
		20513, 20553, 20592, 20631, 20670, 20709, 20748, 20787,
		20825, 20864, 20903, 20942, 20980, 21019, 21057, 21096,
		21134, 21173, 21211, 21249, 21288, 21326, 21364, 21402,
		21440, 21478, 21516, 21554, 21592, 21629, 21667, 21705,
		21743, 21780, 21818, 21855, 21893, 21930, 21967, 22004,
		22042, 22079, 22116, 22153, 22190, 22227, 22264, 22301,
		22338, 22374, 22411, 22448, 22484, 22521, 22557, 22594,
		22630, 22666, 22703, 22739, 22775, 22811, 22847, 22883,
		22919, 22955, 22991, 23027, 23062, 23098, 23134, 23169,
		23205, 23240, 23276, 23311, 23346, 23382, 23417, 23452,
		23487, 23522, 23557, 23592, 23627, 23661, 23696, 23731,
		23766, 23800, 23835, 23869, 23903, 23938, 23972, 24006,
		24041, 24075, 24109, 24143, 24177, 24211, 24244, 24278,
		24312, 24346, 24379, 24413, 24446, 24480, 24513, 24546,
		24580, 24613, 24646, 24679, 24712, 24745, 24778, 24811,
		24844, 24877, 24909, 24942, 24974, 25007, 25039, 25072,
		25104, 25136, 25169, 25201, 25233, 25265, 25297, 25329,
		25361, 25392, 25424, 25456, 25487, 25519, 25550, 25582,
		25613, 25645, 25676, 25707, 25738, 25769, 25800, 25831,
		25862, 25893, 25924, 25954, 25985, 26016, 26046, 26077,
		26107, 26137, 26168, 26198, 26228, 26258, 26288, 26318,
		26348, 26378, 26408, 26437, 26467, 26497, 26526, 26556,
		26585, 26615, 26644, 26673, 26702, 26731, 26760, 26789,
		26818, 26847, 26876, 26905, 26933, 26962, 26990, 27019,
		27047, 27076, 27104, 27132, 27160, 27188, 27216, 27244,
		27272, 27300, 27328, 27355, 27383, 27411, 27438, 27466,
		27493, 27520, 27548, 27575, 27602, 27629, 27656, 27683,
		27710, 27736, 27763, 27790, 27816, 27843, 27869, 27896,
		27922, 27948, 27975, 28001, 28027, 28053, 28079, 28105,
		28131, 28156, 28182, 28208, 28233, 28259, 28284, 28309,
		28335, 28360, 28385, 28410, 28435, 28460, 28485, 28510,
		28534, 28559, 28584, 28608, 28633, 28657, 28681, 28706,
		28730, 28754, 28778, 28802, 28826, 28850, 28874, 28897,
		28921, 28945, 28968, 28992, 29015, 29038, 29062, 29085,
		29108, 29131, 29154, 29177, 29200, 29222, 29245, 29268,
		29290, 29313, 29335, 29358, 29380, 29402, 29424, 29446,
		29468, 29490, 29512, 29534, 29556, 29577, 29599, 29621,
		29642, 29663, 29685, 29706, 29727, 29748, 29769, 29790,
		29811, 29832, 29853, 29873, 29894, 29915, 29935, 29955,
		29976, 29996, 30016, 30036, 30056, 30076, 30096, 30116,
		30136, 30156, 30175, 30195, 30214, 30234, 30253, 30272,
		30291, 30311, 30330, 30349, 30368, 30386, 30405, 30424,
		30442, 30461, 30480, 30498, 30516, 30535, 30553, 30571,
		30589, 30607, 30625, 30643, 30660, 30678, 30696, 30713,
		30731, 30748, 30766, 30783, 30800, 30817, 30834, 30851,
		30868, 30885, 30902, 30918, 30935, 30951, 30968, 30984,
		31001, 31017, 31033, 31049, 31065, 31081, 31097, 31113,
		31128, 31144, 31160, 31175, 31191, 31206, 31221, 31236,
		31252, 31267, 31282, 31297, 31311, 31326, 31341, 31356,
		31370, 31385, 31399, 31413, 31428, 31442, 31456, 31470,
		31484, 31498, 31512, 31525, 31539, 31553, 31566, 31580,
		31593, 31606, 31619, 31633, 31646, 31659, 31672, 31684,
		31697, 31710, 31723, 31735, 31748, 31760, 31772, 31785,
		31797, 31809, 31821, 31833, 31845, 31856, 31868, 31880,
		31891, 31903, 31914, 31926, 31937, 31948, 31959, 31970,
		31981, 31992, 32003, 32014, 32024, 32035, 32046, 32056,
		32066, 32077, 32087, 32097, 32107, 32117, 32127, 32137,
		32147, 32156, 32166, 32176, 32185, 32194, 32204, 32213,
		32222, 32231, 32240, 32249, 32258, 32267, 32275, 32284,
		32293, 32301, 32310, 32318, 32326, 32334, 32342, 32350,
		32358, 32366, 32374, 32382, 32389, 32397, 32404, 32412,
		32419, 32426, 32434, 32441, 32448, 32455, 32462, 32468,
		32475, 32482, 32488, 32495, 32501, 32508, 32514, 32520,
		32526, 32532, 32538, 32544, 32550, 32556, 32561, 32567,
		32572, 32578, 32583, 32588, 32594, 32599, 32604, 32609,
		32614, 32618, 32623, 32628, 32632, 32637, 32641, 32646,
		32650, 32654, 32658, 32662, 32666, 32670, 32674, 32678,
		32681, 32685, 32688, 32692, 32695, 32699, 32702, 32705,
		32708, 32711, 32714, 32717, 32719, 32722, 32725, 32727,
		32729, 32732, 32734, 32736, 32738, 32740, 32742, 32744,
		32746, 32748, 32750, 32751, 32753, 32754, 32755, 32757,
		32758, 32759, 32760, 32761, 32762, 32763, 32763, 32764,
		32765, 32765, 32766, 32766, 32766, 32766, 32766, 32767
};

Int16* p_sine_table = sine_table;
