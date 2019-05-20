/*--- IQA(image quality assessment) ---*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

float fpsnr(uint8_t **y_before,uint8_t **y_after,int WIDHT,int HEIGHT){
    int i,j;
    float sum_denominator=0,sum_numerator=0;
    int psnr =0;

    for( i = 0 ; i < WIDTH ; i++ ){
			for( j = 0 ; j < HEIGHT ; j++ ){
				sum_numerator += ( y_before[i][j] ) * ( y_before[i][j] );
				sum_denominator += ( y_after[i][j] - y_before[i][j] ) * ( y_after[i][j] - y_before[i][j] );
			}
	}
	psnr = 10 * log10( sum_numerator / sum_denominator );
	return psnr;
}

float fssim(uint8_t **y_before,uint8_t **y_after,int WIDHT,int HEIGHT){
	
}