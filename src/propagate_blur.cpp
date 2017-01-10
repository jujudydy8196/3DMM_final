#include <iostream>
#include <cstdlib>
#include "fileIO.h"
#include "propagate.h"

using namespace std;

void imageUchar2Float( uchar* uImage, float* fImage, const int size );
void imageFloat2Uchar( float* fImage, uchar* uImage, const int size );

int main(int argc, char** argv) {
	
	if(argc != 7) {
		cout << "Usage: propagate_blur <original image> <sparse depth image> <full depth image> <lambda> <radius> <gradient_descent[1] / filtering[2]>" << endl;
		return -1;
	}
	
	uchar* I_sparse;
	// uchar* I_full;
	uchar* I_ori;
	int width, height, numPixel, r, mode;
	float lambda;

	sizePGM(width, height, argv[1]);
	lambda = atof(argv[4]);
	r = atof(argv[5]);
	mode = atoi(argv[6]);
	numPixel = width*height;
	I_sparse = new uchar[numPixel];
	int n = numPixel * 3;
	I_ori = new uchar[n];
	readPGM(I_sparse, argv[2]);
	readPPM(I_ori, argv[1]);

	float *I_ori_float = NULL;
	float *I_sparse_float = NULL;
	uchar *I_out = NULL;
	I_ori_float = new float[numPixel];
	I_sparse_float = new float[numPixel];
	I_out = new uchar[numPixel];
	imageUchar2Float(I_ori, I_ori_float, numPixel);
	imageUchar2Float(I_sparse, I_sparse_float, numPixel);

	Vec<float> result( numPixel );
	if(mode==1) propagate( I_ori_float, I_sparse_float, width, height, lambda, r, result );
	else if(mode==2) propagate2( I_ori_float, I_sparse_float, width, height, lambda, r, result );
	else ;
	imageFloat2Uchar( result.getPtr(), I_out, numPixel );
	writePGM(I_out, width, height, "check_result.pgm");

	delete [] I_sparse;
	delete [] I_ori;
	delete [] I_sparse_float;
	delete [] I_ori_float;
	delete [] I_out;
	
	// system("PAUSE");
	return 0;
}

void imageUchar2Float( uchar* uImage, float* fImage, const int size )
{
    for( size_t i = 0; i < size; ++i ){
        fImage[i] = uImage[i] / 255.0f;
    }
}

void imageFloat2Uchar( float* fImage, uchar* uImage, const int size )
{
    for( size_t i = 0; i < size; ++i ){
        uImage[i] = uchar(fImage[i] * 255.0);
    }
}