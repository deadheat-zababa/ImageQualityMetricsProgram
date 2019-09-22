#include<math.h>
#include<stdlib.h>
#include<stdio.h>
#include"quality_evaluation.h"

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

	//printf("sum_nemurator:%lf\n",sum_numerator);
	//fflush(stdout);

	psnr = 10* log10(sum_numerator / sum_denominator);
	
	return psnr;
}
