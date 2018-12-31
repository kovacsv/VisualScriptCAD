import os
import sys
import re
import urllib2
import subprocess

def DownloadFile (url, targetPath):
	print ('Downloading ' + url)
	response = urllib2.urlopen (url)
	content = response.read ()
	file = open (targetPath, 'wb')
	file.write (content)
	file.close ()
	
def UnzipFile (zipFile, targetFolder):
	print ('Unzipping ' + zipFile)
	subprocess.call (['7z', 'x', zipFile, '-o' + targetFolder])

def CmakeProject (projectPath, buildFolderName):
	print ('CmakeProject ' + projectPath)
	buildFolder = os.path.join (projectPath, buildFolderName)
	os.makedirs (buildFolder)
	prevWorkDir = os.getcwd ()
	os.chdir (buildFolder)
	subprocess.call (['cmake', '..', '-G', 'Visual Studio 15 2017 Win64'])
	os.chdir (prevWorkDir)
	
def BuildSolution (msBuildPath, solutionPath, configuration):
	print ('Building ' + solutionPath)
	subprocess.call ([msBuildPath, solutionPath, '/property:Configuration=' + configuration, '/property:Platform=x64'])

def InstallwxWidgets (targetFolder, msBuildPath):
	wxWidgetsName = 'wxWidgets-3.1.2'
	wxWidgetsZipUrl = 'https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.2/wxWidgets-3.1.2.7z'
	wxWidgetsZipPath = os.path.join (targetFolder, wxWidgetsName + '.7z')
	wxWidgetsFolderPath = os.path.join (targetFolder, wxWidgetsName)
	wxWidgetsIncludeFolderPath = os.path.join (wxWidgetsFolderPath, 'include')
	wxWidgetsLibFolderPath = os.path.join (wxWidgetsFolderPath, 'lib', 'vc_x64_lib')
	if not os.path.exists (wxWidgetsFolderPath):
		DownloadFile (wxWidgetsZipUrl, wxWidgetsZipPath)
		UnzipFile (wxWidgetsZipPath, wxWidgetsFolderPath)
		solutionPath = os.path.join (wxWidgetsFolderPath, 'build', 'msw', 'wx_vc15.sln')
		BuildSolution (msBuildPath, solutionPath, 'Debug')
		BuildSolution (msBuildPath, solutionPath, 'Release')

def InstallCGAL (targetFolder, msBuildPath):
	cgalName = 'CGAL-4.13'
	cgalZipUrl = 'https://github.com/CGAL/cgal/releases/download/releases%2FCGAL-4.13/CGAL-4.13.zip'
	cgalZipPath = os.path.join (targetFolder, cgalName + '.zip')
	cgalFolderPath = os.path.join (targetFolder, cgalName)
	gmpZipPath = os.path.join (targetFolder, 'gmp-all-CGAL-3.9.zip')
	mpfrZipPath = os.path.join (targetFolder, 'mpfr-all-CGAL-3.9.zip')
	cgalGmpPath = os.path.join (cgalFolderPath, 'auxiliary', 'gmp')
	if not os.path.exists (cgalFolderPath):
		DownloadFile (cgalZipUrl, cgalZipPath)
		DownloadFile ('https://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/x64/GMP/5.0.1/gmp-all-CGAL-3.9.zip', gmpZipPath)
		DownloadFile ('https://cgal.geometryfactory.com/CGAL/precompiled_libs/auxiliary/x64/MPFR/3.0.0/mpfr-all-CGAL-3.9.zip', mpfrZipPath)
		UnzipFile (cgalZipPath, targetFolder)
		UnzipFile (gmpZipPath, cgalGmpPath)
		UnzipFile (mpfrZipPath, cgalGmpPath)
		CmakeProject (cgalFolderPath, 'build')
		solutionPath = os.path.join (cgalFolderPath, 'build', 'CGAL.sln')
		BuildSolution (msBuildPath, solutionPath, 'Debug')
		BuildSolution (msBuildPath, solutionPath, 'Release')
		
def InstallVisualScriptEngine (targetFolder, msBuildPath):
	vseName = 'VisualScriptEngine-master'
	vseZipUrl = 'https://github.com/kovacsv/VisualScriptEngine/archive/master.zip'
	vseZipPath = os.path.join (targetFolder, vseName + '.zip')
	vseFolderPath = os.path.join (targetFolder, vseName)
	if not os.path.exists (vseFolderPath):
		DownloadFile (vseZipUrl, vseZipPath)
		UnzipFile (vseZipPath, targetFolder)
		CmakeProject (vseFolderPath, 'Build')
		solutionPath = os.path.join (vseFolderPath, 'Build', 'VisualScriptEngine.sln')
		BuildSolution (msBuildPath, solutionPath, 'Debug')
		BuildSolution (msBuildPath, solutionPath, 'Release')
	
def Main (argv):
	if len (argv) != 3:
		print 'usage: installdepswin.py <targetFolder> <msBuildPath>'
		return 1
	
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	targetFolder = sys.argv[1]
	msBuildPath = sys.argv[2] # "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\MSBuild.exe"

	if not os.path.exists (targetFolder):
		os.makedirs (targetFolder)
	
	InstallwxWidgets (targetFolder, msBuildPath)
	InstallCGAL (targetFolder, msBuildPath)
	InstallVisualScriptEngine (targetFolder, msBuildPath)
	return 0
	
sys.exit (Main (sys.argv))
