
#include "nuitka/prelude.h"
#include "structseq.h"

// Sentinel PyObject to be used for all our call iterator endings. It will
// become a PyCObject pointing to NULL. It's address is unique, and that's
// enough for us to use it as sentinel value.
PyObject *_sentinel_value = NULL;

PyObject *Nuitka_dunder_compiled_value = NULL;

// We need to make sure this is local to the package, or else it will
// be taken from any external linkage.
#if defined(_NUITKA_MODULE) && !defined(_NUITKA_CONSTANTS_FROM_RESOURCE) && !defined(__cplusplus)
const unsigned char constant_bin[0];
#endif

PyObject *const_int_0;
PyObject *const_str_dot;
PyObject *const_float_0_0;
PyObject *const_float_1_0;
PyObject *const_int_neg_1;
PyObject *const_int_pos_1;
PyObject *const_int_pos_2;
PyObject *const_int_pos_3;
PyObject *const_int_pos_4;
PyObject *const_str_empty;
PyObject *const_dict_empty;
PyObject *const_str_chr_63;
PyObject *const_bytes_empty;
PyObject *const_str_plain_i;
PyObject *const_str_plain_m;
PyObject *const_tuple_empty;
PyObject *const_str_plain_os;
PyObject *const_str_plain_rb;
PyObject *const_str_plain_re;
PyObject *const_str_plain_day;
PyObject *const_str_plain_end;
PyObject *const_str_plain_int;
PyObject *const_str_plain_len;
PyObject *const_str_plain_now;
PyObject *const_str_plain_sum;
PyObject *const_str_plain_Task;
PyObject *const_str_plain_args;
PyObject *const_str_plain_dict;
PyObject *const_str_plain_file;
PyObject *const_str_plain_icon;
PyObject *const_str_plain_iter;
PyObject *const_str_plain_keys;
PyObject *const_str_plain_link;
PyObject *const_str_plain_name;
PyObject *const_str_plain_open;
PyObject *const_str_plain_path;
PyObject *const_str_plain_read;
PyObject *const_str_plain_repr;
PyObject *const_str_plain_self;
PyObject *const_str_plain_send;
PyObject *const_str_plain_site;
PyObject *const_str_plain_task;
PyObject *const_str_plain_text;
PyObject *const_str_plain_time;
PyObject *const_str_plain_type;
PyObject *const_float_minus_0_0;
PyObject *const_float_minus_1_0;
PyObject *const_str_plain_False;
PyObject *const_str_plain_bytes;
PyObject *const_str_plain_close;
PyObject *const_str_plain_items;
PyObject *const_str_plain_level;
PyObject *const_str_plain_match;
PyObject *const_str_plain_print;
PyObject *const_str_plain_range;
PyObject *const_str_plain_split;
PyObject *const_str_plain_throw;
PyObject *const_str_plain_tuple;
PyObject *const_str_plain_types;
PyObject *const_str_angle_lambda;
PyObject *const_str_plain_Friday;
PyObject *const_str_plain_Monday;
PyObject *const_str_plain_Sunday;
PyObject *const_str_plain_append;
PyObject *const_str_plain_format;
PyObject *const_str_plain_locals;
PyObject *const_str_plain_origin;
PyObject *const_str_plain_remove;
PyObject *const_str_plain_Trigger;
PyObject *const_str_plain_Tuesday;
PyObject *const_str_plain___all__;
PyObject *const_str_plain___cmp__;
PyObject *const_str_plain___doc__;
PyObject *const_str_plain_compile;
PyObject *const_str_plain_getattr;
PyObject *const_str_plain_globals;
PyObject *const_str_plain_inspect;
PyObject *const_str_plain_install;
PyObject *const_str_plain_replace;
PyObject *const_str_plain_trigger;
PyObject *const_str_angle_listcomp;
PyObject *const_str_plain_Saturday;
PyObject *const_str_plain_Thursday;
PyObject *const_str_plain___dict__;
PyObject *const_str_plain___exit__;
PyObject *const_str_plain___file__;
PyObject *const_str_plain___init__;
PyObject *const_str_plain___iter__;
PyObject *const_str_plain___main__;
PyObject *const_str_plain___name__;
PyObject *const_str_plain___path__;
PyObject *const_str_plain___spec__;
PyObject *const_str_plain_datetime;
PyObject *const_str_plain_filename;
PyObject *const_str_plain_fromlist;
PyObject *const_str_plain_get_link;
PyObject *const_str_plain_run_task;
PyObject *const_str_plain_taskname;
PyObject *const_str_plain_triggers;
PyObject *const_str_plain_zoompath;
PyObject *const_str_angle_metaclass;
PyObject *const_str_plain_Wednesday;
PyObject *const_str_plain___class__;
PyObject *const_str_plain___enter__;
PyObject *const_str_plain_build_XML;
PyObject *const_str_plain_bytearray;
PyObject *const_str_plain_metaclass;
PyObject *const_str_plain_write_XML;
PyObject *const_str_plain___cached__;
PyObject *const_str_plain___import__;
PyObject *const_str_plain___loader__;
PyObject *const_str_plain___module__;
PyObject *const_str_plain_startswith;
PyObject *const_str_plain___getitem__;
PyObject *const_str_plain___package__;
PyObject *const_str_plain___prepare__;
PyObject *const_str_plain_classmethod;
PyObject *const_str_plain_delete_task;
PyObject *const_str_plain___builtins__;
PyObject *const_str_plain___compiled__;
PyObject *const_str_plain___qualname__;
PyObject *const_str_plain_has_location;
PyObject *const_str_plain_staticmethod;
PyObject *const_str_plain_TaskFunctions;
PyObject *const_str_plain___metaclass__;
PyObject *const_str_plain__initializing;
PyObject *const_str_plain_get_task_list;
PyObject *const_str_plain_get_task_name;
PyObject *const_str_plain_link_validate;
PyObject *const_str_plain_name_validate;
PyObject *const_str_plain___mro_entries__;
PyObject *const_str_plain_create_XML_tree;
PyObject *const_str_plain_get_next_trigger;
PyObject *const_tuple_str_plain_self_tuple;
PyObject *const_str_plain___class_getitem__;
PyObject *const_tuple_str_plain___class___tuple;
PyObject *const_str_plain_submodule_search_locations;
PyObject *const_str_digest_1e80d3180a55df67723460f9d681041e;
PyObject *const_str_digest_25731c733fd74e8333aa29126ce85686;
PyObject *const_str_digest_3c5ea704501052d09946a50239597ecd;
PyObject *const_str_digest_45e4dde2057b0bf276d6a84f4c917d27;
PyObject *const_str_digest_6a06ccac232ea7b3d934a58f271f7a47;
PyObject *const_str_digest_75fd71b1edada749c2ef7ac810062295;
PyObject *const_str_digest_9816e8d1552296af90d250823c964059;
PyObject *const_str_digest_adc474dd61fbd736d69c1bac5d9712e0;
PyObject *const_str_digest_fdd1f239b4f3ffd6efcccf00e4a637e0;
PyObject *const_tuple_str_plain_self_str_plain_trigger_tuple;
PyObject *const_tuple_anon_function_anon_builtin_function_or_method_tuple;

static void _createGlobalConstants(void) {
    NUITKA_MAY_BE_UNUSED PyObject *exception_type, *exception_value;
    NUITKA_MAY_BE_UNUSED PyTracebackObject *exception_tb;

#ifdef _MSC_VER
    // Prevent unused warnings in case of simple programs, the attribute
    // NUITKA_MAY_BE_UNUSED doesn't work for MSVC.
    (void *)exception_type; (void *)exception_value; (void *)exception_tb;
#endif

    const_int_0 = PyLong_FromUnsignedLong(0ul);
    const_str_dot = UNSTREAM_STRING_ASCII(&constant_bin[ 2027 ], 1, 0);
    const_float_0_0 = UNSTREAM_FLOAT(&constant_bin[ 16837 ]);
    const_float_1_0 = UNSTREAM_FLOAT(&constant_bin[ 16845 ]);
    const_int_neg_1 = PyLong_FromLong(-1l);
    const_int_pos_1 = PyLong_FromUnsignedLong(1ul);
    const_int_pos_2 = PyLong_FromUnsignedLong(2ul);
    const_int_pos_3 = PyLong_FromUnsignedLong(3ul);
    const_int_pos_4 = PyLong_FromUnsignedLong(4ul);
    const_str_empty = UNSTREAM_STRING_ASCII(&constant_bin[ 0 ], 0, 0);
    const_dict_empty = _PyDict_NewPresized( 0 );
    assert(PyDict_Size(const_dict_empty) == 0);
    const_str_chr_63 = UNSTREAM_STRING_ASCII(&constant_bin[ 2001 ], 1, 0);
    const_bytes_empty = UNSTREAM_BYTES(&constant_bin[ 0 ], 0);
    const_str_plain_i = UNSTREAM_STRING_ASCII(&constant_bin[ 18 ], 1, 1);
    const_str_plain_m = UNSTREAM_STRING_ASCII(&constant_bin[ 2 ], 1, 1);
    const_tuple_empty = PyTuple_New(0);
    const_str_plain_os = UNSTREAM_STRING_ASCII(&constant_bin[ 186 ], 2, 1);
    const_str_plain_rb = UNSTREAM_STRING_ASCII(&constant_bin[ 16853 ], 2, 1);
    const_str_plain_re = UNSTREAM_STRING_ASCII(&constant_bin[ 38 ], 2, 1);
    const_str_plain_day = UNSTREAM_STRING_ASCII(&constant_bin[ 2443 ], 3, 1);
    const_str_plain_end = UNSTREAM_STRING_ASCII(&constant_bin[ 2901 ], 3, 1);
    const_str_plain_int = UNSTREAM_STRING_ASCII(&constant_bin[ 1440 ], 3, 1);
    const_str_plain_len = UNSTREAM_STRING_ASCII(&constant_bin[ 576 ], 3, 1);
    const_str_plain_now = UNSTREAM_STRING_ASCII(&constant_bin[ 16855 ], 3, 1);
    const_str_plain_sum = UNSTREAM_STRING_ASCII(&constant_bin[ 16858 ], 3, 1);
    const_str_plain_Task = UNSTREAM_STRING_ASCII(&constant_bin[ 239 ], 4, 1);
    const_str_plain_args = UNSTREAM_STRING_ASCII(&constant_bin[ 3227 ], 4, 1);
    const_str_plain_dict = UNSTREAM_STRING_ASCII(&constant_bin[ 1306 ], 4, 1);
    const_str_plain_file = UNSTREAM_STRING_ASCII(&constant_bin[ 1794 ], 4, 1);
    const_str_plain_icon = UNSTREAM_STRING_ASCII(&constant_bin[ 6720 ], 4, 1);
    const_str_plain_iter = UNSTREAM_STRING_ASCII(&constant_bin[ 16861 ], 4, 1);
    const_str_plain_keys = UNSTREAM_STRING_ASCII(&constant_bin[ 16865 ], 4, 1);
    const_str_plain_link = UNSTREAM_STRING_ASCII(&constant_bin[ 1056 ], 4, 1);
    const_str_plain_name = UNSTREAM_STRING_ASCII(&constant_bin[ 0 ], 4, 1);
    const_str_plain_open = UNSTREAM_STRING_ASCII(&constant_bin[ 2759 ], 4, 1);
    const_str_plain_path = UNSTREAM_STRING_ASCII(&constant_bin[ 1249 ], 4, 1);
    const_str_plain_read = UNSTREAM_STRING_ASCII(&constant_bin[ 7530 ], 4, 1);
    const_str_plain_repr = UNSTREAM_STRING_ASCII(&constant_bin[ 16869 ], 4, 1);
    const_str_plain_self = UNSTREAM_STRING_ASCII(&constant_bin[ 16873 ], 4, 1);
    const_str_plain_send = UNSTREAM_STRING_ASCII(&constant_bin[ 16877 ], 4, 1);
    const_str_plain_site = UNSTREAM_STRING_ASCII(&constant_bin[ 1822 ], 4, 1);
    const_str_plain_task = UNSTREAM_STRING_ASCII(&constant_bin[ 518 ], 4, 1);
    const_str_plain_text = UNSTREAM_STRING_ASCII(&constant_bin[ 5185 ], 4, 1);
    const_str_plain_time = UNSTREAM_STRING_ASCII(&constant_bin[ 99 ], 4, 1);
    const_str_plain_type = UNSTREAM_STRING_ASCII(&constant_bin[ 5939 ], 4, 1);
    const_float_minus_0_0 = UNSTREAM_FLOAT(&constant_bin[ 16881 ]);
    const_float_minus_1_0 = UNSTREAM_FLOAT(&constant_bin[ 16889 ]);
    const_str_plain_False = UNSTREAM_STRING_ASCII(&constant_bin[ 16897 ], 5, 1);
    const_str_plain_bytes = UNSTREAM_STRING_ASCII(&constant_bin[ 16902 ], 5, 1);
    const_str_plain_close = UNSTREAM_STRING_ASCII(&constant_bin[ 3459 ], 5, 1);
    const_str_plain_items = UNSTREAM_STRING_ASCII(&constant_bin[ 16907 ], 5, 1);
    const_str_plain_level = UNSTREAM_STRING_ASCII(&constant_bin[ 456 ], 5, 1);
    const_str_plain_match = UNSTREAM_STRING_ASCII(&constant_bin[ 16912 ], 5, 1);
    const_str_plain_print = UNSTREAM_STRING_ASCII(&constant_bin[ 16917 ], 5, 1);
    const_str_plain_range = UNSTREAM_STRING_ASCII(&constant_bin[ 549 ], 5, 1);
    const_str_plain_split = UNSTREAM_STRING_ASCII(&constant_bin[ 16922 ], 5, 1);
    const_str_plain_throw = UNSTREAM_STRING_ASCII(&constant_bin[ 16927 ], 5, 1);
    const_str_plain_tuple = UNSTREAM_STRING_ASCII(&constant_bin[ 16932 ], 5, 1);
    const_str_plain_types = UNSTREAM_STRING_ASCII(&constant_bin[ 16937 ], 5, 1);
    const_str_angle_lambda = UNSTREAM_STRING_ASCII(&constant_bin[ 6206 ], 8, 0);
    const_str_plain_Friday = UNSTREAM_STRING_ASCII(&constant_bin[ 16942 ], 6, 1);
    const_str_plain_Monday = UNSTREAM_STRING_ASCII(&constant_bin[ 16948 ], 6, 1);
    const_str_plain_Sunday = UNSTREAM_STRING_ASCII(&constant_bin[ 16954 ], 6, 1);
    const_str_plain_append = UNSTREAM_STRING_ASCII(&constant_bin[ 16960 ], 6, 1);
    const_str_plain_format = UNSTREAM_STRING_ASCII(&constant_bin[ 3245 ], 6, 1);
    const_str_plain_locals = UNSTREAM_STRING_ASCII(&constant_bin[ 6198 ], 6, 1);
    const_str_plain_origin = UNSTREAM_STRING_ASCII(&constant_bin[ 16966 ], 6, 1);
    const_str_plain_remove = UNSTREAM_STRING_ASCII(&constant_bin[ 16972 ], 6, 1);
    const_str_plain_Trigger = UNSTREAM_STRING_ASCII(&constant_bin[ 210 ], 7, 1);
    const_str_plain_Tuesday = UNSTREAM_STRING_ASCII(&constant_bin[ 16978 ], 7, 1);
    const_str_plain___all__ = UNSTREAM_STRING_ASCII(&constant_bin[ 16985 ], 7, 1);
    const_str_plain___cmp__ = UNSTREAM_STRING_ASCII(&constant_bin[ 16992 ], 7, 1);
    const_str_plain___doc__ = UNSTREAM_STRING_ASCII(&constant_bin[ 16999 ], 7, 1);
    const_str_plain_compile = UNSTREAM_STRING_ASCII(&constant_bin[ 17006 ], 7, 1);
    const_str_plain_getattr = UNSTREAM_STRING_ASCII(&constant_bin[ 17013 ], 7, 1);
    const_str_plain_globals = UNSTREAM_STRING_ASCII(&constant_bin[ 17020 ], 7, 1);
    const_str_plain_inspect = UNSTREAM_STRING_ASCII(&constant_bin[ 17027 ], 7, 1);
    const_str_plain_install = UNSTREAM_STRING_ASCII(&constant_bin[ 7944 ], 7, 1);
    const_str_plain_replace = UNSTREAM_STRING_ASCII(&constant_bin[ 5340 ], 7, 1);
    const_str_plain_trigger = UNSTREAM_STRING_ASCII(&constant_bin[ 3283 ], 7, 1);
    const_str_angle_listcomp = UNSTREAM_STRING_ASCII(&constant_bin[ 17034 ], 10, 0);
    const_str_plain_Saturday = UNSTREAM_STRING_ASCII(&constant_bin[ 17044 ], 8, 1);
    const_str_plain_Thursday = UNSTREAM_STRING_ASCII(&constant_bin[ 17052 ], 8, 1);
    const_str_plain___dict__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17060 ], 8, 1);
    const_str_plain___exit__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17068 ], 8, 1);
    const_str_plain___file__ = UNSTREAM_STRING_ASCII(&constant_bin[ 1792 ], 8, 1);
    const_str_plain___init__ = UNSTREAM_STRING_ASCII(&constant_bin[ 3319 ], 8, 1);
    const_str_plain___iter__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17076 ], 8, 1);
    const_str_plain___main__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17084 ], 8, 1);
    const_str_plain___name__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17092 ], 8, 1);
    const_str_plain___path__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17100 ], 8, 1);
    const_str_plain___spec__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17108 ], 8, 1);
    const_str_plain_datetime = UNSTREAM_STRING_ASCII(&constant_bin[ 95 ], 8, 1);
    const_str_plain_filename = UNSTREAM_STRING_ASCII(&constant_bin[ 17116 ], 8, 1);
    const_str_plain_fromlist = UNSTREAM_STRING_ASCII(&constant_bin[ 17124 ], 8, 1);
    const_str_plain_get_link = UNSTREAM_STRING_ASCII(&constant_bin[ 5326 ], 8, 1);
    const_str_plain_run_task = UNSTREAM_STRING_ASCII(&constant_bin[ 777 ], 8, 1);
    const_str_plain_taskname = UNSTREAM_STRING_ASCII(&constant_bin[ 17132 ], 8, 1);
    const_str_plain_triggers = UNSTREAM_STRING_ASCII(&constant_bin[ 3786 ], 8, 1);
    const_str_plain_zoompath = UNSTREAM_STRING_ASCII(&constant_bin[ 1245 ], 8, 1);
    const_str_angle_metaclass = UNSTREAM_STRING_ASCII(&constant_bin[ 17140 ], 11, 0);
    const_str_plain_Wednesday = UNSTREAM_STRING_ASCII(&constant_bin[ 17151 ], 9, 1);
    const_str_plain___class__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17160 ], 9, 1);
    const_str_plain___enter__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17169 ], 9, 1);
    const_str_plain_build_XML = UNSTREAM_STRING_ASCII(&constant_bin[ 1214 ], 9, 1);
    const_str_plain_bytearray = UNSTREAM_STRING_ASCII(&constant_bin[ 17178 ], 9, 1);
    const_str_plain_metaclass = UNSTREAM_STRING_ASCII(&constant_bin[ 17141 ], 9, 1);
    const_str_plain_write_XML = UNSTREAM_STRING_ASCII(&constant_bin[ 1183 ], 9, 1);
    const_str_plain___cached__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17187 ], 10, 1);
    const_str_plain___import__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17197 ], 10, 1);
    const_str_plain___loader__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17207 ], 10, 1);
    const_str_plain___module__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17217 ], 10, 1);
    const_str_plain_startswith = UNSTREAM_STRING_ASCII(&constant_bin[ 17227 ], 10, 1);
    const_str_plain___getitem__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17237 ], 11, 1);
    const_str_plain___package__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17248 ], 11, 1);
    const_str_plain___prepare__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17259 ], 11, 1);
    const_str_plain_classmethod = UNSTREAM_STRING_ASCII(&constant_bin[ 17270 ], 11, 1);
    const_str_plain_delete_task = UNSTREAM_STRING_ASCII(&constant_bin[ 839 ], 11, 1);
    const_str_plain___builtins__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17281 ], 12, 1);
    const_str_plain___compiled__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17293 ], 12, 1);
    const_str_plain___qualname__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17305 ], 12, 1);
    const_str_plain_has_location = UNSTREAM_STRING_ASCII(&constant_bin[ 17317 ], 12, 1);
    const_str_plain_staticmethod = UNSTREAM_STRING_ASCII(&constant_bin[ 17329 ], 12, 1);
    const_str_plain_TaskFunctions = UNSTREAM_STRING_ASCII(&constant_bin[ 3674 ], 13, 1);
    const_str_plain___metaclass__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17341 ], 13, 1);
    const_str_plain__initializing = UNSTREAM_STRING_ASCII(&constant_bin[ 17354 ], 13, 1);
    const_str_plain_get_task_list = UNSTREAM_STRING_ASCII(&constant_bin[ 514 ], 13, 1);
    const_str_plain_get_task_name = UNSTREAM_STRING_ASCII(&constant_bin[ 2195 ], 13, 1);
    const_str_plain_link_validate = UNSTREAM_STRING_ASCII(&constant_bin[ 1056 ], 13, 1);
    const_str_plain_name_validate = UNSTREAM_STRING_ASCII(&constant_bin[ 1021 ], 13, 1);
    const_str_plain___mro_entries__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17367 ], 15, 1);
    const_str_plain_create_XML_tree = UNSTREAM_STRING_ASCII(&constant_bin[ 125 ], 15, 1);
    const_str_plain_get_next_trigger = UNSTREAM_STRING_ASCII(&constant_bin[ 3274 ], 16, 1);
    const_tuple_str_plain_self_tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(const_tuple_str_plain_self_tuple, 0, const_str_plain_self); Py_INCREF(const_str_plain_self);
    const_str_plain___class_getitem__ = UNSTREAM_STRING_ASCII(&constant_bin[ 17382 ], 17, 1);
    const_tuple_str_plain___class___tuple = PyTuple_New(1);
    PyTuple_SET_ITEM(const_tuple_str_plain___class___tuple, 0, const_str_plain___class__); Py_INCREF(const_str_plain___class__);
    const_str_plain_submodule_search_locations = UNSTREAM_STRING_ASCII(&constant_bin[ 17399 ], 26, 1);
    const_str_digest_1e80d3180a55df67723460f9d681041e = UNSTREAM_STRING_ASCII(&constant_bin[ 17425 ], 27, 0);
    const_str_digest_25731c733fd74e8333aa29126ce85686 = UNSTREAM_STRING_ASCII(&constant_bin[ 2335 ], 2, 0);
    const_str_digest_3c5ea704501052d09946a50239597ecd = UNSTREAM_STRING_ASCII(&constant_bin[ 17452 ], 64, 0);
    const_str_digest_45e4dde2057b0bf276d6a84f4c917d27 = UNSTREAM_STRING_ASCII(&constant_bin[ 17516 ], 7, 0);
    const_str_digest_6a06ccac232ea7b3d934a58f271f7a47 = UNSTREAM_STRING_ASCII(&constant_bin[ 8176 ], 53, 0);
    const_str_digest_75fd71b1edada749c2ef7ac810062295 = UNSTREAM_STRING_ASCII(&constant_bin[ 17523 ], 46, 0);
    const_str_digest_9816e8d1552296af90d250823c964059 = UNSTREAM_STRING_ASCII(&constant_bin[ 17569 ], 46, 0);
    const_str_digest_adc474dd61fbd736d69c1bac5d9712e0 = UNSTREAM_STRING_ASCII(&constant_bin[ 17615 ], 47, 0);
    const_str_digest_fdd1f239b4f3ffd6efcccf00e4a637e0 = UNSTREAM_STRING_ASCII(&constant_bin[ 17662 ], 48, 0);
    const_tuple_str_plain_self_str_plain_trigger_tuple = PyTuple_New(2);
    PyTuple_SET_ITEM(const_tuple_str_plain_self_str_plain_trigger_tuple, 0, const_str_plain_self); Py_INCREF(const_str_plain_self);
    PyTuple_SET_ITEM(const_tuple_str_plain_self_str_plain_trigger_tuple, 1, const_str_plain_trigger); Py_INCREF(const_str_plain_trigger);
    const_tuple_anon_function_anon_builtin_function_or_method_tuple = PyTuple_New(2);
    PyTuple_SET_ITEM(const_tuple_anon_function_anon_builtin_function_or_method_tuple, 0, (PyObject *)&PyFunction_Type); Py_INCREF((PyObject *)&PyFunction_Type);
    PyTuple_SET_ITEM(const_tuple_anon_function_anon_builtin_function_or_method_tuple, 1, (PyObject *)&PyCFunction_Type); Py_INCREF((PyObject *)&PyCFunction_Type);

#if _NUITKA_EXE
    /* Set the "sys.executable" path to the original CPython executable. */
    PySys_SetObject(
        (char *)"executable",
        const_str_digest_3c5ea704501052d09946a50239597ecd
    );

#ifndef _NUITKA_STANDALONE
    /* Set the "sys.prefix" path to the original one. */
    PySys_SetObject(
        (char *)"prefix",
        const_str_digest_6a06ccac232ea7b3d934a58f271f7a47
    );

    /* Set the "sys.prefix" path to the original one. */
    PySys_SetObject(
        (char *)"exec_prefix",
        const_str_digest_6a06ccac232ea7b3d934a58f271f7a47
    );


#if PYTHON_VERSION >= 300
    /* Set the "sys.base_prefix" path to the original one. */
    PySys_SetObject(
        (char *)"base_prefix",
        const_str_digest_6a06ccac232ea7b3d934a58f271f7a47
    );

    /* Set the "sys.exec_base_prefix" path to the original one. */
    PySys_SetObject(
        (char *)"base_exec_prefix",
        const_str_digest_6a06ccac232ea7b3d934a58f271f7a47
    );

#endif
#endif
#endif

    static PyTypeObject Nuitka_VersionInfoType;

    // Same fields as "sys.version_info" except no serial number.
    static PyStructSequence_Field Nuitka_VersionInfoFields[] = {
        {(char *)"major", (char *)"Major release number"},
        {(char *)"minor", (char *)"Minor release number"},
        {(char *)"micro", (char *)"Micro release number"},
        {(char *)"releaselevel", (char *)"'alpha', 'beta', 'candidate', or 'release'"},
        {0}
    };

    static PyStructSequence_Desc Nuitka_VersionInfoDesc = {
        (char *)"__nuitka_version__",                                    /* name */
        (char *)"__compiled__\n\nVersion information as a named tuple.", /* doc */
        Nuitka_VersionInfoFields,                                        /* fields */
        4
    };

    PyStructSequence_InitType(&Nuitka_VersionInfoType, &Nuitka_VersionInfoDesc);

    Nuitka_dunder_compiled_value = PyStructSequence_New(&Nuitka_VersionInfoType);
    assert(Nuitka_dunder_compiled_value != NULL);

    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 0, PyInt_FromLong(0));
    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 1, PyInt_FromLong(6));
    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 2, PyInt_FromLong(8));

    PyStructSequence_SET_ITEM(Nuitka_dunder_compiled_value, 3, Nuitka_String_FromString("release"));

    // Prevent users from creating the Nuitka version type object.
    Nuitka_VersionInfoType.tp_init = NULL;
    Nuitka_VersionInfoType.tp_new = NULL;


}

// In debug mode we can check that the constants were not tampered with in any
// given moment. We typically do it at program exit, but we can add extra calls
// for sanity.
#ifndef __NUITKA_NO_ASSERT__
void checkGlobalConstants(void) {

}
#endif

void createGlobalConstants(void) {
    if (_sentinel_value == NULL) {
#if PYTHON_VERSION < 300
        _sentinel_value = PyCObject_FromVoidPtr(NULL, NULL);
#else
        // The NULL value is not allowed for a capsule, so use something else.
        _sentinel_value = PyCapsule_New((void *)27, "sentinel", NULL);
#endif
        assert(_sentinel_value);

        _createGlobalConstants();
    }
}
