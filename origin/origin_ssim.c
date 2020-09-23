#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define WINDOWSIZE 8
#define K 0.05
#define L 100
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))
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
      data[i*(width-windowsize+1)+j] += window[k*windowsize+l]*luma[i*width+j+(width*k)+l];
     }

    } 

   }
  }
  printf("\n");
 
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
    lpf[i*f+j] /= sum;
//    printf("sum:%d,lpf:%f\n",sum,lpf[i*f+j]);
   }
  }
 
  padimg = (double*)malloc(sizeof(double)*(f+*width)*(f+*height));
  
  for(i=0;i<(f+*width)*(f+*height);i++){
   padimg[i] = 200;
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
    padimg[i*(*width+f)+j] = padimg[(*width+half_f-(i-(*width+half_f-1)))*(*height+f)+j];
   }
  }
//side
  for(i=0;i<(*height+f);i++){
   for(j=0;j<half_f;j++){
    padimg[i*(*width+f)+j] = padimg[i*(*width+f)+(j+(half_f)+(half_f-(j+(half_f-1))))];
    padimg[i*(*width+f)+(*width+half_f)+j] = padimg[i*(*width+f)+(*width)-j-1];
   }
  }
  printf("%f\n",padimg[(*height+f)*(*width+f)-1]);
  printf("%f\n",luma[(*height**width)-1]);

  //fultering
  tmp = mu(lpf,f, padimg,*width+f, *height+f);

  //tiisakunatteru
  h = (*height+f)-(f-1);
  w = (*width+f)-(f-1);
  

  img = (double*)malloc(sizeof(double)*(int)((w-1)/f)*(int)((h-1)/f));

  for(i=1;i<h;i+=f){
   for(j=1;j<w;j+=f){
   // printf("i[%d], j[%d] ,%d,%d\n",i,j,(int)(i/f)*((h-1)/f)+(int)(j/f),i*h+j);
    //printf("%f\n",tmp[i*h+j]);
    img[(int)(i/f)*((h-1)/f)+(int)(j/f)] = tmp[i*h+j];
    //printf("h:%d,w:%d,new_h:%d,new_w:%d,max:%d\n",h,w,(int)(h-1)/f,(int)(w-1)/f,(int)((w-1)/f)*(int)((h-1)/f));
   }
  }
 
 if(padimg!=NULL) {
   free(padimg);
   padimg = NULL;
  }
  if(tmp != NULL) free(tmp);
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
/*
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
*/

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
    mssim += ssim[i*width+j];
  }
 }
// printf("mssim : %lf\n", mssim); 
 
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

 f = MAX(1,round((double)MIN(width,height)/256));
 printf("f : %d\n",f);

 if(f>1){
  luma1 = y1;
  y1 = filter(y1,&width,&height,f);
  luma2 = y2;
  y2 = filter(y2,&width,&height,f);
  width = (width+f-1)/f;
  height = (height+f-1)/f;

//  for(i=0;i<height;i++){
//   for(j=0;j<width;j++){
   // if(y1[i*height+j] != y2[i*height+j])
 //   printf("diffarence[%d]:%f,%f\n",i*height+j,y1[i*height+j],y2[i*height+j]);
//   }
//  }

 }

 c1 = (K*L)*(K*L);
 c2 = (K*L)*(K*L);

 for(i=0;i<WINDOWSIZE*WINDOWSIZE;i++){
  window[i] = (double)1/(WINDOWSIZE*WINDOWSIZE);
 // printf("window:%lf\n",window[i]);
 }

 ssim = (double*)malloc(sizeof(double)*((width-(WINDOWSIZE-1))*(height-(WINDOWSIZE-1))));
 
// if(f=max(1,rounf(min(M,N),/256))>1){
  //lpf();
  //lpf();
 // printf("f : %d\n",f);
// }

// printf("mu made\n"); 
 mu1 = mu(window, WINDOWSIZE, y1, width, height);
 mu2 = mu(window, WINDOWSIZE, y2, width, height);
 
// printf("m1_sq made\n");  
 mu1_sq  = squared(mu1,mu1,(width-(WINDOWSIZE-1)),(height-(WINDOWSIZE-1)));
// printf("m2_sq made\n");  
 mu2_sq  = squared(mu2,mu2,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));
// printf("m1_m2_sq made\n");
 mu1_mu2 = squared(mu1,mu2,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));
/* 
for(i=0;i<(height-(WINDOWSIZE-1));i++){
  for(j=0;j<(width-(WINDOWSIZE-1));j++){
   printf("mu1[%d][%d]:%lf\n",i,j,mu1[i*(height-(WINDOWSIZE-1))+j]);
   printf("mu2[%d][%d]:%lf\n",i,j,mu2[i*(height-(WINDOWSIZE-1))+j]);
   printf("ave_sq[%d][%d]:%lf\n",i,j,mu1_sq[i*(height-(WINDOWSIZE-1))+j]);
  }
 }
 */
 sigma1_sq  = sigma(window, y1, y1, width ,height, mu1_sq);
 sigma2_sq  = sigma(window, y2, y2, width, height, mu2_sq);
 sigma12    = sigma(window, y1, y2, width, height, mu1_mu2);
 
 for(i=0;i<height-(WINDOWSIZE-1);i++){
  for(j=0;j<width-(WINDOWSIZE-1);j++){
   ssim[(width-(WINDOWSIZE-1))*i+j] = ((2*mu1_mu2[(width-(WINDOWSIZE-1))*i+j]+c1)*(2*sigma12[(width-(WINDOWSIZE-1))*i+j]+c2))/((mu1_sq[(width-(WINDOWSIZE-1))*i+j]+mu2_sq[(width-(WINDOWSIZE-1))*i+j]+c1)*(sigma1_sq[(width-(WINDOWSIZE-1))*i+j]+sigma2_sq[(width-(WINDOWSIZE-1))*i+j]+c2));
  
 // printf("SSIM:%lf\n",ssim[(height-(WINDOWSIZE-1))*i+j]);
  }
 }
 calc_mssim(ssim,width-(WINDOWSIZE-1),height-(WINDOWSIZE-1));
 printf("f : %d\n",f);

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

