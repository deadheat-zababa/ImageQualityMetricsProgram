#include<stdio.h>
#include<stdlib.h>
#include<tiffio.h>

extern void ssim(double *, double *,int,int);

int main(int argc ,char **argv){
 unsigned char *tmp = NULL;
 unsigned char *tmp2 = NULL;
 double *y = NULL;
 double *y2 = NULL;
 TIFF *tiff = NULL;
 TIFF *tiff2 = NULL;
 int width = 0;
 int height = 0;
 int bps = 0;
 int spp = 0;
 int photometric=0;
 int rps;
 int i,j;
 int row;
 int comp;

 tiff = TIFFOpen(argv[1],"r");
 if(tiff==NULL){
  fprintf(stderr,"Cannnot file Open\n");
  exit(1);
 }

 else{
  TIFFGetField(tiff,TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tiff,TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(tiff,TIFFTAG_BITSPERSAMPLE, &bps);
  TIFFGetField(tiff,TIFFTAG_SAMPLESPERPIXEL, &spp);
  TIFFGetField(tiff,TIFFTAG_ROWSPERSTRIP, &row);
  TIFFGetField(tiff,TIFFTAG_PHOTOMETRIC, &photometric);
  TIFFGetField(tiff,TIFFTAG_COMPRESSION, &comp);
   
  printf("width:%d\nheight:%d\nbps:%d\nspp:%d\nrow:%d\nphotometric:%d\ncomp:%d\n",width,height,bps,spp,row,photometric,comp);
 
  tmp = (unsigned char*)_TIFFmalloc(TIFFScanlineSize(tiff));
  y = (double*)malloc(sizeof(double)*width*height);
  printf("scanlinesize:%ld\n",TIFFScanlineSize(tiff));
  for(i=0;i<height;i++){
   TIFFReadScanline(tiff,tmp,i,spp);

   for(j=0;j<width;j++){
    y[i*height+j] = tmp[j];
   }
  }
  free(tmp);
  TIFFClose(tiff);
  tiff = NULL;
  tmp = NULL;
 }
 
 tiff2 = TIFFOpen(argv[2],"r");
 if(tiff2==NULL){
  fprintf(stderr,"Cannnot file Open\n");
  exit(1);
 }

 else{
  TIFFGetField(tiff2,TIFFTAG_IMAGEWIDTH, &width);
  TIFFGetField(tiff2,TIFFTAG_IMAGELENGTH, &height);
  TIFFGetField(tiff2,TIFFTAG_BITSPERSAMPLE, &bps);
  TIFFGetField(tiff2,TIFFTAG_SAMPLESPERPIXEL, &spp);
  TIFFGetField(tiff2,TIFFTAG_ROWSPERSTRIP, &row);
  TIFFGetField(tiff2,TIFFTAG_PHOTOMETRIC, &photometric);
  
  //tmp2 = (unsigned char*)malloc(sizeof(unsigned char)*width*height);
 // y2 = (double*)malloc(sizeof(double)*width*height);
  y2 = (double*)malloc(sizeof(double)*width*height);
 
//  for(i=0;i<height;i+=row)
//   TIFFReadEncodedStrip(tiff2,TIFFComputeStrip(tiff2,i,0),tmp2,row*width*spp);

  tmp2 = (unsigned char*)_TIFFmalloc(TIFFScanlineSize(tiff2));
  y2 = (double*)malloc(sizeof(double)*width*height);
  printf("scanlinesize:%ld\n",TIFFScanlineSize(tiff2));
  for(i=0;i<height;i++){
   TIFFReadScanline(tiff2,tmp2,i,spp);

   for(j=0;j<width;j++){
    y2[i*height+j] = tmp2[j];
   }

  }
  _TIFFfree(tmp2);
  TIFFClose(tiff2);
  tiff2 = NULL;
  tmp2 = NULL;
 }
 
// printf("width:%d,height:%d\n",width,height); 
// for(i=0;i<width*height;i++){
//  printf("%d num\n",i);
//  printf("y_data1:%lf\n",y[i]);
//  printf("y_data2:%lf\n",y2[i]); }
 ssim(y, y2, width, height);

 return 0;
}
