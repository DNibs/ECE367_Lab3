#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct pixel {
  int m,n; // coordinate for pixel
}



// finds connected neighbors of pixel s
// INPUTS
// s - location of pixel whose neighbors
//      will be calculated
// T - threshold value per eq1
// **img - 2d array of pixels img[m][n] denoted by
//      xs in eq1
// width - width of img[height][width]
// height - height of img[height][width]
// OUTPUTS
// int *M - pointer to number of neighbors connected
//       to pixel s. *M <= 4 (4 pt spread)
// struct pixel c[4] - array containing M-connected
//       neighbors to pixel s.
void ConnectedNeighbors(
			struct pixel s,
			double T,
			unsigned char **img,
			int width,
			int height,
			int *M;
			struct pixel c[4]) {

  double Ts; // tested threshold value
  double Tabs; // absolute value of threshold test
  struct pixel r[4];

  *M = 0;

  r[0].m = s.m-1;
  r[0].n = s.n;

  r[1].m = s.m;
  r[1].n = s.n+1;

  r[2].m = s.m+1;
  r[2].n = s.n;

  r[3].m = s.m;
  r[3].n = s.n-1;

  for (i=0, i<4, i++) {
    
    if(r[i].m >= 0) {
      if (r[i].m <= height) {
	if (r[i].n >= 0) {
	  if (r[i].n <= width) {
	    
	    Ts = img[r.m][r.n] - img[s.m][s.n];
	    Tabs = fabs(Ts);
	    
	    if (Tabs <= T) {
	      c[M].m = r[i].m;
	      c[M].n = r[i].n;
	      *M++;
	    }
	    
	  }
	}
      }
    }
    
  }
    
  
  return c[4];
}


void ConnectedSet(
		  struct pixel s,
		  double T,
		  unsigned char **img,
		  int width,
		  int height,
		  int ClassLabel,
		  unsigned int **seg,
		  int *NumConPixels) {
  //***
  struct pixel B[90000]; // non-searched connected pixels (think border)
  int BCount = 0; // count of non-searched connected pixels
  int *M = NULL;
  int nConnects = 0; // number of connected neighbors to s 
  M = &nConnects;
  struct pixel c[4]; // connected neighbors to s
  int i = 0;

  B[0] = s; // B is set to seed
  BCount ++;

  while (BCount > 0) {
    s = B[BCount-1]; // take from top of list of B like a stack
    BCount--; // since we're searching s, BCount removes one from list

    c = ConnectedNeighbors(s, T, img, width, height, M, c);
    // possible error: c is an array, so this might not work?
    // does c need to be passed, or can it be generated in lower func?
    // following the lab format but might not be necessary
    // perhaps make c a pointer to an array and pass pointer

    for(i=0; i<*M; i++) {
      if (seg[c[i].m][c[i].n] == 0) {
	seg[c[i].m][c[i].n] = 1;
	*NumConPixels++;
	B[BCount] = c[i];
	BCount++;
      }
    }

  }

  
  //***
  return *NumConPixels;
}
