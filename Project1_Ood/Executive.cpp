/////////////////////////////////////////////////////////////////////////////
// Executive.cpp - Contains the main function for the application          //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Aakash Patel, Spring 2015                                   //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Application: File Catalogue, 2015                                       //
// Author:     Aakash Patel, Syracuse University,apatel06@syr.edu		   //
/////////////////////////////////////////////////////////////////////////////
#include"Executive.h"
#include "FileSystem.h"

/*
*search_path() method is for creating the datastore map and inserting values in it
* it will uses the FileSystem provided Doctor Fawcett.
*getFiles() and getDirectory() method in FileSystem are used. 
*/
void Executive::search_path(DataStore &data,const std::string& path, Executive::patterns file_pattern, bool recurse){
	std::set<std::string>::iterator pit1 = data.new_pathset().insert(FileSystem::Path::getFullFileSpec(path)).first;
	for (auto pattern : file_pattern){
		files curr_files = FileSystem::Directory::getFiles(path, pattern);
		for (auto one_file : curr_files){
			DataStore::Store::iterator it1 = data.new_store().find(one_file);
			if (it1 == data.new_store().end()){
				std::vector<std::set<std::string>::iterator> temp_vect;
				temp_vect.push_back(pit1);
				data.new_store().insert(std::pair<std::string, std::vector<std::set<std::string>::iterator> >(one_file, temp_vect));
			}
			else
			{				
				if (std::find(it1->second.begin(), it1->second.end(), pit1)==it1->second.end()){
				it1->second.push_back(pit1);
				}
			}
		}
		if (recurse){
			directories temp_directory = FileSystem::Directory::getDirectories(path);
			for (size_t i = 2; i < temp_directory.size(); i++){
				search_path(data, path + temp_directory[i], file_pattern, recurse);
			}
		}
	}
//	final_data = data;
}
/*
*This the main method from which other classes methods are called based on the input 
* found from run.bat
*/
int main(int argc, char * argv[]){
	bool is_recurse = false;
	bool is_duplicate = false;
	bool is_search = false;
	bool is_empty = false;
	DataStore ds;
	Executive exe_object;
	Display disp_object;
	std::string searched_text;
	std::vector<std::string>patterns = { ".h", ".cpp" };
	Executive::patterns all_patterns;
	for (int i = 2; i < argc; i++){
		std::string get_value = argv[i];
		if (std::strcmp(argv[i], "/s") == 0){
			is_recurse = true;
		}
		else if (std::strcmp(argv[i], "/d") == 0){
			is_duplicate = true;
		}
		else if (std::strcmp(argv[i], "/f") == 0){
			is_search = true;
			searched_text = argv[i + 1];
			i = i + 1;
		}
		else
		{
			all_patterns.push_back(get_value);
		}
	}
	if (is_recurse){
		exe_object.search_path(ds,argv[1], all_patterns, true);
	}
	else{
		exe_object.search_path(ds, argv[1], all_patterns, false);
	}
	 if (is_recurse == false && is_duplicate ==false && is_search==false){
		//disp_object.display_number_files(ds);
		}
	 if (is_search == true ){
		 disp_object.get_file_contain(ds, searched_text, patterns);
	}
	if (is_duplicate == true){
		//disp_object.display_duplicate_files(ds);
		disp_object.get_files(ds, patterns);
	}
	//exe_object.commandline(ds);
	}
/*
*This method is for reading the input from commandline from the user.
* the user can enter the text they want to search and the patterns of file
*in which they are looking for the texted search.
*/
void Executive::commandline(DataStore da){
	std::string get_text;
	std::string get_pattern;
	std::cout << "Enter The file pattern: " << get_pattern << std::endl;
	std::getline(std::cin, get_pattern);
	std::cout << "Enter the text to be searched: " << get_text << std::endl;
	std::cin >> get_text;
	Display displayObject;
	displayObject.read_commandline(da, get_pattern, get_text);
}
