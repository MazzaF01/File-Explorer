#include "system.h"

System::System()
	: disk("C:", nullptr), current_location(&disk), m_echo(false)
{
}

bool System::check_valid_name(std::string& name)
{
	//Elimina spazi superflui
	for (int i = name.length() - 1; i >= 0 && name[i] == ' '; --i)
		name.pop_back();

	//Verifica se la stringa � vuota
	if (name.empty()) {
		std::cout << "Errore: nome non valido.\nInserire almeno un carattere.\n\n";
		return false;
	}
	else {
		//Cerca una cartella con "name" e, se trova, stampa un errore 
		Folder* folder_searched = current_location->search_folder(name);
		if (folder_searched != nullptr) {
			std::cout << "Esiste gia' una cartella con questo nome\n";
			return false;
		}
		else {
			//Se non trova una cartella cerca un file
			std::string* file_searched = current_location->search_file(name);
			if (file_searched != nullptr) {
				std::cout << "Esiste gia' un file con questo nome\n";
				return false;
			}
		}

		//Controlla che il nome non abbia i caratteri nella stringa "dont_char"
		std::string dont_char = "\\/:?!\"\'";
		for (int i = 0; i < name.length(); ++i)
			for (int j = 0; j < dont_char.length(); ++j)
				if (name[i] == dont_char[j]) {
					std::cout << "Errore: nome non valido.\nI caratteri \\/:?!\"\' non possono essere inseriti\n\n";
					return false;
				}
	}
	return true;
}

void System::crt_folder(std::string name)
{
	if (check_valid_name(name))
		current_location->folders.push_back(Folder(name, current_location));
};

void System::crt_file(std::string name)
{
	if (check_valid_name(name))
		current_location->files.push_back(name);
}

void System::rename(std::string inst)
{
	std::string oldname, newname;
	void* entity_to_rename = nullptr;
	bool isFolder = true;
	int f = 0;
	for (; inst[f] != ' ' && inst[f] != '\0'; ++f)
		;
	oldname = inst.substr(0,f);
	entity_to_rename = current_location->search_folder(oldname);
	if (entity_to_rename == nullptr) { //Se non trova una cartella cerca un file
		entity_to_rename = current_location->search_file(oldname);
		isFolder = false; //isFile
	}
	if (entity_to_rename == nullptr) {
		std::cout << "Errore: Cartella non trovata";
		return;
	}

	for (; inst[f] == ' ' && inst[f] != '\0'; ++f) //skip spaces
		;
	int i = f;
	for (; inst[f] != ' ' && inst[f] != '\0'; ++f)
		;
	newname = inst.substr(i, f);

	if (check_valid_name(newname))
		if (isFolder)
			((Folder*)entity_to_rename)->setName(newname);
		else
			*(std::string*)entity_to_rename = newname;


}

//Removes a folder, to add: removes a file
void System::remove(std::string name)
{
	bool removed = false;
	
	for (int i = 0; i < current_location->folders.size() && !removed; ++i)
		if (current_location->folders[i].getName() == name) {
			current_location->folders.erase(current_location->folders.begin() + i);
			removed = true;
		}

	for (int i = 0; i < current_location->files.size() && !removed; ++i)
		if (current_location->files[i] == name) {
			current_location->files.erase(current_location->files.begin() + i);
			removed = true;
		}

	if (!removed) std::cout << "Errore: nessun elemento con nome \"" << name << "\" trovato.\n";
}

//Print all the folders and files in the current folder
void System::print_folder()
{
	std::cout << '\n' << folder_to_str(current_location) << '\n';
	for (int i = 0; i < current_location->folders.size(); ++i)
		std::cout << '\\' << current_location->folders[i].getName() << '\n';
	for (int i = 0; i < current_location->files.size(); ++i)
		std::cout << current_location->files[i] << '\n';
	std::cout << '\n';
}

//Print the tree's of all the subfolders, recursive
void System::print_folder_tree(Folder &dir, int n_deepness) //default deepness = 0
{
	if (n_deepness == 0)
		std::cout << "\n";

	std::cout << "\n";
	for (int nspaces = 0; nspaces < n_deepness; ++nspaces)
		std::cout << ' ';
	std::cout << '\\' << dir.getName();
	for (int i = 0; i < dir.folders.size(); ++i)
		print_folder_tree(dir.folders[i], n_deepness+1);
}

//Print all the commands
void System::print_help() {
	std::cout << "\n"
		<< "\tmd/mkdir	\t	Crea una directory.						\n\n"
		<< "\tmf/mkfile		\t	Crea un file.							\n\n"
		<< "\trn/rename		\t	Rinomina una directory o un file.		\n\n"
		<< "\trm/remove		\t	Rimouve una directory o un file.		\n\n"
		<< "\tsd/showdir	\t	Mostra il contenuto di una directory.	\n\n"
		<< "\ttr/tree		\t	Mostra il contenuto di una directory.	\n"
		<<         "\t\t\t\t	e delle sue sottocartelle.				\n\n"
		<< "\tcs/clrscr	\t	Pulisce la console.						\n\n"
		<< "\tcl/color	\t	Cambia il colore delle scritte e		\n"
		<<         "\t\t\t\t	dello sfondo della console.				\n\n"
		<< "\th/help		\t	Mostra tutti i comandi dispoinibili.	\n\n"
		<< "Per navigare tra le cartelle in modo locale puoi:\n"
		<< "\t-Inserire il nome di una sottocartella della posizione corrente per entrarvi\n"
		<< "\t-Usare l'operatore \"..\" per entrare nella cartella padre (Se esiste) della posizione corrente\n";
}

//Return the string of the location (C:\Windows\Users\)
std::string System::folder_to_str(Folder* folder_to_convert)
{
	std::string current_dir = "";
	while (folder_to_convert != nullptr) {
		current_dir = folder_to_convert->getName() + '\\' + current_dir;
		folder_to_convert = folder_to_convert->getPreviousFolder();
	}
	return current_dir;
}

Folder * System::str_to_folder(std::string inst) {
	bool ptr_found = false;
	bool first_folder = true;
	int i = 0, f = 0;
	std::string to_search = "";
	Folder* curr_folder = current_location; //Indirizzo standard = locale

	if (inst == ".")
		ptr_found = true; // "." returns current location

	while (!ptr_found) {
		for (; inst[f] != '\\' && inst[f] != '\0'; ++f)
			;
		to_search = inst.substr(i, f - i);

		if (first_folder && to_search == "C:")
			curr_folder = &disk;
		else if (first_folder && to_search == ".")
			curr_folder = current_location;
		else if (to_search == "..")
			curr_folder = curr_folder->getPreviousFolder();
		else
			curr_folder = curr_folder->search_folder(to_search);

		if (inst[f] == '\0' || curr_folder == nullptr)
			ptr_found = true;
		f += 1;
		i = f;

		first_folder = false;
	}

	return curr_folder;
}
//Check the buffer's dimension of the console and fill it with ' ' char

void System::clr_console()
{
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;

	FillConsoleOutputCharacter(console, ' ', cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

void System::color_console(std::string inst)
{
	bool in_error = false; //input error
	WORD value = 0;
	for (int i = 0; i < inst.length() && !in_error; ++i) {
		value *= 16;
		if (inst[i] >= '0' && inst[i] <= '9')
			value += inst[i] - '0';
		else if (inst[i] >= 'a' && inst[i] <= 'f' || inst[i] >= 'A' && inst[i] <= 'F')
			value += 10 + inst[i] - (inst[i] >= 'a' && inst[i] <= 'f' ? 'a' : 'A');
		else if (inst[i]!=' ')
			in_error = true;
	}
	if (!in_error) {
		HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(console, value);

		CONSOLE_SCREEN_BUFFER_INFO s;
		CONSOLE_CURSOR_INFO c;
		GetConsoleScreenBufferInfo(console, &s);
		GetConsoleCursorInfo(console, &c);
		DWORD written, cells = s.dwSize.X * s.dwSize.Y;

		FillConsoleOutputAttribute(console, s.wAttributes, cells, { 0,0 }, &written);
		SetConsoleCursorInfo(console, &c);
	}
}

void System::change_dir(std::string inst)
{
	if (inst.empty())
		std::cout << folder_to_str(current_location) << "\n\n";
	else
		change_dir( str_to_folder(inst) );
}

//Change current location
void System::change_dir(Folder * newdir)
{
	if (newdir == nullptr)
		std::cout << "Errore: percorso inesistente\n";
	else
		current_location = newdir;
}

void System::echo(std::string inst)
{
	if (inst == "off" || inst == "false")
		m_echo = false;
	else if (inst == "on" || inst == "true")
		m_echo = true;
	else if (inst.empty())
		std::cout << "Echo is " << (m_echo ? "on" : "off") << "\n";
}
