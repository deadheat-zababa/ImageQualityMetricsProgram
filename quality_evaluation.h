#ifndef _QUALITY_EVALUATION_H_
#define _QUALITY_EVALUATION_H_
//quality_evaluation.h

int windowsize = 8;

double cal_ssim(float **y_before,float **y_after,int H,int W);
double yuv_cal_ssim(unsigned char **y_before,unsigned char **y_after,int H,int W);
double cal_psnr(float **y_before,float **y_after,int H,int W);
double yuv_cal_psnr(unsigned char **y_before,unsigned char **y_after,int H,int W);
double origin_yuv_cal_ssim(unsigned char **,unsigned char **,int,int,int);

//extern int test;


#endif
