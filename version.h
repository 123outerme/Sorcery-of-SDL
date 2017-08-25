#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 1;
	static const long BUILD  = 7;
	static const long REVISION  = 22;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 341;
	#define RC_FILEVERSION 0,1,7,22
	#define RC_FILEVERSION_STRING "0, 1, 7, 22\0"
	static const char FULLVERSION_STRING [] = "0.1.7.22";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 4;
	

#endif //VERSION_H
