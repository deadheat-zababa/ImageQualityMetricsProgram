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


#define _FILE_OFFSET_BITS 64
#define _LARGEFILE_SOURCE
//pthread_mutex_t mutex;

void setOptionDefault(optinfo *info){
 info->input1_name=NULL;
 info->input2_name=NULL;
 info->output_name = NULL;
 info->width = 3840;
 info->height = 2160;
 info->format = 1; 
 info->frame_number = 1;
 info->start_number = 0;
 info->thread_id = 0;
 info->psnr_flag = 0;
 info->ssim_flag = 0;
 info->show_flag = 1;
 info->mode = 3;
 info->thread_number = 10;
 info->psnr_value = NULL;
 info->ssim_value = NULL;
}

int setOption(int argc,char **argv,optinfo *info){
 int opt;
 char a=0;
 setOptionDefault(info);
 
 while((opt = getopt(argc,argv,"i:n:o:f:s:m:p:v:"))!= -1){
  switch(opt){
   case 'i':
    printf("-i %s\n",optarg);
    if(a==0){
     info->input1_name = (unsigned char*)malloc(sizeof(unsigned char)*strlen(optarg)+1);
     memcpy(info->input1_name,optarg,sizeof(unsigned char)*strlen(optarg));
    }
    else if(a==1){
     info->input2_name = (unsigned char*)malloc(sizeof(unsigned char)*strlen(optarg)+1);
     memcpy(info->input2_name,optarg,sizeof(unsigned char)*strlen(optarg));
    }
    a++;
   break;
   
   case 'n':
    info->frame_number = atoi(optarg);
    printf("-n %d\n",info->frame_number);
   break;

   case 'o':
    info->output_name = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(optarg)+4+strlen("\0")));
    sprintf(info->output_name,"%s.csv",optarg);
    printf("-o %s\n",info->output_name);
   break;
   
   case 'f':
    if(strcmp("yuv",optarg)==0) info->format = 0;
    else if(strcmp("tiff",optarg)==0) info->format = 1;
    printf("format\n");
   break;

   case 's':
    info->start_number = atoi(optarg);
    printf("-s %d\n",atoi(optarg));
   break;

   case 'm':
    printf("-m mode\n m=1:psnr only\n m=2:ssim only\n m=3 between psnr and ssim\n");
    info->mode = atoi(optarg);
    printf("mode:%d\n",info->mode);
   break;

   case 'p':
    info->thread_number = atoi(optarg);
    printf("therad number:%d\n",info->thread_number);
   break;
   case 'v':
    info->show_flag = atoi(optarg);
    if(info->show_flag > 1){
      info->show_flag=1;
    }
    printf("show:%d\n",info->show_flag);
   break;

   case 'h':
    printf("ganbarimasu\n");
    exit(1);
    break;

   default:
    printf("not option\n");
    break;
   
  }
 }
 return 0; 
}
unsigned long getSize(FILE *fp){
 unsigned long size;
 fseek(fp,0,SEEK_END);
 size = ftell(fp);
 fseek(fp,0,SEEK_SET);
 return size;
}


void openyuvimg(optinfo *info){
 FILE *before_file;
 FILE *after_file;
 unsigned char *before_img;
 unsigned char *after_img;
 unsigned long before_size;
 unsigned long after_size;
 int ret;
 
 before_file = fopen(info->input1_name,"rb");
 before_size = getSize(before_file);
 before_img = (unsigned char*)malloc(sizeof(unsigned char)*(before_size));
 ret = fread(before_img,before_size,1,before_file);
 free(info->input1_name);

 if((after_file=fopen(info->input2_name,"rb"))==NULL){
  perror("print error ");
  printf("after file read error\n");
  fflush(stdout);
 }

 after_size = getSize(after_file);
 after_img = (unsigned char*)malloc(sizeof(unsigned char)*(after_size));
 ret = fread(after_img,after_size,1,after_file);
 free(info->input2_name);

 if(before_size != after_size) printf("you are fool\n"); 
 printf("before_size:%ld,after_size:%ld\n",before_size,after_size);
 printf("width:%d,height:%d\n",info->width,info->height);
 info->frame_number= (before_size/(info->width*info->height*3))<<1;
 printf("info->frame_number:%d\n",info->frame_number);

 info->input1_name = before_img;
 info->input2_name = after_img;
 fclose(before_file);
 fclose(after_file);
}

int main(int argc,char **argv){
 optinfo info;
 int i; 
 double average_psnr;
 double average_ssim;
 unsigned char *name1;
 unsigned char *name2;

  printf("number of device:%d\n",acc_get_num_devices(acc_device_nvidia));
 if(argc<=3){
  printf("Error\n");
  printf("Usage:program.exe -i inputfile -i inputfile -f tiff\n");
  fflush(stdout);
  exit(1);  
 }

 if(setOption(argc,argv,&info)==-1) exit(1);
 fflush(stdout);
 
 info.psnr_value = (double*)malloc(sizeof(double)*(info.frame_number+1));
 info.ssim_value = (double *)malloc(sizeof(double)*(info.frame_number+1));
 

 if(info.output_name==NULL){
  name1 = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(info.input1_name)));
  name2 = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(info.input2_name)));
  name1 = strstr(info.input1_name,"/")+1;
  name2 = strstr(info.input2_name,"/")+1;
  info.output_name = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(name1)+strlen(name2)+7));
  sprintf(info.output_name,"%sand%s.csv",name1,name2);
  if((info.outfile=fopen(info.output_name,"a"))==NULL) printf("EEOR\n");
  fflush(stdout);
  fprintf(info.outfile,"count,psnr,ssim,\n");
  free(name1);
  free(name2);
 }

 if((info.outfile=fopen(info.output_name,"a"))==NULL) printf("ERROR\n");
 fflush(stdout);
 if(info.mode==1){
  fprintf(info.outfile,"count,psnr\n");
 }
 else if(info.mode ==2){
   fprintf(info.outfile,"count,ssim,\n");
 }
 else if(info.mode ==3){
  fprintf(info.outfile,"count,psnr,ssim,\n");
 }
 pthread_t *pt;
 pthread_t tmp_pt;
 pt = (pthread_t*)malloc(sizeof(pthread_t)*info.thread_number);
 pthread_mutex_init(&info.mutex,NULL);

 if(info.format == 0) openyuvimg(&info);
 if(info.frame_number>=60){
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
 }
 else{
  tiff_psnr_ssim(&info);
 }

 pthread_mutex_lock(&info.mutex);
 int wait_number = info.thread_number;
 pthread_mutex_unlock(&info.mutex);

 if(info.frame_number>=60){
  for(i=0;i<wait_number;i++){
   tmp_pt = pt[i];
   pthread_join(tmp_pt, NULL);
  }
 }

for(i=0;i<info.frame_number;i++){
 if(info.mode == 1) fprintf(info.outfile,"%d FRAME,%lf,\n",i,info.psnr_value[i]);
 if(info.mode == 2) fprintf(info.outfile,"%d FRAME,%lf,\n",i,info.ssim_value[i]);
 if(info.mode == 3) fprintf(info.outfile,"%d FRAME,%lf,%lf,\n",i,info.psnr_value[i],info.ssim_value[i]);
}

if(info.mode == 1 ||info.mode ==3) average_psnr = (info.psnr_value[info.frame_number]/info.frame_number);
if(info.mode == 2 ||info.mode ==3) average_ssim = (info.ssim_value[info.frame_number]/info.frame_number);
    
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
 fclose(info.outfile);
 
 printf("end\n");
 return 0;
}
