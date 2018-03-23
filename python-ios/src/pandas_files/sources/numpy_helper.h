#ifndef NUMPY_HELPER_H
#define NUMPY_HELPER_H

#include "Python.h"
#include "numpy/ndarrayobject.h"
#include "numpy/arrayscalars.h"
#include "helper.h"

#define PANDAS_FLOAT 0
#define PANDAS_INT 1
#define PANDAS_BOOL 2
#define PANDAS_STRING 3
#define PANDAS_OBJECT 4
#define PANDAS_DATETIME 5

void init_numpy_helper(void);

int infer_type(PyObject* obj);

npy_int64 get_nat(void);
npy_datetime get_datetime64_value(PyObject* obj);

int is_integer_object(PyObject* obj);
int is_float_object(PyObject* obj);
int is_complex_object(PyObject* obj);
int is_bool_object(PyObject* obj);
int is_string_object(PyObject* obj);
int is_datetime64_object(PyObject *obj);
int is_timedelta64_object(PyObject *obj);
int assign_value_1d(PyArrayObject* ap, Py_ssize_t _i, PyObject* v);
PyObject* get_value_1d(PyArrayObject* ap, Py_ssize_t i);
char* get_c_string(PyObject* obj);
PyObject* char_to_string(char* data);
PyObject* sarr_from_data(PyArray_Descr *descr, int length, void* data);
void transfer_object_column(char *dst, char *src, size_t stride,
                            size_t length);
void set_array_owndata(PyArrayObject *ao);
void set_array_not_contiguous(PyArrayObject *ao);

#endif