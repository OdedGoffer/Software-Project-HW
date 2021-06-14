def inertia(vectors, labels, centers):
	inertia = 0
	for i in range(len(vectors)):
		inertia += sum(np.square(vectors[i]-centers[labels[i]]))
	return inertia