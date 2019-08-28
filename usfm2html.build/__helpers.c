// This file contains helper functions that are automatically created from
// templates.

#include "nuitka/prelude.h"

extern PyObject *callPythonFunction( PyObject *func, PyObject **args, int count );


PyObject *CALL_FUNCTION_WITH_ARGS1( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 1 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 1; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 1 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 1 * sizeof(PyObject *) );
            memcpy( python_pars + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 1 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 1 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 1; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 1 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 1 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 1, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 1 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 1 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_O )
        {
#if 1 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 1 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 1, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 1 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            1
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 1 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS2( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 2; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 2 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 2; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 2 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 2 * sizeof(PyObject *) );
            memcpy( python_pars + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 2 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 2 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 2; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 2 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 2 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 2, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 2 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 2 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_O )
        {
#if 2 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 2 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 2, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 2 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            2
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 2 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS3( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 3; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 3 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 3; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 3 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 3 * sizeof(PyObject *) );
            memcpy( python_pars + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 3 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 3 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 3; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 3 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 3 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 3, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 3 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 3 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_O )
        {
#if 3 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 3 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 3, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 3 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            3
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 3 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS4( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 4; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 4 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 4; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 4 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 4 * sizeof(PyObject *) );
            memcpy( python_pars + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 4 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 4 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 4; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 4 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 4 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 4, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 4 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 4 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_O )
        {
#if 4 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 4 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 4, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 4 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            4
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 4 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_FUNCTION_WITH_ARGS5( PyObject *called, PyObject **args )
{
    CHECK_OBJECT( called );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 5; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    if ( Nuitka_Function_Check( called ) )
    {
        if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
        {
            return NULL;
        }

        struct Nuitka_FunctionObject *function = (struct Nuitka_FunctionObject *)called;
        PyObject *result;

        if ( function->m_args_simple && 5 == function->m_args_positional_count )
        {
            for( Py_ssize_t i = 0; i < 5; i++ )
            {
                Py_INCREF( args[ i ] );
            }

            result = function->m_c_code( function, args );
        }
        else if ( function->m_args_simple && 5 + function->m_defaults_given == function->m_args_positional_count )
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
            PyObject *python_pars[ function->m_args_positional_count ];
#endif
            memcpy( python_pars, args, 5 * sizeof(PyObject *) );
            memcpy( python_pars + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

            for( Py_ssize_t i = 0; i < function->m_args_positional_count; i++ )
            {
                Py_INCREF( python_pars[ i ] );
            }

            result = function->m_c_code( function, python_pars );
        }
        else
        {
#ifdef _MSC_VER
            PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
            PyObject *python_pars[ function->m_args_overall_count ];
#endif
            memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

            if ( parseArgumentsPos( function, python_pars, args, 5 ))
            {
                result = function->m_c_code( function, python_pars );
            }
            else
            {
                result = NULL;
            }
        }

        Py_LeaveRecursiveCall();

        return result;
    }
    else if ( Nuitka_Method_Check( called ) )
    {
        struct Nuitka_MethodObject *method = (struct Nuitka_MethodObject *)called;

        // Unbound method without arguments, let the error path be slow.
        if ( method->m_object != NULL )
        {
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }

            struct Nuitka_FunctionObject *function = method->m_function;

            PyObject *result;

            if ( function->m_args_simple && 5 + 1 == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                for( Py_ssize_t i = 0; i < 5; i++ )
                {
                    python_pars[ i + 1 ] = args[ i ];
                    Py_INCREF( args[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else if ( function->m_args_simple && 5 + 1 + function->m_defaults_given == function->m_args_positional_count )
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_positional_count );
#else
                PyObject *python_pars[ function->m_args_positional_count ];
#endif
                python_pars[ 0 ] = method->m_object;
                Py_INCREF( method->m_object );

                memcpy( python_pars+1, args, 5 * sizeof(PyObject *) );
                memcpy( python_pars+1 + 5, &PyTuple_GET_ITEM( function->m_defaults, 0 ), function->m_defaults_given * sizeof(PyObject *) );

                for( Py_ssize_t i = 1; i < function->m_args_overall_count; i++ )
                {
                    Py_INCREF( python_pars[ i ] );
                }

                result = function->m_c_code( function, python_pars );
            }
            else
            {
#ifdef _MSC_VER
                PyObject **python_pars = (PyObject **)_alloca( sizeof( PyObject * ) * function->m_args_overall_count );
#else
                PyObject *python_pars[ function->m_args_overall_count ];
#endif
                memset( python_pars, 0, function->m_args_overall_count * sizeof(PyObject *) );

                if ( parseArgumentsMethodPos( function, python_pars, method->m_object, args, 5 ) )
                {
                    result = function->m_c_code( function, python_pars );
                }
                else
                {
                    result = NULL;
                }
            }

            Py_LeaveRecursiveCall();

            return result;
        }
    }
    else if ( PyCFunction_Check( called ) )
    {
        // Try to be fast about wrapping the arguments.
        int flags = PyCFunction_GET_FLAGS( called ) & ~(METH_CLASS | METH_STATIC | METH_COEXIST);

        if ( flags & METH_NOARGS )
        {
#if 5 == 0
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, NULL );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_O )
        {
#if 5 == 1
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

            PyObject *result = (*method)( self, args[0] );

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
            // Some buggy C functions do set an error, but do not indicate it
            // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
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
        }
        else if ( flags & METH_VARARGS )
        {
            PyCFunction method = PyCFunction_GET_FUNCTION( called );
            PyObject *self = PyCFunction_GET_SELF( called );

            PyObject *pos_args = MAKE_TUPLE( args, 5 );

            PyObject *result;

            // Recursion guard is not strictly necessary, as we already have
            // one on our way to here.
#ifdef _NUITKA_FULL_COMPAT
            if (unlikely( Py_EnterRecursiveCall( (char *)" while calling a Python object" ) ))
            {
                return NULL;
            }
#endif

#if PYTHON_VERSION < 360
            if ( flags & METH_KEYWORDS )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#else
            if ( flags == ( METH_VARARGS | METH_KEYWORDS ) )
            {
                result = (*(PyCFunctionWithKeywords)method)( self, pos_args, NULL );
            }
            else if ( flags == METH_FASTCALL )
            {
#if PYTHON_VERSION < 370
                result = (*(_PyCFunctionFast)method)( self, &PyTuple_GET_ITEM( pos_args, 0 ), 5, NULL );;
#else
                result = (*(_PyCFunctionFast)method)( self, &pos_args, 5 );;
#endif
            }
            else
            {
                result = (*method)( self, pos_args );
            }
#endif

#ifdef _NUITKA_FULL_COMPAT
            Py_LeaveRecursiveCall();
#endif

            if ( result != NULL )
            {
                // Some buggy C functions do set an error, but do not indicate it
                // and Nuitka inner workings can get upset/confused from it.
                DROP_ERROR_OCCURRED();

                Py_DECREF( pos_args );
                return result;
            }
            else
            {
                // Other buggy C functions do this, return NULL, but with
                // no error set, not allowed.
                if (unlikely( !ERROR_OCCURRED() ))
                {
                    PyErr_Format(
                        PyExc_SystemError,
                        "NULL result without error in PyObject_Call"
                    );
                }

                Py_DECREF( pos_args );
                return NULL;
            }
        }
    }
    else if ( PyFunction_Check( called ) )
    {
        return callPythonFunction(
            called,
            args,
            5
        );
    }

    PyObject *pos_args = MAKE_TUPLE( args, 5 );

    PyObject *result = CALL_FUNCTION(
        called,
        pos_args,
        NULL
    );

    Py_DECREF( pos_args );

    return result;
}

PyObject *CALL_METHOD_WITH_ARGS1( PyObject *source, PyObject *attr_name, PyObject **args )
{
    CHECK_OBJECT( source );
    CHECK_OBJECT( attr_name );

    // Check if arguments are valid objects in debug mode.
#ifndef __NUITKA_NO_ASSERT__
    for( size_t i = 0; i < 1; i++ )
    {
        CHECK_OBJECT( args[ i ] );
    }
#endif

    PyTypeObject *type = Py_TYPE( source );

    if ( type->tp_getattro == PyObject_GenericGetAttr )
    {
        // Unfortunately this is required, although of cause rarely necessary.
        if (unlikely( type->tp_dict == NULL ))
        {
            if (unlikely( PyType_Ready( type ) < 0 ))
            {
                return NULL;
            }
        }

        PyObject *descr = _PyType_Lookup( type, attr_name );
        descrgetfunc func = NULL;

        if ( descr != NULL )
        {
            Py_INCREF( descr );

#if PYTHON_VERSION < 300
            if ( PyType_HasFeature( Py_TYPE( descr ), Py_TPFLAGS_HAVE_CLASS ) )
            {
#endif
                func = Py_TYPE( descr )->tp_descr_get;

                if ( func != NULL && PyDescr_IsData( descr ) )
                {
                    PyObject *called_object = func( descr, source, (PyObject *)type );
                    Py_DECREF( descr );

                    PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                        called_object,
                        args
                    );
                    Py_DECREF( called_object );
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

        if ( dict != NULL )
        {
            CHECK_OBJECT( dict );

            Py_INCREF( dict );

            PyObject *called_object = PyDict_GetItem( dict, attr_name );

            if ( called_object != NULL )
            {
                Py_INCREF( called_object );
                Py_XDECREF( descr );
                Py_DECREF( dict );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );
                return result;
            }

            Py_DECREF( dict );
        }

        if ( func != NULL )
        {
            if ( func == Nuitka_Function_Type.tp_descr_get )
            {
                PyObject *result = Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)descr,
                    source,
                    args,
                    1
                );

                Py_DECREF( descr );

                return result;
            }
            else
            {
                PyObject *called_object = func( descr, source, (PyObject *)type );
                CHECK_OBJECT( called_object );

                Py_DECREF( descr );

                PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                    called_object,
                    args
                );
                Py_DECREF( called_object );

                return result;
            }
        }

        if ( descr != NULL )
        {
            CHECK_OBJECT( descr );

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                descr,
                args
            );
            Py_DECREF( descr );

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
    else if ( type == &PyInstance_Type )
    {
        PyInstanceObject *source_instance = (PyInstanceObject *)source;

        // The special cases have their own variant on the code generation level
        // as we are called with constants only.
        assert( attr_name != const_str_plain___dict__ );
        assert( attr_name != const_str_plain___class__ );

        // Try the instance dict first.
        PyObject *called_object = GET_STRING_DICT_VALUE(
            (PyDictObject *)source_instance->in_dict,
            (PyStringObject *)attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            return CALL_FUNCTION_WITH_ARGS1( called_object, args );
        }

        // Then check the class dictionaries.
        called_object = FIND_ATTRIBUTE_IN_CLASS(
            source_instance->in_class,
            attr_name
        );

        // Note: The "called_object" was found without taking a reference,
        // so we need not release it in this branch.
        if ( called_object != NULL )
        {
            descrgetfunc descr_get = Py_TYPE( called_object )->tp_descr_get;

            if ( descr_get == Nuitka_Function_Type.tp_descr_get )
            {
                return Nuitka_CallMethodFunctionPosArgs(
                    (struct Nuitka_FunctionObject const *)called_object,
                    source,
                    args,
                    1
                );
            }
            else if ( descr_get != NULL )
            {
                PyObject *method = descr_get(
                    called_object,
                    source,
                    (PyObject *)source_instance->in_class
                );

                if (unlikely( method == NULL ))
                {
                    return NULL;
                }

                PyObject *result = CALL_FUNCTION_WITH_ARGS1( method, args );
                Py_DECREF( method );
                return result;
            }
            else
            {
                return CALL_FUNCTION_WITH_ARGS1( called_object, args );
            }

        }
        else if (unlikely( source_instance->in_class->cl_getattr == NULL ))
        {
            PyErr_Format(
                PyExc_AttributeError,
                "%s instance has no attribute '%s'",
                PyString_AS_STRING( source_instance->in_class->cl_name ),
                PyString_AS_STRING( attr_name )
            );

            return NULL;
        }
        else
        {
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

            if (unlikely( called_object == NULL ))
            {
                return NULL;
            }

            PyObject *result = CALL_FUNCTION_WITH_ARGS1(
                called_object,
                args
            );
            Py_DECREF( called_object );
            return result;
        }
    }
#endif
    else if ( type->tp_getattro != NULL )
    {
        PyObject *called_object = (*type->tp_getattro)(
            source,
            attr_name
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else if ( type->tp_getattr != NULL )
    {
        PyObject *called_object = (*type->tp_getattr)(
            source,
            (char *)Nuitka_String_AsString_Unchecked( attr_name )
        );

        if (unlikely( called_object == NULL ))
        {
            return NULL;
        }

        PyObject *result = CALL_FUNCTION_WITH_ARGS1(
            called_object,
            args
        );
        Py_DECREF( called_object );
        return result;
    }
    else
    {
        PyErr_Format(
            PyExc_AttributeError,
            "'%s' object has no attribute '%s'",
            type->tp_name,
            Nuitka_String_AsString_Unchecked( attr_name )
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
MOD_INIT_DECL( __main__ );
static struct Nuitka_MetaPathBasedLoaderEntry meta_path_loader_entries[] =
{
    { "__main__", MOD_INIT_NAME( __main__ ), 0, 0, NUITKA_COMPILED_MODULE },
    { "_asyncio", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_bisect", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_blake2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_bz2", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_cn", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_hk", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_iso2022", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_jp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_kr", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_codecs_tw", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_contextvars", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_crypt", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_csv", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ctypes", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_curses", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_curses_panel", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_datetime", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_dbm", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_decimal", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_elementtree", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_hashlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_heapq", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_json", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_lsprof", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_lzma", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multibytecodec", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_multiprocessing", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_opcode", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_pickle", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_posixsubprocess", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_queue", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_random", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_scproxy", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_sha3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_socket", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_sqlite3", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_ssl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_struct", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_tkinter", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "_uuid", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "array", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "audioop", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "binascii", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "fcntl", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "grp", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "math", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "mmap", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "pyexpat", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "readline", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "select", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "termios", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "unicodedata", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "zlib", NULL, 0, 0, NUITKA_SHLIB_FLAG },
    { "__future__", NULL, 2013, 4143, NUITKA_BYTECODE_FLAG },
    { "_bootlocale", NULL, 6156, 1260, NUITKA_BYTECODE_FLAG },
    { "_compat_pickle", NULL, 7416, 5819, NUITKA_BYTECODE_FLAG },
    { "_dummy_thread", NULL, 13235, 4864, NUITKA_BYTECODE_FLAG },
    { "_markupbase", NULL, 18099, 7796, NUITKA_BYTECODE_FLAG },
    { "_osx_support", NULL, 25895, 9589, NUITKA_BYTECODE_FLAG },
    { "_py_abc", NULL, 35484, 4665, NUITKA_BYTECODE_FLAG },
    { "_pyio", NULL, 40149, 72807, NUITKA_BYTECODE_FLAG },
    { "_sitebuiltins", NULL, 112956, 3476, NUITKA_BYTECODE_FLAG },
    { "_strptime", NULL, 116432, 16115, NUITKA_BYTECODE_FLAG },
    { "_sysconfigdata_m_darwin_darwin", NULL, 132547, 19336, NUITKA_BYTECODE_FLAG },
    { "_threading_local", NULL, 151883, 6423, NUITKA_BYTECODE_FLAG },
    { "aifc", NULL, 158306, 26154, NUITKA_BYTECODE_FLAG },
    { "argparse", NULL, 184460, 61924, NUITKA_BYTECODE_FLAG },
    { "ast", NULL, 246384, 12091, NUITKA_BYTECODE_FLAG },
    { "asynchat", NULL, 258475, 6845, NUITKA_BYTECODE_FLAG },
    { "asyncio", NULL, 265320, 701, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "asyncio.base_events", NULL, 266021, 47678, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_futures", NULL, 313699, 2113, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_subprocess", NULL, 315812, 9202, NUITKA_BYTECODE_FLAG },
    { "asyncio.base_tasks", NULL, 325014, 1877, NUITKA_BYTECODE_FLAG },
    { "asyncio.constants", NULL, 326891, 602, NUITKA_BYTECODE_FLAG },
    { "asyncio.coroutines", NULL, 327493, 6389, NUITKA_BYTECODE_FLAG },
    { "asyncio.events", NULL, 333882, 27866, NUITKA_BYTECODE_FLAG },
    { "asyncio.format_helpers", NULL, 361748, 2328, NUITKA_BYTECODE_FLAG },
    { "asyncio.futures", NULL, 364076, 10756, NUITKA_BYTECODE_FLAG },
    { "asyncio.locks", NULL, 374832, 15924, NUITKA_BYTECODE_FLAG },
    { "asyncio.log", NULL, 390756, 251, NUITKA_BYTECODE_FLAG },
    { "asyncio.proactor_events", NULL, 391007, 19690, NUITKA_BYTECODE_FLAG },
    { "asyncio.protocols", NULL, 410697, 8739, NUITKA_BYTECODE_FLAG },
    { "asyncio.queues", NULL, 419436, 8184, NUITKA_BYTECODE_FLAG },
    { "asyncio.runners", NULL, 427620, 1925, NUITKA_BYTECODE_FLAG },
    { "asyncio.selector_events", NULL, 429545, 28491, NUITKA_BYTECODE_FLAG },
    { "asyncio.sslproto", NULL, 458036, 21222, NUITKA_BYTECODE_FLAG },
    { "asyncio.streams", NULL, 479258, 20299, NUITKA_BYTECODE_FLAG },
    { "asyncio.subprocess", NULL, 499557, 6763, NUITKA_BYTECODE_FLAG },
    { "asyncio.tasks", NULL, 506320, 21858, NUITKA_BYTECODE_FLAG },
    { "asyncio.transports", NULL, 528178, 12222, NUITKA_BYTECODE_FLAG },
    { "asyncio.unix_events", NULL, 540400, 32117, NUITKA_BYTECODE_FLAG },
    { "asyncore", NULL, 572517, 15855, NUITKA_BYTECODE_FLAG },
    { "bdb", NULL, 588372, 24404, NUITKA_BYTECODE_FLAG },
    { "binhex", NULL, 612776, 12070, NUITKA_BYTECODE_FLAG },
    { "bisect", NULL, 624846, 2709, NUITKA_BYTECODE_FLAG },
    { "cProfile", NULL, 627555, 4477, NUITKA_BYTECODE_FLAG },
    { "calendar", NULL, 632032, 27435, NUITKA_BYTECODE_FLAG },
    { "cgi", NULL, 659467, 26596, NUITKA_BYTECODE_FLAG },
    { "cgitb", NULL, 686063, 10123, NUITKA_BYTECODE_FLAG },
    { "chunk", NULL, 696186, 4930, NUITKA_BYTECODE_FLAG },
    { "cmd", NULL, 701116, 12601, NUITKA_BYTECODE_FLAG },
    { "code", NULL, 713717, 9869, NUITKA_BYTECODE_FLAG },
    { "codeop", NULL, 723586, 6301, NUITKA_BYTECODE_FLAG },
    { "colorsys", NULL, 729887, 3308, NUITKA_BYTECODE_FLAG },
    { "compileall", NULL, 733195, 9010, NUITKA_BYTECODE_FLAG },
    { "concurrent", NULL, 742205, 157, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures", NULL, 742362, 1097, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "concurrent.futures._base", NULL, 743459, 20873, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.process", NULL, 764332, 19572, NUITKA_BYTECODE_FLAG },
    { "concurrent.futures.thread", NULL, 783904, 5184, NUITKA_BYTECODE_FLAG },
    { "configparser", NULL, 789088, 45873, NUITKA_BYTECODE_FLAG },
    { "contextlib", NULL, 834961, 19911, NUITKA_BYTECODE_FLAG },
    { "contextvars", NULL, 854872, 268, NUITKA_BYTECODE_FLAG },
    { "copy", NULL, 855140, 7112, NUITKA_BYTECODE_FLAG },
    { "crypt", NULL, 862252, 3145, NUITKA_BYTECODE_FLAG },
    { "csv", NULL, 865397, 11843, NUITKA_BYTECODE_FLAG },
    { "ctypes", NULL, 877240, 16160, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes._aix", NULL, 893400, 9832, NUITKA_BYTECODE_FLAG },
    { "ctypes._endian", NULL, 903232, 1967, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib", NULL, 905199, 324, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "ctypes.macholib.dyld", NULL, 905523, 4357, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.dylib", NULL, 909880, 1951, NUITKA_BYTECODE_FLAG },
    { "ctypes.macholib.framework", NULL, 911831, 2231, NUITKA_BYTECODE_FLAG },
    { "ctypes.util", NULL, 914062, 7760, NUITKA_BYTECODE_FLAG },
    { "curses", NULL, 921822, 1806, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "curses.ascii", NULL, 923628, 3914, NUITKA_BYTECODE_FLAG },
    { "curses.has_key", NULL, 927542, 4278, NUITKA_BYTECODE_FLAG },
    { "curses.panel", NULL, 931820, 248, NUITKA_BYTECODE_FLAG },
    { "curses.textpad", NULL, 932068, 5917, NUITKA_BYTECODE_FLAG },
    { "dataclasses", NULL, 937985, 21439, NUITKA_BYTECODE_FLAG },
    { "datetime", NULL, 959424, 56482, NUITKA_BYTECODE_FLAG },
    { "dbm", NULL, 1015906, 4177, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "dbm.dumb", NULL, 1020083, 8411, NUITKA_BYTECODE_FLAG },
    { "dbm.ndbm", NULL, 1028494, 227, NUITKA_BYTECODE_FLAG },
    { "decimal", NULL, 1028721, 162183, NUITKA_BYTECODE_FLAG },
    { "difflib", NULL, 1190904, 59449, NUITKA_BYTECODE_FLAG },
    { "distutils", NULL, 1250353, 409, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.archive_util", NULL, 1250762, 6389, NUITKA_BYTECODE_FLAG },
    { "distutils.bcppcompiler", NULL, 1257151, 6512, NUITKA_BYTECODE_FLAG },
    { "distutils.ccompiler", NULL, 1263663, 33227, NUITKA_BYTECODE_FLAG },
    { "distutils.cmd", NULL, 1296890, 13919, NUITKA_BYTECODE_FLAG },
    { "distutils.command", NULL, 1310809, 566, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "distutils.command.bdist", NULL, 1311375, 3665, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_dumb", NULL, 1315040, 3579, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_rpm", NULL, 1318619, 12504, NUITKA_BYTECODE_FLAG },
    { "distutils.command.bdist_wininst", NULL, 1331123, 8018, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build", NULL, 1339141, 3846, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_clib", NULL, 1342987, 4896, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_ext", NULL, 1347883, 15773, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_py", NULL, 1363656, 10402, NUITKA_BYTECODE_FLAG },
    { "distutils.command.build_scripts", NULL, 1374058, 4301, NUITKA_BYTECODE_FLAG },
    { "distutils.command.check", NULL, 1378359, 4821, NUITKA_BYTECODE_FLAG },
    { "distutils.command.clean", NULL, 1383180, 2103, NUITKA_BYTECODE_FLAG },
    { "distutils.command.config", NULL, 1385283, 10207, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install", NULL, 1395490, 13530, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_data", NULL, 1409020, 2296, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_egg_info", NULL, 1411316, 2994, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_headers", NULL, 1414310, 1711, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_lib", NULL, 1416021, 5089, NUITKA_BYTECODE_FLAG },
    { "distutils.command.install_scripts", NULL, 1421110, 2153, NUITKA_BYTECODE_FLAG },
    { "distutils.command.register", NULL, 1423263, 8488, NUITKA_BYTECODE_FLAG },
    { "distutils.command.sdist", NULL, 1431751, 14519, NUITKA_BYTECODE_FLAG },
    { "distutils.command.upload", NULL, 1446270, 5101, NUITKA_BYTECODE_FLAG },
    { "distutils.config", NULL, 1451371, 3496, NUITKA_BYTECODE_FLAG },
    { "distutils.core", NULL, 1454867, 6621, NUITKA_BYTECODE_FLAG },
    { "distutils.cygwinccompiler", NULL, 1461488, 8539, NUITKA_BYTECODE_FLAG },
    { "distutils.debug", NULL, 1470027, 219, NUITKA_BYTECODE_FLAG },
    { "distutils.dep_util", NULL, 1470246, 2735, NUITKA_BYTECODE_FLAG },
    { "distutils.dir_util", NULL, 1472981, 5829, NUITKA_BYTECODE_FLAG },
    { "distutils.dist", NULL, 1478810, 34450, NUITKA_BYTECODE_FLAG },
    { "distutils.errors", NULL, 1513260, 5505, NUITKA_BYTECODE_FLAG },
    { "distutils.extension", NULL, 1518765, 6916, NUITKA_BYTECODE_FLAG },
    { "distutils.fancy_getopt", NULL, 1525681, 10628, NUITKA_BYTECODE_FLAG },
    { "distutils.file_util", NULL, 1536309, 5914, NUITKA_BYTECODE_FLAG },
    { "distutils.filelist", NULL, 1542223, 9849, NUITKA_BYTECODE_FLAG },
    { "distutils.log", NULL, 1552072, 2292, NUITKA_BYTECODE_FLAG },
    { "distutils.msvccompiler", NULL, 1554364, 14582, NUITKA_BYTECODE_FLAG },
    { "distutils.spawn", NULL, 1568946, 5000, NUITKA_BYTECODE_FLAG },
    { "distutils.sysconfig", NULL, 1573946, 11869, NUITKA_BYTECODE_FLAG },
    { "distutils.text_file", NULL, 1585815, 8456, NUITKA_BYTECODE_FLAG },
    { "distutils.unixccompiler", NULL, 1594271, 6490, NUITKA_BYTECODE_FLAG },
    { "distutils.util", NULL, 1600761, 15066, NUITKA_BYTECODE_FLAG },
    { "distutils.version", NULL, 1615827, 7367, NUITKA_BYTECODE_FLAG },
    { "distutils.versionpredicate", NULL, 1623194, 5114, NUITKA_BYTECODE_FLAG },
    { "doctest", NULL, 1628308, 75621, NUITKA_BYTECODE_FLAG },
    { "dummy_threading", NULL, 1703929, 1135, NUITKA_BYTECODE_FLAG },
    { "email", NULL, 1705064, 1702, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email._encoded_words", NULL, 1706766, 5664, NUITKA_BYTECODE_FLAG },
    { "email._header_value_parser", NULL, 1712430, 75636, NUITKA_BYTECODE_FLAG },
    { "email._parseaddr", NULL, 1788066, 12359, NUITKA_BYTECODE_FLAG },
    { "email._policybase", NULL, 1800425, 14861, NUITKA_BYTECODE_FLAG },
    { "email.base64mime", NULL, 1815286, 3246, NUITKA_BYTECODE_FLAG },
    { "email.charset", NULL, 1818532, 11540, NUITKA_BYTECODE_FLAG },
    { "email.contentmanager", NULL, 1830072, 7306, NUITKA_BYTECODE_FLAG },
    { "email.encoders", NULL, 1837378, 1675, NUITKA_BYTECODE_FLAG },
    { "email.errors", NULL, 1839053, 5990, NUITKA_BYTECODE_FLAG },
    { "email.feedparser", NULL, 1845043, 10636, NUITKA_BYTECODE_FLAG },
    { "email.generator", NULL, 1855679, 12511, NUITKA_BYTECODE_FLAG },
    { "email.header", NULL, 1868190, 16391, NUITKA_BYTECODE_FLAG },
    { "email.headerregistry", NULL, 1884581, 21308, NUITKA_BYTECODE_FLAG },
    { "email.iterators", NULL, 1905889, 1943, NUITKA_BYTECODE_FLAG },
    { "email.message", NULL, 1907832, 37951, NUITKA_BYTECODE_FLAG },
    { "email.mime", NULL, 1945783, 157, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "email.mime.application", NULL, 1945940, 1468, NUITKA_BYTECODE_FLAG },
    { "email.mime.audio", NULL, 1947408, 2627, NUITKA_BYTECODE_FLAG },
    { "email.mime.base", NULL, 1950035, 1093, NUITKA_BYTECODE_FLAG },
    { "email.mime.image", NULL, 1951128, 1913, NUITKA_BYTECODE_FLAG },
    { "email.mime.message", NULL, 1953041, 1342, NUITKA_BYTECODE_FLAG },
    { "email.mime.multipart", NULL, 1954383, 1564, NUITKA_BYTECODE_FLAG },
    { "email.mime.nonmultipart", NULL, 1955947, 779, NUITKA_BYTECODE_FLAG },
    { "email.mime.text", NULL, 1956726, 1326, NUITKA_BYTECODE_FLAG },
    { "email.parser", NULL, 1958052, 5758, NUITKA_BYTECODE_FLAG },
    { "email.policy", NULL, 1963810, 9651, NUITKA_BYTECODE_FLAG },
    { "email.quoprimime", NULL, 1973461, 7675, NUITKA_BYTECODE_FLAG },
    { "email.utils", NULL, 1981136, 9478, NUITKA_BYTECODE_FLAG },
    { "filecmp", NULL, 1990614, 8318, NUITKA_BYTECODE_FLAG },
    { "fileinput", NULL, 1998932, 13209, NUITKA_BYTECODE_FLAG },
    { "fnmatch", NULL, 2012141, 3348, NUITKA_BYTECODE_FLAG },
    { "formatter", NULL, 2015489, 17564, NUITKA_BYTECODE_FLAG },
    { "fractions", NULL, 2033053, 18439, NUITKA_BYTECODE_FLAG },
    { "ftplib", NULL, 2051492, 28077, NUITKA_BYTECODE_FLAG },
    { "getopt", NULL, 2079569, 6250, NUITKA_BYTECODE_FLAG },
    { "getpass", NULL, 2085819, 4175, NUITKA_BYTECODE_FLAG },
    { "gettext", NULL, 2089994, 14179, NUITKA_BYTECODE_FLAG },
    { "glob", NULL, 2104173, 4270, NUITKA_BYTECODE_FLAG },
    { "gzip", NULL, 2108443, 17196, NUITKA_BYTECODE_FLAG },
    { "hashlib", NULL, 2125639, 6546, NUITKA_BYTECODE_FLAG },
    { "hmac", NULL, 2132185, 6124, NUITKA_BYTECODE_FLAG },
    { "html", NULL, 2138309, 3408, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "html.entities", NULL, 2141717, 50480, NUITKA_BYTECODE_FLAG },
    { "html.parser", NULL, 2192197, 11118, NUITKA_BYTECODE_FLAG },
    { "http", NULL, 2203315, 5933, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "http.client", NULL, 2209248, 34054, NUITKA_BYTECODE_FLAG },
    { "http.cookiejar", NULL, 2243302, 53418, NUITKA_BYTECODE_FLAG },
    { "http.cookies", NULL, 2296720, 15255, NUITKA_BYTECODE_FLAG },
    { "http.server", NULL, 2311975, 33378, NUITKA_BYTECODE_FLAG },
    { "imaplib", NULL, 2345353, 41409, NUITKA_BYTECODE_FLAG },
    { "imghdr", NULL, 2386762, 4153, NUITKA_BYTECODE_FLAG },
    { "imp", NULL, 2390915, 9717, NUITKA_BYTECODE_FLAG },
    { "importlib.abc", NULL, 2400632, 13497, NUITKA_BYTECODE_FLAG },
    { "importlib.resources", NULL, 2414129, 8350, NUITKA_BYTECODE_FLAG },
    { "importlib.util", NULL, 2422479, 9367, NUITKA_BYTECODE_FLAG },
    { "ipaddress", NULL, 2431846, 63005, NUITKA_BYTECODE_FLAG },
    { "json", NULL, 2494851, 12349, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "json.decoder", NULL, 2507200, 9835, NUITKA_BYTECODE_FLAG },
    { "json.encoder", NULL, 2517035, 11317, NUITKA_BYTECODE_FLAG },
    { "json.scanner", NULL, 2528352, 2007, NUITKA_BYTECODE_FLAG },
    { "json.tool", NULL, 2530359, 1488, NUITKA_BYTECODE_FLAG },
    { "lib2to3", NULL, 2531847, 154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.btm_matcher", NULL, 2532001, 4904, NUITKA_BYTECODE_FLAG },
    { "lib2to3.btm_utils", NULL, 2536905, 6154, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_base", NULL, 2543059, 6246, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixer_util", NULL, 2549305, 12069, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes", NULL, 2561374, 160, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.fixes.fix_apply", NULL, 2561534, 1698, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_asserts", NULL, 2563232, 1283, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_basestring", NULL, 2564515, 673, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_buffer", NULL, 2565188, 818, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_dict", NULL, 2566006, 3330, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_except", NULL, 2569336, 2828, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exec", NULL, 2572164, 1159, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_execfile", NULL, 2573323, 1683, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_exitfunc", NULL, 2575006, 2306, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_filter", NULL, 2577312, 2371, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_funcattrs", NULL, 2579683, 984, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_future", NULL, 2580667, 794, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_getcwdu", NULL, 2581461, 798, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_has_key", NULL, 2582259, 2928, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_idioms", NULL, 2585187, 3913, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_import", NULL, 2589100, 2795, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports", NULL, 2591895, 4359, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_imports2", NULL, 2596254, 558, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_input", NULL, 2596812, 960, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_intern", NULL, 2597772, 1167, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_isinstance", NULL, 2598939, 1565, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools", NULL, 2600504, 1554, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_itertools_imports", NULL, 2602058, 1590, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_long", NULL, 2603648, 715, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_map", NULL, 2604363, 3103, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_metaclass", NULL, 2607466, 5356, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_methodattrs", NULL, 2612822, 946, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ne", NULL, 2613768, 817, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_next", NULL, 2614585, 3070, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_nonzero", NULL, 2617655, 933, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_numliterals", NULL, 2618588, 1029, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_operator", NULL, 2619617, 4246, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_paren", NULL, 2623863, 1408, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_print", NULL, 2625271, 2335, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raise", NULL, 2627606, 2259, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_raw_input", NULL, 2629865, 805, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reduce", NULL, 2630670, 1140, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_reload", NULL, 2631810, 1167, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_renames", NULL, 2632977, 2003, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_repr", NULL, 2634980, 855, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_set_literal", NULL, 2635835, 1687, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_standarderror", NULL, 2637522, 730, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_sys_exc", NULL, 2638252, 1411, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_throw", NULL, 2639663, 1812, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_tuple_params", NULL, 2641475, 4606, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_types", NULL, 2646081, 1839, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_unicode", NULL, 2647920, 1551, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_urllib", NULL, 2649471, 5971, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_ws_comma", NULL, 2655442, 1133, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xrange", NULL, 2656575, 2550, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_xreadlines", NULL, 2659125, 1127, NUITKA_BYTECODE_FLAG },
    { "lib2to3.fixes.fix_zip", NULL, 2660252, 1591, NUITKA_BYTECODE_FLAG },
    { "lib2to3.main", NULL, 2661843, 8547, NUITKA_BYTECODE_FLAG },
    { "lib2to3.patcomp", NULL, 2670390, 5622, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2", NULL, 2676012, 190, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "lib2to3.pgen2.driver", NULL, 2676202, 5151, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.grammar", NULL, 2681353, 7027, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.literals", NULL, 2688380, 1569, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.parse", NULL, 2689949, 6315, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.pgen", NULL, 2696264, 9791, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.token", NULL, 2706055, 1883, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pgen2.tokenize", NULL, 2707938, 15217, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pygram", NULL, 2723155, 1209, NUITKA_BYTECODE_FLAG },
    { "lib2to3.pytree", NULL, 2724364, 25014, NUITKA_BYTECODE_FLAG },
    { "lib2to3.refactor", NULL, 2749378, 20403, NUITKA_BYTECODE_FLAG },
    { "logging", NULL, 2769781, 61506, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "logging.config", NULL, 2831287, 22856, NUITKA_BYTECODE_FLAG },
    { "logging.handlers", NULL, 2854143, 42976, NUITKA_BYTECODE_FLAG },
    { "lzma", NULL, 2897119, 11950, NUITKA_BYTECODE_FLAG },
    { "macpath", NULL, 2909069, 5818, NUITKA_BYTECODE_FLAG },
    { "mailbox", NULL, 2914887, 63659, NUITKA_BYTECODE_FLAG },
    { "mailcap", NULL, 2978546, 6495, NUITKA_BYTECODE_FLAG },
    { "mimetypes", NULL, 2985041, 15479, NUITKA_BYTECODE_FLAG },
    { "modulefinder", NULL, 3000520, 15363, NUITKA_BYTECODE_FLAG },
    { "multiprocessing", NULL, 3015883, 538, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.connection", NULL, 3016421, 24922, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.context", NULL, 3041343, 13124, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.dummy", NULL, 3054467, 3816, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "multiprocessing.dummy.connection", NULL, 3058283, 2524, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.forkserver", NULL, 3060807, 7775, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.heap", NULL, 3068582, 6435, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.managers", NULL, 3075017, 34005, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.pool", NULL, 3109022, 21247, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.popen_fork", NULL, 3130269, 2537, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.popen_forkserver", NULL, 3132806, 2370, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.popen_spawn_posix", NULL, 3135176, 2152, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.process", NULL, 3137328, 9437, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.queues", NULL, 3146765, 9437, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.reduction", NULL, 3156202, 8022, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.resource_sharer", NULL, 3164224, 5214, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.semaphore_tracker", NULL, 3169438, 3751, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.sharedctypes", NULL, 3173189, 6928, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.spawn", NULL, 3180117, 6489, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.synchronize", NULL, 3186606, 11197, NUITKA_BYTECODE_FLAG },
    { "multiprocessing.util", NULL, 3197803, 9958, NUITKA_BYTECODE_FLAG },
    { "netrc", NULL, 3207761, 3774, NUITKA_BYTECODE_FLAG },
    { "nntplib", NULL, 3211535, 33762, NUITKA_BYTECODE_FLAG },
    { "ntpath", NULL, 3245297, 12930, NUITKA_BYTECODE_FLAG },
    { "nturl2path", NULL, 3258227, 1626, NUITKA_BYTECODE_FLAG },
    { "numbers", NULL, 3259853, 12203, NUITKA_BYTECODE_FLAG },
    { "optparse", NULL, 3272056, 47904, NUITKA_BYTECODE_FLAG },
    { "pathlib", NULL, 3319960, 41268, NUITKA_BYTECODE_FLAG },
    { "pdb", NULL, 3361228, 46773, NUITKA_BYTECODE_FLAG },
    { "pickle", NULL, 3408001, 42977, NUITKA_BYTECODE_FLAG },
    { "pickletools", NULL, 3450978, 65368, NUITKA_BYTECODE_FLAG },
    { "pipes", NULL, 3516346, 7814, NUITKA_BYTECODE_FLAG },
    { "pkgutil", NULL, 3524160, 16371, NUITKA_BYTECODE_FLAG },
    { "platform", NULL, 3540531, 27585, NUITKA_BYTECODE_FLAG },
    { "plistlib", NULL, 3568116, 25106, NUITKA_BYTECODE_FLAG },
    { "poplib", NULL, 3593222, 13346, NUITKA_BYTECODE_FLAG },
    { "pprint", NULL, 3606568, 15830, NUITKA_BYTECODE_FLAG },
    { "profile", NULL, 3622398, 13852, NUITKA_BYTECODE_FLAG },
    { "pstats", NULL, 3636250, 22305, NUITKA_BYTECODE_FLAG },
    { "pty", NULL, 3658555, 3894, NUITKA_BYTECODE_FLAG },
    { "py_compile", NULL, 3662449, 7041, NUITKA_BYTECODE_FLAG },
    { "pyclbr", NULL, 3669490, 10384, NUITKA_BYTECODE_FLAG },
    { "pydoc", NULL, 3679874, 84434, NUITKA_BYTECODE_FLAG },
    { "pydoc_data", NULL, 3764308, 157, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "pydoc_data.topics", NULL, 3764465, 411407, NUITKA_BYTECODE_FLAG },
    { "queue", NULL, 4175872, 11483, NUITKA_BYTECODE_FLAG },
    { "random", NULL, 4187355, 19367, NUITKA_BYTECODE_FLAG },
    { "rlcompleter", NULL, 4206722, 5758, NUITKA_BYTECODE_FLAG },
    { "runpy", NULL, 4212480, 7956, NUITKA_BYTECODE_FLAG },
    { "sched", NULL, 4220436, 6532, NUITKA_BYTECODE_FLAG },
    { "secrets", NULL, 4226968, 2195, NUITKA_BYTECODE_FLAG },
    { "selectors", NULL, 4229163, 16959, NUITKA_BYTECODE_FLAG },
    { "shelve", NULL, 4246122, 9517, NUITKA_BYTECODE_FLAG },
    { "shlex", NULL, 4255639, 7001, NUITKA_BYTECODE_FLAG },
    { "shutil", NULL, 4262640, 30556, NUITKA_BYTECODE_FLAG },
    { "signal", NULL, 4293196, 2523, NUITKA_BYTECODE_FLAG },
    { "site", NULL, 4295719, 12884, NUITKA_BYTECODE_FLAG },
    { "smtpd", NULL, 4308603, 26615, NUITKA_BYTECODE_FLAG },
    { "smtplib", NULL, 4335218, 35317, NUITKA_BYTECODE_FLAG },
    { "sndhdr", NULL, 4370535, 6914, NUITKA_BYTECODE_FLAG },
    { "socket", NULL, 4377449, 22032, NUITKA_BYTECODE_FLAG },
    { "socketserver", NULL, 4399481, 24197, NUITKA_BYTECODE_FLAG },
    { "sqlite3", NULL, 4423678, 185, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "sqlite3.dbapi2", NULL, 4423863, 2504, NUITKA_BYTECODE_FLAG },
    { "sqlite3.dump", NULL, 4426367, 1947, NUITKA_BYTECODE_FLAG },
    { "ssl", NULL, 4428314, 39415, NUITKA_BYTECODE_FLAG },
    { "statistics", NULL, 4467729, 18175, NUITKA_BYTECODE_FLAG },
    { "string", NULL, 4485904, 7846, NUITKA_BYTECODE_FLAG },
    { "subprocess", NULL, 4493750, 38390, NUITKA_BYTECODE_FLAG },
    { "sunau", NULL, 4532140, 17222, NUITKA_BYTECODE_FLAG },
    { "symbol", NULL, 4549362, 2576, NUITKA_BYTECODE_FLAG },
    { "symtable", NULL, 4551938, 10456, NUITKA_BYTECODE_FLAG },
    { "sysconfig", NULL, 4562394, 15285, NUITKA_BYTECODE_FLAG },
    { "tabnanny", NULL, 4577679, 6989, NUITKA_BYTECODE_FLAG },
    { "tarfile", NULL, 4584668, 61796, NUITKA_BYTECODE_FLAG },
    { "telnetlib", NULL, 4646464, 18113, NUITKA_BYTECODE_FLAG },
    { "tempfile", NULL, 4664577, 22156, NUITKA_BYTECODE_FLAG },
    { "textwrap", NULL, 4686733, 13623, NUITKA_BYTECODE_FLAG },
    { "this", NULL, 4700356, 1288, NUITKA_BYTECODE_FLAG },
    { "timeit", NULL, 4701644, 11658, NUITKA_BYTECODE_FLAG },
    { "tkinter", NULL, 4713302, 179164, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "tkinter.colorchooser", NULL, 4892466, 1146, NUITKA_BYTECODE_FLAG },
    { "tkinter.commondialog", NULL, 4893612, 1127, NUITKA_BYTECODE_FLAG },
    { "tkinter.constants", NULL, 4894739, 1679, NUITKA_BYTECODE_FLAG },
    { "tkinter.dialog", NULL, 4896418, 1455, NUITKA_BYTECODE_FLAG },
    { "tkinter.dnd", NULL, 4897873, 11197, NUITKA_BYTECODE_FLAG },
    { "tkinter.filedialog", NULL, 4909070, 12281, NUITKA_BYTECODE_FLAG },
    { "tkinter.font", NULL, 4921351, 6198, NUITKA_BYTECODE_FLAG },
    { "tkinter.messagebox", NULL, 4927549, 3063, NUITKA_BYTECODE_FLAG },
    { "tkinter.scrolledtext", NULL, 4930612, 2190, NUITKA_BYTECODE_FLAG },
    { "tkinter.simpledialog", NULL, 4932802, 10557, NUITKA_BYTECODE_FLAG },
    { "tkinter.tix", NULL, 4943359, 83677, NUITKA_BYTECODE_FLAG },
    { "tkinter.ttk", NULL, 5027036, 57861, NUITKA_BYTECODE_FLAG },
    { "trace", NULL, 5084897, 19391, NUITKA_BYTECODE_FLAG },
    { "tracemalloc", NULL, 5104288, 17287, NUITKA_BYTECODE_FLAG },
    { "tty", NULL, 5121575, 1105, NUITKA_BYTECODE_FLAG },
    { "turtle", NULL, 5122680, 130802, NUITKA_BYTECODE_FLAG },
    { "typing", NULL, 5253482, 49939, NUITKA_BYTECODE_FLAG },
    { "unittest", NULL, 5303421, 3022, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "unittest.case", NULL, 5306443, 48084, NUITKA_BYTECODE_FLAG },
    { "unittest.loader", NULL, 5354527, 14280, NUITKA_BYTECODE_FLAG },
    { "unittest.main", NULL, 5368807, 7449, NUITKA_BYTECODE_FLAG },
    { "unittest.mock", NULL, 5376256, 62833, NUITKA_BYTECODE_FLAG },
    { "unittest.result", NULL, 5439089, 7263, NUITKA_BYTECODE_FLAG },
    { "unittest.runner", NULL, 5446352, 6992, NUITKA_BYTECODE_FLAG },
    { "unittest.signals", NULL, 5453344, 2205, NUITKA_BYTECODE_FLAG },
    { "unittest.suite", NULL, 5455549, 9214, NUITKA_BYTECODE_FLAG },
    { "unittest.util", NULL, 5464763, 4532, NUITKA_BYTECODE_FLAG },
    { "urllib", NULL, 5469295, 153, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "urllib.error", NULL, 5469448, 2787, NUITKA_BYTECODE_FLAG },
    { "urllib.parse", NULL, 5472235, 29630, NUITKA_BYTECODE_FLAG },
    { "urllib.request", NULL, 5501865, 72314, NUITKA_BYTECODE_FLAG },
    { "urllib.response", NULL, 5574179, 3260, NUITKA_BYTECODE_FLAG },
    { "urllib.robotparser", NULL, 5577439, 7072, NUITKA_BYTECODE_FLAG },
    { "uu", NULL, 5584511, 3624, NUITKA_BYTECODE_FLAG },
    { "uuid", NULL, 5588135, 22810, NUITKA_BYTECODE_FLAG },
    { "venv", NULL, 5610945, 13639, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wave", NULL, 5624584, 18299, NUITKA_BYTECODE_FLAG },
    { "weakref", NULL, 5642883, 19121, NUITKA_BYTECODE_FLAG },
    { "webbrowser", NULL, 5662004, 16445, NUITKA_BYTECODE_FLAG },
    { "wsgiref", NULL, 5678449, 749, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "wsgiref.handlers", NULL, 5679198, 16158, NUITKA_BYTECODE_FLAG },
    { "wsgiref.headers", NULL, 5695356, 7770, NUITKA_BYTECODE_FLAG },
    { "wsgiref.simple_server", NULL, 5703126, 5216, NUITKA_BYTECODE_FLAG },
    { "wsgiref.util", NULL, 5708342, 5191, NUITKA_BYTECODE_FLAG },
    { "wsgiref.validate", NULL, 5713533, 14687, NUITKA_BYTECODE_FLAG },
    { "xdrlib", NULL, 5728220, 8335, NUITKA_BYTECODE_FLAG },
    { "xml", NULL, 5736555, 717, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom", NULL, 5737272, 5469, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.dom.NodeFilter", NULL, 5742741, 984, NUITKA_BYTECODE_FLAG },
    { "xml.dom.domreg", NULL, 5743725, 2801, NUITKA_BYTECODE_FLAG },
    { "xml.dom.expatbuilder", NULL, 5746526, 27031, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minicompat", NULL, 5773557, 2830, NUITKA_BYTECODE_FLAG },
    { "xml.dom.minidom", NULL, 5776387, 55630, NUITKA_BYTECODE_FLAG },
    { "xml.dom.pulldom", NULL, 5832017, 10503, NUITKA_BYTECODE_FLAG },
    { "xml.dom.xmlbuilder", NULL, 5842520, 12450, NUITKA_BYTECODE_FLAG },
    { "xml.etree", NULL, 5854970, 156, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.etree.ElementInclude", NULL, 5855126, 1592, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementPath", NULL, 5856718, 6360, NUITKA_BYTECODE_FLAG },
    { "xml.etree.ElementTree", NULL, 5863078, 44824, NUITKA_BYTECODE_FLAG },
    { "xml.etree.cElementTree", NULL, 5907902, 198, NUITKA_BYTECODE_FLAG },
    { "xml.parsers", NULL, 5908100, 330, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.parsers.expat", NULL, 5908430, 359, NUITKA_BYTECODE_FLAG },
    { "xml.sax", NULL, 5908789, 3154, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xml.sax._exceptions", NULL, 5911943, 5498, NUITKA_BYTECODE_FLAG },
    { "xml.sax.expatreader", NULL, 5917441, 12406, NUITKA_BYTECODE_FLAG },
    { "xml.sax.handler", NULL, 5929847, 12374, NUITKA_BYTECODE_FLAG },
    { "xml.sax.saxutils", NULL, 5942221, 12827, NUITKA_BYTECODE_FLAG },
    { "xml.sax.xmlreader", NULL, 5955048, 16935, NUITKA_BYTECODE_FLAG },
    { "xmlrpc", NULL, 5971983, 153, NUITKA_BYTECODE_FLAG | NUITKA_PACKAGE_FLAG },
    { "xmlrpc.client", NULL, 5972136, 34559, NUITKA_BYTECODE_FLAG },
    { "xmlrpc.server", NULL, 6006695, 29624, NUITKA_BYTECODE_FLAG },
    { "zipapp", NULL, 6036319, 5814, NUITKA_BYTECODE_FLAG },
    { "zipfile", NULL, 6042133, 49428, NUITKA_BYTECODE_FLAG },
    { NULL, NULL, 0, 0, 0 }
};

void setupMetaPathBasedLoader( void )
{
    static bool init_done = false;

    if ( init_done == false )
    {
        registerMetaPathBasedUnfreezer( meta_path_loader_entries );
        init_done = true;
    }
}
