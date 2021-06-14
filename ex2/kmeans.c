#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

struct vector;
struct S;

/* Centroid struct. Each centroid has a vector indicating
 * it's center and a list of vectors representing all
 * vectors that belong to it. Each centroid points to the next centroid*/
struct S {
	struct vector* vectors;
	struct vector* center;
	struct S* next;
};

/* Vector struct. Each vector holds points to an array indicating
 * it's value, and holds a size. Each vector points to a centroid S
 * to which it belongs. Vectors also point to the next and previous
 * vectors in the current centroid*/
struct vector {
	struct S* S;
	double* vector;
	int size;
	struct vector* next;
	struct vector* prev;
};

typedef struct vector vector;
typedef struct S S;

/* Initiate vector of size N with value vals. All other attributes
 * of the vector are set to NULL*/
void vector_init(double* vals, int N, vector* v) {
	  
	  assert(v != NULL && vals != NULL);
	  v->vector = (double*)malloc(sizeof(double)*N);
	  assert (v->vector != NULL);
	  memcpy(v->vector, vals, sizeof(double)*N);
	  v->size = N;
	  v->S = NULL;
	  v->next = NULL;
	  v->prev = NULL;
}

/* Sentinal vector of size -1 indicates the end of an array of
 * vectors*/
void sentinal_vector_init(vector* v) {
	  
	  assert(v != NULL);
	  v->size = -1;
	  v->vector = NULL;
	  v->S = NULL;
	  v->next = NULL;
	  v->prev = NULL;
}

void printVec(vector* v) {
	
	int i;
	int vec_size;

	assert(v != NULL);

	vec_size = v->size;
	for (i=0; i<vec_size-1; i++) {
		printf("%.4f, ", v->vector[i]);
	}
	printf("%.4f\n", v->vector[i]);
}

/* Initiate an empty centroid with center at vector*/
S* S_init(vector* v) {

	S* Set;

	assert(v!=NULL);
	Set = (S*)malloc(sizeof(S));
	assert(Set != NULL);
	Set->vectors = v;
	v->S = Set;
	Set->center = (vector*)malloc(sizeof(vector));
	assert(Set->center != NULL);
	vector_init(v->vector,v->size,Set->center);
	Set->next = NULL;

	return Set; 
}

/* Initiate K empty centrois with centers at the first K vectors*/
S* clusters_init(vector* vectors, int K) {
	
	S* head;
	S* curr;
	S* prev;

	assert(vectors!=NULL);
	head = S_init(vectors);
	prev = head;

	while(K>1){
		vectors++;
		if (vectors->size == 0) {
			printf("%s\n", "Not enough vectors!");
			exit(EXIT_FAILURE);
		}
		curr = S_init(vectors);
		prev->next = curr;
		prev = curr;
		K--;
	}
	prev->next = NULL;

	return head;
}

/* Add vector v2 to vector v2 inplace*/
void add(vector* v1, vector* v2) {
	
	int i;

	assert (v1 != NULL && v2 != NULL);

	for (i=0; i<(v1->size); i++) {
		v1->vector[i] = (v1->vector[i]) + (v2->vector[i]);
	}
}

/* Divide vector v by scalar c inplace*/
void divide(vector* v, double c) {
	
	int i;

	assert (v != NULL);
	for (i=0; i<v->size; i++) {
		v->vector[i] = v->vector[i]/c;
	}
}

/* Return distance between two vectors*/
double dist(vector* v1, vector* v2) {
	
	int i;
	double sum = 0;
	
	for (i=0; i<v1->size; i++) {
		sum += pow(v1->vector[i] - v2->vector[i], 2);
	}
	return sum;
}

/* Zero a vector inplace*/
void zero(vector* v) {

	int i;

	for(i=0; i<v->size; i++) {
		v->vector[i] = 0.0;
	}
}

/* Recalculate the center of centroid S, as defined in Kmeans algorithm*/
void recenter(S* S) {
	
	vector* current;
	double n = 0.0;

	assert(S!=NULL);
	current = S->vectors;
	zero(S->center);
	if(current == NULL) {
		return;
	}
	while(current!=NULL) {
		n += 1.0;
		add(S->center,current);
		current = current->next;
	}
	divide(S->center,n);
}

/* Detach vector v from its current centroid*/
void remove_S(vector* v) {
	
	vector* next;
	vector* prev;
	S* S;

	prev = v->prev;
	next = v->next;
	S = v->S;
	if(S == NULL){
		return;
	}
	v->S = NULL;

	if (prev == NULL && next == NULL) { 
		S->vectors = NULL;
	} else if (prev == NULL) {
		S->vectors = next;
		next->prev = NULL;
	} else if (next == NULL) {
		prev->next = NULL;
	} else { 
		next->prev = prev;
		prev->next = next;
	}
	v->next = NULL;
	v->prev = NULL;		
}

/* Add vector v to cluter around centroid S. This DOES NOT update
 * the center of cluster S*/
void add_S(S* S, vector* v) {
	
	remove_S(v);
	v->S = S;
	v->next = S->vectors;
	if(S->vectors != NULL) {
		(S->vectors)->prev = v;
	}
	S->vectors = v;
}

/* Parse input array into an array of vector structs*/
vector* read_vectors(double* num_arr, int N, int d) {

	int k = 2;
	int i = 0;
	int j = 0;
	int p = 0;
	vector* vectors;
	double vals[100];

	vectors = (vector*)malloc(k*sizeof(vector));
	assert(vectors != NULL);

	for (i=0; i<N; i++) {
		for (j=0; j<d; j++) {
			vals[j] = num_arr[(d*i)+j];
		}
		vector_init(vals, d, &vectors[p]);
		p++;

		if(p == k){
			k *= 2;
			vectors = (vector*)realloc(vectors, k*sizeof(vector));
			assert(vectors != NULL);
		}
	}

	sentinal_vector_init(&vectors[p]);

	return vectors;
}

/* Free all vectors in array 'vectors'*/
void free_vectors(vector* vectors) {

	vector* arr;
	arr = vectors;

	while(vectors->size != -1) {
		free(vectors->vector);
		vectors++;
	}
	free(vectors->vector);
	free(arr);
}

/* Return pointer to S, the closest cluster to vector v*/
S* closest_clust(vector* v, S* clusters) {

	S* closest_clust;
	double min_dist;
	vector* curr;

	curr = clusters->center;
	min_dist = dist(v, curr);
	closest_clust = clusters;
	while (clusters->next != NULL) {
		clusters = clusters->next;
		curr = clusters->center;
		if (min_dist > dist(v, curr)) {
			closest_clust = clusters;
			min_dist = dist(v, curr);
		}
	
	}

	return closest_clust;
}

/*Free all clusters */
void free_clusters(S* clusters) {
	S* curr;
	while(clusters != NULL){
		curr = clusters;
		clusters = clusters->next;
		free(curr->center->vector);
		free(curr->center);
		free(curr);
	}
}

/* Classic kmeans algorithm*/
static double* kmeans (double* num_arr, int N, int d, int K, int MAX_ITER) {

	S* clusters;
	S* curr_S;
	S* min_S;
	int i = 0;
	int CHANGE = 0;
	vector* vectors;
	int p = 0;
	double* centroids;

	vectors = read_vectors(num_arr, N, d);

	clusters = clusters_init(vectors, K);

	while(i<MAX_ITER){
		i++;
		CHANGE = 0;
		p = 0;
		while(vectors[p].size != -1){
			min_S = closest_clust(&vectors[p], clusters); /* Find closest cluster*/
			if (vectors[p].S != min_S){ /*if vector does not belong to the closest cluster*/
				CHANGE = 1;
				add_S(min_S, &vectors[p]);
			}
			p++;
		}

		if (CHANGE == 0){
			break;
		}

		curr_S = clusters;
		while(curr_S != NULL){
			recenter(curr_S); /*Recalculate the center of current S*/
			curr_S = curr_S -> next;
		}
	}

	centroids = (double*)malloc(K*d*sizeof(double));
	assert(centroids!=NULL);

	p = 0;
	curr_S = clusters;
	while(curr_S != NULL) {
		memcpy(centroids+(p*d), curr_S->center->vector, d*sizeof(double));
		p++;
		curr_S = curr_S -> next;
	}

	free_clusters(clusters);
	free_vectors(vectors);
	return centroids;
} 

static PyObject* kmeans_capi(PyObject* self, PyObject* args) {
	double* num_arr;
  double* return_arr;
	PyObject *pList;
	PyObject *pItem;
	Py_ssize_t size;
	int N;
	int d;
	int K;
	int MAX_ITER;
	int i;
	PyObject* python_lst;
	PyObject* python_float;

	if (!PyArg_ParseTuple(args, "O!iiii", &PyList_Type, &pList, &N, &d, &K, &MAX_ITER)) {
		PyErr_SetString(PyExc_TypeError, "parameter must be a list.");
		return NULL;
	}

	num_arr = (double*)malloc(N*d*sizeof(double));
	assert(num_arr!=NULL);

	size = PyList_Size(pList);
	for (i=0; i<size; i++) {
		pItem = PyList_GetItem(pList, i);
		if(!PyFloat_Check(pItem)) {
			PyErr_SetString(PyExc_TypeError, "list items must be float.");
			return NULL;
		}
	
		num_arr[i] = PyFloat_AsDouble(pItem);
	}
	return_arr = (double*)malloc(K*d*sizeof(double));
	return_arr = kmeans(num_arr, N, d, K, MAX_ITER);
	python_lst = PyList_New(K*d);

	for (i=0; i<K*d; i++) {
			python_float = Py_BuildValue("d", return_arr[i]);
			PyList_SetItem(python_lst, i, python_float);
	}
	
	free(num_arr);
  free(return_arr);
	return python_lst;
}

static PyMethodDef capiMethods[] = {
	{
		"fit", (PyCFunction) kmeans_capi, METH_VARARGS,
		PyDoc_STR("Implements kmeans algorithm. Usage: fit(data_points, N, d, k, MAX_ITER)")
	},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,
	"kmeanssp",
	NULL,
	-1,
	capiMethods
};

PyMODINIT_FUNC
PyInit_kmeanssp(void)
{
	PyObject *m;
	m = PyModule_Create(&moduledef);
	if (!m) {
		return NULL;
	}
	return m;
}
