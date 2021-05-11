import sys

class vector:

	def __init__(self, vec):
		self.len = len(vec)
		self.vec = vec
		self.S = None

	def __add__(self, c):
		assert self.len == c.len
		return vector(tuple(x+y for x,y in zip(self.vec,c.vec)))

	def __truediv__(self, c):
		return vector(tuple(x/c for x in self.vec))

	def __str__(self):
		res = ""
		for i in self.vec:
			res += str(round(i,4)) + ", "
		return res[:-2]

	def dist(self, c):
		assert self.len == c.len
		return sum((x-y)**2 for x,y in zip(self.vec,c.vec))

	def zero(self):
		c = tuple(0 for i in range(self.len))
		self.vec = c

class S:

	def __init__(self, vec):
		self.S = set({vec})
		self.u = vector(vec.vec)
		vec.S = self

	def __str__(self):
		return str(self.u)

	def remove(self, vec):
		self.S.remove(vec)
		vec.S = None

	def add(self, vec):
		if vec.S != None:
			vec.S.remove(vec)
		self.S.add(vec)
		vec.S = self

	def recentre(self):
		self.u.zero()
		if len(self.S) == 0:
			return
		for v in self.S:
			self.u = self.u + v
		self.u = self.u/len(self.S)

def kmeans(K, MAX_ITER):
	vectors = []
	clusters = []
	for line in sys.stdin:
		line.strip()
		vectors.append(vector(tuple(map(float, line.split(',')))))

	for i in range(K):
		clusters.append(S(vectors[i]))

	i = 0
	while(i < MAX_ITER):
		i+=1
		change = False
		for vec in vectors:
			minS = min(clusters, key = lambda S: vec.dist(S.u))
			if vec.S != minS:
				change = True
				minS.add(vec)

		if change == False:
			break

		for Si in clusters:
			Si.recentre()

	for Si in clusters:
		print(Si)

if __name__ == "__main__":

	agrs = sys.argv
	K = 0;
	MAX_ITER = 200;

	try:
		K = agrs[1]
		if not K.isnumeric():
			print("You must provide an approriate K value")
			exit(0)
		K = int(K)
		if K <= 0:
			print("K must be a positive number")
			exit(0)

	except IndexError:
		print("You must provide a K value")
		exit(0)

	try:
		MAX_ITER = agrs[2]
		if not MAX_ITER.isnumeric():
			print("You must provide an approriate maximum iteration")
			exit(0)
		MAX_ITER = int(MAX_ITER)
		if MAX_ITER <= 0:
			print("Maximum iterations must be positive")
			exit(0)
	except IndexError:
		pass

	kmeans(K,MAX_ITER)
