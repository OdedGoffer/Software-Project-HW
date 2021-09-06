#define PY_SSIZE_T_CLEAN
#include <Python.h>

static matrix* args_to_matrix(PyObject* args) {
	int n, m, i, j, size;
	double* values;
	vector* v;
	matrix* res;
	PyObject* pList;
	PyObject* pItem;

	if (!PyArg_ParseTuple(args, "O!ii", &PyList_Type, &pList, &n, &m)) return NULL;

	size = n * m;
	if ( size != PyList_Size(pList)) {
		PyErr_SetString(PrExc_IndexError, "bad matrix size");
		return NULL;
	}

	for (i = 0; i < size; i++) {
		pItem =
	}
}

static PyObject* api_WAM(PyObject* self, PyObject* args) {

}