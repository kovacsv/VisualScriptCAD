import os
import Model
	
class ObjImporter:
	def __init__ (self):
		pass
		
	def Import (self, mtlFile, objFile):
		if not os.path.exists (mtlFile) or not os.path.exists (objFile):
			return None
		model = Model.Model ()
		with open (objFile) as file:
			lines = file.readlines ()
			for line in lines:
				line = line.strip ()
				self.ProcessLine (model, line)
		return model
	
	def ProcessLine (self, model, line):
		splitted = line.split (' ')
		if len (splitted) == 0:
			return
		if splitted[0] == 'v':
			model.AddVertex (float (splitted[1]), float (splitted[2]), float (splitted[3]))
		elif splitted[0] == 'f':
			a = splitted[1].split ('/')[0]
			b = splitted[2].split ('/')[0]
			c = splitted[3].split ('/')[0]
			model.AddTriangle (int (a) - 1, int (b) - 1, int (c) - 1)
