#include "Folder.h"



Folder::Folder(std::string name, Folder* prevFolder)
	: m_name(name), m_prevFolder(prevFolder)
{
}

Folder * Folder::search_folder(std::string _name)
{
	for (int i = 0; i < folders.size(); ++i)
		if (folders[i].getName() == _name)
			return &folders[i];
	return nullptr;
}

std::string * Folder::search_file(std::string _name)
{
	for (int i = 0; i < files.size(); ++i)
		if (files[i] == _name)
			return &files[i];
	return nullptr;
}