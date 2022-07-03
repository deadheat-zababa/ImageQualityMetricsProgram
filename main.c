#include<stdio.h>
#include<stdlib.h>
#include<getopt.h>
#include<math.h>
#include<stdint.h>
#include<string.h>
#include<string.h>
#include<tiffio.h>
#include<openacc.h>
#include<pthread.h>
#include"processing.h"
#include "readconfig.h"
#include "log.h"

#define VIDEOMODE 0
#define YUVMODE 1
#define TIFFMODE 2

#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
//pthread_mutex_t mutex;

logger log;

log.init = logSetting;
log.init(&log,1);

unsigned long getSize(FILE *fp){
 unsigned long size;
 fseek(fp,0,SEEK_END);
 size = ftell(fp);
 fseek(fp,0,SEEK_SET);
 return size;
}

/**
 * @brief 
 * 
 * @param info 
 */
void openyuvimg(optinfo *info){
// FILE *before_file;
// FILE *after_file;
 unsigned char *before_img;
 unsigned char *after_img;
 unsigned long before_size;
 unsigned long after_size;
 int ret;
 
 info->infile1 = fopen(info->input1_name,"rb");
 before_size = getSize(info->infile1);
 free(info->input1_name);

 if((info->infile2=fopen(info->input2_name,"rb"))==NULL){
  perror("print error ");
  printf("after file read error\n");
 }

 after_size = getSize(info->infile2);
 free(info->input2_name);

 if(before_size != after_size){
   printf("you are fool\n"); 
 }

 printf("before_size:%ld,after_size:%ld\n",before_size,after_size);
 printf("width:%d,height:%d\n",info->width,info->height);
 
 if(info->frame_number==0){
  info->frame_number= (before_size/(info->width*info->height*3))<<1;
  printf("info->frame_number:%d\n",info->frame_number);
 }

 printf("filep :%p\n",info->infile1);
}


/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc,char **argv){
 optinfo info;
 int i; 
 double average_psnr;
 double average_ssim;
 unsigned char *name1;
 unsigned char *name2;

 //read config
 if(setOption(argc,argv,&info)==-1){
   log.errorLog("config Error");
   exit(1);
 }

 log.init(&log,info.loglevel);

 if(info.output_name == NULL){
  log.debugLog("Output CSV filename : NULL");

  name1 = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(info.input1_name)));
  name2 = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(info.input2_name)));
  
  name1 = strstr(info.input1_name,"/") + 1;
  name2 = strstr(info.input2_name,"/") + 1;
  
  info.output_name = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(name1)+strlen(name2)+7));
    
  free(name1);
  free(name2);
 }

 if(AUTOMODE==info.format){

 }
 else if(YUVMODE==info.format){

 }
 else if(TIFFMODE==info.format){

 }
 else{

   return;
 }
 
 sprintf(info.output_name,"%sand%s.csv",name1,name2);
 log.infoLog("Output CSV filename : info.output_name");


 if((info.outfile=fopen(info.output_name,"a"))==NULL){
   log.errorLog("ERROR\n");
 }

 fprintf(info.outfile,"\n");
 fprintf(info.outfile,"\n");

 if(info.mode==1){
  fprintf(info.outfile,"count,psnr\n");
 }
 else if(info.mode ==2){
   fprintf(info.outfile,"count,ssim,\n");
 }
 else if(info.mode ==3){
  fprintf(info.outfile,"count,psnr,ssim,\n");
 }
 else{
   log.errorLog("mode error");
   return -1;
 }

 pthread_t *pt;
 pthread_t tmp_pt;

 if(VIDEOMODE== info.format){ 
   openyuvimg();
 }
 else if(YUVMODE == info.format){
   openyuvimg(&info);
 }
 else if(TIFFMODE == info.format){
  if(info.frame_number ==0) {
    info.frame_number = 1;
  }
 }

 printf("filep :%p\n",info.infile1);
 
 if(info.frame_number < info.thread_number){
  info.thread_number = info.frame_number;
 }
 
 info.psnr_value = (double*)malloc(sizeof(double)*(info.frame_number+1));
 info.ssim_value = (double *)malloc(sizeof(double)*(info.frame_number+1));

 pt = (pthread_t*)malloc(sizeof(pthread_t)*info.thread_number);
 pthread_mutex_init(&info.mutex,NULL);

  for(i=0;i<info.thread_number;i++){
   pthread_mutex_lock(&info.mutex);
   if(info.format==0){
    if(pthread_create(&pt[i],NULL,(void*)yuv_psnr_ssim,&info)<0){
     printf("pthread_create error\n");
     fflush(stdout);
     exit(1);
    }
   }
   else if(info.format==1){
    if(pthread_create(&pt[i],NULL,(void*)tiff_psnr_ssim,&info)<0){
     printf("pthread_create error\n");
     fflush(stdout);
     exit(1);
    }
   }
   pthread_mutex_unlock(&info.mutex);
  }

 pthread_mutex_lock(&info.mutex);
 int wait_number = info.thread_number;
 pthread_mutex_unlock(&info.mutex);

  for(i=0;i<wait_number;i++){
   tmp_pt = pt[i];
   pthread_join(tmp_pt, NULL);
  }

printf("info.frame_number:%d\n",info.frame_number);

for(i=0;i<info.frame_number;i++){
 //printf("test.csv %p\n",info.outfile);
 if(info.mode == 1) fprintf(info.outfile,"%d FRAME,%lf,\n",i,info.psnr_value[i]);
 if(info.mode == 2) fprintf(info.outfile,"%d FRAME,%lf,\n",i,info.ssim_value[i]);
 if(info.mode == 3) fprintf(info.outfile,"%d FRAME,%lf,%lf,\n",i,info.psnr_value[i],info.ssim_value[i]);
}

if(info.mode == 1 ||info.mode ==3) {
  average_psnr = (info.psnr_value[info.frame_number]/info.frame_number);
}

if(info.mode == 2 ||info.mode ==3) {
  average_ssim = (info.ssim_value[info.frame_number]/info.frame_number);
}

 fprintf(info.outfile,"\n");
 if(info.mode == 1 ||info.mode ==3){
  fprintf(info.outfile,"SUM(PSNR),%lf,\n",info.psnr_value[info.frame_number]);
  fprintf(info.outfile,"AVE(PSNR),%lf,\n",average_psnr);
 }
 if(info.mode == 2 ||info.mode ==3){
  fprintf(info.outfile,"SUM(SSIM),%lf,\n",info.ssim_value[info.frame_number]);
  fprintf(info.outfile,"AVE(SSIM),%lf,\n",average_ssim);
 }

 pthread_mutex_destroy(&info.mutex);
 free(info.psnr_value);
 free(info.ssim_value);
 fclose(info.outfile);
 fclose(info.infile1);
 fclose(info.infile2); 
 printf("end\n");
 return 0;
}
