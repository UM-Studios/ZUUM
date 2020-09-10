// This provides the frozen (compiled bytecode) files that are included if
// any.
#include <Python.h>

#include "nuitka/constants_blob.h"

// Blob from which modules are unstreamed.
#define stream_data constant_bin

// These modules should be loaded as bytecode. They may e.g. have to be loadable
// during "Py_Initialize" already, or for irrelevance, they are only included
// in this un-optimized form. These are not compiled by Nuitka, and therefore
// are not accelerated at all, merely bundled with the binary or module, so
// that CPython library can start out finding them.

struct frozen_desc {
    char const *name;
    ssize_t start;
    int size;
};

void copyFrozenModulesTo(struct _frozen *destination) {
    struct frozen_desc frozen_modules[] = {
        {"_collections_abc", 430499, 28764},
        {"_compression", 464787, 4169},
        {"_weakrefset", 597241, 7623},
        {"abc", 604864, 5357},
        {"base64", 17515, 17092},
        {"bz2", 1109800, 11468},
        {"codecs", 1217604, 33936},
        {"collections", 1257836, -46458},
        {"collections.abc", 430499, 28764},
        {"copyreg", 1439374, 4341},
        {"dis", 1761028, 15825},
        {"encodings", 2576088, -3926},
        {"encodings.aliases", 2580014, 6353},
        {"encodings.ascii", 2586367, 1904},
        {"encodings.base64_codec", 2588271, 2422},
        {"encodings.big5", 2590693, 1432},
        {"encodings.big5hkscs", 2592125, 1442},
        {"encodings.bz2_codec", 2593567, 3313},
        {"encodings.charmap", 2596880, 2914},
        {"encodings.cp037", 2599794, 2445},
        {"encodings.cp1006", 2602239, 2521},
        {"encodings.cp1026", 2604760, 2449},
        {"encodings.cp1125", 2607209, 8152},
        {"encodings.cp1140", 2615361, 2435},
        {"encodings.cp1250", 2617796, 2472},
        {"encodings.cp1251", 2620268, 2469},
        {"encodings.cp1252", 2622737, 2472},
        {"encodings.cp1253", 2625209, 2485},
        {"encodings.cp1254", 2627694, 2474},
        {"encodings.cp1255", 2630168, 2493},
        {"encodings.cp1256", 2632661, 2471},
        {"encodings.cp1257", 2635132, 2479},
        {"encodings.cp1258", 2637611, 2477},
        {"encodings.cp273", 2640088, 2431},
        {"encodings.cp424", 2642519, 2475},
        {"encodings.cp437", 2644994, 7869},
        {"encodings.cp500", 2652863, 2445},
        {"encodings.cp720", 2655308, 2542},
        {"encodings.cp737", 2657850, 8191},
        {"encodings.cp775", 2666041, 7899},
        {"encodings.cp850", 2673940, 7530},
        {"encodings.cp852", 2681470, 7907},
        {"encodings.cp855", 2689377, 8160},
        {"encodings.cp856", 2697537, 2507},
        {"encodings.cp857", 2700044, 7510},
        {"encodings.cp858", 2707554, 7500},
        {"encodings.cp860", 2715054, 7848},
        {"encodings.cp861", 2722902, 7863},
        {"encodings.cp862", 2730765, 8052},
        {"encodings.cp863", 2738817, 7863},
        {"encodings.cp864", 2746680, 8007},
        {"encodings.cp865", 2754687, 7863},
        {"encodings.cp866", 2762550, 8196},
        {"encodings.cp869", 2770746, 7887},
        {"encodings.cp874", 2778633, 2573},
        {"encodings.cp875", 2781206, 2442},
        {"encodings.cp932", 2783648, 1434},
        {"encodings.cp949", 2785082, 1434},
        {"encodings.cp950", 2786516, 1434},
        {"encodings.euc_jis_2004", 2787950, 1448},
        {"encodings.euc_jisx0213", 2789398, 1448},
        {"encodings.euc_jp", 2790846, 1436},
        {"encodings.euc_kr", 2792282, 1436},
        {"encodings.gb18030", 2793718, 1438},
        {"encodings.gb2312", 2795156, 1436},
        {"encodings.gbk", 2796592, 1430},
        {"encodings.hex_codec", 2798022, 2409},
        {"encodings.hp_roman8", 2800431, 2646},
        {"encodings.hz", 2803077, 1428},
        {"encodings.idna", 2804505, 5640},
        {"encodings.iso2022_jp", 2810145, 1449},
        {"encodings.iso2022_jp_1", 2811594, 1453},
        {"encodings.iso2022_jp_2", 2813047, 1453},
        {"encodings.iso2022_jp_2004", 2814500, 1459},
        {"encodings.iso2022_jp_3", 2815959, 1453},
        {"encodings.iso2022_jp_ext", 2817412, 1457},
        {"encodings.iso2022_kr", 2818869, 1449},
        {"encodings.iso8859_1", 2820318, 2444},
        {"encodings.iso8859_10", 2822762, 2449},
        {"encodings.iso8859_11", 2825211, 2543},
        {"encodings.iso8859_13", 2827754, 2452},
        {"encodings.iso8859_14", 2830206, 2470},
        {"encodings.iso8859_15", 2832676, 2449},
        {"encodings.iso8859_16", 2835125, 2451},
        {"encodings.iso8859_2", 2837576, 2444},
        {"encodings.iso8859_3", 2840020, 2451},
        {"encodings.iso8859_4", 2842471, 2444},
        {"encodings.iso8859_5", 2844915, 2445},
        {"encodings.iso8859_6", 2847360, 2489},
        {"encodings.iso8859_7", 2849849, 2452},
        {"encodings.iso8859_8", 2852301, 2483},
        {"encodings.iso8859_9", 2854784, 2444},
        {"encodings.johab", 2857228, 1434},
        {"encodings.koi8_r", 2858662, 2496},
        {"encodings.koi8_t", 2861158, 2407},
        {"encodings.koi8_u", 2863565, 2482},
        {"encodings.kz1048", 2866047, 2459},
        {"encodings.latin_1", 2868506, 1916},
        {"encodings.mac_arabic", 2870422, 7763},
        {"encodings.mac_centeuro", 2878185, 2483},
        {"encodings.mac_croatian", 2880668, 2491},
        {"encodings.mac_cyrillic", 2883159, 2481},
        {"encodings.mac_farsi", 2885640, 2425},
        {"encodings.mac_greek", 2888065, 2465},
        {"encodings.mac_iceland", 2890530, 2484},
        {"encodings.mac_latin2", 2893014, 2625},
        {"encodings.mac_roman", 2895639, 2482},
        {"encodings.mac_romanian", 2898121, 2492},
        {"encodings.mac_turkish", 2900613, 2485},
        {"encodings.mbcs", 2903098, 1734},
        {"encodings.oem", 2904832, 1547},
        {"encodings.palmos", 2906379, 2472},
        {"encodings.ptcp154", 2908851, 2566},
        {"encodings.punycode", 2911417, 6334},
        {"encodings.quopri_codec", 2917751, 2438},
        {"encodings.raw_unicode_escape", 2920189, 1785},
        {"encodings.rot_13", 2921974, 3024},
        {"encodings.shift_jis", 2924998, 1442},
        {"encodings.shift_jis_2004", 2926440, 1452},
        {"encodings.shift_jisx0213", 2927892, 1452},
        {"encodings.tis_620", 2929344, 2534},
        {"encodings.undefined", 2931878, 2118},
        {"encodings.unicode_escape", 2933996, 1765},
        {"encodings.utf_16", 2935761, 4895},
        {"encodings.utf_16_be", 2940656, 1673},
        {"encodings.utf_16_le", 2942329, 1673},
        {"encodings.utf_32", 2944002, 4788},
        {"encodings.utf_32_be", 2948790, 1566},
        {"encodings.utf_32_le", 2950356, 1566},
        {"encodings.utf_7", 2951922, 1594},
        {"encodings.utf_8", 2953516, 1653},
        {"encodings.utf_8_sig", 2955169, 4569},
        {"encodings.uu_codec", 2959738, 3221},
        {"encodings.zlib_codec", 2962959, 3132},
        {"enum", 2966091, 24422},
        {"functools", 3079913, 27920},
        {"genericpath", 3107833, 4009},
        {"heapq", 3169501, 14093},
        {"importlib", 3453218, -3781},
        {"importlib._bootstrap", 3456999, 28628},
        {"importlib._bootstrap_external", 3485627, 43733},
        {"importlib.machinery", 3542956, 985},
        {"inspect", 3578936, 80394},
        {"io", 3659330, 3477},
        {"keyword", 3759850, 1021},
        {"linecache", 3998155, 3862},
        {"locale", 4002017, 34712},
        {"ntpath", 4616374, 14308},
        {"opcode", 4644531, 5443},
        {"operator", 4649974, 13714},
        {"os", 91608, 31346},
        {"quopri", 5597150, 5771},
        {"re", 122954, 14331},
        {"reprlib", 5623052, 5326},
        {"sre_compile", 5849376, 15165},
        {"sre_constants", 5864541, 6382},
        {"sre_parse", 5870923, 21670},
        {"stat", 5937212, 4395},
        {"stringprep", 5982462, 11040},
        {"struct", 5993502, 353},
        {"threading", 6166196, 39952},
        {"token", 6405261, 2508},
        {"tokenize", 6407769, 17183},
        {"types", 6612315, 9200},
        {"warnings", 7033610, 13779},
        {NULL, 0, 0}
    };

    struct frozen_desc *current = frozen_modules;

    for(;;) {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}