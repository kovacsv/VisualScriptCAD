import os

class Model:
	def __init__ (self):
		self.vertices = []
		self.triangles = []

	def AddVertex (self, x, y, z):
		self.vertices.append ((x, y, z))
	
	def AddTriangle (self, a, b, c):
		self.triangles.append ((a, b, c))
	
	def GetBoundingBox (self):
		minVertex = [float('Inf'), float('Inf'), float('Inf')]
		maxVertex = [float('-Inf'), float('-Inf'), float('-Inf')]
		for vertex in self.vertices:
			minVertex[0] = min (minVertex[0], vertex[0])
			minVertex[1] = min (minVertex[1], vertex[1])
			minVertex[2] = min (minVertex[2], vertex[2])
			maxVertex[0] = max (maxVertex[0], vertex[0])
			maxVertex[1] = max (maxVertex[1], vertex[1])
			maxVertex[2] = max (maxVertex[2], vertex[2])
		return (minVertex, maxVertex)
	
	def GetSurface (self):
		return 0.0
