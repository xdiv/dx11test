// gml.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gml.h"


// This is an example of an exported variable
GML_API int ngml=0;

// This is an example of an exported function.
GML_API int fngml(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see gml.h for the class definition
Cgml::Cgml()
{
	return;
}
