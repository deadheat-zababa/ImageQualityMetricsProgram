# image_quality_assessment
 ## psnr and ssim calculation
  - main.c : main file
  - processing.c : main processing(read some files,write a result file,etc...)
  - quality_evaluation.c (psnr,ssim)
  
 ## Usage
   - tiff
  `./q_test.ext -i original_file -i decodeimagefile -m 3 -n 30 -s 0 -o output`
  
    input file : original_file00000.tiff ~ original_file999999.tiff
                decodeimage_file00000.tiff ~ decodeimage_file99999.tiff
   - yuv
   `./q_test.ext -f yuv -i original_file.yuv -i decodeimagefile.yuv -m 3 -n 30 -s 0 -p 30 -o output`
  
    default size :3840x2160
