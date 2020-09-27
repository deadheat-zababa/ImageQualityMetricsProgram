#ifndef _PROCESSING_H_
#define _PROCESSING_H_
#include<pthread.h>

typedef struct OptionInfo{
 unsigned char *input1_name;
 unsigned char *input2_name;
 FILE *infile1;
 FILE *infile2;
 unsigned char *output_name;
 FILE *outfile;
 unsigned short width;
 unsigned short height;
 unsigned char format;
 unsigned short frame_number;
 unsigned int start_number;
 unsigned short thread_id;
 char psnr_flag;
 char ssim_flag;
 char show_flag;
 char mode;
 char ssim_mode;
 char thread_number;
 double *ssim_value;
 double *psnr_value;
 pthread_mutex_t mutex;
}optinfo;

//extern pthread_mutex_t mutex;

int tiff_psnr_ssim(optinfo *);
int yuv_psnr_ssim(optinfo *);
//int bmp_psnr_ssim(optinfo *);




#endif
