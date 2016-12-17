#ifndef _IMAGEPROCESSING_H_
#define _IMAGEPROCESSING_H_


class Imageprocessing {
public:
	// Variables , Image headers.
	int numberOfColumns, numberOfRows, numberOfBands, highVal, totalPixels, header;
	unsigned char * Image;
	unsigned char * Output;

	// Functions
	void readImage(char **argv);
	void invertImage(char **argv);
	void mirrorImage(char **argv);
	void brightImage(char **argv);
	void contrastImage(char **argv);
	void saltPepper(char **argv);
	void binary(char **argv);
};

#endif