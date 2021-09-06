#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../include/matrix_utils.h"
#include "../include/WAM.h"
#include "../include/DDG.h"

matrix* list_to_matrix(PyObject* pList, int n, int m) {
	int i, j;
	double val;
	matrix* mat;
	PyObject* pItem;


	if ((n * m) != PyList_Size(pList)) {
		PyErr_SetString(PyExc_IndexError, "bad matrix size");
		return NULL;
	}
	mat = matrix_init(n, m);
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			pItem = PyList_GetItem(pList, (n * i) + j);
			val = PyFloat_AsDouble(pItem);
			matrix_set(i, j, mat, val);
		}
	}

	if (PyErr_Occurred()) {
		matrix_free(mat);
		return NULL;
	}

	return mat;
}

PyObject* matrix_to_list(matrix* mat) {
	PyObject* pList;
	PyObject* pFloat;
	int n, m, i, j;
	assert(mat);

	n = mat->n;
	m = mat->m;

	pList = PyList_New(n*m);
	for (i = 0; i < m; i++) {
		for (j = 0; j < n; j++) {
			pFloat = Py_BuildValue("d", matrix_get(i, j, mat));
			PyList_SetItem(pList, (n * i) + j, pFloat);
		}
	}

	if (PyErr_Occurred()) {
		return NULL;
	}

	return pList;
}



static PyObject* api_WAM(PyObject* self, PyObject* args) {
	int n, m;
	matrix *input, *output;
	PyObject *pListIn, *pListOut;

	if (!PyArg_ParseTuple(args, "O!ii", &PyList_Type, &pListIn, &n, &m)) return NULL;

	input = list_to_matrix(pListIn, n, m);
	if (!input) return NULL;

	output = WAM(input);
	pListOut = matrix_to_list(output);

	matrix_free(input);
	matrix_free(output);

	return pListOut;
}

static PyObject* api_DDG(PyObject* self, PyObject* args) {
	int n, m;
	matrix *input, *output;
	PyObject *pListIn, *pListOut;

	if (!PyArg_ParseTuple(args, "O!ii", &PyList_Type, &pListIn, &n, &m)) return NULL;

	input = list_to_matrix(pListIn, n, m);
	if (!input) return NULL;

	output = DDG(input);
	pListOut = matrix_to_list(output);

	matrix_free(input);
	matrix_free(output);

	return pListOut;
}

static PyMethodDef spkmeansMethods[] = {
	{
		"fit_WAM", (PyCFunction) api_WAM, METH_VARARGS,
		PyDoc_STR("Weighted Adjacency Matrix. Usage: fit_WAM(data_points, n, m)")
	},
	{
		"fit_DDG", (PyCFunction) api_DDG, METH_VARARGS,
		PyDoc_STR("Diagonal Degree Matrix. Usage: fit_DDG(data_points, n, m)")
	},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	"spkmeans",
	NULL,
	-1,
	spkmeansMethods
};

PyMODINIT_FUNC PyInit_spkmeans(void) {
	PyObject *m;
	m = PyModule_Create(&moduledef);
	if (!m) {
		return NULL;
	}
	return m;
}
