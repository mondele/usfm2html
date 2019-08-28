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

void copyFrozenModulesTo( struct _frozen *destination )
{
    struct frozen_desc frozen_modules[] = {
        { "_collections_abc", 6091561, 28953 },
        { "_compression", 6120514, 4135 },
        { "_weakrefset", 6124649, 7473 },
        { "abc", 6132122, 6462 },
        { "base64", 6138584, 17070 },
        { "bz2", 6155654, 11192 },
        { "codecs", 6166846, 33909 },
        { "collections", 6200755, -46621 },
        { "collections.abc", 6091561, 28953 },
        { "copyreg", 6247376, 4255 },
        { "dis", 6251631, 15216 },
        { "encodings", 6266847, -3958 },
        { "encodings.aliases", 6270805, 6307 },
        { "encodings.ascii", 6277112, 1895 },
        { "encodings.base64_codec", 6279007, 2434 },
        { "encodings.big5", 6281441, 1455 },
        { "encodings.big5hkscs", 6282896, 1465 },
        { "encodings.bz2_codec", 6284361, 3296 },
        { "encodings.charmap", 6287657, 2948 },
        { "encodings.cp037", 6290605, 2440 },
        { "encodings.cp1006", 6293045, 2516 },
        { "encodings.cp1026", 6295561, 2444 },
        { "encodings.cp1125", 6298005, 8137 },
        { "encodings.cp1140", 6306142, 2430 },
        { "encodings.cp1250", 6308572, 2467 },
        { "encodings.cp1251", 6311039, 2464 },
        { "encodings.cp1252", 6313503, 2467 },
        { "encodings.cp1253", 6315970, 2480 },
        { "encodings.cp1254", 6318450, 2469 },
        { "encodings.cp1255", 6320919, 2488 },
        { "encodings.cp1256", 6323407, 2466 },
        { "encodings.cp1257", 6325873, 2474 },
        { "encodings.cp1258", 6328347, 2472 },
        { "encodings.cp273", 6330819, 2426 },
        { "encodings.cp424", 6333245, 2470 },
        { "encodings.cp437", 6335715, 7854 },
        { "encodings.cp500", 6343569, 2440 },
        { "encodings.cp720", 6346009, 2537 },
        { "encodings.cp737", 6348546, 8176 },
        { "encodings.cp775", 6356722, 7884 },
        { "encodings.cp850", 6364606, 7515 },
        { "encodings.cp852", 6372121, 7892 },
        { "encodings.cp855", 6380013, 8145 },
        { "encodings.cp856", 6388158, 2502 },
        { "encodings.cp857", 6390660, 7497 },
        { "encodings.cp858", 6398157, 7485 },
        { "encodings.cp860", 6405642, 7833 },
        { "encodings.cp861", 6413475, 7848 },
        { "encodings.cp862", 6421323, 8037 },
        { "encodings.cp863", 6429360, 7848 },
        { "encodings.cp864", 6437208, 7994 },
        { "encodings.cp865", 6445202, 7848 },
        { "encodings.cp866", 6453050, 8181 },
        { "encodings.cp869", 6461231, 7874 },
        { "encodings.cp874", 6469105, 2568 },
        { "encodings.cp875", 6471673, 2437 },
        { "encodings.cp932", 6474110, 1457 },
        { "encodings.cp949", 6475567, 1457 },
        { "encodings.cp950", 6477024, 1457 },
        { "encodings.euc_jis_2004", 6478481, 1471 },
        { "encodings.euc_jisx0213", 6479952, 1471 },
        { "encodings.euc_jp", 6481423, 1459 },
        { "encodings.euc_kr", 6482882, 1459 },
        { "encodings.gb18030", 6484341, 1461 },
        { "encodings.gb2312", 6485802, 1459 },
        { "encodings.gbk", 6487261, 1453 },
        { "encodings.hex_codec", 6488714, 2421 },
        { "encodings.hp_roman8", 6491135, 2641 },
        { "encodings.hz", 6493776, 1451 },
        { "encodings.idna", 6495227, 5735 },
        { "encodings.iso2022_jp", 6500962, 1472 },
        { "encodings.iso2022_jp_1", 6502434, 1476 },
        { "encodings.iso2022_jp_2", 6503910, 1476 },
        { "encodings.iso2022_jp_2004", 6505386, 1482 },
        { "encodings.iso2022_jp_3", 6506868, 1476 },
        { "encodings.iso2022_jp_ext", 6508344, 1480 },
        { "encodings.iso2022_kr", 6509824, 1472 },
        { "encodings.iso8859_1", 6511296, 2439 },
        { "encodings.iso8859_10", 6513735, 2444 },
        { "encodings.iso8859_11", 6516179, 2538 },
        { "encodings.iso8859_13", 6518717, 2447 },
        { "encodings.iso8859_14", 6521164, 2465 },
        { "encodings.iso8859_15", 6523629, 2444 },
        { "encodings.iso8859_16", 6526073, 2446 },
        { "encodings.iso8859_2", 6528519, 2439 },
        { "encodings.iso8859_3", 6530958, 2446 },
        { "encodings.iso8859_4", 6533404, 2439 },
        { "encodings.iso8859_5", 6535843, 2440 },
        { "encodings.iso8859_6", 6538283, 2484 },
        { "encodings.iso8859_7", 6540767, 2447 },
        { "encodings.iso8859_8", 6543214, 2478 },
        { "encodings.iso8859_9", 6545692, 2439 },
        { "encodings.johab", 6548131, 1457 },
        { "encodings.koi8_r", 6549588, 2491 },
        { "encodings.koi8_t", 6552079, 2402 },
        { "encodings.koi8_u", 6554481, 2477 },
        { "encodings.kz1048", 6556958, 2454 },
        { "encodings.latin_1", 6559412, 1907 },
        { "encodings.mac_arabic", 6561319, 7748 },
        { "encodings.mac_centeuro", 6569067, 2478 },
        { "encodings.mac_croatian", 6571545, 2486 },
        { "encodings.mac_cyrillic", 6574031, 2476 },
        { "encodings.mac_farsi", 6576507, 2420 },
        { "encodings.mac_greek", 6578927, 2460 },
        { "encodings.mac_iceland", 6581387, 2479 },
        { "encodings.mac_latin2", 6583866, 2620 },
        { "encodings.mac_roman", 6586486, 2477 },
        { "encodings.mac_romanian", 6588963, 2487 },
        { "encodings.mac_turkish", 6591450, 2480 },
        { "encodings.palmos", 6593930, 2467 },
        { "encodings.ptcp154", 6596397, 2561 },
        { "encodings.punycode", 6598958, 6428 },
        { "encodings.quopri_codec", 6605386, 2454 },
        { "encodings.raw_unicode_escape", 6607840, 1780 },
        { "encodings.rot_13", 6609620, 3040 },
        { "encodings.shift_jis", 6612660, 1465 },
        { "encodings.shift_jis_2004", 6614125, 1475 },
        { "encodings.shift_jisx0213", 6615600, 1475 },
        { "encodings.tis_620", 6617075, 2529 },
        { "encodings.undefined", 6619604, 2174 },
        { "encodings.unicode_escape", 6621778, 1760 },
        { "encodings.unicode_internal", 6623538, 1770 },
        { "encodings.utf_16", 6625308, 4844 },
        { "encodings.utf_16_be", 6630152, 1645 },
        { "encodings.utf_16_le", 6631797, 1645 },
        { "encodings.utf_32", 6633442, 4737 },
        { "encodings.utf_32_be", 6638179, 1538 },
        { "encodings.utf_32_le", 6639717, 1538 },
        { "encodings.utf_7", 6641255, 1566 },
        { "encodings.utf_8", 6642821, 1625 },
        { "encodings.utf_8_sig", 6644446, 4527 },
        { "encodings.uu_codec", 6648973, 3236 },
        { "encodings.zlib_codec", 6652209, 3134 },
        { "enum", 6655343, 23758 },
        { "functools", 6679101, 23825 },
        { "genericpath", 6702926, 3759 },
        { "heapq", 6706685, 14373 },
        { "importlib", 6721058, -3743 },
        { "importlib._bootstrap", 6724801, 29189 },
        { "importlib._bootstrap_external", 6753990, 41829 },
        { "importlib.machinery", 6795819, 983 },
        { "inspect", 6796802, 79681 },
        { "io", 6876483, 3420 },
        { "keyword", 6879903, 1820 },
        { "linecache", 6881723, 3800 },
        { "locale", 6885523, 34566 },
        { "opcode", 6920089, 5389 },
        { "operator", 6925478, 13911 },
        { "os", 6939389, 29704 },
        { "posixpath", 6969093, 10407 },
        { "quopri", 6979500, 5782 },
        { "re", 6985282, 13815 },
        { "reprlib", 6999097, 5361 },
        { "sre_compile", 7004458, 15214 },
        { "sre_constants", 7019672, 6302 },
        { "sre_parse", 7025974, 21368 },
        { "stat", 7047342, 3884 },
        { "stringprep", 7051226, 10043 },
        { "struct", 7061269, 345 },
        { "threading", 7061614, 37346 },
        { "token", 7098960, 3610 },
        { "tokenize", 7102570, 17741 },
        { "traceback", 7120311, 19524 },
        { "types", 7139835, 8985 },
        { "warnings", 7148820, 13777 },
        { NULL, 0, 0 }
    };

    struct frozen_desc *current = frozen_modules;

    for(;;)
    {
        destination->name = (char *)current->name;
        destination->code = (unsigned char *)&constant_bin[ current->start ];
        destination->size = current->size;

        if (destination->name == NULL) break;

        current += 1;
        destination += 1;
    };
}
