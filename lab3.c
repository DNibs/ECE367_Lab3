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
  int **imgcomp;
  int *NumConPixels;
  int NumPix = 0;
  NumConPixels = &NumPix;
  int m = 0;
  int n = 0;
  int mm = 0;
  int nn = 0;
  int compindex = 0;


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


   // allocate mem for 2d array 
  // imgcomp is the composite for seg applied over every pixel
  // only seg > 100 pixels will be included
  imgcomp = malloc(sizeof(*imgcomp) * width);
  for(m=0; m<width; m++) {
    imgcomp[m] = malloc(sizeof(**imgcomp) * height);
  }

  // set seg to 0 for all elements
  for (m=0; m<width; m++) {
    for (n=0; n<height; n++) {
      imgcomp[m][n] = 0;
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

  // allocate the "working image", copied from input tiff
  img1 = (double **)get_img(input_img.height,input_img.width,sizeof(double));

  for (m=0; m<width; m++) {
    for (n=0; n<height; n++) {
      img1[m][n] = input_img.mono[n][m]; // switched ht/wt conventions
    }
  }

  // Set seed
  s.m = 0;
  s.n = 0;

  // Threshhold Value
  T = 3;

  // value of connected pixels
  ClassLabel = 1;

  for (m=0; m<width; m++) { // TESTING M INIT CHANGE FOR FINAL!!!!!
    fprintf(stderr, "%d / 384\n", s.m); //*********************************
    for (n=0; n<height; n++) {
      s.m = m;
      s.n = n;

  // Find all connected pixels
  // save connected pixels to seg, where each connected pixel
  // equals the ClassLabel
      if(seg[m][n] == 0) {
	*NumConPixels = 0;
	*NumConPixels = ConnectedSet(s, T, img1, width, height, ClassLabel, seg, NumConPixels);

	if( *NumConPixels > 100) {
	  compindex++;
	  for (mm=0; mm<width; mm++) {
	    for (nn=0; nn<height; nn++) {

	      //add segments to composition
	      //without reseting alread allocated
	      //segments
	      if (imgcomp[mm][nn] == 0) {
		imgcomp[mm][nn] = seg[mm][nn]*(compindex+1);
	      }
	      // reset seg to avoid spillage from other sets
	    }
	  }
	}
      }
    }
  }

	   
	      
  fprintf(stderr, "compindex %d\n", compindex);


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
      output_img.mono[n][m] = imgcomp[m][n] * 10;
    }
  }


  //--------------------------------------------------
    /* open output image file */
  if ( ( fp = fopen ( "segmentation.tif", "wb" ) ) == NULL ) {
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


  for (m=0; m<width; m++) {
    free(imgcomp[m]);
  }
  free(imgcomp);
  

  return EXIT_SUCCESS;
}



