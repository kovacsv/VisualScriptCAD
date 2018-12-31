import os
import sys
import re
import subprocess
import zipfile

def GetVersionFromInnoSetupScript (scriptPath):
	file = open (scriptPath, 'rb')
	content = file.read ()
	file.close ()
	match = re.search (r'#define MyAppVersion "([0-9\.]+)"', content)
	return match.group (1)

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	requiredFiles = [
		os.path.join ('Build', 'Release', 'VisualScriptCAD.exe'),
		os.path.join ('Build', 'Release', 'CGAL-vc140-mt-4.13.dll'),
		os.path.join ('Build', 'Release', 'libgmp-10.dll'),
		os.path.join ('Build', 'Release', 'libmpfr-4.dll')
	]
	
	for file in requiredFiles:
		if not os.path.exists (file):
			print 'File does not exists: ' + file
			return 1

	packagePath = os.path.abspath (os.path.join ('Build', 'Package'))
	installerScriptPath = os.path.abspath (os.path.join ('Installer', 'VisualScriptCADInstaller.iss'))
	innoSetupPath = 'C:\\Program Files (x86)\\Inno Setup 5\\ISCC.exe'

	version = GetVersionFromInnoSetupScript (installerScriptPath)
	zipPath = os.path.abspath (os.path.join (packagePath, 'VisualScriptCAD v' + version + '.zip'))
	
	subprocess.call ([innoSetupPath, installerScriptPath])
	
	zip = zipfile.ZipFile (zipPath, 'w')
	for file in requiredFiles:
		zip.write (file, os.path.basename (file))
	zip.close ()
	
	return 0
	
sys.exit (Main (sys.argv))
