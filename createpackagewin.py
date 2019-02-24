import os
import sys
import re
import subprocess
import zipfile

def PrintInfo (message):
	print 'INFO: ' + message

def PrintError (message):
	print 'ERROR: ' + message

def GetVersionFromCMakeLists (cMakeListsPath):
	file = open (cMakeListsPath, 'rb')
	content = file.read ()
	file.close ()
	match1 = re.search (r'set \(VSCAD_VERSION_1 ([0-9]+)\)', content)
	match2 = re.search (r'set \(VSCAD_VERSION_2 ([0-9]+)\)', content)
	match3 = re.search (r'set \(VSCAD_VERSION_3 ([0-9]+)\)', content)
	return ".".join ([match1.group (1), match2.group (1), match3.group (1)])
	
def GetVersionFromInnoSetupScript (scriptPath):
	file = open (scriptPath, 'rb')
	content = file.read ()
	file.close ()
	match = re.search (r'#define MyAppVersion "([0-9\.]+)"', content)
	return match.group (1)

def GetVersion ():
	cMakeListsPath = os.path.abspath ('CMakeLists.txt')
	cMakeVersion = GetVersionFromCMakeLists (cMakeListsPath)
	installerScriptPath = os.path.abspath (os.path.join ('Installer', 'VisualScriptCADInstaller.iss'))
	installerVersion = GetVersionFromInnoSetupScript (installerScriptPath)
	if cMakeVersion != installerVersion:
		return None
	return installerVersion

def Main (argv):
	if len (argv) != 3:
		print 'usage: createpackagewin.py <innoSetupPath> <msBuildConfiguration>'
		return 1

	innoSetupPath = argv[1] # C:\\Program Files (x86)\\Inno Setup 5\\ISCC.exe
	msBuildConfiguration = argv[2]
		
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)
	
	if msBuildConfiguration != 'Release':
		PrintInfo ('No package creation needed')
		return 0

	version = GetVersion ()
	if version == None:
		PrintError ('Different versions in CMakeLists.txt and VisualScriptCADInstaller.iss')
		return 1
	
	requiredFiles = [
		os.path.join ('Build', msBuildConfiguration, 'VisualScriptCAD.exe'),
		os.path.join ('Build', msBuildConfiguration, 'CGAL-vc140-mt-4.13.dll'),
		os.path.join ('Build', msBuildConfiguration, 'libgmp-10.dll'),
		os.path.join ('Build', msBuildConfiguration, 'libmpfr-4.dll')
	]
	
	for file in requiredFiles:
		if not os.path.exists (file):
			PrintError ('File does not exists: ' + file)
			return 1

	packagePath = os.path.abspath (os.path.join ('Build', 'Package'))
	installerScriptPath = os.path.abspath (os.path.join ('Installer', 'VisualScriptCADInstaller.iss'))
	subprocess.call ([innoSetupPath, installerScriptPath])
	
	zipPath = os.path.abspath (os.path.join (packagePath, 'VisualScriptCAD_v' + version + '_Portable.zip'))
	zip = zipfile.ZipFile (zipPath, 'w')
	for file in requiredFiles:
		zip.write (file, os.path.basename (file))
	zip.close ()
	
	return 0
	
sys.exit (Main (sys.argv))
