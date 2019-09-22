#ifndef _QUALITY_EVALUATION_H_
#define _QUALITY_EVALUATION_H_
//quality_evaluation.h

#define WINDOW 12
double cal_ssim(float **y_before,float **y_after,int H,int W);
double cal_psnr(float **y_before,float **y_after,int H,int W);

//extern int test;


#endif
