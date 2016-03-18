/////////////////////////////////////////////////////////////////////////////
// Executive.h - Contains the main function for the application            //
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
*This class is the main class of the File Catalogue.
*It contains the main() method through which we execute all the methods.

*Executive Class has the following methods:
--void search_path(DataStore &data,const std::string& path, patterns file_pattern, bool recurse);
--void commandline();

*Required Files:
--Display.h

* Maintenance History:
--version 1.0
*/



#include<vector>
#include<string>
#include <iostream>
#include "Display.h"
class Executive{

public: 

	typedef std::vector<std::string> files;
	typedef std::vector<std::string> directories;
	typedef std::vector<std::string> patterns;
	void search_path(DataStore &data,const std::string& path, patterns file_pattern, bool recurse);
	void commandline(DataStore d);
	DataStore::PathSet ps;
	DataStore::Store ds;
	DataStore final_data;

private:
	std::string path;
	std::vector<std::string> found_data;
	files get_files;
	patterns get_pattern;
	directories get_directories;
	

};
