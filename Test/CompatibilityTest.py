import os
import sys
import shutil
import subprocess
from TestLib import ObjImporter

def Error (message):
	print ('ERROR: ' + message)

def IsEqual (current, reference):
	if abs (current - reference) > 0.001:
		return False
	return True
	
def IsEqualPoint (current, reference):
	for i in range (0, 3):
		if not IsEqual (current[i], reference[i]):
			return False
	return True
	
def IsEqualBox (current, reference):
	if not IsEqualPoint (current[0], reference[0]):
		return False
	if not IsEqualPoint (current[1], reference[1]):
		return False
	return True
	
def Main (argv):
	if len (argv) != 2:
		print ('usage: compatiblitytest.py <msBuildConfiguration>')
		return 1
	
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	msBuildConfiguration = sys.argv[1]
	cliPath = os.path.abspath (os.path.join (currentDir, '..', 'Build', msBuildConfiguration, 'VisualScriptCADCLI.exe'))
	examplesPath = os.path.abspath (os.path.join (currentDir, '..', 'Build', msBuildConfiguration, 'Examples'))
	resultPath = os.path.abspath (os.path.join (currentDir, '..', 'Build', msBuildConfiguration, 'TestResults'))

	if os.path.exists (resultPath):
		shutil.rmtree (resultPath)
	os.makedirs (resultPath)
	
	examples = [
		{
			'name' : 'simple_box.vsc',
			'boundingBox' : ([0.0, 0.0, 0.0], [3.0, 2.0, 1.0]),
			'surface' : 22.0
		},
		{
			'name' : 'lego_brick.vsc',
			'boundingBox' : ([0.0, 0.0, 0.0], [1.6, 2.4, 1.12]),
			'surface' : 26.684
		},
		{
			'name' : 'box_sphere_diff.vsc',
			'boundingBox' : ([0.0, 0.0, 0.0], [1.0, 1.0, 1.0]),
			'surface' : 5.501
		},
		{
			'name' : 'vscad_logo.vsc',
			'boundingBox' : ([-0.5, -0.5, -0.5], [0.5, 0.5, 0.5]),
			'surface' : 5.801
		}
	]
	
	for example in examples:
		exampleName = example['name']
		print ('open_export_obj: ' + exampleName)
		examplePath = os.path.join (examplesPath, exampleName)
		modelName = exampleName[0 : exampleName.find ('.')]
		result = subprocess.call ([cliPath, 'open_export_obj', examplePath, resultPath, modelName])
		mtlFilePath = os.path.join (resultPath, modelName + '.mtl')
		objFilePath = os.path.join (resultPath, modelName + '.obj')
		importer = ObjImporter.ObjImporter ()
		model = importer.Import (mtlFilePath, objFilePath)
		if model == None:
			Error ('Failed to import model')
			return 1
		boundingBox = model.GetBoundingBox ()
		if not IsEqualBox (boundingBox, example['boundingBox']):
			Error ('Bounding box checking failed: ' + str (boundingBox))
			return 1
		surface = model.GetSurface ()
		if not IsEqual (surface, example['surface']):
			Error ('Surface checking failed: ' + str (surface))
			return 1

	shutil.rmtree (resultPath)
	return 0
	
sys.exit (Main (sys.argv))
