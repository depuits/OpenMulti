# OpenMulti
OpenMulti is a small program to start one or more other applications. It is useful to open files from an application that expects an executables (like the windows 8 start screen).

### Usage
	OpenMulti [OPTION]... [FILE]...
	
### Commands
	-b
		toggle the break on end (default: disabled)
	-c
		enter command loop
	-e
		exit command loop
	-f
		toggles file mode which reads the given files to get the execute files"
		one program per line and lines starting with # are ignored"
		if no file was specifeid it reads \"" << g_defaultFile << "\" by default"
	?"
		show help
		