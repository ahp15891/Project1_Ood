/////////////////////////////////////////////////////////////////////////////
// DataSore.cpp - Creating a Data structure which store all files information//
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Aakash Patel, Spring 2015                                   //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Application: File Catalogue, 2015                                       //
// Author:     Aakash Patel, Syracuse University,apatel06@syr.edu		   //
/////////////////////////////////////////////////////////////////////////////
#include "DataStore.h"

// This method will store the paths of the directory path.
DataStore:: PathSet &DataStore::new_pathset(void){
	return *&ps;

}
//this is the for storing the main map. stores the filename and its path associated with it in a map.
DataStore::Store &DataStore::new_store(void){
	return *&store;

}
//----< test stub >--------------------------------------------
#ifdef TEST_DATASTORE
#include"DataStore.h"
int main(int argc, char* argv[])
{
	DataStore data;

	data.new_store(void);
	data.new_pathset(void);
}
#endif

