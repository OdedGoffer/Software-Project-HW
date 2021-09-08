#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "../include/matrix_utils.h"
#include "../include/WAM.h"
#include "../include/DDG.h"
#include "../include/LNORM.h"
#include "../include/jacobi.h"
#include "../include/kmeans.h"
#include "../include/calculate_centroids.h"
#include "../include/eigengap.h"

matrix* list_to_matrix(PyObject* pList, int n, int m) {
	int i, j;
	double val;
	matrix* mat;
	PyObject* pItem;


	if ((n * m) != PyList_Size(pList)) {
		PyErr_SetString(PyExc_ValueError, "bad matrix size");
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

int* list_to_int_array(PyObject* pList, int m) {
	int i, val;
	int *res;
	PyObject* pItem;

	if (m != PyList_Size(pList)) {
		PyErr_SetString(PyExc_ValueError, "bad list size");
		return NULL;
	}

	res = (int*) malloc(m * sizeof(int));
	assert(res);

	for (i = 0; i < m; i++) {
		pItem = PyList_GetItem(pList, i);
		val = (int) PyLong_AsLong(pItem);
		res[i] = val;
	}

	if (PyErr_Occurred()) {
		free(res);
		return NULL;
	}

	return res;
}

double* list_to_double_array(PyObject* pList, int m) {
	int i;
	double val;
	double* res;
	PyObject* pItem;

	if (m != PyList_Size(pList)) {
		PyErr_SetString(PyExc_ValueError, "bad list size");
		return NULL;
	}

	res = (double*) malloc(m * sizeof(double));
	assert(res);

	for (i = 0; i < m; i++) {
		pItem = PyList_GetItem(pList, i);
		val = PyFloat_AsDouble(pItem);
		res[i] = val;
	}

	if (PyErr_Occurred()) {
		free(res);
		return NULL;
	}

	return res;
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

PyObject* int_array_to_list(int* arr, int n) {
	PyObject* pList;
	PyObject* pFloat;
	int i;

	pList = PyList_New(n);
	for (i = 0; i < n; i++) {
		pFloat = Py_BuildValue("i", arr[i]);
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
	vectors_values_pair output;
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

static PyObject* api_kmeans(PyObject* self, PyObject* args) {
	int n, m, k;
	matrix* input;
	int* output;
	PyObject *pListIn, *pListOut;

	if (!PyArg_ParseTuple(args, "O!iii", &PyList_Type, &pListIn, &n, &m, &k)) return NULL;

	input = list_to_matrix(pListIn, n, m);
	if (!input) return NULL;

	output = kmeans(input, k);

	pListOut = int_array_to_list(output, m);

	return pListOut;
}

static PyObject* api_calculate_centroids(PyObject* self, PyObject* args) {
	int n, m, k;
	int* centroids_arr;
	matrix *vectors, *centroids;
	PyObject *pListVectors, *pListCentroidsArr, *pListCentroids;

	if (!PyArg_ParseTuple(args, "O!O!iii", &PyList_Type, &pListCentroidsArr, &PyList_Type, &pListVectors, &n, &m, &k)) return NULL;

	vectors = list_to_matrix(pListVectors, n ,m);
	if (!vectors) return NULL;
	centroids_arr = list_to_int_array(pListCentroidsArr, m);
	if (!centroids_arr) return NULL;

	centroids = calculate_centroids(centroids_arr, vectors, k);

	pListCentroids = matrix_to_list(centroids);

	matrix_free(vectors);
	matrix_free(centroids);
	free(centroids_arr);

	return pListCentroids;
}

static PyObject* api_eigengap(PyObject* self, PyObject* args) {
	int n, k;
	vectors_values_pair vvp;
	vectors_k_pair vkp;
	PyObject *pListVectors, *pListValues, *pListRes, *pTuple;

	if (!PyArg_ParseTuple(args, "O!O!i", &PyList_Type, &pListVectors, &PyList_Type, &pListValues, &k)) return NULL;

	n = PyList_Size(pListValues);

	vvp.eigenvectors = list_to_matrix(pListVectors, n , n);
	if (!vvp.eigenvectors) return NULL;
	vvp.eigenvalues = list_to_double_array(pListValues, n);
	if (!vvp.eigenvalues) return NULL;
	vvp.n = n;

	vkp = eigengap_heuristic(vvp, k);

	pListRes = matrix_to_list(vkp.vectors);

	pTuple = Py_BuildValue("Oi", pListRes, vkp.k);
	Py_DECREF(pListRes);

	eigenvectors_free(vvp);
	matrix_free(vkp.vectors);

	return pTuple;
}


static PyMethodDef spkmeansMethods[] = {
	{
		"WAM", (PyCFunction) api_WAM, METH_VARARGS,
		PyDoc_STR("Weighted Adjacency Matrix. Usage: WAM(mat A, int n, int m)")
	},
	{
		"DDG", (PyCFunction) api_DDG, METH_VARARGS,
		PyDoc_STR("Diagonal Degree Matrix. Usage: DDG(mat A, int n)")
	},
	{
		 "LNORM", (PyCFunction) api_LNORM, METH_VARARGS,
		 PyDoc_STR("Calculate L-Norm matrix. Usage: DDG(mat W, mat D, int n)")
	},
	{
		 "jacobi", (PyCFunction) api_jacobi, METH_VARARGS,
		 PyDoc_STR("Use Jacobi method to calculate eigenvectors. Output is transposed so that vectors are rows.\n"
				   "Usage: DDG(mat A, int n)")
	},
	{
		 "kmeans", (PyCFunction) api_kmeans, METH_VARARGS,
		 PyDoc_STR("Classic K-Means algorithm. Output in a list of integers, indicating to which cluster each vector belongs.\n"
				   "Usage: kmeans(mat V, int dimension, int num_of_vectors, int k)")
	},
	{
			"calculate_centroids", (PyCFunction) api_calculate_centroids, METH_VARARGS,
					PyDoc_STR("Calculate actual cluster centroids given a list of vectors and a list indicating to which "
							  "cluster each vector belongs.\n"
							  "Usage: calculate_centroids(list centroids_array, mat vectors, int dimension, int num_of_vectors, int k)")
	},
	{
			"eigengap_heuristic", (PyCFunction) api_eigengap, METH_VARARGS,
					PyDoc_STR("Eigengap Heuristic algorithm.\n"
							  "Usage: eigengap_heuristic(mat eigenvectors, list eigenvalues, int k)")
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
