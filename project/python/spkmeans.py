import argparse, sys, os
import spkmeans as c_api
import numpy as np
import pandas as pd

np.random.seed(0)

###############################
# Parse command line arguments
###############################

def parse_arguments():
	parser = argparse.ArgumentParser()
	parser.add_argument('k', type=int, help="Number of clusters to create")
	parser.add_argument('goal', type=str, help="requested algorithm to preform")
	parser.add_argument('file_name', type=str, help="File name")

	args = parser.parse_args()
	if args.k < 0:
		print("K must be non-negative")
		sys.exit()

	if args.goal not in ['spk', 'wam', 'ddg', 'lnorm', 'jacobi']:
		print("Please enter a valid goal")
		sys.exit()

	return (args.k, args.goal, args.file_name)

########
# Utils
########

def print_val(val, end = "\n"):
	if abs(val) < 0.00005:
		val = 0
	print(f"{val:.4f}", end=end)

def print_vector(vec):
	[print_val(val, end=',') for val in vec[:-1]]
	print_val(vec[-1])


def print_matrix(mat, dim, vec_num):
	matrix = [mat[i:i + dim] for i in range(0, vec_num * dim, dim)]
	for row in matrix:
		print_vector(row)

##########################
# Algo 1.1 from Exercise 2
##########################

# Returns the distance between two vectors.
def dist(vec1, vec2):
	return sum(np.square(vec1 - vec2))


# Returns the distance from vector to closest centroid.
def closest_dist(centroids, vector):
	D = dist(vector[:-2], centroids.iloc[0].to_numpy()[:-2])
	for index, centroid in centroids.iterrows():
		tmp_val = dist(vector[:-2], centroid.to_numpy()[:-2])
		if (tmp_val <= D):
			D = tmp_val
	return D


def print_chosen_centroids(centroids):
	print(str(centroids.index.tolist()).strip("[]").replace(" ", ""))


# Returns a single python list of all vectors, where k centroids are first.
def format_T(centroids, T):
	T = T.drop(['Dist', 'Prob'], axis=1)
	centroids = centroids.drop(['Dist', 'Prob'], axis=1)
	T = T.drop([i for i in centroids.index])
	T = pd.concat([centroids, T])
	return T.to_numpy().flatten().tolist()

def format_vectors(new_vectors, vectors):
	vectors = vectors.drop([i for i in new_vectors.index])
	vectors = pd.concat([new_vectors, vectors])
	return vectors.to_numpy().flatten().tolist()


# Algo 1.1
def smart_centroids(T, vectors, K):
	N, d = T.shape
	if N <= K:
		return (None, None, None)

	T['Dist'] = T['Prob'] = 0.0
	idx = np.random.choice(N)
	centroids = T.iloc[[idx]]
	new_vectors = vectors.iloc[[idx]]

	Z = 1
	while (Z < K):
		for i in range(N):
			T['Dist'][i] = closest_dist(centroids, T.iloc[i].to_numpy())
		sumd = sum(T['Dist'])
		for i in range(N):
			T['Prob'][i] = T['Dist'][i] / sumd
		Z += 1
		row_num = np.random.choice(N, p=T['Prob'])
		centroids = centroids.append(T.iloc[row_num])
		new_vectors = new_vectors.append(vectors.iloc[row_num])

	print_chosen_centroids(centroids)
	centroids_arr = format_T(centroids, T)
	vectors_arr = format_vectors(new_vectors, vectors)
	return centroids_arr, vectors_arr

########
# C API
########

def do_wam(mat, dim, vec_num):
	try:
		func_name = 'WAM'
		W = c_api.WAM(mat, dim, vec_num)
	except ValueError as e:
		print(f"{func_name} method returned an error: {e}")
		sys.exit()
	
	print_matrix(W, vec_num, vec_num)


def do_ddg(mat, dim, vec_num):
	try:
		func_name = 'WAM'
		W = c_api.WAM(mat, dim, vec_num)
		func_name = 'DDG'
		D = c_api.DDG(W, vec_num)

	except ValueError as e:
		print(f"{func_name} method returned an error: {e}")
		sys.exit()

	print_matrix(D, vec_num, vec_num)


def do_Lnorm(mat, dim, vec_num):
	try:
		func_name = 'WAM'
		W = c_api.WAM(mat, dim, vec_num)
		func_name = 'DDG'
		D = c_api.DDG(W, vec_num)
		func_name = 'LNORM'
		L = c_api.LNORM(W, D, vec_num)

	except ValueError as e:
		print(f"{func_name} method returned an error: {e}")
		sys.exit()

	print_matrix(L, vec_num, vec_num)


def do_jacobi(mat, dim):
	try:
		eigenvectors, eigenvalues = c_api.jacobi(mat, dim)
	except TimeoutError:
		print("Jacobi method reached maximum iterations with no convergence.")
		sys.exit()
	except ValueError as e:
		print(f"Jacobi method returned an error: {e}")
		sys.exit()

	print_vector(eigenvalues)
	print_matrix(eigenvectors, dim, dim)


def do_spk(mat, dim, vec_num, k):
	try:
		func_name = 'WAM'
		W = c_api.WAM(mat, dim, vec_num)

		func_name = 'DDG'
		D = c_api.DDG(W, vec_num)

		func_name = 'LNORM'
		L = c_api.LNORM(W, D, vec_num)

		func_name = 'jacobi'
		eigenvectors, eigenvalues = c_api.jacobi(L, vec_num)

		func_name = 'eigengap'
		T, k = c_api.eigengap_heuristic(eigenvectors, eigenvalues, k)
		T = [T[i:i + k] for i in range(0, vec_num * k, k)]
		T = pd.DataFrame(T)

		func_name = 'smart_centroids'
		vectors = [mat[i:i + dim] for i in range(0, dim * vec_num, dim)]
		vectors = pd.DataFrame(vectors)
		T, mat = smart_centroids(T, vectors, k)

		func_name = 'kmeans'
		index_list = c_api.kmeans(T, k, vec_num, k)

		func_name = 'calculate_centroids'
		centroids = c_api.calculate_centroids(index_list, mat, dim, vec_num, k)

	except ValueError as e:
		print(f"{func_name} method returned an error: {e}")
		sys.exit()

	print_matrix(centroids, dim, k)

#######
# Main
#######

if __name__ == "__main__":

	k, GOAL, FILENAME = parse_arguments()

	if not os.path.isfile(FILENAME):
		raise FileNotFoundError(f"No such file: {FILENAME}")

	data = pd.read_csv(FILENAME, header=None)
	dim = data.columns.size
	vec_num = data.shape[0]
	mat = data.to_numpy().flatten().tolist()

	if k >= vec_num:
		raise ValueError(f"BAD K: {k}; must be less than vectors count.")

	if GOAL == 'wam':
		do_wam(mat, dim, vec_num)
	elif GOAL == 'ddg':
		do_ddg(mat, dim, vec_num)
	elif GOAL == 'lnorm':
		do_Lnorm(mat, dim, vec_num)
	elif GOAL == 'jacobi':
		do_jacobi(mat, dim)
	elif GOAL == 'spk':
		do_spk(mat, dim, vec_num, k)