#include "imageprocessing.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#define FALSE 0;
#define TRUE 1;

using namespace std;

// Reading image from PGM image
void Imageprocessing::readImage(char **argv){

	FILE * fpIn;
	char * string;
	int doneReading = FALSE;
	char c;

	// Opning the fine mentined in the argument in shell.sh
	fpIn = fopen(argv[1],"rb");
	if(fpIn == NULL){
		cerr<<"Error reading the file";
	}


	string = (char *) calloc(256,1);
	// reading the file type such as P2, P5 and etc
	while(!doneReading && (c = (char) fgetc(fpIn)) != '\0')
		switch(c){
			case 'P':
				c = (char) fgetc(fpIn);
				switch(c){
					case '2':
						this->header=2;
						this->numberOfBands=1;
						cout<<"enters case 2"<<endl;
						//pgm plain
					break;
					case '5':
						this->header=5;
						this->numberOfBands=1;
						//pgm plain
					break;
					case '3':
						this->header=3;
						this->numberOfBands=3;
						//pgm plain
					break;
					case '6':
						this->header=6;
						this->numberOfBands=3;
						//pgm plain
					break;
				}
				c = (char) fgetc(fpIn);
				if(c != 0x0A){
					ungetc(c, fpIn);
				}
				else{
					ungetc(c, fpIn);
					fgets(string, 256, fpIn);
					cout<<"File you enters is "<<string<<"\n";
				}
			break;
			
			case '#':
				fgets(string, 256, fpIn);
				cout<<"File you enters is "<<string<<"\n";
			break;

			case'1':
			case'2':
			case'3':
			case'4':
			case'5':
			case'6':
			case'7':
			case'8':
			case'9':

				// reading the image characteristics into variables
				ungetc(c, fpIn);
				fscanf(fpIn, "%d %d %d", &(this->numberOfColumns), &(this->numberOfRows), &(this->highVal));
				doneReading = TRUE;
				fgetc(fpIn);
				cout<<"reading"<<endl;
			break;
		} // end of while loop

	// Reading the image data to unsigned char image 
	this->totalPixels = this->numberOfRows*this->numberOfColumns*this->numberOfBands;
	this->Image = (unsigned char *) malloc (this->totalPixels);
	this->Output = (unsigned char *) malloc (this->totalPixels*this->numberOfBands);

	fread(this->Image,1,this->totalPixels,fpIn);
	cout<<"Reading the image "<<argv[1]<<" was sucessfull\n";

}

// converting the read image to invert image and save it to invert.pgm
void Imageprocessing::invertImage(char **argv){
	FILE * fpOut;

	// Changing the pixel values buy subtracting it from 255
	for (int i = 0; i < this->totalPixels; i++)
		{
			this->Output[i] = 255- this->Image[i];
		}

	fpOut = fopen(argv[2], "wb");
	if(fpOut == NULL){
		cerr<<"Error while writing the image"<<argv[2]<<"\n";
	}

	// saving the data to a .pgm format file.
	fprintf(fpOut, "P%d\n%d %d\n%d\n",this->header,this->numberOfColumns, this->numberOfRows, this->highVal );
	fwrite(this->Output,1,this->totalPixels,fpOut);
	cout<<"Wrote the image into "<<argv[2]<<"\n";
}

// converting the read image to mirror image and save it to mirror.pgm
void Imageprocessing::mirrorImage(char **argv){
	FILE * fpOut;

	// Changing the pixel values by interchanging the location. 
	for (int y = 0; y < this->numberOfRows; y++)
	{
		for (int x = 0; x < this->numberOfColumns; x++)
		{
			this->Output[(y*this->numberOfColumns)+x] = this->Image[(y*this->numberOfColumns)+(this->numberOfColumns-x)];
		}	
	}

	fpOut = fopen(argv[2], "wb");
	if(fpOut == NULL){
		cerr<<"Error while writing the image"<<argv[2]<<"\n";
	}

	// saving the data to a .pgm format file.	
	fprintf(fpOut, "P%d\n%d %d\n%d\n",this->header,this->numberOfColumns, this->numberOfRows, this->highVal );
	fwrite(this->Output,1,this->totalPixels,fpOut);
	cout<<"Wrote the image into "<<argv[2]<<"\n";
}

// It is used to increase the brightness of the Image
// The amount of brightness to increase is given in the 
// command line, which can be between 0 tp 100.
void Imageprocessing::brightImage(char **argv){
	FILE * fpOut;

	fpOut = fopen(argv[2], "wb");
	if(fpOut == NULL){
		cerr<<"Error while writing the image"<<argv[2]<<"\n";
	}
    int brightness  = stoi(argv[3]);
    if(brightness > 100) brightness = 100;
    if(brightness < 0) brightness = 0;
    cout<<"brightness incresed by "<<brightness<<endl;


    // Changing the pixel values by interchanging the location. 
	for (int y = 0; y < this->totalPixels; y++)
	{
		int newPixels = this->Image[y] + brightness;
		if(newPixels > 255) newPixels = 255;

		this->Output[y] = newPixels;
	}

	// saving the data to a .pgm format file.	
	fprintf(fpOut, "P%d\n%d %d\n%d\n",this->header,this->numberOfColumns, this->numberOfRows, this->highVal );
	fwrite(this->Output,1,this->totalPixels,fpOut);
	cout<<"Wrote the image into "<<argv[2]<<"\n";
}

// This funtion is called tp increase the contr
void Imageprocessing::contrastImage(char **argv){
	FILE * fpOut;


	fpOut = fopen(argv[2], "wb");
	if(fpOut == NULL){
		cerr<<"Error while writing the image"<<argv[2]<<"\n";
	}
    float newContrast  = stoi(argv[3]);
    float contrast = 0.0f;
    float newPixels = 0.0f;
    if(newContrast > 100) newContrast = 100;
    if(newContrast < 0) newContrast = 0;
    cout<<"newContrast incresed by "<<newContrast<<endl;

    // Changing the pixel values by interchanging the location. 
	for (int y = 0; y < this->totalPixels; y++)
	{
		contrast = (100.0+newContrast)/100.0;
		contrast *= contrast;

		newPixels = (this->Image[y])/255.0;
		newPixels -= 0.5;
		newPixels *= contrast;
		newPixels += 0.5;
		newPixels *= 255;
		if(newPixels > 255) newPixels = 255;
		if(newPixels < 0) newPixels = 0;		
		this->Output[y] = newPixels;
	}

	// saving the data to a .pgm format file.	
	fprintf(fpOut, "P%d\n%d %d\n%d\n",this->header,this->numberOfColumns, this->numberOfRows, this->highVal );
	fwrite(this->Output,1,this->totalPixels,fpOut);
	cout<<"Wrote the image into "<<argv[2]<<"\n";
}

void Imageprocessing::saltPepper(char **argv){
	FILE * fpOut;


	fpOut = fopen(argv[2], "wb");
	if(fpOut == NULL){
		cerr<<"Error while writing the image"<<argv[2]<<"\n";
	}

    float Percent  = stoi(argv[3]);

    // Changing the pixel values by interchanging the location. 
	for (int y = 0; y < this->numberOfRows; y++)
	{
		for (int x = 0; x < this->numberOfColumns; x++)
		{
			this->Output[y*this->numberOfColumns+x]=Image[y*this->numberOfColumns+x];
			int randVal = rand()%100;
			if (randVal < Percent)
			{
				int randVal1 = rand()%2;
				if (randVal1 == 0)
				{
					this->Output[y*this->numberOfColumns+x] = 0;
				}else{
					this->Output[y*this->numberOfColumns+x] = 255;
				}
			}
		}
			
	}

	// saving the data to a .pgm format file.	
	fprintf(fpOut, "P%d\n%d %d\n%d\n",this->header,this->numberOfColumns, this->numberOfRows, this->highVal );
	fwrite(this->Output,1,this->totalPixels,fpOut);
	cout<<"Wrote the image into "<<argv[2]<<"\n";
}

void Imageprocessing::binary(char **argv){
	FILE * fpOut;


	fpOut = fopen(argv[2], "wb");
	if(fpOut == NULL){
		cerr<<"Error while writing the image"<<argv[2]<<"\n";
	}

    float level  = stoi(argv[3]);

    // Changing the pixel values by interchanging the location. 
	for (int y = 0; y < this->numberOfRows; y++)
	{
		for (int x = 0; x < this->numberOfColumns; x++)
		{
			if (this->Image[y*this->numberOfColumns+x] < level)
				{
					this->Output[y*this->numberOfColumns+x] = 0;
				}else{
					this->Output[y*this->numberOfColumns+x] = 255;
			}
		}
	}
		


	// saving the data to a .pgm format file.	
	fprintf(fpOut, "P%d\n%d %d\n%d\n",this->header,this->numberOfColumns, this->numberOfRows, this->highVal );
	fwrite(this->Output,1,this->totalPixels,fpOut);
	cout<<"Wrote the image into "<<argv[2]<<"\n";
}