/////////////////////////////////////////////////////////////////////////////
// Display.h - For Displaying the data to the command line				   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Aakash Patel, Spring 2015                                   //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Application: File Catalogue, 2015                                       //
// Author:     Aakash Patel, Syracuse University,apatel06@syr.edu		   //
/////////////////////////////////////////////////////////////////////////////
/*
*Module Operatipons:
*
*This class is for displaying the output the user on commandline 
*in a presentable way which makes it easy for the user to understand.

*This class has the following methods:
--void get_file_contain(DataStore curr_data)
--void display_duplicate_files(DataStore file_data)
--void display_number_files(DataStore number_file)
--void read_commandline(DataStore files,std::string get_pattern, std::string get_text)

*Required Files:
--DataStore.h

* Maintenance History:
--version 1.0
*/
#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <streambuf>
#include <cstdio>
#include <cerrno>
#include "DataStore.h"

class Display{

public:
	void get_file_contain(DataStore curr_data,std::string searched_text,std::vector<std::string> pattern);   
	void display_duplicate_files(DataStore file_data);
	void display_number_files(DataStore number_file);
	void read_commandline(DataStore files,std::string get_pattern, std::string get_text);
	void get_files(DataStore data, std::vector<std::string>patterns);
};
#endif
