#include <fstream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <map>
#include <string>

#include "PprzParser.h"

#include <sys/stat.h>

int fileExists (char * fileName)
{
   struct stat buf;
   int i = stat ( fileName, &buf );
     if ( i == 0 )
     {
       return 1;
     }
     return 0;
       
}

int main( int argc, char** argv )
{
	char fileName[128];

	std::cout << "Compiling '" << argv[1] << "' " << std::endl;
	if (fileExists(argv[1]) == 0)
	{
		std::cout << "File '"  << argv[1] <<  "'does not exist\n";
		return 1;
	}
	strcpy(fileName,argv[1]);


	////////////////////////
	// PARSE
	parsepprz(fileName);

	return 0;
}

