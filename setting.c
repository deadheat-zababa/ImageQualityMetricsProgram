readConfig(){
 xmlTextReaderPtr reader;
 reader = xmlNewTextReaderFilename("setting.xml");
 ret = xmlTextRederRead(reader);

 while(ret==1){
  processNode(reader);
  ret = xmlTextRederRead(reader);

 }

 xmlFreeTextReader(reader);
}



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
 info->psnr_value = NULL;
 info->ssim_value = NULL;
}

int setOption(int argc,char **argv,optinfo *info){
 int opt;
 char a=0;
 setOptionDefault(info);
 
 while((opt = getopt(argc,argv,"i:n:o:f:s:m:p:v:x:W:H:h"))!= -1){
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
