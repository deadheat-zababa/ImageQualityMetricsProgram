#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include<openacc.h>
#include"quality_evaluation.h"
#define K 0.05
#define L 100
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

double yuv_Sd(unsigned char **y_before,unsigned char **y_after, float ave_before ,float ave_after ,int i ,int j,int H,int W){
    double sd = 0;
    int n=0,m=0;

    //#pragma acc data
    //#pragma acc kernels
    //#pragma acc loop independent  
    for(n=i-5;n<=i+5;n++){
        for(m=j-5;m<=j+5;m++){
            sd += (ave_before-y_before[n][m])* (ave_after-y_after[n][m]);
        }
    }
    
    sd = sd/(11*11);
  //  printf("sd:%lf\n",sd);
    return sd;
}

double Sd(float **y_before,float **y_after, float ave_before ,float ave_after ,int i ,int j,int H,int W){
    double sd = 0;
    int n=0,m=0;
    int p=0,q=0;
    for(n=i-5;n<=i+5;n++){
        for(m=j-5;m<=j+5;m++){
          p=n;
          q=m;
          //printf("%d,%d\n",n,m) ; 
            if(n<0){
              p = p+10;
            }
            else if(n>=H){
                p = p-10;
            }

            if(m<0){
                q = q+10; 
                //y_before[p][q]=0;
                  //y_after[p][q]=0;
            }

            else if(m>=W){
                q = q-10;
                //y_before[p][q]=0;
                  //y_after[p][q]=0;
            }
            sd += (ave_before-y_before[p][q])* (ave_after - y_after[p][q]);
        }
    }
    
    sd = sd/(WINDOW*WINDOW);
 //   printf("sd:%lf\n",sd);
    return sd;
}

//----------------------------------------------------SSIM計算------------------------------------------------------------------------------------------------------------
double cal_ssim(float **y_before,float **y_after,int H,int W){
    double ssim_sum = 0;
    double ssim_ave = 0;
    float c1 =(0.01 * 255)*(0.01*255);
    float c2 =(0.03*255)*(0.03*255);
    float ave_before=0;
    float ave_after=0;
    float sd_before = 0;
    float sd_after = 0;
    float sd_ab = 0;
    int i,j,n,m;
    int p=0,q=0;


    //11*11window  
    for(i=0;i<=H-1;i++){
        for(j=0;j<=W-1;j++){
            
            //printf("ssim_sum:%lf[%d][%d]\n",ssim_sum,i,j);
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

            for(n=i-5;n<=i+5;n++){
                for(m=j-5;m<=j+5;m++){
                    p=n;
                    q=m;
          //printf("%d,%d\n",n,m) ; 
                if(n<0){
                 //y_before[p][q]=0;
                //y_after[p][q]=0;
                    p = p+10; 
                }
                else if(n>=H){
                    p = p-10;
               // y_before[p][q]=0;
               // y_after[p][q]=0;
                }

            if(m<0){
                q = q+10; 
               // y_before[p][q]=0;
                //y_after[p][q]=0;
            }

            else if(m>=W){
                q = q-10;
               // y_before[p][q]=0;
                //y_after[p][q]=0;
            }
                ave_before += y_before[p][q];
                ave_after += y_after[p][q];
            }
        }
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        ave_before = ave_before/(11*11);
        ave_after = ave_after/(11*11);

        //printf("[%d][%d],ave_b:%f,ave_a:%f\n",i,j,ave_before,ave_after);
        sd_before = sqrt(Sd(y_before,y_before,ave_before,ave_before,i,j,H,W));
        sd_after = sqrt(Sd(y_after,y_after,ave_after,ave_after,i,j,H,W));
        sd_ab = Sd(y_before,y_after,ave_before,ave_after,i,j,H,W);                            
            
            float a = (2*ave_after*ave_before) + c1;
            float b = 2*sd_ab +c2;
            float c = (ave_before*ave_before) + (ave_after*ave_after) + c1;
            float d = (sd_before*sd_before) + (sd_after*sd_after) + c2;
          //  if(i==251&&j==70)printf("[%d][%d]a:%f,b:%f,c:%f,d:%f\n",i,j,a,b,c,d);
            ssim_sum += (a*b)/(c*d);
            
            //if(i<=1070 && j<=5) {
               //printf("ssim_sum:%lf[%d][%d]\n",ssim_sum,i,j);
            ave_after = 0;
            ave_before = 0;
        }
    }

    ssim_ave = ssim_sum/(W*H);
    //printf("ssim:%lf\n",ssim_ave);
    return ssim_ave;
}

double cal_psnr(float **y_before,float **y_after,int height,int width){
	int i,j;
	double sum_numerator= 0;
	double sum_denominator = 0;
	double psnr = 0;

	sum_numerator = height*width;
	sum_numerator = sum_numerator*255*255;

	for(i=0;i<height;i++){
	 for(j=0;j<width;j++){
			sum_denominator += (y_before[i][j]-y_after[i][j])*(y_before[i][j]-y_after[i][j]);
		}
	}

	printf("sum_nemurator:%lf\n",sum_numerator);
	fflush(stdout);

	psnr = 10* log10(sum_numerator / sum_denominator);
	
	return psnr;
}


double yuv_cal_psnr(unsigned char **y_before,unsigned char **y_after,int height,int width){
 int i,j;
 double sum_numerator= 0;
 double sum_denominator = 0;
 double psnr = 0;
 double mse = 0;
 double check_psnr=0;

 sum_numerator = height*width;
 sum_numerator = sum_numerator*255*255;
// #pragma acc data
// {
// #pragma acc kernels
// #pragma acc loop independent
  for(i=5;i<height+5;i++){
   for(j=5;j<width+5;j++){
    sum_denominator += (y_before[i][j]-y_after[i][j])*(y_before[i][j]-y_after[i][j]);
   }
  }
// }
 //printf("sum_nemurator:%lf\n",sum_numerator);
 //fflush(stdout);
 
 mse = sum_denominator/(height*width);
 check_psnr = 10 * log10((255*255)/mse);
 psnr = 10* log10(sum_numerator / sum_denominator);
 if(psnr==check_psnr){
  printf("OK\n");
  printf("psnr:%lf\n",psnr);
 }
 else{
  printf("unmatching\n");
  printf("mse:%lf,check_psnr:%lf,psnr:%lf\n",mse,check_psnr,psnr);
 }

 return psnr;
}


//----------------------------------------------------SSIM計算------------------------------------------------------------------------------------------------------------
double yuv_cal_ssim(unsigned char **y_before,unsigned char **y_after,int H,int W){
    double ssim_sum = 0;
    double ssim_ave = 0;
    float c1 =(0.01 * 255)*(0.01*255);
    float c2 =(0.03*255)*(0.03*255);
    float ave_before=0;
    float ave_after=0;
    float sd_before = 0;
    float sd_after = 0;
    float sd_ab = 0;
    int i,j,n,m;

    //11x11window 
    #pragma acc data
    {
    #pragma acc kernels
    #pragma acc loop independent  
    for(i=5;i<H+5;i++){
        for(j=5;j<W+5;j++){ 
            //printf("ssim_sum:%lf[%d][%d]\n",ssim_sum,i,j);
         for(n=i-5;n<=i+5;n++){
          for(m=j-5;m<=j+5;m++){
           ave_before += y_before[n][m];
           ave_after += y_after[n][m];
          }
         }
        ave_before = ave_before/(11*11);
        ave_after = ave_after/(11*11);

        //printf("[%d][%d],ave_b:%f,ave_a:%f\n",i,j,ave_before,ave_after);
        sd_before = sqrt(yuv_Sd(y_before,y_before,ave_before,ave_before,i,j,H,W));
        sd_after = sqrt(yuv_Sd(y_after,y_after,ave_after,ave_after,i,j,H,W));
        sd_ab = yuv_Sd(y_before,y_after,ave_before,ave_after,i,j,H,W); 
            
        float a = (2*ave_after*ave_before) + c1;
        float b = 2*sd_ab +c2;
        float c = (ave_before*ave_before) + (ave_after*ave_after) + c1;
        float d = (sd_before*sd_before) + (sd_after*sd_after) + c2;
       //  if(i==251&&j==70)printf("[%d][%d]a:%f,b:%f,c:%f,d:%f\n",i,j,a,b,c,d);
        ssim_sum += (a*b)/(c*d);
            
            //if(i<=1070 && j<=5) {
               //printf("ssim_sum:%lf[%d][%d]\n",ssim_sum,i,j);
            ave_after = 0;
            ave_before = 0;
        }
    }
    }

    ssim_ave = ssim_sum/(W*H);
    //printf("ssim:%lf\n",ssim_ave);
    return ssim_ave;
}


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

 data = (double*)malloc(sizeof(double)*(width-(windowsize-1))*(height-(windowsize-1)));
 if(data==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 sq = (double*)malloc(sizeof(double)*(width*height));
 if(sq==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 mu1 = (double*)malloc(sizeof(double)*(width-(windowsize-1))*(height-(windowsize-1)));
 if(mu1==NULL){
  fprintf(stderr,"muri");
  return NULL;
 }
 
 sq = squared(luma1, luma2, width, height);
 mu1 = mu(window,windowsize, sq, width ,height);
 

 for(i=0;i<(height-(windowsize-1));i++){
  for(j=0;j<(width-(windowsize-1));j++){
   data[i*(width-(windowsize-1))+j] = mu1[i*(width-(windowsize-1))+j] - ave[i*(width-(windowsize-1))+j];
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
double origin_ssim(double *y1, double *y2,int width,int height,int windowsize_){
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
 double *window;
 int i,j;

 windowsize = windowsize_;

 window = (double*)malloc(sizeof(double) * windowsize * windowsize);
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

 for(i=0;i<windowsize*windowsize;i++){
  window[i] = (double)1/(windowsize*windowsize);
 }

 ssim = (double*)malloc(sizeof(double)*((width-(windowsize-1))*(height-(windowsize-1))));
 for(i=0;i<((width-(windowsize-1))*(height-(windowsize-1)));i++){
  ssim[i] = 0;
 }

 
 mu1 = mu(window, windowsize, y1, width, height);
 mu2 = mu(window, windowsize, y2, width, height);

 mu1_sq  = squared(mu1,mu1,(width-(windowsize-1)),(height-(windowsize-1)));
  
 mu2_sq  = squared(mu2,mu2,width-(windowsize-1),height-(windowsize-1));

 mu1_mu2 = squared(mu1,mu2,width-(windowsize-1),height-(windowsize-1));


 sigma1_sq  = sigma(window, y1, y1, width ,height, mu1_sq);
 sigma2_sq  = sigma(window, y2, y2, width, height, mu2_sq);
 sigma12    = sigma(window, y1, y2, width, height, mu1_mu2);
 
 for(i=0;i<height-(windowsize-1);i++){
  for(j=0;j<width-(windowsize-1);j++){
   ssim[(width-(windowsize-1))*i+j] = ((2*mu1_mu2[(width-(windowsize-1))*i+j]+c1)*(2*sigma12[(width-(windowsize-1))*i+j]+c2))/((mu1_sq[(width-(windowsize-1))*i+j]+mu2_sq[(width-(windowsize-1))*i+j]+c1)*(sigma1_sq[(width-(windowsize-1))*i+j]+sigma2_sq[(width-(windowsize-1))*i+j]+c2));
  
// printf("SSIM:%lf\n",ssim[(width-(windowsize-1))*i+j]);
  }
 }
 mssim =  calc_mssim(ssim,width-(windowsize-1),height-(windowsize-1));

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


double origin_yuv_cal_ssim(unsigned char **y_before,unsigned char **y_after,int H,int W,int windowsize){
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

 result = origin_ssim(y_before1d, y_after1d, W, H,windowsize);

 if(y_before1d!=NULL) free(y_before1d);
 if(y_after1d!=NULL) free(y_after1d);
 
 return result;
}