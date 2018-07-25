#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

#include "Folder.h"

class System
{
public:
	System();

	bool check_valid_name(std::string& name); //Check if name is correct (Creating or renaming folder/file
	void crt_folder(std::string name); //Create a folder in current location
	void crt_file(std::string name); //Create a file in current location

	/* TODO
	void copy()
	void move()
	*/
	void rename(std::string inst);
	void remove(std::string inst);

	void print_folder(); //Print every file and folder of the current location
	void print_folder_tree(Folder &dir, int n_deepness = 0);
	void clr_console(); //Clear the console (only windows)
	void change_dir(std::string inst);
	void change_dir(Folder* newdir); //change the current location
	Folder* get_curr_loc() { return current_location; } //Get the pointer to the current location
	std::string folder_to_str(Folder* folder_to_convert); //return the string of the location: "C:\Folder1\Folder2"
	Folder* str_to_folder(std::string inst);
private:
	Folder disk; //The "C:" Folder
	Folder* current_location; //Pointer to the current location
};

