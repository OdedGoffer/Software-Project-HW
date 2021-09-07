#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../include/matrix_utils.h"
#include "../include/WAM.h"
#include "../include/DDG.h"
#include "../include/LNORM.h"
#include "../include/jacobi.h"

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

PyObject* double_array_to_list(double* arr, int n) {
	PyObject* pList;
	PyObject* pFloat;
	int i;

	pList = PyList_New(n);
	for (i = 0; i < n; i++) {
		pFloat = Py_BuildValue("d", arr[i]);
		PyList_SetItem(pList, i, pFloat);
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
	int n;
	matrix *input, *output;
	PyObject *pListIn, *pListOut;

	if (!PyArg_ParseTuple(args, "O!i", &PyList_Type, &pListIn, &n)) return NULL;

	input = list_to_matrix(pListIn, n, n);
	if (!input) return NULL;

	output = DDG(input);
	pListOut = matrix_to_list(output);

	matrix_free(input);
	matrix_free(output);

	return pListOut;
}

static PyObject* api_LNORM(PyObject* self, PyObject* args) {
	int n;
	matrix *W, *D, *output;
	PyObject *pListW, *pListD, *pListOut;

	if (!PyArg_ParseTuple(args, "O!O!i", &PyList_Type, &pListW, &PyList_Type, &pListD, &n)) return NULL;

	W = list_to_matrix(pListW, n, n);
	if (!W) return NULL;

	D = list_to_matrix(pListD, n, n);
	if (!D) return NULL;

	output = LNORM(W, D);
	pListOut = matrix_to_list(output);

	matrix_free(W);
	matrix_free(D);
	matrix_free(output);

	return pListOut;
}

static PyObject* api_jacobi(PyObject* self, PyObject* args) {
	int n;
	matrix* input;
	vector_values_pair output;
	PyObject *pListIn, *pListVectors, *pListValues,  *pTuple;

	if (!PyArg_ParseTuple(args, "O!i", &PyList_Type, &pListIn, &n)) return NULL;

	input = list_to_matrix(pListIn, n, n);
	if (!input) return NULL;

	output = jacobi(input);

	pListVectors = matrix_to_list(output.eigenvectors);
	pListValues = double_array_to_list(output.eigenvalues, output.n);

	eigenvectors_free(output);

	pTuple = Py_BuildValue("OO", pListVectors, pListValues);
	Py_DECREF(pListVectors);
	Py_DECREF(pListValues);

	return pTuple;
}


static PyMethodDef spkmeansMethods[] = {
	{
		"fit_WAM", (PyCFunction) api_WAM, METH_VARARGS,
		PyDoc_STR("Weighted Adjacency Matrix. Usage: fit_WAM(mat A, int n, int m)")
	},
	{
		"fit_DDG", (PyCFunction) api_DDG, METH_VARARGS,
		PyDoc_STR("Diagonal Degree Matrix. Usage: fit_DDG(mat A, int n)")
	},
	{
			"fit_LNORM", (PyCFunction) api_LNORM, METH_VARARGS,
			PyDoc_STR("Calculate L-Norm matrix. Usage: fit_DDG(mat W, mat D, int n)")
	},
	{
			"fit_jacobi", (PyCFunction) api_jacobi, METH_VARARGS,
			PyDoc_STR("Use Jacobi method to calculate eigenvectors. Output is transposed so that vectors are rows. Usage: fit_DDG(mat A, int n)")
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
