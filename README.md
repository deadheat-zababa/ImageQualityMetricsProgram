# image_quality_assessment
## This program calculates PSNR and SSIM.
  - main.c : main file (read imagefiles, set some options)
  - processing : main processing(read some files,write a result file,etc...)
  - quality_evaluation (psnr,ssim)
  
 ## Usage
  `./q_test.ext -f tiff -i original_file.tiff -i decodeimagefile.tiff -m 3 -n 30 -s 0 -o output`
  `./q_test.ext -f yuv -i original_file.yuv -i decodeimagefile.yuv -m 3 -n 30 -s 0 -o output`
