# image_quality_assessment
 ## psnr and ssim calculation
  - main.c : main file
  - processing.c : main processing(read some files,write a result file,etc...)
  - quality_evaluation.c (psnr,ssim)
  
 ## Usage
   - tiff
  `./q_test.ext -i original_file.tiff -i decodeimagefile.tiff -m 3 -n 30 -s 0 -o output`
   - yuv
   `./q_test.ext -f yuv -i original_file.tiff -i decodeimagefile.tiff -m 3 -n 30 -s 0 -p 30 -o output`
