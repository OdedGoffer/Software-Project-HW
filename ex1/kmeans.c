#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

struct vector;
struct S;

struct S {
	struct vector* vectors;
	struct vector* center;
	struct S* next;
};

struct vector {
	struct S* S;
	double* vector;
	int size;
	struct vector* next;
	struct vector* prev;
};

typedef struct vector vector;
typedef struct S S;

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

void add(vector* v1, vector* v2) {
	
	int i;

	assert (v1 != NULL && v2 != NULL);

	for (i=0; i<(v1->size); i++) {
		v1->vector[i] = (v1->vector[i]) + (v2->vector[i]);
	}
}

void divide(vector* v, double c) {
	
	int i;

	assert (v != NULL);
	for (i=0; i<v->size; i++) {
		v->vector[i] = v->vector[i]/c;
	}
}

double dist(vector* v1, vector* v2) {
	
	int i;
	double sum = 0;
	
	for (i=0; i<v1->size; i++) {
		sum += pow(v1->vector[i] - v2->vector[i], 2);
	}
	return sum;
}

void zero(vector* v) {

	int i;

	for(i=0; i<v->size; i++) {
		v->vector[i] = 0.0;
	}
}

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


void add_S(S* S, vector* v) {
	
	remove_S(v);
	v->S = S;
	v->next = S->vectors;
	if(S->vectors != NULL) {
		(S->vectors)->prev = v;
	}
	S->vectors = v;
}

vector* read_vectors() {

	int k = 2;
	int N;
	int i = 0;
	int p = 0;
	vector* vectors;
	char c;
	double val;
	double vals[100];

	vectors = (vector*)malloc(k*sizeof(vector));
	assert(vectors != NULL);

	while (scanf("%lf%c", &val, &c) == 2) {
		if (c == ',') {
			vals[i] = val;
			i++;
		} else if (c == '\n') {
			vals[i] = val;
			if (p == 0) {
				N = i+1;
			}
			vector_init(vals, N, &vectors[p]);
			p++;
			i = 0;
		}

		if(p == k){
			k *= 2;
			vectors = (vector*)realloc(vectors, k*sizeof(vector));
			assert(vectors != NULL);
		}
	}

	sentinal_vector_init(&vectors[p]);

	return vectors;
}

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

int main (int argc, char* argv[]) {

	int K;
	int MAX_ITER;
	S* clusters;
	S* curr_S;
	S* min_S;
	int i = 0;
	int CHANGE = 0;
	vector* vectors;
	int p = 0;

	K = atoi(argv[1]);
	MAX_ITER = argc == 3 ? atoi(argv[2]) : 200;

	vectors = read_vectors();

	clusters = clusters_init(vectors, K);
	curr_S = clusters;

	while(i<MAX_ITER){
		i++;
		CHANGE = 0;
		p = 0;
		while(vectors[p].size != -1){
			min_S = closest_clust(&vectors[p], clusters);
			if (vectors[p].S != min_S){
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
			recenter(curr_S);
			curr_S = curr_S -> next;
		}
	}

	curr_S = clusters;
	while(curr_S != NULL){
		printVec(curr_S->center);
		curr_S = curr_S -> next;
	}

	free_clusters(clusters);

	free_vectors(vectors);

	return 0;
} 