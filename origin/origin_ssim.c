#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define WINDOWSIZE 8
#define K 0.05
#define L 100


double *lpf(double *luma,int *width,int *height,int f){
 //automatic downsampling
 //downsampling by f
 //use a simple lowpass filter
  double *img = NULL;
  double *padimg = NULL;
  int *lpf = NULL;
  int i,j;
  int sum = 0;

  lpf = (unsigned int*)malloc(sizeof(unsigned int)*f*f);//ones(f,f);
  
  for(i=0;i<f;i++){
   for(i=0;i<f;i++){
    lpf[i*f+j] = 1;
    sum++;
   }
  }

  for(i=0;i<f;i++){
   for(i=0;i<f;i++)
    lpf[i*f+j] = lpf[i*f+j]/sum;
  }
 
  padimg = (double*)malloc((f+width)*(f+height));
  
  for(i=0;i;<(f+width)*(f*height);i++){
   padimg[i] = 0;
  }
// 
  for(i=0;i;<f;i++){
   for(j=f;j<width;j++){
    padimg[i*(height+f)+j] = luma[((f-1)-i)*height+(j-f)];
   }
  }
  for(i=f;i;<height;i++){
   for(j=f;j<width;j++){
    padimg[i*(height+f)+j] = luma[(i-f)*height+(j-f)];
   }
  }
  for(i=height;i;<height+f;i++){
   for(j=f;j<width;j++){
    padimg[i*(height+f)+j] = luma[(i-(f+1))*height+(j-f)];
   }
  }


  for(i=0;i;<height+f;i++){
   for(j=0;j<f;j++){
    padimg[i*(height+f)+j] = padimg[i*(height+2*f)+(j+(f-1))];
    padimg[i*(height+f)+width+j] = padimg[i*(height+2*f)+(width+2*f)-j];
  }
  
  img = (double*)malloc((f+width)*(f+height));

  for(i=0;i;<height;i++){
   for(j=0;j<width;j++){
    img[] = padimg*
   }
  }

  if(padimg != NULL) free(padimg);
  if(lpf != NULL) free(lpf);
 
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
 mu -> µ
 average
*/
double *mu(double *window, double *luma,int width, int height){
 double *data = NULL;
 //double *tmp = NULL;
 int i,j,k,l;
 
 data = (double*)malloc(sizeof(double)*(width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1)));
 if(data==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }

  for(i=0;i<height-WINDOWSIZE+1;i++){
   for(j=0;j<width-WINDOWSIZE+1;j++){
    data[i*(height-WINDOWSIZE+1)+j] =0;
    for(k=0;k<WINDOWSIZE;k++){
     for(l=0;l<WINDOWSIZE;l++){
      data[i*(height-WINDOWSIZE+1)+j] += window[k*WINDOWSIZE+l]*luma[i*height+j+(width*k)+l];
     }

    } 

   }
  }
  printf("\n");
 
 return data;
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
 mu1 = mu(window, sq, width ,height);
 

 for(i=0;i<(height-(WINDOWSIZE-1));i++){
  for(j=0;j<(width-(WINDOWSIZE-1));j++){
   data[i*(height-(WINDOWSIZE-1))+j] = mu1[i*(height-(WINDOWSIZE-1))+j] - ave[i*(height-(WINDOWSIZE-1))+j];
  // printf("mu[%d][%d]:%lf\n",i,j,mu1[i*(height-(WINDOWSIZE-1))+j]);
  // printf("ave[%d][%d]:%lf\n",i,j,ave[i*(height-(WINDOWSIZE-1))+j]);
  // printf("SIGMA[%d][%d]:%lf\n",i,j,data[i*(height-(WINDOWSIZE-1))+j]);
  }
 }
  if(sq!=NULL) free(sq);
  if(mu!=NULL) free(mu1);
  printf("\n");

 return data;
}

void calc_mssim(double *ssim,int width,int height){
 double mssim = 0.0;
 int i,j;
 
 printf("width:%d,height:%d\n",width,height);
 for(i=0;i<height;i++){
  for(j=0;j<width;j++){
    mssim += ssim[i*height+j];
  }
 }
 printf("mssim : %lf\n", mssim); 
 
 mssim /= (height*width);
 
 printf("mssim : %lf\n", mssim); 

}

/*----------------------------------------------------
------------------------------------------------------
256x256 -> 249x249

            (2*µxy + C1)  　 *   (2*σxy +C2)
SSIM  =  -----------------------------------------
          (µx^2 + µy^2 + C1) *(σx^2 + σy^2 + C2)

------------------------------------------------------
-----------------------------------------------------*/
void ssim(double *y1, double *y2,int width,int height){
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

 f = max(1,(min(M,N)/256));

 if(f>1){
  y1 = filter(y1,width,height,f);
  y2 = filter(y2, width,height,f);
 }

 c1 = (K*L)*(K*L);
 c2 = (K*L)*(K*L);

 for(i=0;i<WINDOWSIZE*WINDOWSIZE;i++){
  window[i] = (double)1/(WINDOWSIZE*WINDOWSIZE);
  printf("window:%lf\n",window[i]);
 }

 ssim = (double*)malloc(sizeof(double)*((width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1))));
 
// if(f=max(1,rounf(min(M,N),/256))>1){
  //lpf();
  //lpf();
 // printf("f : %d\n",f);
// }

 printf("mu made\n"); 
 mu1 = mu(window, y1, width, height);
 mu2 = mu(window, y2, width, height);
 
 printf("m1_sq made\n");  
 mu1_sq  = squared(mu1,mu1,(width-(WINDOWSIZE-1)),(height-(WINDOWSIZE-1)));
 printf("m2_sq made\n");  
 mu2_sq  = squared(mu2,mu2,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));
 printf("m1_m2_sq made\n");
 mu1_mu2 = squared(mu1,mu2,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));
/* 
for(i=0;i<(height-(WINDOWSIZE-1));i++){
  for(j=0;j<(width-(WINDOWSIZE-1));j++){
   printf("mu1[%d][%d]:%lf\n",i,j,mu1[i*(height-(WINDOWSIZE-1))+j]);
   printf("ave_sq[%d][%d]:%lf\n",i,j,mu1_sq[i*(height-(WINDOWSIZE-1))+j]);
  }
 }
 */
 sigma1_sq  = sigma(window, y1, y1, width ,height, mu1_sq);
 sigma2_sq  = sigma(window, y2, y2, width, height, mu2_sq);
 sigma12    = sigma(window, y1, y2, width, height, mu1_mu2);
 
 for(i=0;i<height-(WINDOWSIZE-1);i++){
  for(j=0;j<width-(WINDOWSIZE-1);j++){
   ssim[(height-(WINDOWSIZE-1))*i+j] = ((2*mu1_mu2[(height-(WINDOWSIZE-1))*i+j]+c1)*(2*sigma12[(height-(WINDOWSIZE-1))*i+j]+c2))/((mu1_sq[(height-(WINDOWSIZE-1))*i+j]+mu2_sq[(height-(WINDOWSIZE-1))*i+j]+c1)*(sigma1_sq[(height-(WINDOWSIZE-1))*i+j]+sigma2_sq[(height-(WINDOWSIZE-1))*i+j]+c2));
  
   //printf("SSIM:%lf\n",ssim[(height-(WINDOWSIZE-1))*i+j]);
  }
 }
 calc_mssim(ssim,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));

 free(luma1);
 free(luma2);
 free(mu1);
 free(mu2);
 free(mu1_sq);
 free(mu2_sq);
 free(mu1_mu2);
 free(sigma1_sq);
 free(sigma2_sq);
 free(sigma12);

}

