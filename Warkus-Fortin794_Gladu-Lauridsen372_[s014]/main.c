#include <stdio.h>
int mainBuffer(int, char**);
int mainScanner(int, char**);

int main(int argc, char** argv) {
	//mainBuffer(argc, argv); /* causes a warning as it is undefined and just assuming the return type*/
	mainScanner(argc,argv);
	return 0;
}