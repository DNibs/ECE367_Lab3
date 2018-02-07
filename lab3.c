#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//--------------------------------------------------
#include "allocate.h"
#include "tiff.h"
#include "typeutil.h"
#include "randlib.h"
#include "subroutine.h"
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^




int main(int argc, char **argv) {

  struct pixel s;
  double T;
  int width=384; // manually pulled from img data
  int height=256; // manually pulled from img data

  int ClassLabel;
  int **seg;
  int *NumConPixels;
  int NumPix = 0;
  NumConPixels = &NumPix;
  int m = 0;
  int n = 0;


  // allocate mem for 2d array seg
  // seg indicates connected pixels, such that seg[m][n] = 1
  seg = malloc(sizeof(*seg) * width);
  for(m=0; m<width; m++) {
    seg[m] = malloc(sizeof(**seg) * height);
  }

  // set seg to 0 for all elements
  for (m=0; m<width; m++) {
    for (n=0; n<height; n++) {
      seg[m][n] = 0;
    }
  }

  

//-------------------------------------------------------
  FILE *fp;
  struct TIFF_img input_img, output_img;
  double **img1;

  if ( argc != 2 ){
    fprintf(stderr, "Not enough arguments\n");
  }

  /* open image file */
  if ( ( fp = fopen ( argv[1], "rb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file %s\n", argv[1] );
    exit ( 1 );
  }

  /* read image */
  if ( read_TIFF ( fp, &input_img ) ) {
    fprintf ( stderr, "error reading file %s\n", argv[1] );
    exit ( 1 );
  }

  /* close image file */
  fclose ( fp );

  /* check the type of image data */
  if ( input_img.TIFF_type != 'g' ) {
    fprintf ( stderr, "error:  image must be grayscale\n" );
    exit ( 1 );
  }
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


  img1 = (double **)get_img(input_img.height,input_img.width,sizeof(double));

  for (m=0; m<width; m++) {
    for (n=0; n<height; n++) {
      img1[m][n] = input_img.mono[n][m]; // switched ht/wt conventions******
    }
  }

  // Set seed
  s.m = 67;
  s.n = 45;

  // Threshhold Value
  T = 2;

  // value of connected pixels
  ClassLabel = 1;

  // Find all connected pixels
  // save connected pixels to img1, where each connected pixel
  // equals the ClassLabel
  *NumConPixels = ConnectedSet(s, T, img1, width, height, ClassLabel, seg, NumConPixels);

  //fprintf(stderr, "NumConPixels %d\n", *NumConPixels);



  //------------------------------------------------------
  /* set up structure for output achromatic image */
  /* to allocate a full color image use type 'c' */
  get_TIFF ( &output_img, input_img.height, input_img.width, 'g' );
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


  
  // copy connected pixels to new image
  // set connected pixel value to 150
  // ****** try dif values to see how img looks
  for (m=0; m<width; m++) {
    for (n=0; n<height; n++) {
      output_img.mono[n][m] = seg[m][n] * 250;
    }
  }


  //--------------------------------------------------
    /* open output image file */
  if ( ( fp = fopen ( "output.tif", "wb" ) ) == NULL ) {
    fprintf ( stderr, "cannot open file output.tif\n");
    exit ( 1 );
  }

  /* write output image */
  if ( write_TIFF ( fp, &output_img ) ) {
    fprintf ( stderr, "error writing TIFF file %s\n", argv[2] );
    exit ( 1 );
  }

  /* close output image file */
  fclose ( fp );


  /* de-allocate space which was used for the images */
  free_TIFF ( &(input_img) );
  free_TIFF ( &(output_img) );
  free_img( (void**)img1);
  //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  
  for (m=0; m<width; m++) {
    free(seg[m]);
  }
  free(seg);
  

  return EXIT_SUCCESS;
}



