/////////////////////////////////////////////////////////////////////////////
// DataSore.h - Creating a Data structure which store all files information//
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

*This class creates a data structure of type map which stores 
*all the files information. It stores the file name and the paths 
*associated with the files. STL containers and iteratores are used 
*in this class.
	
* This class has following methods:
--new_pathset(void) 
--new_store(void)

*Required Files:

* Maintenance History:
--version 1.0
*/
#ifndef DATASTORE_H
#define DATASTORE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

//DataStore class is the main data structure that is used throughout the File Catalogues
//It has a map having filename as the key and paths as list of iterators.
class DataStore{
public:
	using File = std::string;
	using Paths = std::set<std::string>::iterator;
	using PathSet = std::set<std::string>;
	using Store = std::map <File, std::vector<Paths> >;
	
	PathSet &new_pathset(void);
	Store &new_store(void);
	
private:
	PathSet ps;
	Store store;
};
#endif