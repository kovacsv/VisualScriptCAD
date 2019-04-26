import os
import sys
import shutil
import subprocess

def Error (message):
	print ('ERROR: ' + message)

def CheckFileSize (filePath, referenceSize):
	fileSize = os.path.getsize (filePath)
	if (abs (fileSize - referenceSize) > 10):
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
	examplesPath = os.path.abspath (os.path.join (currentDir, '..', 'Examples'))
	resultPath = os.path.abspath (os.path.join (currentDir, '..', 'Build', msBuildConfiguration, 'TestResults'))
	if os.path.exists (resultPath):
		shutil.rmtree (resultPath)
	os.makedirs (resultPath)
	
	examples = [
		{
			'name' : 'box_sphere_diff.vsc',
			'mtlSize' : 70,
			'objSize' : 63408
		},
		{
			'name' : 'vscad_logo.vsc',
			'mtlSize' : 105,
			'objSize' : 602882
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
		if not os.path.exists (mtlFilePath):
			Error ('File does not exists: ' + mtlFilePath)
			return 1			
		if not os.path.exists (objFilePath):
			Error ('File does not exists: ' + objFilePath)
			return 1			
		if not CheckFileSize (mtlFilePath, example['mtlSize']):
			Error ('Invalid file size: ' + mtlFilePath)
			return 1			
		if not CheckFileSize (objFilePath, example['objSize']):
			Error ('Invalid file size: ' + objFilePath)
			return 1			

	shutil.rmtree (resultPath)
	return 0
	
sys.exit (Main (sys.argv))
