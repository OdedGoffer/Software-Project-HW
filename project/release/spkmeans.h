
/*
 *
 * LOGGER
 *
 */

void invalid_input();

void error_occured();


/*
 *
 * VECTOR UTILS
 *
 */

typedef struct vector {
	double* values;
	int size;
} vector;

/*Creates a new vector object.*/
vector* vector_init(double* values, int size);

vector* vector_init_zero(int size);

void vector_free(vector* v);

/*Adds v2 to v1 in place.*/
void vector_add(vector* v1, vector* v2);

/*Divide all values of v by c in place.*/
void vector_divide(vector* v, double c);

/*Make v equal to zero vector in place.*/
void vector_zero(vector* v);

/*Creates a deep copy of v.*/
vector* vector_copy(vector* v);

/*Euclidean distance between vectors*/
double vector_dist(vector* v1, vector* v2);

void vector_print(vector* v);

/*Returns the sum of all values in vector->values*/
double vector_values_sum(vector* v);

double vector_length (vector* v);



/*
 *
 * MATRIX UTILS
 *
 */

typedef struct matrix {
	vector** rows;
	/*Number of columns.*/
	int n;
	/*Number of rows.*/
	int m;
	/*Size of rows array.*/
	int row_cap;
} matrix;

matrix* matrix_init(int n, int m);

void matrix_free(matrix* mat);

void matrix_add_row(matrix* mat, vector* row);

matrix* matrix_copy(matrix* mat);

void matrix_print(matrix* mat);

/*Raises the value of the diagonal of mat by the power of a*/
void matrix_diagonal_pow(matrix* mat, double a);

/*Returns a pointer to the Identity matrix of size n*/
matrix* matrix_eye(int n);

/*Returns A - B*/
matrix* matrix_subtract(matrix* A, matrix* B);

/*Returns A * B*/
matrix* matrix_mult(matrix* A, matrix* B);

double matrix_off(matrix* mat);

void matrix_set(int i, int j, matrix* mat, double val);

double matrix_get(int i, int j, matrix* mat);

matrix* matrix_transpose(matrix* mat);

void matrix_swap(matrix* mat, int i, int j);

void matrix_slice(matrix* mat, int k);

void matrix_normalize_rows (matrix* mat);



/*
 *
 * PARSE FILE
 *
 */

/*Parse csv file at path 'filename' to matrix.*/
matrix* read_csv(char* filename);



/*
 *
 * WAM
 *
 */

matrix* WAM(matrix* X);



/*
 *
 * DDG
 *
 */

matrix* DDG(matrix* W);



/*
 *
 * LNORM
 *
 */

matrix* LNORM(matrix* W, matrix* D);



/*
 *
 * JACOBI UTILS
 *
 */

/*Type 'ituple' is an integer tuple*/
typedef struct ituple {
	int i;
	int j;
} ituple;

/*Returns 'i', 'j'; indeces of largest off-center element in diagonal matrix.*/
ituple get_largest_off_i_j(matrix* mat);

/*Returns 'P'; transition matrix of A*/
matrix* get_P_and_update_A(matrix* A);



/*
 *
 * JACOBI
 *
 */

/* Type 'vectors_values_pair' holds a matrix of eigenvectors and an
 * array of corresponding eigenvalues.*/
typedef struct vectors_values_pair {
	matrix* eigenvectors;
	double* eigenvalues;
	int n;
} vectors_values_pair;

/* Use Jacobi method to calculate eigenvectors.*/
vectors_values_pair jacobi(matrix* A);

void eigenvectors_free(vectors_values_pair vvp);



/*
 *
 * EIGENGAP
 *
 */

/* Type 'vectors_k_pair' holds a matrix of normalized vectors and the
 * updates value of k (using Eigengap algorithm if required).*/
typedef struct vectors_k_pair {
	matrix* vectors;
	int k;
} vectors_k_pair;

void stableSelectionSort(matrix* mat, double* a, int n);

vectors_k_pair eigengap_heuristic(vectors_values_pair pair, int k);

int get_k(double* eigenvalues, int k, int n);



/*
 *
 * KMEANS
 *
 */

/*Classic K-means algorithm.*/
matrix* kmeans(matrix* inpt, int K);

