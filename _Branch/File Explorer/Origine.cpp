#include <iostream>
#include <vector>
#include <string>
#include "system.h"

#define LOG(x) std::cout << x << '\n'
void getCommand(System& sys, std::string& _input);

int main() {
	System system;
	std::cout << "UNS Com.Lin. OS [Versione 1.0.001]\n(c) 2018 UNSdev Corporation. Tutti i diritti sono riservati.\n\n";
	std::string input;
	for (;;) {
		std::cout << system.folder_to_str(system.get_curr_loc()) << "> "; //Print the current folder
		std::getline(std::cin, input);
		if (!input.empty()) {
			getCommand(system, input);
			std::cout << '\n';
		}
	}
	return 0;
}

void getCommand(System& sys, std::string& _input) {
	std::string command, instructions;
	int f = 0; //contains the final character's position
	for (; f < _input.length() && _input[f] != ' '; ++f)
		;
	command = _input.substr(0, f);

	for (; _input[f] == ' ' && f < _input.length(); ++f) //Skip th space between command and instructions
		;
	instructions = _input.substr(f, _input.length());

	//for (int i = 0; i < command.length(); ++i)
	//	if (command[i] >= 'A' && command[i] <= 'Z')
	//		command[i] = command[i] - ('A' - 'a');

	if (command == "clr" || command == "clrscr" || command == "cs")
		sys.clr_console();
	else if (command == "showdir" || command == "sd")
		sys.print_folder();
	else if (command == "mkdir" || command == "md")
		sys.crt_folder(instructions); //Instructions: name
	else if (command == "mkfile" || command == "mf")
		sys.crt_file(instructions); //Instructions: name
	else if (command == "rename" || command == "rn")
		sys.rename(instructions); //Instructions: folder/file to rename, newname
	else if (command == "remove" || command == "rm")
		sys.remove(instructions); //Instructions: folder/file to remove
	else if (command == "showtree" || command == "st")
		sys.print_folder_tree(*sys.get_curr_loc());
	else if (command == "cd" || command == "changedir")
		sys.change_dir(instructions);
	else if (command == "..") {
		if (sys.get_curr_loc()->getName() != "C:")
			sys.change_dir(sys.get_curr_loc()->getPreviousFolder());
	}
	else {
		Folder* folder_searched = sys.get_curr_loc()->search_folder(command);

		if (folder_searched != nullptr)
			sys.change_dir(folder_searched);
		else if (command == "..")
			sys.change_dir(command);
		else
			std::cout << "Errore: comando inesistente\nDigitare \"help\" per visualizzare la lista dei comandi.\n";
	}
}