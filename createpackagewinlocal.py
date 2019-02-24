import os
import sys
import re
import subprocess
import zipfile
import shutil

def PrintInfo (message):
	print 'INFO: ' + message

def PrintError (message):
	print 'ERROR: ' + message

def RunCommand (command):
	process = subprocess.Popen (command, stdout=subprocess.PIPE, shell=True)
	process.communicate ()
	return process.returncode
	
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
	PrintInfo ('Detect version')
	cMakeListsPath = os.path.abspath ('CMakeLists.txt')
	cMakeVersion = GetVersionFromCMakeLists (cMakeListsPath)
	installerScriptPath = os.path.abspath (os.path.join ('Installer', 'VisualScriptCADInstaller.iss'))
	installerVersion = GetVersionFromInnoSetupScript (installerScriptPath)
	if cMakeVersion != installerVersion:
		PrintError ('Different versions in CMakeLists.txt and VisualScriptCADInstaller.iss')
		return None
	return installerVersion

def RunCmake (cmakePath):
	return RunCommand ([cmakePath, '..', '-G', 'Visual Studio 15 2017 Win64'])

def BuildProject (msBuildPath, projectPath, configuration):
	return RunCommand ([msBuildPath, projectPath, '/property:Configuration=' + configuration, '/property:Platform=x64'])
	
def BuildVisualScriptEngine (cmakePath, msBuildPath):
	vseDevKitDir = os.getenv ('VSE_DEVKIT_DIR')
	vseBuildDir = os.path.dirname (vseDevKitDir)
	vseRootDir = os.path.dirname (vseBuildDir)
	if os.path.exists (vseBuildDir):
		shutil.rmtree (vseBuildDir)
	os.mkdir (vseBuildDir)
	prevWorkDir = os.getcwd ()
	os.chdir (vseBuildDir)
	PrintInfo ('Create VisualScriptEngine solution')
	result = RunCmake (cmakePath)
	if result != 0:
		PrintError ('Failed to create VisualScriptEngine solution')
		return False
	vseSolutionPath = os.path.join (vseBuildDir, 'VisualScriptEngine.sln')
	vseSolutionInstallProjectPath = os.path.join (vseBuildDir, 'INSTALL.vcxproj')
	for configuration in ['Debug', 'Release']:
		PrintInfo ('Create VisualScriptEngine ' + configuration + ' build')
		buildResultSln = BuildProject (msBuildPath, vseSolutionPath, configuration)
		buildResultInstall = BuildProject (msBuildPath, vseSolutionInstallProjectPath, configuration)
		if buildResultSln != 0 or buildResultInstall != 0:
			PrintError ('Failed to create VisualScriptEngine ' + configuration + ' Build')
			return False
	os.chdir (prevWorkDir)
	return True

def BuildVisualScriptCAD (cmakePath, msBuildPath):
	vsCADBuildDir = os.path.join (os.getcwd (), 'Build')
	vsCADRootDir = os.path.dirname (vsCADBuildDir)
	if os.path.exists (vsCADBuildDir):
		shutil.rmtree (vsCADBuildDir)
	os.mkdir (vsCADBuildDir)
	prevWorkDir = os.getcwd ()
	os.chdir (vsCADBuildDir)
	PrintInfo ('Create VisualScriptCAD solution')
	result = RunCmake (cmakePath)
	if result != 0:
		PrintError ('Failed to create VisualScriptCAD solution')
		return False
	vsCADSolutionPath = os.path.join (vsCADBuildDir, 'VisualScriptCAD.sln')
	for configuration in ['Debug', 'Release']:
		PrintInfo ('Create VisualScriptCAD ' + configuration + ' build')
		buildResultSln = BuildProject (msBuildPath, vsCADSolutionPath, configuration)
		if buildResultSln != 0:
			PrintError ('Failed to create VisualScriptCAD ' + configuration + ' Build')
			return False
	os.chdir (prevWorkDir)
	return True
	
def CreatePackage (innoSetupPath):
	requiredFiles = [
		os.path.join ('Build', 'Release', 'VisualScriptCAD.exe'),
		os.path.join ('Build', 'Release', 'CGAL-vc140-mt-4.13.dll'),
		os.path.join ('Build', 'Release', 'libgmp-10.dll'),
		os.path.join ('Build', 'Release', 'libmpfr-4.dll')
	]
	
	for file in requiredFiles:
		if not os.path.exists (file):
			PrintError ('File does not exists: ' + file)
			return False

	packagePath = os.path.abspath (os.path.join ('Build', 'Package'))
	installerScriptPath = os.path.abspath (os.path.join ('Installer', 'VisualScriptCADInstaller.iss'))

	version = GetVersionFromInnoSetupScript (installerScriptPath)
	zipPath = os.path.abspath (os.path.join (packagePath, 'VisualScriptCAD_v' + version + '_Portable.zip'))
	
	PrintInfo ('Create VisualScriptCAD installer')
	innoSetupResult = RunCommand ([innoSetupPath, installerScriptPath])
	if innoSetupResult != 0:
		PrintError ('Failed to create VisualScriptCAD installer')
		return False
		
	PrintInfo ('Create VisualScriptCAD portable package')
	zip = zipfile.ZipFile (zipPath, 'w')
	for file in requiredFiles:
		zip.write (file, os.path.basename (file))
	zip.close ()
	return True
	
def Main (argv):
	if len (argv) != 4:
		print 'usage: createpackagewin.py <cmakePath> <msBuildPath> <innoSetupPath>'
		return 1
		
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	cmakePath = argv[1]
	msBuildPath = argv[2] # C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe
	innoSetupPath = argv[3] # C:\\Program Files (x86)\\Inno Setup 5\\ISCC.exe
	
	version = GetVersion ()
	if version == None:
		return 1

	if not BuildVisualScriptEngine (cmakePath, msBuildPath):
		return 1
	
	if not BuildVisualScriptCAD (cmakePath, msBuildPath):
		return 1
	
	if not CreatePackage (innoSetupPath):
		return 1
	
	return 0
	
sys.exit (Main (sys.argv))
