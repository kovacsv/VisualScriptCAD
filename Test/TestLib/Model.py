import os
import math

def GetTriangleArea (a, b, c):
	s = (a + b + c) / 2.0;
	areaSquare = s * (s - a) * (s - b) * (s - c);
	if (areaSquare < 0.0):
		return 0.0;
	return math.sqrt (areaSquare);	

def PointDistance (a, b):
	distanceSquare = (a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) + (a[2] - b[2]) * (a[2] - b[2])
	return math.sqrt (distanceSquare)
	
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
		surface = 0.0
		for triangle in self.triangles:
			v1 = self.vertices[triangle[0]]
			v2 = self.vertices[triangle[1]]
			v3 = self.vertices[triangle[2]]
			a = PointDistance (v1, v2)
			b = PointDistance (v2, v3)
			c = PointDistance (v3, v1)
			surface += GetTriangleArea (a, b, c)
		return surface
