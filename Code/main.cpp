#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>

bool g_bBreak = false;
bool g_bFileMode = false;
bool g_bCommandLoop = false;

bool g_bFileParsed = false;
const std::string g_defaultFile = "default.om";

void ExecuteFile (const char* file)
{
	std::cout << "starting: " << file << "\n";
	ShellExecuteA (NULL, 0, file, NULL, NULL, SW_SHOWNORMAL);
}
void ParseFile (const char* file)
{
	g_bFileParsed = true;
	std::cout << "Reading: " << file << "\n";

	std::ifstream fStream (file);
	if (fStream.is_open ())
	{
		std::string line;
		while (std::getline (fStream, line))
		{
			if (line.empty () || line.front () == '#')
			{
				continue;
			}

			ExecuteFile (line.c_str ());
		}

		fStream.close ();
	}
	else
	{
		std::cerr << "Failed to open the file\n";
	}
}
void HandleFile (const char* file)
{
	if (g_bFileMode)
	{
		ParseFile (file);
	}
	else
	{
		ExecuteFile (file);
	}
}

void PrintHelp ()
{
	std::cout << "OpenMulti is a tool to quickly open multiple applications / files at ones."
		<< "\nUsage "
		<< "\n	OpenMulti [OPTION]... [FILE]..."
		<< "\nCommands:"
		<< "\n	-b"
		<< "\n		toggle the break on end (default: disabled)"
		<< "\n	-c"
		<< "\n		enter command loop"
		<< "\n	-e"
		<< "\n		exit command loop"
		<< "\n	-f"
		<< "\n		toggles file mode which reads the given files to get the execute files"
		<< "\n		one program per line and lines starting with # are ignored"
		<< "\n		if no file was specifeid it reads \"" << g_defaultFile << "\" by default"
		<< "\n	?"
		<< "\n		show help"
		<< "\n";
}
void PrintCNF (char com)
{
	std::cerr << "Command not found: " << com << "\n";
}

void ParseCommand (char com)
{
	switch (com)
	{
	case 'b':
		g_bBreak = !g_bBreak;
		if (g_bBreak)
			std::cout << "Break enabled\n";
		else
			std::cout << "Break disabled\n";
		break;

	case 'c':
		g_bCommandLoop = true;
		std::cout << "Enabling command loop\n";
		break;

	case 'e':
		g_bCommandLoop = false;
		std::cout << "Disabling command loop\n";
		break;

	case 'f':
		g_bFileMode = !g_bFileMode;
		if (g_bFileMode)
			std::cout << "File mode enabled\n";
		else
			std::cout << "File mode disabled\n";
		break;

	case '?':
		PrintHelp ();
		break;

	// ignore whitespace for command loop
	case '\n':
	case ' ':
		break;

	default:
		PrintCNF (com);
		break;
	}
}

int main (int argc, char *argv[])
{
	//parse command-line arguments
	//skip the first argument, which is always the program name
	if (argc > 1)
	{
		for (int i (1); i < argc; ++i)
		{
			switch (argv[i][0])
			{
			// option identifier, so parse options
			case '-':
			{
				std::string str (argv[i] + 1); // add 1 to skip the first '-' character
				for (std::string::iterator it (str.begin ()); it != str.end (); ++it)
				{
					ParseCommand (*it);
				}
				break;
			}
			// the help can be requested without option identifier
			case '?':
				ParseCommand ('?');
				break;

			default:
				HandleFile (argv[i]);
				break;
			}
		}
	}
	else
	{
		// print the help when nothing was specified
		PrintHelp ();
		g_bBreak = true;
	}

	while (g_bCommandLoop)
	{
		char com = ' ';
		std::cout << ":";

		while (com != '\n')
		{
			std::cin.get (com);
			ParseCommand (com);
		}
	}

	// if we enable file mode but there was no file parsed then we parse the default file
	if (g_bFileMode && !g_bFileParsed)
	{
		ParseFile (g_defaultFile.c_str ());
	}

	if (g_bBreak)
		std::cin.get ();

	return 0;
}


