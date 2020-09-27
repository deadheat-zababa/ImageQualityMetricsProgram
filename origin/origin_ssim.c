#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ssim.h>
#define WINDOWSIZE 8
#define K 0.05
#define L 100
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

//int aaa=1;
//test ban 0.95625
/*
 mu -> µ
 average
*/
double *mu(double *window,int windowsize, double *luma,int width, int height){
 double *data = NULL;
 //double *tmp = NULL;
 int i,j,k,l;
 
 data = (double*)malloc(sizeof(double)*(width-(windowsize-1))*(height-(windowsize-1)));
 if(data==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }

  for(i=0;i<height-windowsize+1;i++){
   for(j=0;j<width-windowsize+1;j++){
    data[i*(width-windowsize+1)+j] =0;

    for(k=0;k<windowsize;k++){
     for(l=0;l<windowsize;l++){
      data[i*(width-windowsize+1)+j] += window[k*windowsize+l]*luma[i*width+j+((width)*k)+l];
     } 
    }
   }
  }
 
 return data;
}

double *filter(double *luma,int *width,int *height,int f){
 //automatic downsampling
 //downsampling by f
 //use a simple lowpass filter
  double *img = NULL;
  double *padimg = NULL;
  double *tmp = NULL;
  double *lpf = NULL;
  int i,j;
  int sum = 0;
  int half_f = f/2;
  int w=0;
  int h=0;

  lpf = (double*)malloc(sizeof(double)*f*f);//ones(f,f);
  
  for(i=0;i<f;i++){
   for(j=0;j<f;j++){
    lpf[i*f+j] = 1;
    sum++;
   }
  }

  for(i=0;i<f;i++){
   for(j=0;j<f;j++){
    lpf[i*f+j] = (double)lpf[i*f+j]/sum;
   }
  }
 
  padimg = (double*)malloc(sizeof(double)*(f+*width)*(f+*height));
  
  for(i=0;i<(f+*width)*(f+*height);i++){
   padimg[i] = 0;
  }
  
  for(i=half_f;i<*height+half_f;i++){
   for(j=half_f;j<*width+half_f;j++){
    padimg[i*(*width+f)+j] = luma[(i-half_f)*(*width)+(j-half_f)];
   }
  }

//up
  for(i=0;i<half_f;i++){
   for(j=half_f;j<*width+half_f;j++){
    padimg[i*(*width+f)+j] = padimg[((f-1)-i)*(*width+f)+j];
   }
  }
//down
  for(i=*height+half_f;i<(*height+f);i++){
   for(j=half_f;j<*width+half_f;j++){
    padimg[i*(*width+f)+j] = padimg[(*height+half_f-(i-(*height+half_f-1)))*(*width+f)+j];
   }
  }
//side
  for(i=0;i<(*height+f);i++){
   for(j=0;j<half_f;j++){
    padimg[i*(*width+f)+j] = padimg[i*(*width+f)+(f-1)-j];
    padimg[i*(*width+f)+(*width+half_f)+j] = padimg[i*(*width+f)+(*width+half_f)-j-1];
   }
  }

  //fultering
  tmp = mu(lpf,f,padimg,*width+f,*height+f);
//  printf("%p\n",tmp);
 

  //tiisakunatteru
  h = (*height+f)-(f-1);
  w = (*width+f)-(f-1);
/*
  if(padimg!=NULL) {
   free(padimg);
   padimg = (double*)malloc(sizeof(double)*(*height)*(*width));
   if(padimg==NULL)printf("ERROR\n"); 
  }

  printf("h:%d,w:%d\n",h,w);
  for(i=1;i<h;i++){
   for(j=1;j<w;j++){
     padimg[(i-1)*(*width)+(j-1)] = tmp[i*w+j];
   }
  }

  w = *width;
  h = *height; 
*/
  img = (double*)malloc(sizeof(double)*(int)((w-1)/f)*(int)((h-1)/f));

  for(i=1;i<h;i+=f){
   for(j=1;j<w;j+=f){
   // printf("[%d]\n",i*w+j);
    img[(int)(i/f)*((w-1)/f)+(int)(j/f)] = tmp[i*w+j];
   }
  }
 
  if(tmp != NULL){
  // printf("%p\n",tmp);
   free(tmp);
   tmp = NULL;
  }

 // printf("KOKOKOKO\n\n");

  if(lpf != NULL) {
   free(lpf);
   lpf = NULL;
  }
  if(padimg!=NULL)free(padimg);

  return img;
}

/*
 squared
*/
double *squared(double *luma1,double *luma2,int width,int height){
 double *result=NULL;
 int i;
 
 if((result = (double *)malloc(sizeof(double)*width*height))==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 
 for(i=0;i<width*height;i++){
  result[i] = luma1[i] * luma2[i];
  //printf("luma :%lf sq : %lf\n",luma1[i],result[i]);
 }
 
 return result;
}

/*
standard deviation
*/
double *sigma(double *window, double *luma1, double *luma2 ,int width, int height,double *ave){
 double *data = NULL;
 double *sq = NULL;
 double *mu1 = NULL;
 int i,j;

 data = (double*)malloc(sizeof(double)*(width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1)));
 if(data==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 sq = (double*)malloc(sizeof(double)*(width*height));
 if(sq==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 mu1 = (double*)malloc(sizeof(double)*(width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1)));
 if(mu1==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 
 sq = squared(luma1, luma2, width, height);
 mu1 = mu(window,WINDOWSIZE, sq, width ,height);
 

 for(i=0;i<(height-(WINDOWSIZE-1));i++){
  for(j=0;j<(width-(WINDOWSIZE-1));j++){
   data[i*(width-(WINDOWSIZE-1))+j] = mu1[i*(width-(WINDOWSIZE-1))+j] - ave[i*(width-(WINDOWSIZE-1))+j];
  }
 }
  if(sq!=NULL) free(sq);
  if(mu!=NULL) free(mu1);
//  printf("\n");

 return data;
}

double calc_mssim(double *ssim,int width,int height){
 double mssim = 0.0;
 int i,j;
 
 for(i=0;i<height;i++){
  for(j=0;j<width;j++){
    mssim += ssim[i*width+j];
  }
 }
// printf("mssim : %lf\n", mssim); 
 
 mssim /= (height*width);
 
 printf("mssim : %lf\n", mssim); 
 return mssim;
}

/*----------------------------------------------------
------------------------------------------------------
256x256 -> 249x249

            (2*µxy + C1)  　 *   (2*σxy +C2)
SSIM  =  -----------------------------------------
          (µx^2 + µy^2 + C1) *(σx^2 + σy^2 + C2)

------------------------------------------------------
-----------------------------------------------------*/
double ssim(double *y1, double *y2,int width,int height){
 double *ssim = NULL;
 double mssim = 0;
 double *luma1;
 double *luma2;
 double *mu1 = NULL;
 double *mu2 = NULL;
 double *mu1_sq = NULL;
 double *mu2_sq = NULL;
 double *mu1_mu2 = NULL;
 double *sigma1_sq = NULL;
 double *sigma2_sq = NULL;
 double *sigma12 = NULL;
 double c1 = 0;
 double c2 = 0;
 int f=0;
 double window[WINDOWSIZE*WINDOWSIZE];
 int i,j;

 f = MAX(1,round((double)MIN(width,height)/256));
// printf("f : %d\n",f);

 if(f>1){
  luma1 = y1;
  y1 = filter(y1,&width,&height,f);
  luma2 = y2;
  y2 = filter(y2,&width,&height,f);
  width = (width+f-1)/f;
  height = (height+f-1)/f;
 }

 c1 = (K*L)*(K*L);
 c2 = (K*L)*(K*L);

 for(i=0;i<WINDOWSIZE*WINDOWSIZE;i++){
  window[i] = (double)1/(WINDOWSIZE*WINDOWSIZE);
 }

 ssim = (double*)malloc(sizeof(double)*((width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1))));
 for(i=0;i<((width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1)));i++){
  ssim[i] = 0;
 }

 
 mu1 = mu(window, WINDOWSIZE, y1, width, height);
 mu2 = mu(window, WINDOWSIZE, y2, width, height);

 mu1_sq  = squared(mu1,mu1,(width-(WINDOWSIZE-1)),(height-(WINDOWSIZE-1)));
  
 mu2_sq  = squared(mu2,mu2,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));

 mu1_mu2 = squared(mu1,mu2,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));


 sigma1_sq  = sigma(window, y1, y1, width ,height, mu1_sq);
 sigma2_sq  = sigma(window, y2, y2, width, height, mu2_sq);
 sigma12    = sigma(window, y1, y2, width, height, mu1_mu2);
 
 for(i=0;i<height-(WINDOWSIZE-1);i++){
  for(j=0;j<width-(WINDOWSIZE-1);j++){
   ssim[(width-(WINDOWSIZE-1))*i+j] = ((2*mu1_mu2[(width-(WINDOWSIZE-1))*i+j]+c1)*(2*sigma12[(width-(WINDOWSIZE-1))*i+j]+c2))/((mu1_sq[(width-(WINDOWSIZE-1))*i+j]+mu2_sq[(width-(WINDOWSIZE-1))*i+j]+c1)*(sigma1_sq[(width-(WINDOWSIZE-1))*i+j]+sigma2_sq[(width-(WINDOWSIZE-1))*i+j]+c2));
  
// printf("SSIM:%lf\n",ssim[(width-(WINDOWSIZE-1))*i+j]);
  }
 }
 mssim =  calc_mssim(ssim,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));

 free(y1);
 free(y2);
 free(mu1);
 free(mu2);
 free(mu1_sq);
 free(mu2_sq);
 free(mu1_mu2);
 free(sigma1_sq);
 free(sigma2_sq);
 free(sigma12);

 return mssim;
}


double origin_yuv_cal_ssim(unsigned char **y_before,unsigned char **y_after,int H,int W){
 double *y_before1d = NULL;
 double *y_after1d = NULL;
 double result = 0;
 int i,j;
 
 y_before1d = (double*)malloc(sizeof(double)*H*W);
 y_after1d = (double*)malloc(sizeof(double)*H*W);
 

 for(i=0;i<H;i++){
  for(j=0;j<W;j++){
  y_before1d[i*W+j] = y_before[i][j];
  y_after1d[i*W+j] = y_after[i][j];
  }
 }

 result = ssim(y_before1d, y_after1d, W, H);

 if(y_before1d!=NULL) free(y_before1d);
 if(y_after1d!=NULL) free(y_after1d);
 
 return result;
}
