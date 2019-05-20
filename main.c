#include<stdio.h>
#include"iqa.h"

init(unsigned int original_rgba,unsigned int compress_rgba,
    unsigned char original_rgb,unsigned char compress_rgb,
    unsigned char **original_y,unsigned char **compress_y,
    int HEIGHT,int WIDTH){

    memset(original_rgba,0,sizeof(unsigned int)*HEIGHT*WIDTH);
    memset(compress_rgba,0,sizeof(unsigned int)*HEIGHT*WIDTH);
    memset(original_rgb,0,sizeof(unsigned char)*HEIGHT*WIDTH*3);
    memset(compress_rgb,0,sizeof(unsigned char)*HEIGHT*WIDTH*3);
    memset(y_before[0][0],0,sizeof(uint8_t)*HEIGHT*WIDTH);
    memset(y_after[0][0],0,sizeof(uint8_t)*HEIGHT*WIDTH);
}


int main(int argc,char **argv){
    int WIDTH;
    int HEIGHT;
    unsigned char file_before[512];
    unsigned char file_after[512];
    FILE *fp_before;
    FILE *fp_after;
    FILE fp_write;
    unsigned char connect[2] = "_";			// ファイル名調整用変数
	unsigned char tiff[6] = ".tiff";
	unsigned char csv[5] = ".csv";
    unsigned char count_string[512];
    TIFF *orignal_tiff;
    TIFF *compress_tiff;
    unsigned int *original_rgba;
    unsigned int *compress_rgba;
    unsigned char *original_rgb;
    unsigned char *compress_rgb;

    uint8_t **y_before;
    uint8_t **y_before;
    float fPSNR
    float psnr_sum;
    float fSSIM;
    float ssim_sum;

    file_before[512] = argv[1];
    file_after[512] = argv[2];
    FRAME = atoi(argv[3]);
    
    sprintf(fp_write,"%sと%s%s", argv[1] , argv[2] , csv);// 結果を書き込むファイル名の設定
    
    if((fp_write = fopen(file_write,"a"))==NULL){
            printf("Cant't Create image_write FILE");
            exit(1);
    }

    fprintf(fp_write,"回数,PSNR,SSIM, \n"); // 出力 ⇒ 横並び:「"%lf, "」	縦並び:「"%lf, \n"」

    for(count=0;count<FRAME;count++){
        sprintf(count_string,"%d",count+1);							// 整数を文字列として配列に格納する
		strcat(file_before , argv[1]);			// 例）ex1
		strcat(file_before , connect);			// 例）ex1_
		strcat(file_before , count_string);		// 例）ex1_1
		strcat(file_before , tiff);			// 例）ex1_1.bmp

		strcat(file_after , argv[2]);
		strcat(file_after , connect);
		strcat(file_after , count_string);
		strcat(file_after , tiff);

        if(((original_tiff = TIFFOpen(file_before,"rb"))||(compress_tiff = TIFFOpen(file_after,"rb")))==NULL){
            printf("Cant't Open image FILE");
            exit(1);
        }

       /*get width&height*/
       if(count==0){
           if(!TIFFGetField(original_tiff,TIFFTAG_IMAGELENGTH,HEIGHT)) printf("TIFFGetField height error\n");
           if(!TIFFGetField(original_tiff,TIFFTAG_IMAGEWIDTH,WIDTH)) printf("TIFFGetField width error\n");
           original_rgba = (unsigned int*)malloc(sizeof(unsigned int)*WIDTH*HEIGHT);
           compress_rgba = (unsigned int*)malloc(sizeof(unsigned int)*WIDTH*HEIGHT);
           original_rgb = (unsigned char*)malloc(sizeof(unsigned char)*WIDTH*file_info.height*3);
           compress_rgb = (unsigned char*)malloc(sizeof(unsigned char)*WIDTH*file_info.height*3);
           y_before = (uint8_t **)malloc(sizeof(uint8_t *)*HEIGHT);
           y_after = (uint8_t **)malloc(sizeof(uint8_t *)*HEIGHT);
           for(i=0;i<HEIGHT;i++){
               y_before[i] = (uint8_t *)malloc(sizeof(uint8_t)*WIDTH);
               y_after[i] = (uint8_t *)malloc(sizeof(uint8_t)*WIDTH);
           }
       }
       /*-----------------------------------------------------------------------------------------*/

       init(original_rgba,compress_rgba,original_rgb,compress_rgb,y_before,y_before,HEIGHT,WIDTH);

       if(orignal_rgba==NULL||comparison_rgba==NULL){
            printf("rgba error\n");
            TIFFClose(original_tiff);
            TIFFClose(compress_tiff2);
            eixt(1);
        }

        if((!TIFFReadRGBAImage(orifinal_tiff,WIDTH, HEIGHT, original_rgba, 0))||
	   (!TIFFReadRGBAImage(compress_tiff,WIDTH, HEIGHT, original_rgba, 0))){
            printf("TIFFReadRGBAImage error\n");
            free(original_rgb);
	    free(compress_rgb);
            TIFFClose(original_tiff);
            TIFFClose(compress_tiff2);
            eixt(1);
        }

        TIFFClose(original_tiff);
        TIFFClose(compress_tiff);

        if(original_rgb==NULL||compress_rgb==NULL){
        printf("malloc rgb error\n");
        free(original_rgba);
        free(compress_rgba);
        return -1;
    }

        for(j = 0; j < HEIGHT; j++){
            for(i = 0; i < WIDTH; i++){
                original_rgb[3 * (i + j * WIDTH) + 0] = TIFFGetR(rgbaData[i + (HEIGHT - 1 - j) * width]);
                original_rgb[3 * (i + j * WIDTH) + 1] = TIFFGetG(rgbaData[i + (HEIGHT - 1 - j) * width]);
                original_rgb[3 * (i + j * WIDTH) + 2] = TIFFGetB(rgbaData[i + (HEIGHT - 1 - j) * width]);
                compress_rgb[3 * (i + j * WIDTH) + 0] = TIFFGetR(rgbaData[i + (HEIGHT - 1 - j) * width]);
                compress_rgb[3 * (i + j * WIDTH) + 1] = TIFFGetG(rgbaData[i + (HEIGHT - 1 - j) * width]);
                compress_rgb[3 * (i + j * WIDTH) + 2] = TIFFGetB(rgbaData[i + (HEIGHT - 1 - j) * width]);
            }
        }

        for(i=0;i<HEIGHT;i++){
            for(j=0;j<WIDTH;j++){
                y_before[i][j] = 0.299 * (*original_rgb) +
					        0.587 * (*(original_rgb++)) +
					        0.114 * (*(original_rgb++));

                y_after[i][j] = 0.299 * (*compress_rgb) +
					        0.587 * (*(compress_rgb++)) +
					        0.114 * (*(compress_rgb++));
                original_rgb++;
                compress_rgb++;
            }
        }

        fPSNR = psnr(y_before,y_after,WIDTH,HEIGHT);
        fSSIM = ssim(y_before,y_after,WIDTH,HEIGHT);
        cFRAME = frame(fPSNR,fSSIM);
        sum_psnr += fPSNR;
        sum_ssim += fSSIM;
        /*ファイル書き込み*/
        fprintf(fp_write,"%d,%f,%f, \n",count,fPSNR,fSSIM,cFRAME);
    }


    /*メモリの開放*/
    for(i=0;i<HEIGHT;i++) {
        free(y_before[i]);
        free(y_after[i]);
    }
    free(y_before);
    free(y_after);
    free(original_rgba);
    free(compress_rgba);
    free(original_rgb);
    free(compress_rgb);    
    free(fp_before);
    free(fp_after);
    free(fp_write);
    return 0;
}
