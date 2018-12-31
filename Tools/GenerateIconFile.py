import os
import sys
import re

def Main (argv):
	currentDir = os.path.dirname (os.path.abspath (__file__))
	os.chdir (currentDir)

	bin2cPath = os.path.abspath (os.path.join (currentDir, '..', 'Libs', 'bin2c-1.1', 'bin2c.exe'))
	famfamIconsPath = os.path.abspath (os.path.join (currentDir, '..', 'Libs', 'famfamfam_silk_icons_v013', 'icons'))
	appIconsPath = os.path.abspath (os.path.join (currentDir, 'AppIcons'))
	sourcesPath = os.path.abspath (os.path.join (currentDir, '..', 'Sources', 'VisualScriptCAD', 'Application'))
	hppPath = os.path.join (sourcesPath, 'IconStore.hpp')
	cppPath = os.path.join (sourcesPath, 'IconStore.cpp')
	
	icons = [
		[famfamIconsPath, 'page'],
		[famfamIconsPath, 'page_save'],
		[famfamIconsPath, 'page_go'],
		[famfamIconsPath, 'arrow_undo'],
		[famfamIconsPath, 'arrow_redo'],
		[famfamIconsPath, 'control_end_blue'],
		[famfamIconsPath, 'control_pause_blue'],
		[famfamIconsPath, 'control_play_blue'],
		[famfamIconsPath, 'application_tile_horizontal'],
		[famfamIconsPath, 'application_side_contract'],
		[famfamIconsPath, 'application_side_expand'],
		[famfamIconsPath, 'arrow_in'],
		[appIconsPath, 'appicon16'],
		[appIconsPath, 'appicon32'],
		[appIconsPath, 'appicon48']
	]
	command = bin2cPath
	command += ' -d ' + hppPath
	command += ' -o ' + cppPath
	for icon in icons:
		path = icon[0]
		name = icon[1]
		command += ' -n ' + name + ' ' + os.path.join (path, name + '.png')
	os.system (command)

	for fileName in [hppPath, cppPath]:
		content = ''
		with open (fileName, 'r') as file:
			content = file.read ()
		content = re.sub ('#ifndef .*', '#ifndef ICONSTORE_HPP', content);
		content = re.sub ('#define .*', '#define ICONSTORE_HPP', content);
		content = re.sub ('\/\*[^*]+\*\/\n', '', content);
		if fileName == cppPath:
			content = '#include "IconStore.hpp"\n' + content
		with open (fileName, 'w') as file:
			file.write (content)
	
	return 0
	
sys.exit (Main (sys.argv))
