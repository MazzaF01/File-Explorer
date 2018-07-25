#pragma once
#include <string>
#include <vector>

class Folder
{
public:
	Folder(std::string name, Folder* prevFolder);
	std::vector<Folder> folders;
	std::vector<std::string> files;
	Folder* getPreviousFolder() { return m_prevFolder; }
	std::string getName() {	return m_name; }
	void setName(std::string newname) { m_name = newname; }
	Folder* search_folder(std::string _name);
	std::string* search_file(std::string _name);
private:
	std::string m_name;
	Folder* m_prevFolder;
};
