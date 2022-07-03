#include <libxml/xmlreader.h>
#include "readconfig.h"
#include "log.h"
#define PARAM_NUM 14

extern logger log;

void setOptionDefault(optinfo *info){
 info->input1_name=NULL;
 info->input2_name=NULL;
 info->infile1 = NULL;
 info->infile2 = NULL;
 info->output_name = NULL;
 info->width = 3840;
 info->height = 2160;
 info->format = 1; 
 info->frame_number = 0;
 info->start_number = 0;
 info->thread_id = 0;
 info->psnr_flag = 0;
 info->ssim_flag = 0;
 info->show_flag = 1;
 info->mode = 3;
 info->ssim_mode = 0;
 info->thread_number = 10;
 info->loglevel = 1;
 info->psnr_value = NULL;
 info->ssim_value = NULL;
}

int setOption(optinfo *info){
 int opt;
 int param_cnt=0;
 int ret = 1;
 xmlTextReaderPtr reader;
 

 //init
 setOptionDefault(info);
 
 reader = xmlNewTextReaderFilename("config/config.xml");
 
 if(NULL == reader){
   return -1;
 }
 
 ret = xmlTextreaderRead(reader);

 if(1==ret){
   while(1){
     xmlChar *name = NULL;
     xmlChar *value = NULL;
    
     name = xmlTextReaderName(reader);

     if(name == NULL){
         log.debugLog(&log,"read config finish");
         break;
     }

     if("mode"==name){
      printf("-m mode\n m=1:psnr only\n m=2:ssim only\n m=3 between psnr and ssim\n");
      value = xmlTextReaderValue(reader);
      info->mode = atoi(value);
      printf("mode:%d\n",info->mode);
      if(!(info->mode == 1 || info->mode == 2 || info->mode == 3)){
       return -1;
      }
      param_cnt++;
     }
     else if("prefile"==name){
       value = xmlTextReaderValue(reader);
       info->input1_name = (unsigned char*)malloc(sizeof(unsigned char)*strlen(value)+1);
       memcpy(info->input1_name,value,sizeof(unsigned char)*strlen(value));
       param_cnt++;
     }
     else if("postfile"==name){
       value = xmlTextReaderValue(reader);
       info->input2_name = (unsigned char*)malloc(sizeof(unsigned char)*strlen(value)+1);
       memcpy(info->input1_name,value,sizeof(unsigned char)*strlen(value));
       param_cnt++;
     }
     else if("csvresultfile"==name){
      value = xmlTextReaderValue(reader);
      info->output_name = (unsigned char*)malloc(sizeof(unsigned char)*(strlen(value)+4+strlen("\0")));
      sprintf(info->output_name, "%s.csv",value);
      printf("-o %s\n",info->output_name);
      param_cnt++;
     }
     else if("startnumber"==name){
      value = xmlTextReaderValue(reader);
      info->start_number = atoi(value);
      printf("-s %d\n",atoi(value));
      param_cnt++;
     }
     else if("framenum"==name){
      value = xmlTextReaderValue(reader);
      info->frame_number = atoi(value);
      printf("-n %d\n",info->frame_number);
      param_cnt++;
     }
     else if("width"==name){
      value = xmlTextReaderValue(reader);
      info->width = (unsigned int)atoi(value);
      printf("width : %d\n",info->width);
      param_cnt++;
     }
     else if("height"==name){
      value = xmlTextReaderValue(reader);
      info->height = (unsigned int)atoi(value);
      printf("height : %d\n",info->height);
      param_cnt++;
     }
     else if("format"==name){
      value = xmlTextReaderValue(reader);
      if(1==value){
       info->format = 1;
      }
      else if(2==value){
       info->format = 1;
      }
      else{
       info->format = 0;
      }
      printf("format\n");
      param_cnt++;
     }
     else if("show_flag"==name){
      value = xmlTextReaderValue(reader);
      int tmp = atoi(value);
      if(tmp >= 1){
       info->show_flag = 1;
      }
      else{
       info->show_flag = 0;
      }
      printf("show:%d\n",info->show_flag);
      param_cnt++;
     }

     else if("threadnum"==name){
      value = xmlTextReaderValue(reader);
      info->thread_number = atoi(value);
      printf("therad number:%d\n",info->thread_number);
      param_cnt++;
     }
     else if("ssim_mode"==name){
      value = xmlTextReaderValue(reader);
      int tmp = atoi(value);
      
      if(tmp>=1){
       info->ssim_mode=1;
      }
      else{
       info->ssim_mode=0;
      }
 
      printf("ssim_mode%d\n",info->ssim_mode);
      param_cnt++;
     }
     else if("ssim_windowsize"==name){
      value = xmlTextReaderValue(reader);
      param_cnt++;
     }
     else if("ssim_filter"==name){
      value = xmlTextReaderValue(reader);
      param_cnt++;
     }
     else{
      log.errorLog(&log,"unknow parameter");
     }

   }
   
   if(PARAM_NUM!=param_cnt){
    log.errorLog(&log,"parameter num error");
    return -1;
   }

 }

 xmlFreeTextReader(reader);
 return 0; 
}