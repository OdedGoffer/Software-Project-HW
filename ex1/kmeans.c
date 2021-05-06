#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

struct vector;
struct list;
struct S;

struct list {
	struct vector* vector;
	struct list* next;
	struct list* prev;
};

struct S {
	struct list* vectors;
	struct vector* center;
	struct S* next;
};

struct vector {
	struct S* S;
	float* vector;
	int size;
	struct list* list;
};

typedef struct vector vector;
typedef struct list list;
typedef struct S S;

vector* vector_init(float* vals, int N) {
      vector* v;

      if (vals == NULL) {
      	return NULL;
      }
      v = (vector*)malloc(sizeof(vector));
      assert(v != NULL);
      v->vector = (float*)malloc(sizeof(float)*N);
      assert (v->vector != NULL);
      memcpy(v->vector, vals, sizeof(float)*N);
      v->list = (list*)malloc(sizeof(list));
      assert (v->list != NULL);
      v->size = N;
      v->S = NULL;
      return v;
}

S* S_init(vector* v){
	S* Set;

	assert(v!=NULL);
	Set = (S*)malloc(sizeof(S));
	assert(Set != NULL);
	Set->vectors = NULL;
	Set->center = v;
	Set->next = NULL;

	return Set; 
}


S* clusters_init(list* vectors, int K){
	S* head;
	S* curr = NULL;
	S* prev;

	assert(vectors!=NULL);
	head = S_init(vectors->vector);
	prev = head;

	while(K>0){
		if (vectors == NULL){
			printf("%s\n", "Not enough vectors!");
			exit(EXIT_FAILURE);
		}
		curr = S_init(vectors->vector);
		prev->next = curr;
		vectors = vectors->next;
		K--;
	}
	prev->next = curr;

	return head;
}


void vector_free(vector* v){
		assert(v != NULL);
		if (v->vector != NULL){
			free(v->vector);
		}
		if (v->list != NULL){
			free(v->list);
		}
		free(v);
	}

void add(vector* v1, vector* v2) {
	float* vals;
	int i;
	vector* sum;

	assert (v1 != NULL && v2 != NULL);

	vals = malloc((v1->size)*sizeof(float));

	for (i=0; i<v1->size; i++) {
		v1->vector[i] = v1->vector[i] + v2->vector[i];
	}
	free(vals);	
	return;
}

void divide(vector* v, float c) {
	int i;

	assert (v != NULL);
	for (i=0; i<v->size; i++) {
		v->vector[i] = v->vector[i]/c;
	}
}


double dist(vector* v1, vector* v2) {
	double sum;
	
	for (int i=0; i<v1->size; i++) {
		sum += pow(v1->vector[i] - v2->vector[i], 2);
	}
	return sum;
}

void zero(vector* v) {
	for(int i=0; i<v->size; i++) {
		v->vector[i] = 0.0;
	}
}

void recenter(S* S){
	list* current;
	int n = 0;

	assert(S!=NULL);
	current = S->vectors;

	zero(S->center);
	if(current == NULL){
		return;
	}
	while(current!=NULL){
		n++;
		add(S->center,current->vector);
		current = current->next;
	}
	divide(S->center,n);

}

void remove_S(vector* v) {
	list* next;
	list* prev;
	S* S;

	prev = v->list->prev;
	next = v->list->next;
	S = v->S;
	v->S = NULL;

	if (prev == NULL && next == NULL) { 
		S->vectors = NULL;
	} if (prev == NULL) {
		S->vectors = next;
		next->prev = NULL;
	} if (next == NULL) {
		prev->next = NULL;
	} next->prev = prev;
	prev->next = next;
}


void add_to_S(S* S, vector* v) { 
	remove_S(v);
	v->S = S;
	v->list->next = S->vectors;
	S->vectors = v->list;
}


void printVec(vector* v) {
	int i;
	int vec_size;

	vec_size = v->size;
	for (i=0; i<vec_size-1; i++) {
		printf("%.4f, ", v->vector[i]);
	}
	printf("%.4f\n ", v->vector[i]);
}

int getN(char* filename) {

	FILE* fp;
	char buff[255];
	int N = 0;
	char* token;

	fp = fopen(filename, "r");

	if (fp == NULL) {
		exit(EXIT_FAILURE);
	}
	
	fgets(buff, 255, fp);
	token = strtok(buff, ",");
	while (token != NULL) {
		N++;
		token = strtok(NULL, ",");
	}
	fclose(fp);
	return N;
}

list* read_vectors(char* filename){

	list* head;
	list* now;
	list* tmp;
	FILE* fp;
	int N;
	char* token;
	char* line = NULL;
	size_t len = 0;
	ssize_t read;
	float* vals;
	int i = 0;
	vector* v;

	head = (list*)malloc(sizeof(list));
	assert(head != NULL);
	head->prev = NULL;

	now = head;
	N = getN(filename);

	fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("%s\n", "Not a valid filename!");
		exit(EXIT_FAILURE);
	}

	vals = (float*)malloc(N*sizeof(float));
	assert(vals != NULL);

	while ((read = getline(&line, &len, fp)) != -1) {
		i=0;
		token = strtok(line, ",");
		while (token != NULL) {
			float num = atof(token);
			vals[i] = num;
			i++;
			token = strtok(NULL, ",");
		}
		v = vector_init(vals, N);
		now->vector = (struct vector*)v;
		tmp = now;
		now = (list*)malloc(sizeof(list));
		now->prev = (struct list*)tmp;
		tmp->next = (struct list*)now;
	}

	now = (list*)now->prev;
	free(now->next);
	now->next = NULL;
	free(vals);
	free(line);
	fclose(fp);
	return head;
}

int free_vectors(list* head){

	list* now = head;
	list* tmp;

	while(now->next != NULL){
		tmp = now;
		now = (list*)now->next;
		
		free(tmp);
	}
	vector_free((vector*)now->vector);
	free(now);
	return 0;
}

int main () {

	/*
	char filename[100];
	*/
	list* vectors;
	list* current;

/*
	printf("%s\n", "Please enter filename:");
	scanf("%s", &filename);

*/
	vectors = read_vectors("tests/input_1.txt");
	current = vectors;

	while(current != NULL){
		printVec((vector*)current->vector);
		current = (list*)current->next;
	}

	return free_vectors(vectors);
} 