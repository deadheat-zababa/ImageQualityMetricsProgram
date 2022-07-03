

typedef struct OptionInfo{
 unsigned char *input1_name;
 unsigned char *input2_name;
 FILE *infile1;
 FILE *infile2;
 unsigned char *output_name;
 FILE *outfile;
 unsigned short width;
 unsigned short height;
 unsigned short thread_id;
 unsigned short frame_number;
 unsigned int start_number;
 int format;
 int psnr_flag;
 int ssim_flag;
 int show_flag;
 int mode;
 int ssim_mode;
 int thread_number;
 int loglevel;
 double *ssim_value;
 double *psnr_value;
 pthread_mutex_t mutex;
}optinfo;