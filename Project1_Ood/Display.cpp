/////////////////////////////////////////////////////////////////////////////
// Display.cpp - For Displaying the data to the command line			   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Aakash Patel, Spring 2015                                   //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Application: File Catalogue, 2015                                       //
// Author:     Aakash Patel, Syracuse University,apatel06@syr.edu		   //
/////////////////////////////////////////////////////////////////////////////

#include "Display.h"
#include <sstream>
#include <unordered_map>
#include <regex>
/*
*The method get_file_contain is called when the "/f" is found.
*It will show whether the searched string is
*present in any files that are stored in the DataStore.
* And it will display the files in commandline in which the searched string is found
*/
void Display::get_file_contain(DataStore curr_data, std::string searched_text,std::vector<std::string>pattern){
	std::string file_data; std::smatch m; std::regex e(searched_text);
	std::map<std::string,std::string> file_search;
	std::string file_name;
	std::string get_path;
	std::map<DataStore::File, std::vector<DataStore::Paths>>::iterator iterator = curr_data.new_store().begin();
	std::cout << "\n" << std::endl;
	//std::cout << "******* When command is '/f' ********" << std::endl;
	for (; iterator != curr_data.new_store().end(); iterator++) {
		int i = 0;
		file_name = iterator->first;
		for (size_t i = 0; i < pattern.size(); i++){
			if (file_name.find(pattern[i]) != std::string::npos){
				std::vector <DataStore::Paths> second_one = iterator->second;
				std::vector <DataStore::Paths>::iterator second_iterator = second_one.begin();
				for (; second_iterator != second_one.end(); second_iterator++) {

					std::string test = (std::string)*(*second_iterator) + "\\" + file_name;
					std::ifstream in(test, std::ios::in | std::ios::binary);
					if (in)
					{
						file_data = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
					}
					while (std::regex_search(file_data, m, e)) {
						for (auto x : m) /*std::cout << x << " ";*/
						//std::cout << std::endl;
						file_data = m.suffix().str(); file_search.insert(std::pair<std::string, std::string>(file_name, test));
					}
					/*if (file_data.find(searched_text) != std::string::npos){
						
					}*/
				}
			}
		}
	}
	}
void Display::get_files(DataStore curr_data, std::vector<std::string>patterns){
	std::string file_data;
	std::unordered_map<std::string, std::string> file_search;
	std::string file_name;
	std::string get_path;
	std::map<DataStore::File, std::vector<DataStore::Paths>>::iterator iterator = curr_data.new_store().begin();
	std::cout << "\n" << std::endl;
	//std::cout << "******* When command is '/f' ********" << std::endl;
	for (; iterator != curr_data.new_store().end(); iterator++) {
		int i = 0;
		file_name = iterator->first;
		for (size_t i = 0; i < patterns.size(); i++){
			if (file_name.find(patterns[i]) != std::string::npos){
				std::vector <DataStore::Paths> second_one = iterator->second;
				std::vector <DataStore::Paths>::iterator second_iterator = second_one.begin();
				for (; second_iterator != second_one.end(); second_iterator++) {
					std::string test = (std::string)*(*second_iterator) + "\\" + file_name;
					file_search.insert(std::pair<std::string, std::string>(test, file_name));
				}
			}
		}
	}
}
/*
* display_duplicate_files method is called "/d" is found.
*this method will display all the duplicate files along with their paths in commandline
*/
void Display :: display_duplicate_files(DataStore file_data){
	std::map<DataStore::File, std::vector<DataStore::Paths>>::iterator iterator = file_data.new_store().begin();
	std::cout << "\n" << std::endl;
	std::cout << "******* When command is '/d' *******" << std::endl;
	for (; iterator != file_data.new_store().end(); iterator++) {
		std::string same_file = iterator->first;
		std::string check_name;
		std::vector <DataStore::Paths> second_one = iterator->second;
		std::vector <DataStore::Paths>::iterator second_iterator = second_one.begin();
		if (second_one.size() > 1){
			int i = 0;
					if(second_one.size()>1){
				for (; second_iterator != second_one.end(); second_iterator++) {
					std::string test = (std::string)*(*second_iterator);
					std::cout << "\n" << std::endl;
					std::cout << "********************************************************************" << std::endl;
					std::cout << "File =" + same_file << std::endl;
					std::cout << "Path = " + test << std::endl;
					std::cout << "********************************************************************" << std::endl;
									}
			}

		}
	}
}
/*
* display_number_files is called when no options is found.
* It will simply display the number of files and in that directory.
*/
void Display::display_number_files(DataStore number_file){
	int file_count = 0;
	int directory_count = 0;
	directory_count = number_file.new_pathset().size();
	file_count = number_file.new_store().size();
	std::cout << "Total Files = " << file_count << std::endl;
	std::cout << "Total Directory = " << directory_count << std::endl;

}
/*
*read_commandline() method is for reading input from the user.
*the user after the map is created and all options are checked in file catalogue
* is given a option of searching text in a particular file pattern. 
* It will read the file pattern and the text to be searched.
*/
void Display::read_commandline(DataStore files, std::string get_pattern, std::string get_text){
	std::string file_data;
	std::string file_name;
	std::string get_patt;
	bool tag = false;
	std::istringstream iss(get_pattern);
	std::vector<std::string> new_pattern;
	std::copy(std::istream_iterator<std::string>(iss),std::istream_iterator<std::string>(),back_inserter(new_pattern));
	std::map<DataStore::File, std::vector<DataStore::Paths>>::iterator iterator = files.new_store().begin();
	for (; iterator != files.new_store().end(); iterator++) {
		signed int i = 0;
		file_name = iterator->first;
		for (size_t i = 0; i < new_pattern.size(); i++){
			if (file_name.find(new_pattern[i]) != std::string::npos){
				std::vector <DataStore::Paths> second_one = iterator->second;
				std::vector <DataStore::Paths>::iterator second_iterator = second_one.begin();
				for (; second_iterator != second_one.end(); second_iterator++) {

					std::string test = (std::string)*(*second_iterator) + "\\" + file_name;
					std::ifstream in(test, std::ios::in | std::ios::binary);
					if (in)
					{
						file_data = std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
					}
					if (file_data.find(get_text) != std::string::npos){
						std::cout << "\n" << std::endl;
						std::cout << "********************************************************************" << std::endl;
						std::cout << "File =" + file_name << std::endl;
						std::cout << "Path = " + test << std::endl;
						std::cout << "********************************************************************" << std::endl;
						tag = true;
					}
					
				}
			}
			}
				}
	if (tag == false){
		std::cout << "\n" << std::endl;
		std::cout << "Text not Found in any Files in DataStore" << std::endl;
	}
}
//----< test stub >--------------------------------------------
#ifdef TEST_DISPLAY

#include "DataStore.h"
#include <string>
int main(int argc, char* argv[])
{
	Display display;
	DataStore data;
	std::string searched_text="Hello";
	std::string pattern =".h";
	std::string text = ".cpp"
	display.get_file_contain(data,searched_text);   
	display.display_duplicate_files(data);
	display.display_number_files(data);
	display.read_commandline(data, pattern, text);
}
#endif