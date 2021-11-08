#ifndef GREENLET_EXCEPTIONS_HPP
#define GREENLET_EXCEPTIONS_HPP

#include <Python.h>
#include <stdexcept>

#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunused-function"
#endif

namespace greenlet {

    class PyErrOccurred : public std::runtime_error
    {
    public:
        PyErrOccurred() : std::runtime_error("")
        {
            assert(PyErr_Occurred());
        }

        PyErrOccurred(PyObject* exc_kind, const char* const msg)
            : std::runtime_error(msg)
        {
            PyErr_SetString(exc_kind, msg);
        }
    };

    class TypeError : public PyErrOccurred
    {
    public:
        TypeError(const char* const what)
            : PyErrOccurred(PyExc_TypeError, what)
        {
        }
    };

    class ValueError : public PyErrOccurred
    {
    public:
        ValueError(const char* const what)
            : PyErrOccurred(PyExc_ValueError, what)
        {
        }
    };

    class AttributeError : public PyErrOccurred
    {
    public:
        AttributeError(const char* const what)
            : PyErrOccurred(PyExc_AttributeError, what)
        {
        }
    };

    /**
     * Calls `Py_FatalError` when constructed, so you can't actually
     * throw this. It just makes static analysis easier.
     */
    class PyFatalError : public std::runtime_error
    {
    public:
        PyFatalError(const char* const msg)
            : std::runtime_error(msg)
        {
            Py_FatalError(msg);
        }
    };

    static inline PyObject*
    Require(PyObject* p)
    {
        if (!p) {
            throw PyErrOccurred();
        }
        return p;
    };

    static inline void
    Require(const int retval)
    {
        if (retval < 0) {
            throw PyErrOccurred();
        }
    };


};
#ifdef __clang__
#    pragma clang diagnostic pop
#endif

#endif
