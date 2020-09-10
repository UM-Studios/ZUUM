// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 1; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 1 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 1; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 1 * sizeof(PyObject *));
            memcpy(python_pars + 1, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 1)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 1 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 1; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 1 * sizeof(PyObject *));
                memcpy(python_pars+1 + 1, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 1)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (1 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (1 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 1);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 1, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 1);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 1);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 2; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 2 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 2; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 2 * sizeof(PyObject *));
            memcpy(python_pars + 2, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 2)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 2 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 2; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 2 * sizeof(PyObject *));
                memcpy(python_pars+1 + 2, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 2)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (2 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (2 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 2);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 2, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 2);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 2);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 3; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 3 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 3; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 3 * sizeof(PyObject *));
            memcpy(python_pars + 3, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 3)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 3 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 3; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 3 * sizeof(PyObject *));
                memcpy(python_pars+1 + 3, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 3)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (3 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (3 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 3);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 3, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 3);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 3);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 4; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 4 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 4; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 4 * sizeof(PyObject *));
            memcpy(python_pars + 4, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 4)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 4 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 4; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 4 * sizeof(PyObject *));
                memcpy(python_pars+1 + 4, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 4)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (4 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (4 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 4);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 4, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 4);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 4);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5(PyObject *called, PyObject **args) {
    CHECK_OBJECT(called);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 5; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    if (Nuitka_Function_Check(called)) {
        if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if (function->m_args_simple && 5 == function->m_args_positional_count){
            for (Py_ssize_t i = 0; i < 5; i++) {
                Py_INCREF(args[i]);
            }

            result = function->m_c_code(function, args);
        } else if (function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count) {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
            PyObject *python_pars[function->m_args_positional_count];
#endif
            memcpy(python_pars, args, 5 * sizeof(PyObject *));
            memcpy(python_pars + 5, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

            for (Py_ssize_t i = 0; i < function->m_args_positional_count; i++) {
                Py_INCREF(python_pars[i]);
            }

            result = function->m_c_code(function, python_pars);
        } else {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
            PyObject *python_pars[function->m_args_overall_count];
#endif
            memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

            if (parseArgumentsPos(function, python_pars, args, 5)) {
                result = function->m_c_code(function, python_pars);
            } else {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    } else if (Nuitka_Method_Check(called)) {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if (method->m_object != NULL) {
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if (function->m_args_simple && 5 + 1 == function->m_args_positional_count) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                for (Py_ssize_t i = 0; i < 5; i++) {
                    python_pars[i+1] = args[i];
                    Py_INCREF(args[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count ) {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_positional_count);
#else
                PyObject *python_pars[function->m_args_positional_count];
#endif
                python_pars[0] = method->m_object;
                Py_INCREF(method->m_object);

                memcpy(python_pars+1, args, 5 * sizeof(PyObject *));
                memcpy(python_pars+1 + 5, &PyTuple_GET_ITEM(function->m_defaults, 0), function->m_defaults_given * sizeof(PyObject *));

                for (Py_ssize_t i = 1; i < function->m_args_overall_count; i++) {
                    Py_INCREF(python_pars[i]);
                }

                result = function->m_c_code(function, python_pars);
            } else {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca(sizeof(PyObject *) * function->m_args_overall_count);
#else
                PyObject *python_pars[function->m_args_overall_count];
#endif
                memset(python_pars, 0, function->m_args_overall_count * sizeof(PyObject *));

                if (parseArgumentsMethodPos(function, python_pars, method->m_object, args, 5)) {
                    result = function->m_c_code(function, python_pars);
                } else {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    } else if (PyCFunction_Check(called)) {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS(called) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if (flags & METH_NOARGS) {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(
                PyExc_TypeError,
                "%s() takes no arguments (5 given)",
                ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_O) {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                return NULL;
            }
#else
            PyErr_Format(PyExc_TypeError,
                "%s() takes exactly one argument (5 given)",
                 ((PyCFunctionObject *)called)->m_ml->ml_name
            );
            return NULL;
#endif
        } else if (flags & METH_VARARGS) {
            PyCFunction method = PyCFunction_GET_FUNCTION(called);
            PyObject *self = PyCFunction_GET_SELF(called);

            PyObject *pos_args = MAKE_TUPLE(args, 5);

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely(Py_EnterRecursiveCall((char *)" while calling a Python object"))) {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if (flags & METH_KEYWORDS) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else {
                result = (*method)(self, pos_args);
            }
#else
            if (flags == (METH_VARARGS|METH_KEYWORDS)) {
                result = (*(PyCFunctionWithKeywords)method)(self, pos_args, NULL);
            } else if (flags == METH_FASTCALL) {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)(self, &PyTuple_GET_ITEM(pos_args, 0), 5, NULL);
#else
                result = (*(_PyCFunctionFast)method)(self, &pos_args, 5);
#endif
            } else {
                result = (*method)(self, pos_args);
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if (result != NULL) {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF(pos_args);
                return result;
            } else {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely(!ERROR_OCCURRED())) {
                    SET_CURRENT_EXCEPTION_TYPE0_STR(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF(pos_args);
                return NULL;
            }
        }
    } else if (PyFunction_Check(called)) {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE(args, 5);

    PyObject *result = CALL_FUNCTION(called, pos_args, NULL);

    Py_DECREF(pos_args);

    return result;
}

PyObject *CALL_METHOD_WITH_ARGS1(PyObject *source, PyObject *attr_name, PyObject **args) {
    CHECK_OBJECT(source);
    CHECK_OBJECT(attr_name);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 1; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    PyTypeObject *type = Py_TYPE(source);

    if (type->tp_getattro == PyObject_GenericGetAttr) {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely(type->tp_dict == NULL)) {
            if (unlikely(PyType_Ready(type) < 0)) {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup(type, attr_name);
        descrgetfunc func = NULL;

        if (descr != NULL)
        {
            Py_INCREF(descr);

#if PYTHON_VERSION < 300
            if (PyType_HasFeature(Py_TYPE(descr), Py_TPFLAGS_HAVE_CLASS)) {
#endif
                func = Py_TYPE(descr)->tp_descr_get;

                if (func != NULL && PyDescr_IsData(descr))
                {
                    PyObject *called_object = func(descr, source, (PyObject *)type);
                    Py_DECREF(descr);

                    PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                        called_object,
                        args
                    );
                    Py_DECREF(called_object);
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if (dict != NULL)
        {
            CHECK_OBJECT(dict);

            Py_INCREF(dict);

            PyObject *called_object = PyDict_GetItem(dict, attr_name);

            if (called_object != NULL)
            {
                Py_INCREF(called_object);
                Py_XDECREF(descr);
                Py_DECREF(dict);

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF(called_object);
                return result;
            }

            Py_DECREF(dict);
        }

        if (func != NULL) {
            if (func == Nuitka_Function_Type.tp_descr_get) {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    1
                );

                Py_DECREF(descr);

                return result;
            } else {
                PyObject *called_object = func(descr, source, (PyObject *)type);
                CHECK_OBJECT(called_object);

                Py_DECREF(descr);

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF(called_object);

                return result;
            }
        }

        if (descr != NULL) {
            CHECK_OBJECT(descr);

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                descr,
                args
            );
            Py_DECREF(descr);

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if (type == &PyInstance_Type) {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert(attr_name != const_str_plain___dict__);
        assert(attr_name != const_str_plain___class__);

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            return CALL_FUNCTION_WITH_ARGS1(called_object, args);
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            descrgetfunc descr_get = Py_TYPE(called_object)->tp_descr_get;

            if (descr_get == Nuitka_Function_Type.tp_descr_get) {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    1
                );
            } else if (descr_get != NULL) {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely(method == NULL)) {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(method, args);
                Py_DECREF(method);
                return result;
            } else {
                return CALL_FUNCTION_WITH_ARGS1(called_object, args);
            }

        } else if (unlikely(source_instance->in_class->cl_getattr == NULL)) {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        } else {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely(called_object == NULL))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                called_object,
                args
            );
            Py_DECREF(called_object);
            return result;
        }
    }
#endif
    else if (type->tp_getattro != NULL) {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely(called_object == NULL)) {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else if (type->tp_getattr != NULL) {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked(attr_name)
        );

        if (unlikely(called_object == NULL))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked(attr_name)
        );

        return NULL;
    }
}

PyObject *CALL_METHOD_WITH_ARGS2(PyObject *source, PyObject *attr_name, PyObject **args) {
    CHECK_OBJECT(source);
    CHECK_OBJECT(attr_name);

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for (size_t i = 0; i < 2; i++) {
        CHECK_OBJECT(args[i]);
    }
#endif

    PyTypeObject *type = Py_TYPE(source);

    if (type->tp_getattro == PyObject_GenericGetAttr) {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely(type->tp_dict == NULL)) {
            if (unlikely(PyType_Ready(type) < 0)) {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup(type, attr_name);
        descrgetfunc func = NULL;

        if (descr != NULL)
        {
            Py_INCREF(descr);

#if PYTHON_VERSION < 300
            if (PyType_HasFeature(Py_TYPE(descr), Py_TPFLAGS_HAVE_CLASS)) {
#endif
                func = Py_TYPE(descr)->tp_descr_get;

                if (func != NULL && PyDescr_IsData(descr))
                {
                    PyObject *called_object = func(descr, source, (PyObject *)type);
                    Py_DECREF(descr);

                    PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                        called_object,
                        args
                    );
                    Py_DECREF(called_object);
                    return result;
                }
#if PYTHON_VERSION < 300
            }
#endif
        }

        Py_ssize_t dictoffset = type->tp_dictoffset;
        PyObject *dict = NULL;

        if ( dictoffset != 0 )
        {
            // Negative dictionary offsets have special meaning.
            if ( dictoffset < 0 )
            {
                Py_ssize_t tsize;
                size_t size;

                tsize = ((PyVarObject *)source)->ob_size;
                if (tsize < 0)
                    tsize = -tsize;
                size = _PyObject_VAR_SIZE( type, tsize );

                dictoffset += (long)size;
            }

            PyObject **dictptr = (PyObject **) ((char *)source + dictoffset);
            dict = *dictptr;
        }

        if (dict != NULL)
        {
            CHECK_OBJECT(dict);

            Py_INCREF(dict);

            PyObject *called_object = PyDict_GetItem(dict, attr_name);

            if (called_object != NULL)
            {
                Py_INCREF(called_object);
                Py_XDECREF(descr);
                Py_DECREF(dict);

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF(called_object);
                return result;
            }

            Py_DECREF(dict);
        }

        if (func != NULL) {
            if (func == Nuitka_Function_Type.tp_descr_get) {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    2
                );

                Py_DECREF(descr);

                return result;
            } else {
                PyObject *called_object = func(descr, source, (PyObject *)type);
                CHECK_OBJECT(called_object);

                Py_DECREF(descr);

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                    called_object,
                    args
                );
                Py_DECREF(called_object);

                return result;
            }
        }

        if (descr != NULL) {
            CHECK_OBJECT(descr);

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                descr,
                args
            );
            Py_DECREF(descr);

            return result;
        }

#if PYTHON_VERSION < 300
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            PyString_AS_STRING( attr_name )
        );
#else
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%U'",
            type->tp_name,
            attr_name
        );
#endif
        return NULL;
    }
#if PYTHON_VERSION < 300
    else if (type == &PyInstance_Type) {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert(attr_name != const_str_plain___dict__);
        assert(attr_name != const_str_plain___class__);

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            return CALL_FUNCTION_WITH_ARGS2(called_object, args);
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if (called_object != NULL) {
            descrgetfunc descr_get = Py_TYPE(called_object)->tp_descr_get;

            if (descr_get == Nuitka_Function_Type.tp_descr_get) {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    2
                );
            } else if (descr_get != NULL) {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely(method == NULL)) {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS2(method, args);
                Py_DECREF(method);
                return result;
            } else {
                return CALL_FUNCTION_WITH_ARGS2(called_object, args);
            }

        } else if (unlikely(source_instance->in_class->cl_getattr == NULL)) {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        } else {
            // Finally allow the "__getattr__" override to provide it or else
            // it's an error.

            PyObject *args2[] = {
                source,
                attr_name
            };

            called_object = CALL_FUNCTION_WITH_ARGS2(
                source_instance->in_class->cl_getattr,
                args2
            );

            if (unlikely(called_object == NULL))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS2(
                called_object,
                args
            );
            Py_DECREF(called_object);
            return result;
        }
    }
#endif
    else if (type->tp_getattro != NULL) {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely(called_object == NULL)) {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else if (type->tp_getattr != NULL) {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked(attr_name)
        );

        if (unlikely(called_object == NULL))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS2(
            called_object,
            args
        );
        Py_DECREF(called_object);
        return result;
    } else {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked(attr_name)
        );

        return NULL;
    }
}
/* Code to register embedded modules for meta path based loading if any. */

#include "nuitka/unfreezing.h"

/* Table for lookup to find compiled or bytecode modules included in this
 * binary or module, or put along this binary as extension modules. We do
 * our own loading for each of these.
 */
extern PyObject *modulecode_TaskFunctions(PyObject *);
extern PyObject *modulecode___main__(PyObject *);
extern PyObject *modulecode_pywin32_bootstrap(PyObject *);
extern PyObject *modulecode_tkinter$$45$preLoad(PyObject *);
extern PyObject *modulecode_zmlogo(PyObject *);
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] = {
    {"TaskFunctions", modulecode_TaskFunctions, 0, 0, },
    {"__main__", modulecode___main__, 0, 0, },
    {"_asyncio", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_decimal", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_lzma", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_msi", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_overlapped", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_queue", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_socket", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"base64", NULL, 17515, 17092, NUITKA_BYTECODE_FLAG},
    {"datetime", NULL, 34607, 57001, NUITKA_BYTECODE_FLAG},
    {"os", NULL, 91608, 31346, NUITKA_BYTECODE_FLAG},
    {"pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"pywin32_bootstrap", modulecode_pywin32_bootstrap, 0, 0, },
    {"re", NULL, 122954, 14331, NUITKA_BYTECODE_FLAG},
    {"select", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"site", NULL, 137285, 13330, NUITKA_BYTECODE_FLAG},
    {"subprocess", NULL, 150615, 41938, NUITKA_BYTECODE_FLAG},
    {"tkinter", NULL, 192553, 176022, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"tkinter-preLoad", modulecode_tkinter$$45$preLoad, 0, 0, },
    {"unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG},
    {"xml", NULL, 368575, 715, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.etree", NULL, 369290, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.etree.ElementTree", NULL, 369444, 55635, NUITKA_BYTECODE_FLAG},
    {"zmlogo", modulecode_zmlogo, 0, 0, },
    {"__future__", NULL, 425079, 4154, NUITKA_BYTECODE_FLAG},
    {"_bootlocale", NULL, 429233, 1266, NUITKA_BYTECODE_FLAG},
    {"_collections_abc", NULL, 430499, 28764, NUITKA_BYTECODE_FLAG},
    {"_compat_pickle", NULL, 459263, 5524, NUITKA_BYTECODE_FLAG},
    {"_compression", NULL, 464787, 4169, NUITKA_BYTECODE_FLAG},
    {"_dummy_thread", NULL, 468956, 6060, NUITKA_BYTECODE_FLAG},
    {"_markupbase", NULL, 475016, 7813, NUITKA_BYTECODE_FLAG},
    {"_osx_support", NULL, 482829, 9656, NUITKA_BYTECODE_FLAG},
    {"_py_abc", NULL, 492485, 4693, NUITKA_BYTECODE_FLAG},
    {"_pyio", NULL, 497178, 74023, NUITKA_BYTECODE_FLAG},
    {"_sitebuiltins", NULL, 571201, 3504, NUITKA_BYTECODE_FLAG},
    {"_strptime", NULL, 574705, 16067, NUITKA_BYTECODE_FLAG},
    {"_threading_local", NULL, 590772, 6469, NUITKA_BYTECODE_FLAG},
    {"_weakrefset", NULL, 597241, 7623, NUITKA_BYTECODE_FLAG},
    {"abc", NULL, 604864, 5357, NUITKA_BYTECODE_FLAG},
    {"aifc", NULL, 610221, 25497, NUITKA_BYTECODE_FLAG},
    {"argparse", NULL, 635718, 62321, NUITKA_BYTECODE_FLAG},
    {"ast", NULL, 698039, 16359, NUITKA_BYTECODE_FLAG},
    {"asynchat", NULL, 714398, 6874, NUITKA_BYTECODE_FLAG},
    {"asyncio", NULL, 721272, 773, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"asyncio.base_events", NULL, 722045, 50642, NUITKA_BYTECODE_FLAG},
    {"asyncio.base_futures", NULL, 772687, 1746, NUITKA_BYTECODE_FLAG},
    {"asyncio.base_subprocess", NULL, 774433, 9439, NUITKA_BYTECODE_FLAG},
    {"asyncio.base_tasks", NULL, 783872, 1915, NUITKA_BYTECODE_FLAG},
    {"asyncio.constants", NULL, 785787, 604, NUITKA_BYTECODE_FLAG},
    {"asyncio.coroutines", NULL, 786391, 6677, NUITKA_BYTECODE_FLAG},
    {"asyncio.events", NULL, 793068, 28080, NUITKA_BYTECODE_FLAG},
    {"asyncio.exceptions", NULL, 821148, 2521, NUITKA_BYTECODE_FLAG},
    {"asyncio.format_helpers", NULL, 823669, 2355, NUITKA_BYTECODE_FLAG},
    {"asyncio.futures", NULL, 826024, 11149, NUITKA_BYTECODE_FLAG},
    {"asyncio.locks", NULL, 837173, 16391, NUITKA_BYTECODE_FLAG},
    {"asyncio.log", NULL, 853564, 249, NUITKA_BYTECODE_FLAG},
    {"asyncio.proactor_events", NULL, 853813, 24099, NUITKA_BYTECODE_FLAG},
    {"asyncio.protocols", NULL, 877912, 8637, NUITKA_BYTECODE_FLAG},
    {"asyncio.queues", NULL, 886549, 8404, NUITKA_BYTECODE_FLAG},
    {"asyncio.runners", NULL, 894953, 1961, NUITKA_BYTECODE_FLAG},
    {"asyncio.selector_events", NULL, 896914, 29424, NUITKA_BYTECODE_FLAG},
    {"asyncio.sslproto", NULL, 926338, 21641, NUITKA_BYTECODE_FLAG},
    {"asyncio.staggered", NULL, 947979, 4129, NUITKA_BYTECODE_FLAG},
    {"asyncio.streams", NULL, 952108, 21051, NUITKA_BYTECODE_FLAG},
    {"asyncio.subprocess", NULL, 973159, 7381, NUITKA_BYTECODE_FLAG},
    {"asyncio.tasks", NULL, 980540, 23769, NUITKA_BYTECODE_FLAG},
    {"asyncio.transports", NULL, 1004309, 12282, NUITKA_BYTECODE_FLAG},
    {"asyncio.trsock", NULL, 1016591, 8506, NUITKA_BYTECODE_FLAG},
    {"asyncio.windows_events", NULL, 1025097, 24606, NUITKA_BYTECODE_FLAG},
    {"asyncio.windows_utils", NULL, 1049703, 4494, NUITKA_BYTECODE_FLAG},
    {"asyncore", NULL, 1054197, 16051, NUITKA_BYTECODE_FLAG},
    {"base64", NULL, 17515, 17092, NUITKA_BYTECODE_FLAG},
    {"bdb", NULL, 1070248, 25017, NUITKA_BYTECODE_FLAG},
    {"binhex", NULL, 1095265, 12158, NUITKA_BYTECODE_FLAG},
    {"bisect", NULL, 1107423, 2377, NUITKA_BYTECODE_FLAG},
    {"bz2", NULL, 1109800, 11468, NUITKA_BYTECODE_FLAG},
    {"cProfile", NULL, 1121268, 5357, NUITKA_BYTECODE_FLAG},
    {"calendar", NULL, 1126625, 27087, NUITKA_BYTECODE_FLAG},
    {"cgi", NULL, 1153712, 26275, NUITKA_BYTECODE_FLAG},
    {"cgitb", NULL, 1179987, 10173, NUITKA_BYTECODE_FLAG},
    {"chunk", NULL, 1190160, 4862, NUITKA_BYTECODE_FLAG},
    {"cmd", NULL, 1195022, 12649, NUITKA_BYTECODE_FLAG},
    {"code", NULL, 1207671, 9933, NUITKA_BYTECODE_FLAG},
    {"codecs", NULL, 1217604, 33936, NUITKA_BYTECODE_FLAG},
    {"codeop", NULL, 1251540, 6296, NUITKA_BYTECODE_FLAG},
    {"collections", NULL, 1257836, 46458, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"collections.abc", NULL, 430499, 28764, NUITKA_BYTECODE_FLAG},
    {"colorsys", NULL, 1304294, 3263, NUITKA_BYTECODE_FLAG},
    {"compileall", NULL, 1307557, 9199, NUITKA_BYTECODE_FLAG},
    {"concurrent", NULL, 1316756, 155, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"concurrent.futures", NULL, 1316911, 1131, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"concurrent.futures._base", NULL, 1318042, 21923, NUITKA_BYTECODE_FLAG},
    {"concurrent.futures.process", NULL, 1339965, 20323, NUITKA_BYTECODE_FLAG},
    {"concurrent.futures.thread", NULL, 1360288, 5841, NUITKA_BYTECODE_FLAG},
    {"configparser", NULL, 1366129, 45741, NUITKA_BYTECODE_FLAG},
    {"contextlib", NULL, 1411870, 20252, NUITKA_BYTECODE_FLAG},
    {"contextvars", NULL, 1432122, 266, NUITKA_BYTECODE_FLAG},
    {"copy", NULL, 1432388, 6986, NUITKA_BYTECODE_FLAG},
    {"copyreg", NULL, 1439374, 4341, NUITKA_BYTECODE_FLAG},
    {"csv", NULL, 1443715, 11933, NUITKA_BYTECODE_FLAG},
    {"ctypes", NULL, 1455648, 16250, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"ctypes._aix", NULL, 1471898, 9867, NUITKA_BYTECODE_FLAG},
    {"ctypes._endian", NULL, 1481765, 1952, NUITKA_BYTECODE_FLAG},
    {"ctypes.macholib", NULL, 1483717, 322, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"ctypes.macholib.dyld", NULL, 1484039, 4382, NUITKA_BYTECODE_FLAG},
    {"ctypes.macholib.dylib", NULL, 1488421, 1939, NUITKA_BYTECODE_FLAG},
    {"ctypes.macholib.framework", NULL, 1490360, 2219, NUITKA_BYTECODE_FLAG},
    {"ctypes.util", NULL, 1492579, 7784, NUITKA_BYTECODE_FLAG},
    {"ctypes.wintypes", NULL, 1500363, 5126, NUITKA_BYTECODE_FLAG},
    {"dataclasses", NULL, 1505489, 23347, NUITKA_BYTECODE_FLAG},
    {"datetime", NULL, 34607, 57001, NUITKA_BYTECODE_FLAG},
    {"dbm", NULL, 1528836, 4215, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"dbm.dumb", NULL, 1533051, 7796, NUITKA_BYTECODE_FLAG},
    {"decimal", NULL, 1540847, 160726, NUITKA_BYTECODE_FLAG},
    {"difflib", NULL, 1701573, 59455, NUITKA_BYTECODE_FLAG},
    {"dis", NULL, 1761028, 15825, NUITKA_BYTECODE_FLAG},
    {"distutils", NULL, 1776853, 407, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"distutils._msvccompiler", NULL, 1777260, 14003, NUITKA_BYTECODE_FLAG},
    {"distutils.archive_util", NULL, 1791263, 6562, NUITKA_BYTECODE_FLAG},
    {"distutils.bcppcompiler", NULL, 1797825, 6546, NUITKA_BYTECODE_FLAG},
    {"distutils.ccompiler", NULL, 1804371, 33320, NUITKA_BYTECODE_FLAG},
    {"distutils.cmd", NULL, 1837691, 13971, NUITKA_BYTECODE_FLAG},
    {"distutils.command", NULL, 1851662, 566, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"distutils.command.bdist", NULL, 1852228, 3689, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_dumb", NULL, 1855917, 3615, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_msi", NULL, 1859532, 19646, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_rpm", NULL, 1879178, 12451, NUITKA_BYTECODE_FLAG},
    {"distutils.command.bdist_wininst", NULL, 1891629, 8512, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build", NULL, 1900141, 3904, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_clib", NULL, 1904045, 4837, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_ext", NULL, 1908882, 15992, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_py", NULL, 1924874, 10485, NUITKA_BYTECODE_FLAG},
    {"distutils.command.build_scripts", NULL, 1935359, 4347, NUITKA_BYTECODE_FLAG},
    {"distutils.command.check", NULL, 1939706, 4908, NUITKA_BYTECODE_FLAG},
    {"distutils.command.clean", NULL, 1944614, 2123, NUITKA_BYTECODE_FLAG},
    {"distutils.command.config", NULL, 1946737, 10250, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install", NULL, 1956987, 13595, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_data", NULL, 1970582, 2312, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_egg_info", NULL, 1972894, 3019, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_headers", NULL, 1975913, 1731, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_lib", NULL, 1977644, 5136, NUITKA_BYTECODE_FLAG},
    {"distutils.command.install_scripts", NULL, 1982780, 2164, NUITKA_BYTECODE_FLAG},
    {"distutils.command.register", NULL, 1984944, 8478, NUITKA_BYTECODE_FLAG},
    {"distutils.command.sdist", NULL, 1993422, 14539, NUITKA_BYTECODE_FLAG},
    {"distutils.command.upload", NULL, 2007961, 4962, NUITKA_BYTECODE_FLAG},
    {"distutils.config", NULL, 2012923, 3532, NUITKA_BYTECODE_FLAG},
    {"distutils.core", NULL, 2016455, 6637, NUITKA_BYTECODE_FLAG},
    {"distutils.cygwinccompiler", NULL, 2023092, 8635, NUITKA_BYTECODE_FLAG},
    {"distutils.debug", NULL, 2031727, 217, NUITKA_BYTECODE_FLAG},
    {"distutils.dep_util", NULL, 2031944, 2737, NUITKA_BYTECODE_FLAG},
    {"distutils.dir_util", NULL, 2034681, 5846, NUITKA_BYTECODE_FLAG},
    {"distutils.dist", NULL, 2040527, 34515, NUITKA_BYTECODE_FLAG},
    {"distutils.errors", NULL, 2075042, 5273, NUITKA_BYTECODE_FLAG},
    {"distutils.extension", NULL, 2080315, 6946, NUITKA_BYTECODE_FLAG},
    {"distutils.fancy_getopt", NULL, 2087261, 10679, NUITKA_BYTECODE_FLAG},
    {"distutils.file_util", NULL, 2097940, 5956, NUITKA_BYTECODE_FLAG},
    {"distutils.filelist", NULL, 2103896, 9890, NUITKA_BYTECODE_FLAG},
    {"distutils.log", NULL, 2113786, 2338, NUITKA_BYTECODE_FLAG},
    {"distutils.msvc9compiler", NULL, 2116124, 17533, NUITKA_BYTECODE_FLAG},
    {"distutils.msvccompiler", NULL, 2133657, 14746, NUITKA_BYTECODE_FLAG},
    {"distutils.spawn", NULL, 2148403, 5129, NUITKA_BYTECODE_FLAG},
    {"distutils.sysconfig", NULL, 2153532, 12124, NUITKA_BYTECODE_FLAG},
    {"distutils.text_file", NULL, 2165656, 8462, NUITKA_BYTECODE_FLAG},
    {"distutils.unixccompiler", NULL, 2174118, 6641, NUITKA_BYTECODE_FLAG},
    {"distutils.util", NULL, 2180759, 15569, NUITKA_BYTECODE_FLAG},
    {"distutils.version", NULL, 2196328, 7334, NUITKA_BYTECODE_FLAG},
    {"distutils.versionpredicate", NULL, 2203662, 5158, NUITKA_BYTECODE_FLAG},
    {"doctest", NULL, 2208820, 75853, NUITKA_BYTECODE_FLAG},
    {"dummy_threading", NULL, 2284673, 1133, NUITKA_BYTECODE_FLAG},
    {"email", NULL, 2285806, 1714, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"email._encoded_words", NULL, 2287520, 5709, NUITKA_BYTECODE_FLAG},
    {"email._header_value_parser", NULL, 2293229, 79558, NUITKA_BYTECODE_FLAG},
    {"email._parseaddr", NULL, 2372787, 12477, NUITKA_BYTECODE_FLAG},
    {"email._policybase", NULL, 2385264, 14833, NUITKA_BYTECODE_FLAG},
    {"email.base64mime", NULL, 2400097, 3258, NUITKA_BYTECODE_FLAG},
    {"email.charset", NULL, 2403355, 11482, NUITKA_BYTECODE_FLAG},
    {"email.contentmanager", NULL, 2414837, 7366, NUITKA_BYTECODE_FLAG},
    {"email.encoders", NULL, 2422203, 1635, NUITKA_BYTECODE_FLAG},
    {"email.errors", NULL, 2423838, 5928, NUITKA_BYTECODE_FLAG},
    {"email.feedparser", NULL, 2429766, 10665, NUITKA_BYTECODE_FLAG},
    {"email.generator", NULL, 2440431, 12505, NUITKA_BYTECODE_FLAG},
    {"email.header", NULL, 2452936, 16462, NUITKA_BYTECODE_FLAG},
    {"email.headerregistry", NULL, 2469398, 21923, NUITKA_BYTECODE_FLAG},
    {"email.iterators", NULL, 2491321, 1943, NUITKA_BYTECODE_FLAG},
    {"email.message", NULL, 2493264, 37901, NUITKA_BYTECODE_FLAG},
    {"email.mime", NULL, 2531165, 155, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"email.mime.application", NULL, 2531320, 1482, NUITKA_BYTECODE_FLAG},
    {"email.mime.audio", NULL, 2532802, 2647, NUITKA_BYTECODE_FLAG},
    {"email.mime.base", NULL, 2535449, 1064, NUITKA_BYTECODE_FLAG},
    {"email.mime.image", NULL, 2536513, 1927, NUITKA_BYTECODE_FLAG},
    {"email.mime.message", NULL, 2538440, 1305, NUITKA_BYTECODE_FLAG},
    {"email.mime.multipart", NULL, 2539745, 1525, NUITKA_BYTECODE_FLAG},
    {"email.mime.nonmultipart", NULL, 2541270, 787, NUITKA_BYTECODE_FLAG},
    {"email.mime.text", NULL, 2542057, 1334, NUITKA_BYTECODE_FLAG},
    {"email.parser", NULL, 2543391, 5745, NUITKA_BYTECODE_FLAG},
    {"email.policy", NULL, 2549136, 9681, NUITKA_BYTECODE_FLAG},
    {"email.quoprimime", NULL, 2558817, 7701, NUITKA_BYTECODE_FLAG},
    {"email.utils", NULL, 2566518, 9570, NUITKA_BYTECODE_FLAG},
    {"encodings", NULL, 2576088, 3926, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"encodings.aliases", NULL, 2580014, 6353, NUITKA_BYTECODE_FLAG},
    {"encodings.ascii", NULL, 2586367, 1904, NUITKA_BYTECODE_FLAG},
    {"encodings.base64_codec", NULL, 2588271, 2422, NUITKA_BYTECODE_FLAG},
    {"encodings.big5", NULL, 2590693, 1432, NUITKA_BYTECODE_FLAG},
    {"encodings.big5hkscs", NULL, 2592125, 1442, NUITKA_BYTECODE_FLAG},
    {"encodings.bz2_codec", NULL, 2593567, 3313, NUITKA_BYTECODE_FLAG},
    {"encodings.charmap", NULL, 2596880, 2914, NUITKA_BYTECODE_FLAG},
    {"encodings.cp037", NULL, 2599794, 2445, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1006", NULL, 2602239, 2521, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1026", NULL, 2604760, 2449, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1125", NULL, 2607209, 8152, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1140", NULL, 2615361, 2435, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1250", NULL, 2617796, 2472, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1251", NULL, 2620268, 2469, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1252", NULL, 2622737, 2472, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1253", NULL, 2625209, 2485, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1254", NULL, 2627694, 2474, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1255", NULL, 2630168, 2493, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1256", NULL, 2632661, 2471, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1257", NULL, 2635132, 2479, NUITKA_BYTECODE_FLAG},
    {"encodings.cp1258", NULL, 2637611, 2477, NUITKA_BYTECODE_FLAG},
    {"encodings.cp273", NULL, 2640088, 2431, NUITKA_BYTECODE_FLAG},
    {"encodings.cp424", NULL, 2642519, 2475, NUITKA_BYTECODE_FLAG},
    {"encodings.cp437", NULL, 2644994, 7869, NUITKA_BYTECODE_FLAG},
    {"encodings.cp500", NULL, 2652863, 2445, NUITKA_BYTECODE_FLAG},
    {"encodings.cp720", NULL, 2655308, 2542, NUITKA_BYTECODE_FLAG},
    {"encodings.cp737", NULL, 2657850, 8191, NUITKA_BYTECODE_FLAG},
    {"encodings.cp775", NULL, 2666041, 7899, NUITKA_BYTECODE_FLAG},
    {"encodings.cp850", NULL, 2673940, 7530, NUITKA_BYTECODE_FLAG},
    {"encodings.cp852", NULL, 2681470, 7907, NUITKA_BYTECODE_FLAG},
    {"encodings.cp855", NULL, 2689377, 8160, NUITKA_BYTECODE_FLAG},
    {"encodings.cp856", NULL, 2697537, 2507, NUITKA_BYTECODE_FLAG},
    {"encodings.cp857", NULL, 2700044, 7510, NUITKA_BYTECODE_FLAG},
    {"encodings.cp858", NULL, 2707554, 7500, NUITKA_BYTECODE_FLAG},
    {"encodings.cp860", NULL, 2715054, 7848, NUITKA_BYTECODE_FLAG},
    {"encodings.cp861", NULL, 2722902, 7863, NUITKA_BYTECODE_FLAG},
    {"encodings.cp862", NULL, 2730765, 8052, NUITKA_BYTECODE_FLAG},
    {"encodings.cp863", NULL, 2738817, 7863, NUITKA_BYTECODE_FLAG},
    {"encodings.cp864", NULL, 2746680, 8007, NUITKA_BYTECODE_FLAG},
    {"encodings.cp865", NULL, 2754687, 7863, NUITKA_BYTECODE_FLAG},
    {"encodings.cp866", NULL, 2762550, 8196, NUITKA_BYTECODE_FLAG},
    {"encodings.cp869", NULL, 2770746, 7887, NUITKA_BYTECODE_FLAG},
    {"encodings.cp874", NULL, 2778633, 2573, NUITKA_BYTECODE_FLAG},
    {"encodings.cp875", NULL, 2781206, 2442, NUITKA_BYTECODE_FLAG},
    {"encodings.cp932", NULL, 2783648, 1434, NUITKA_BYTECODE_FLAG},
    {"encodings.cp949", NULL, 2785082, 1434, NUITKA_BYTECODE_FLAG},
    {"encodings.cp950", NULL, 2786516, 1434, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_jis_2004", NULL, 2787950, 1448, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_jisx0213", NULL, 2789398, 1448, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_jp", NULL, 2790846, 1436, NUITKA_BYTECODE_FLAG},
    {"encodings.euc_kr", NULL, 2792282, 1436, NUITKA_BYTECODE_FLAG},
    {"encodings.gb18030", NULL, 2793718, 1438, NUITKA_BYTECODE_FLAG},
    {"encodings.gb2312", NULL, 2795156, 1436, NUITKA_BYTECODE_FLAG},
    {"encodings.gbk", NULL, 2796592, 1430, NUITKA_BYTECODE_FLAG},
    {"encodings.hex_codec", NULL, 2798022, 2409, NUITKA_BYTECODE_FLAG},
    {"encodings.hp_roman8", NULL, 2800431, 2646, NUITKA_BYTECODE_FLAG},
    {"encodings.hz", NULL, 2803077, 1428, NUITKA_BYTECODE_FLAG},
    {"encodings.idna", NULL, 2804505, 5640, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp", NULL, 2810145, 1449, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_1", NULL, 2811594, 1453, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_2", NULL, 2813047, 1453, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_2004", NULL, 2814500, 1459, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_3", NULL, 2815959, 1453, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_jp_ext", NULL, 2817412, 1457, NUITKA_BYTECODE_FLAG},
    {"encodings.iso2022_kr", NULL, 2818869, 1449, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_1", NULL, 2820318, 2444, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_10", NULL, 2822762, 2449, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_11", NULL, 2825211, 2543, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_13", NULL, 2827754, 2452, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_14", NULL, 2830206, 2470, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_15", NULL, 2832676, 2449, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_16", NULL, 2835125, 2451, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_2", NULL, 2837576, 2444, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_3", NULL, 2840020, 2451, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_4", NULL, 2842471, 2444, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_5", NULL, 2844915, 2445, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_6", NULL, 2847360, 2489, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_7", NULL, 2849849, 2452, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_8", NULL, 2852301, 2483, NUITKA_BYTECODE_FLAG},
    {"encodings.iso8859_9", NULL, 2854784, 2444, NUITKA_BYTECODE_FLAG},
    {"encodings.johab", NULL, 2857228, 1434, NUITKA_BYTECODE_FLAG},
    {"encodings.koi8_r", NULL, 2858662, 2496, NUITKA_BYTECODE_FLAG},
    {"encodings.koi8_t", NULL, 2861158, 2407, NUITKA_BYTECODE_FLAG},
    {"encodings.koi8_u", NULL, 2863565, 2482, NUITKA_BYTECODE_FLAG},
    {"encodings.kz1048", NULL, 2866047, 2459, NUITKA_BYTECODE_FLAG},
    {"encodings.latin_1", NULL, 2868506, 1916, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_arabic", NULL, 2870422, 7763, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_centeuro", NULL, 2878185, 2483, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_croatian", NULL, 2880668, 2491, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_cyrillic", NULL, 2883159, 2481, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_farsi", NULL, 2885640, 2425, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_greek", NULL, 2888065, 2465, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_iceland", NULL, 2890530, 2484, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_latin2", NULL, 2893014, 2625, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_roman", NULL, 2895639, 2482, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_romanian", NULL, 2898121, 2492, NUITKA_BYTECODE_FLAG},
    {"encodings.mac_turkish", NULL, 2900613, 2485, NUITKA_BYTECODE_FLAG},
    {"encodings.mbcs", NULL, 2903098, 1734, NUITKA_BYTECODE_FLAG},
    {"encodings.oem", NULL, 2904832, 1547, NUITKA_BYTECODE_FLAG},
    {"encodings.palmos", NULL, 2906379, 2472, NUITKA_BYTECODE_FLAG},
    {"encodings.ptcp154", NULL, 2908851, 2566, NUITKA_BYTECODE_FLAG},
    {"encodings.punycode", NULL, 2911417, 6334, NUITKA_BYTECODE_FLAG},
    {"encodings.quopri_codec", NULL, 2917751, 2438, NUITKA_BYTECODE_FLAG},
    {"encodings.raw_unicode_escape", NULL, 2920189, 1785, NUITKA_BYTECODE_FLAG},
    {"encodings.rot_13", NULL, 2921974, 3024, NUITKA_BYTECODE_FLAG},
    {"encodings.shift_jis", NULL, 2924998, 1442, NUITKA_BYTECODE_FLAG},
    {"encodings.shift_jis_2004", NULL, 2926440, 1452, NUITKA_BYTECODE_FLAG},
    {"encodings.shift_jisx0213", NULL, 2927892, 1452, NUITKA_BYTECODE_FLAG},
    {"encodings.tis_620", NULL, 2929344, 2534, NUITKA_BYTECODE_FLAG},
    {"encodings.undefined", NULL, 2931878, 2118, NUITKA_BYTECODE_FLAG},
    {"encodings.unicode_escape", NULL, 2933996, 1765, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_16", NULL, 2935761, 4895, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_16_be", NULL, 2940656, 1673, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_16_le", NULL, 2942329, 1673, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_32", NULL, 2944002, 4788, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_32_be", NULL, 2948790, 1566, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_32_le", NULL, 2950356, 1566, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_7", NULL, 2951922, 1594, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_8", NULL, 2953516, 1653, NUITKA_BYTECODE_FLAG},
    {"encodings.utf_8_sig", NULL, 2955169, 4569, NUITKA_BYTECODE_FLAG},
    {"encodings.uu_codec", NULL, 2959738, 3221, NUITKA_BYTECODE_FLAG},
    {"encodings.zlib_codec", NULL, 2962959, 3132, NUITKA_BYTECODE_FLAG},
    {"enum", NULL, 2966091, 24422, NUITKA_BYTECODE_FLAG},
    {"filecmp", NULL, 2990513, 8450, NUITKA_BYTECODE_FLAG},
    {"fileinput", NULL, 2998963, 13396, NUITKA_BYTECODE_FLAG},
    {"fnmatch", NULL, 3012359, 3355, NUITKA_BYTECODE_FLAG},
    {"formatter", NULL, 3015714, 17568, NUITKA_BYTECODE_FLAG},
    {"fractions", NULL, 3033282, 18759, NUITKA_BYTECODE_FLAG},
    {"ftplib", NULL, 3052041, 27872, NUITKA_BYTECODE_FLAG},
    {"functools", NULL, 3079913, 27920, NUITKA_BYTECODE_FLAG},
    {"genericpath", NULL, 3107833, 4009, NUITKA_BYTECODE_FLAG},
    {"getopt", NULL, 3111842, 6294, NUITKA_BYTECODE_FLAG},
    {"getpass", NULL, 3118136, 4201, NUITKA_BYTECODE_FLAG},
    {"gettext", NULL, 3122337, 17906, NUITKA_BYTECODE_FLAG},
    {"glob", NULL, 3140243, 4366, NUITKA_BYTECODE_FLAG},
    {"gzip", NULL, 3144609, 18142, NUITKA_BYTECODE_FLAG},
    {"hashlib", NULL, 3162751, 6750, NUITKA_BYTECODE_FLAG},
    {"heapq", NULL, 3169501, 14093, NUITKA_BYTECODE_FLAG},
    {"hmac", NULL, 3183594, 5890, NUITKA_BYTECODE_FLAG},
    {"html", NULL, 3189484, 3641, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"html.entities", NULL, 3193125, 50552, NUITKA_BYTECODE_FLAG},
    {"html.parser", NULL, 3243677, 11221, NUITKA_BYTECODE_FLAG},
    {"http", NULL, 3254898, 6087, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"http.client", NULL, 3260985, 34042, NUITKA_BYTECODE_FLAG},
    {"http.cookiejar", NULL, 3295027, 53617, NUITKA_BYTECODE_FLAG},
    {"http.cookies", NULL, 3348644, 15291, NUITKA_BYTECODE_FLAG},
    {"http.server", NULL, 3363935, 33959, NUITKA_BYTECODE_FLAG},
    {"imaplib", NULL, 3397894, 41365, NUITKA_BYTECODE_FLAG},
    {"imghdr", NULL, 3439259, 4127, NUITKA_BYTECODE_FLAG},
    {"imp", NULL, 3443386, 9832, NUITKA_BYTECODE_FLAG},
    {"importlib", NULL, 3453218, 3781, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"importlib._bootstrap", NULL, 3456999, 28628, NUITKA_BYTECODE_FLAG},
    {"importlib._bootstrap_external", NULL, 3485627, 43733, NUITKA_BYTECODE_FLAG},
    {"importlib.abc", NULL, 3529360, 13596, NUITKA_BYTECODE_FLAG},
    {"importlib.machinery", NULL, 3542956, 985, NUITKA_BYTECODE_FLAG},
    {"importlib.metadata", NULL, 3543941, 19177, NUITKA_BYTECODE_FLAG},
    {"importlib.resources", NULL, 3563118, 6503, NUITKA_BYTECODE_FLAG},
    {"importlib.util", NULL, 3569621, 9315, NUITKA_BYTECODE_FLAG},
    {"inspect", NULL, 3578936, 80394, NUITKA_BYTECODE_FLAG},
    {"io", NULL, 3659330, 3477, NUITKA_BYTECODE_FLAG},
    {"ipaddress", NULL, 3662807, 59582, NUITKA_BYTECODE_FLAG},
    {"json", NULL, 3722389, 12631, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"json.decoder", NULL, 3735020, 9867, NUITKA_BYTECODE_FLAG},
    {"json.encoder", NULL, 3744887, 11194, NUITKA_BYTECODE_FLAG},
    {"json.scanner", NULL, 3756081, 1974, NUITKA_BYTECODE_FLAG},
    {"json.tool", NULL, 3758055, 1795, NUITKA_BYTECODE_FLAG},
    {"keyword", NULL, 3759850, 1021, NUITKA_BYTECODE_FLAG},
    {"lib2to3", NULL, 3760871, 152, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"lib2to3.btm_matcher", NULL, 3761023, 4900, NUITKA_BYTECODE_FLAG},
    {"lib2to3.btm_utils", NULL, 3765923, 6173, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixer_base", NULL, 3772096, 6276, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixer_util", NULL, 3778372, 12221, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes", NULL, 3790593, 158, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"lib2to3.fixes.fix_apply", NULL, 3790751, 1714, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_asserts", NULL, 3792465, 1295, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_basestring", NULL, 3793760, 679, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_buffer", NULL, 3794439, 824, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_dict", NULL, 3795263, 3351, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_except", NULL, 3798614, 2834, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_exec", NULL, 3801448, 1165, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_execfile", NULL, 3802613, 1713, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_exitfunc", NULL, 3804326, 2322, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_filter", NULL, 3806648, 2377, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_funcattrs", NULL, 3809025, 992, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_future", NULL, 3810017, 800, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_getcwdu", NULL, 3810817, 804, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_has_key", NULL, 3811621, 2955, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_idioms", NULL, 3814576, 3937, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_import", NULL, 3818513, 2803, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_imports", NULL, 3821316, 4411, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_imports2", NULL, 3825727, 562, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_input", NULL, 3826289, 966, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_intern", NULL, 3827255, 1175, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_isinstance", NULL, 3828430, 1567, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_itertools", NULL, 3829997, 1566, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_itertools_imports", NULL, 3831563, 1594, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_long", NULL, 3833157, 721, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_map", NULL, 3833878, 3113, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_metaclass", NULL, 3836991, 5363, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_methodattrs", NULL, 3842354, 954, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_ne", NULL, 3843308, 827, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_next", NULL, 3844135, 3099, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_nonzero", NULL, 3847234, 939, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_numliterals", NULL, 3848173, 1039, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_operator", NULL, 3849212, 4236, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_paren", NULL, 3853448, 1406, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_print", NULL, 3854854, 2359, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_raise", NULL, 3857213, 2269, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_raw_input", NULL, 3859482, 811, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_reduce", NULL, 3860293, 1144, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_reload", NULL, 3861437, 1187, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_renames", NULL, 3862624, 2023, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_repr", NULL, 3864647, 861, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_set_literal", NULL, 3865508, 1699, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_standarderror", NULL, 3867207, 736, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_sys_exc", NULL, 3867943, 1427, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_throw", NULL, 3869370, 1824, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_tuple_params", NULL, 3871194, 4607, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_types", NULL, 3875801, 1853, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_unicode", NULL, 3877654, 1565, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_urllib", NULL, 3879219, 6015, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_ws_comma", NULL, 3885234, 1135, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_xrange", NULL, 3886369, 2545, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_xreadlines", NULL, 3888914, 1139, NUITKA_BYTECODE_FLAG},
    {"lib2to3.fixes.fix_zip", NULL, 3890053, 1599, NUITKA_BYTECODE_FLAG},
    {"lib2to3.main", NULL, 3891652, 8631, NUITKA_BYTECODE_FLAG},
    {"lib2to3.patcomp", NULL, 3900283, 5662, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2", NULL, 3905945, 188, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"lib2to3.pgen2.driver", NULL, 3906133, 5164, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.grammar", NULL, 3911297, 5674, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.literals", NULL, 3916971, 1577, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.parse", NULL, 3918548, 6521, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.pgen", NULL, 3925069, 9799, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.token", NULL, 3934868, 1887, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pgen2.tokenize", NULL, 3936755, 15289, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pygram", NULL, 3952044, 1283, NUITKA_BYTECODE_FLAG},
    {"lib2to3.pytree", NULL, 3953327, 24365, NUITKA_BYTECODE_FLAG},
    {"lib2to3.refactor", NULL, 3977692, 20463, NUITKA_BYTECODE_FLAG},
    {"linecache", NULL, 3998155, 3862, NUITKA_BYTECODE_FLAG},
    {"locale", NULL, 4002017, 34712, NUITKA_BYTECODE_FLAG},
    {"logging", NULL, 4036729, 64882, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"logging.config", NULL, 4101611, 23219, NUITKA_BYTECODE_FLAG},
    {"logging.handlers", NULL, 4124830, 43179, NUITKA_BYTECODE_FLAG},
    {"lzma", NULL, 4168009, 12041, NUITKA_BYTECODE_FLAG},
    {"mailbox", NULL, 4180050, 60287, NUITKA_BYTECODE_FLAG},
    {"mailcap", NULL, 4240337, 6499, NUITKA_BYTECODE_FLAG},
    {"mimetypes", NULL, 4246836, 15891, NUITKA_BYTECODE_FLAG},
    {"modulefinder", NULL, 4262727, 16135, NUITKA_BYTECODE_FLAG},
    {"msilib", NULL, 4278862, 16032, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"msilib.schema", NULL, 4294894, 52193, NUITKA_BYTECODE_FLAG},
    {"msilib.sequence", NULL, 4347087, 2623, NUITKA_BYTECODE_FLAG},
    {"msilib.text", NULL, 4349710, 8485, NUITKA_BYTECODE_FLAG},
    {"multiprocessing", NULL, 4358195, 685, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"multiprocessing.connection", NULL, 4358880, 25197, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.context", NULL, 4384077, 12993, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.dummy", NULL, 4397070, 3926, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"multiprocessing.dummy.connection", NULL, 4400996, 2571, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.forkserver", NULL, 4403567, 8316, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.heap", NULL, 4411883, 7660, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.managers", NULL, 4419543, 41512, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.pool", NULL, 4461055, 25073, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.popen_spawn_win32", NULL, 4486128, 3484, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.process", NULL, 4489612, 10949, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.queues", NULL, 4500561, 9577, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.reduction", NULL, 4510138, 8226, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.resource_sharer", NULL, 4518364, 5279, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.resource_tracker", NULL, 4523643, 4966, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.shared_memory", NULL, 4528609, 14314, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.sharedctypes", NULL, 4542923, 7072, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.spawn", NULL, 4549995, 6737, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.synchronize", NULL, 4556732, 11313, NUITKA_BYTECODE_FLAG},
    {"multiprocessing.util", NULL, 4568045, 10531, NUITKA_BYTECODE_FLAG},
    {"netrc", NULL, 4578576, 3800, NUITKA_BYTECODE_FLAG},
    {"nntplib", NULL, 4582376, 33998, NUITKA_BYTECODE_FLAG},
    {"ntpath", NULL, 4616374, 14308, NUITKA_BYTECODE_FLAG},
    {"nturl2path", NULL, 4630682, 1624, NUITKA_BYTECODE_FLAG},
    {"numbers", NULL, 4632306, 12225, NUITKA_BYTECODE_FLAG},
    {"opcode", NULL, 4644531, 5443, NUITKA_BYTECODE_FLAG},
    {"operator", NULL, 4649974, 13714, NUITKA_BYTECODE_FLAG},
    {"optparse", NULL, 4663688, 48080, NUITKA_BYTECODE_FLAG},
    {"os", NULL, 91608, 31346, NUITKA_BYTECODE_FLAG},
    {"pathlib", NULL, 4711768, 42898, NUITKA_BYTECODE_FLAG},
    {"pdb", NULL, 4754666, 47156, NUITKA_BYTECODE_FLAG},
    {"pickle", NULL, 4801822, 46898, NUITKA_BYTECODE_FLAG},
    {"pickletools", NULL, 4848720, 67227, NUITKA_BYTECODE_FLAG},
    {"pipes", NULL, 4915947, 7818, NUITKA_BYTECODE_FLAG},
    {"pkgutil", NULL, 4923765, 16332, NUITKA_BYTECODE_FLAG},
    {"platform", NULL, 4940097, 24244, NUITKA_BYTECODE_FLAG},
    {"plistlib", NULL, 4964341, 26676, NUITKA_BYTECODE_FLAG},
    {"poplib", NULL, 4991017, 13482, NUITKA_BYTECODE_FLAG},
    {"posixpath", NULL, 5004499, 10451, NUITKA_BYTECODE_FLAG},
    {"pprint", NULL, 5014950, 16304, NUITKA_BYTECODE_FLAG},
    {"profile", NULL, 5031254, 14637, NUITKA_BYTECODE_FLAG},
    {"pstats", NULL, 5045891, 22076, NUITKA_BYTECODE_FLAG},
    {"py_compile", NULL, 5067967, 7417, NUITKA_BYTECODE_FLAG},
    {"pyclbr", NULL, 5075384, 10474, NUITKA_BYTECODE_FLAG},
    {"pydoc", NULL, 5085858, 84140, NUITKA_BYTECODE_FLAG},
    {"pydoc_data", NULL, 5169998, 155, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"pydoc_data.topics", NULL, 5170153, 416348, NUITKA_BYTECODE_FLAG},
    {"queue", NULL, 5586501, 10649, NUITKA_BYTECODE_FLAG},
    {"quopri", NULL, 5597150, 5771, NUITKA_BYTECODE_FLAG},
    {"random", NULL, 5602921, 20131, NUITKA_BYTECODE_FLAG},
    {"re", NULL, 122954, 14331, NUITKA_BYTECODE_FLAG},
    {"reprlib", NULL, 5623052, 5326, NUITKA_BYTECODE_FLAG},
    {"rlcompleter", NULL, 5628378, 5778, NUITKA_BYTECODE_FLAG},
    {"runpy", NULL, 5634156, 8107, NUITKA_BYTECODE_FLAG},
    {"sched", NULL, 5642263, 6555, NUITKA_BYTECODE_FLAG},
    {"secrets", NULL, 5648818, 2213, NUITKA_BYTECODE_FLAG},
    {"selectors", NULL, 5651031, 16958, NUITKA_BYTECODE_FLAG},
    {"shelve", NULL, 5667989, 9513, NUITKA_BYTECODE_FLAG},
    {"shlex", NULL, 5677502, 7513, NUITKA_BYTECODE_FLAG},
    {"shutil", NULL, 5685015, 36188, NUITKA_BYTECODE_FLAG},
    {"signal", NULL, 5721203, 2866, NUITKA_BYTECODE_FLAG},
    {"site", NULL, 137285, 13330, NUITKA_BYTECODE_FLAG},
    {"smtpd", NULL, 5724069, 26486, NUITKA_BYTECODE_FLAG},
    {"smtplib", NULL, 5750555, 35336, NUITKA_BYTECODE_FLAG},
    {"sndhdr", NULL, 5785891, 6998, NUITKA_BYTECODE_FLAG},
    {"socket", NULL, 5792889, 27796, NUITKA_BYTECODE_FLAG},
    {"socketserver", NULL, 5820685, 24029, NUITKA_BYTECODE_FLAG},
    {"sqlite3", NULL, 5844714, 183, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"sqlite3.dbapi2", NULL, 5844897, 2527, NUITKA_BYTECODE_FLAG},
    {"sqlite3.dump", NULL, 5847424, 1952, NUITKA_BYTECODE_FLAG},
    {"sre_compile", NULL, 5849376, 15165, NUITKA_BYTECODE_FLAG},
    {"sre_constants", NULL, 5864541, 6382, NUITKA_BYTECODE_FLAG},
    {"sre_parse", NULL, 5870923, 21670, NUITKA_BYTECODE_FLAG},
    {"ssl", NULL, 5892593, 44619, NUITKA_BYTECODE_FLAG},
    {"stat", NULL, 5937212, 4395, NUITKA_BYTECODE_FLAG},
    {"statistics", NULL, 5941607, 33532, NUITKA_BYTECODE_FLAG},
    {"string", NULL, 5975139, 7323, NUITKA_BYTECODE_FLAG},
    {"stringprep", NULL, 5982462, 11040, NUITKA_BYTECODE_FLAG},
    {"struct", NULL, 5993502, 353, NUITKA_BYTECODE_FLAG},
    {"subprocess", NULL, 150615, 41938, NUITKA_BYTECODE_FLAG},
    {"sunau", NULL, 5993855, 17103, NUITKA_BYTECODE_FLAG},
    {"symbol", NULL, 6010958, 2427, NUITKA_BYTECODE_FLAG},
    {"symtable", NULL, 6013385, 11090, NUITKA_BYTECODE_FLAG},
    {"sysconfig", NULL, 6024475, 15643, NUITKA_BYTECODE_FLAG},
    {"tabnanny", NULL, 6040118, 7053, NUITKA_BYTECODE_FLAG},
    {"tarfile", NULL, 6047171, 62557, NUITKA_BYTECODE_FLAG},
    {"telnetlib", NULL, 6109728, 18260, NUITKA_BYTECODE_FLAG},
    {"tempfile", NULL, 6127988, 23382, NUITKA_BYTECODE_FLAG},
    {"textwrap", NULL, 6151370, 13542, NUITKA_BYTECODE_FLAG},
    {"this", NULL, 6164912, 1284, NUITKA_BYTECODE_FLAG},
    {"threading", NULL, 6166196, 39952, NUITKA_BYTECODE_FLAG},
    {"timeit", NULL, 6206148, 11762, NUITKA_BYTECODE_FLAG},
    {"tkinter", NULL, 192553, 176022, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"tkinter.colorchooser", NULL, 6217910, 1148, NUITKA_BYTECODE_FLAG},
    {"tkinter.commondialog", NULL, 6219058, 1145, NUITKA_BYTECODE_FLAG},
    {"tkinter.constants", NULL, 6220203, 1677, NUITKA_BYTECODE_FLAG},
    {"tkinter.dialog", NULL, 6221880, 1495, NUITKA_BYTECODE_FLAG},
    {"tkinter.dnd", NULL, 6223375, 11285, NUITKA_BYTECODE_FLAG},
    {"tkinter.filedialog", NULL, 6234660, 12251, NUITKA_BYTECODE_FLAG},
    {"tkinter.font", NULL, 6246911, 6220, NUITKA_BYTECODE_FLAG},
    {"tkinter.messagebox", NULL, 6253131, 3014, NUITKA_BYTECODE_FLAG},
    {"tkinter.scrolledtext", NULL, 6256145, 2200, NUITKA_BYTECODE_FLAG},
    {"tkinter.simpledialog", NULL, 6258345, 10639, NUITKA_BYTECODE_FLAG},
    {"tkinter.tix", NULL, 6268984, 79539, NUITKA_BYTECODE_FLAG},
    {"tkinter.ttk", NULL, 6348523, 56738, NUITKA_BYTECODE_FLAG},
    {"token", NULL, 6405261, 2508, NUITKA_BYTECODE_FLAG},
    {"tokenize", NULL, 6407769, 17183, NUITKA_BYTECODE_FLAG},
    {"trace", NULL, 6424952, 20101, NUITKA_BYTECODE_FLAG},
    {"traceback", NULL, 6445053, 19913, NUITKA_BYTECODE_FLAG},
    {"tracemalloc", NULL, 6464966, 17386, NUITKA_BYTECODE_FLAG},
    {"turtle", NULL, 6482352, 129963, NUITKA_BYTECODE_FLAG},
    {"types", NULL, 6612315, 9200, NUITKA_BYTECODE_FLAG},
    {"typing", NULL, 6621515, 61842, NUITKA_BYTECODE_FLAG},
    {"unittest", NULL, 6683357, 3149, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"unittest.async_case", NULL, 6686506, 4163, NUITKA_BYTECODE_FLAG},
    {"unittest.case", NULL, 6690669, 50494, NUITKA_BYTECODE_FLAG},
    {"unittest.loader", NULL, 6741163, 14559, NUITKA_BYTECODE_FLAG},
    {"unittest.main", NULL, 6755722, 7557, NUITKA_BYTECODE_FLAG},
    {"unittest.mock", NULL, 6763279, 77612, NUITKA_BYTECODE_FLAG},
    {"unittest.result", NULL, 6840891, 7314, NUITKA_BYTECODE_FLAG},
    {"unittest.runner", NULL, 6848205, 7039, NUITKA_BYTECODE_FLAG},
    {"unittest.signals", NULL, 6855244, 2243, NUITKA_BYTECODE_FLAG},
    {"unittest.suite", NULL, 6857487, 9958, NUITKA_BYTECODE_FLAG},
    {"unittest.util", NULL, 6867445, 4454, NUITKA_BYTECODE_FLAG},
    {"urllib", NULL, 6871899, 151, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"urllib.error", NULL, 6872050, 2832, NUITKA_BYTECODE_FLAG},
    {"urllib.parse", NULL, 6874882, 33950, NUITKA_BYTECODE_FLAG},
    {"urllib.request", NULL, 6908832, 72398, NUITKA_BYTECODE_FLAG},
    {"urllib.response", NULL, 6981230, 3312, NUITKA_BYTECODE_FLAG},
    {"urllib.robotparser", NULL, 6984542, 7350, NUITKA_BYTECODE_FLAG},
    {"uu", NULL, 6991892, 3575, NUITKA_BYTECODE_FLAG},
    {"uuid", NULL, 6995467, 23689, NUITKA_BYTECODE_FLAG},
    {"venv", NULL, 7019156, 14454, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"warnings", NULL, 7033610, 13779, NUITKA_BYTECODE_FLAG},
    {"wave", NULL, 7047389, 18171, NUITKA_BYTECODE_FLAG},
    {"weakref", NULL, 7065560, 19541, NUITKA_BYTECODE_FLAG},
    {"webbrowser", NULL, 7085101, 16501, NUITKA_BYTECODE_FLAG},
    {"wsgiref", NULL, 7101602, 747, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"wsgiref.handlers", NULL, 7102349, 16469, NUITKA_BYTECODE_FLAG},
    {"wsgiref.headers", NULL, 7118818, 7779, NUITKA_BYTECODE_FLAG},
    {"wsgiref.simple_server", NULL, 7126597, 5284, NUITKA_BYTECODE_FLAG},
    {"wsgiref.util", NULL, 7131881, 5435, NUITKA_BYTECODE_FLAG},
    {"wsgiref.validate", NULL, 7137316, 14778, NUITKA_BYTECODE_FLAG},
    {"xdrlib", NULL, 7152094, 8244, NUITKA_BYTECODE_FLAG},
    {"xml", NULL, 368575, 715, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.dom", NULL, 7160338, 5553, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.dom.NodeFilter", NULL, 7165891, 990, NUITKA_BYTECODE_FLAG},
    {"xml.dom.domreg", NULL, 7166881, 2873, NUITKA_BYTECODE_FLAG},
    {"xml.dom.expatbuilder", NULL, 7169754, 27364, NUITKA_BYTECODE_FLAG},
    {"xml.dom.minicompat", NULL, 7197118, 2765, NUITKA_BYTECODE_FLAG},
    {"xml.dom.minidom", NULL, 7199883, 55397, NUITKA_BYTECODE_FLAG},
    {"xml.dom.pulldom", NULL, 7255280, 10714, NUITKA_BYTECODE_FLAG},
    {"xml.dom.xmlbuilder", NULL, 7265994, 12517, NUITKA_BYTECODE_FLAG},
    {"xml.etree", NULL, 369290, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.etree.ElementInclude", NULL, 7278511, 1602, NUITKA_BYTECODE_FLAG},
    {"xml.etree.ElementPath", NULL, 7280113, 8457, NUITKA_BYTECODE_FLAG},
    {"xml.etree.ElementTree", NULL, 369444, 55635, NUITKA_BYTECODE_FLAG},
    {"xml.etree.cElementTree", NULL, 7288570, 196, NUITKA_BYTECODE_FLAG},
    {"xml.parsers", NULL, 7288766, 328, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.parsers.expat", NULL, 7289094, 357, NUITKA_BYTECODE_FLAG},
    {"xml.sax", NULL, 7289451, 3242, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xml.sax._exceptions", NULL, 7292693, 5467, NUITKA_BYTECODE_FLAG},
    {"xml.sax.expatreader", NULL, 7298160, 12518, NUITKA_BYTECODE_FLAG},
    {"xml.sax.handler", NULL, 7310678, 12445, NUITKA_BYTECODE_FLAG},
    {"xml.sax.saxutils", NULL, 7323123, 12942, NUITKA_BYTECODE_FLAG},
    {"xml.sax.xmlreader", NULL, 7336065, 16867, NUITKA_BYTECODE_FLAG},
    {"xmlrpc", NULL, 7352932, 151, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG},
    {"xmlrpc.client", NULL, 7353083, 34562, NUITKA_BYTECODE_FLAG},
    {"xmlrpc.server", NULL, 7387645, 29431, NUITKA_BYTECODE_FLAG},
    {"zipapp", NULL, 7417076, 5877, NUITKA_BYTECODE_FLAG},
    {"zipfile", NULL, 7422953, 56383, NUITKA_BYTECODE_FLAG},
    {NULL, NULL, 0, 0, 0}
};

void setupMetaPathBasedLoader(void) {
    static bool init_done = false;

    if (init_done == false) {
        registerMetaPathBasedUnfreezer(meta_path_loader_entries);
        init_done = true;
    }
}
