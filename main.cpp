#include "src/imageprocessing.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv){

	Imageprocessing m;
	
	if(argc < 1){
		cout<<"Incorrect values entered";
		return 0;
	}

	m.readImage(argv);
	// m.invertImage(argv);
	// m.mirrorImage(argv);
	// m.brightImage(argv);
	m.contrastImage(argv);
	return 0;
	hi
}