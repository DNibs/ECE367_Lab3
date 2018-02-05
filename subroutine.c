#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "subroutine.h"


// Define functions
void ConnectedSet(struct pixel s, double T, double **img, int width, int height, int ClassLabel, int **seg, int *NumConPixels) {
  //***
  struct pixel B[90000]; // non-searched connected pixels (think border)
  int BCount = 0; // count of non-searched connected pixels
  int *M = NULL;
  int nConnects = 0; // number of connected neighbors to s 
  M = &nConnects;
  struct pixel *c; // connected neighbors to s
  int i = 0;

  M = malloc(sizeof(*M) *4);

  c = malloc(sizeof(*c) * 4);

  B[0] = s; // B is set to seed
  BCount ++;

  while (BCount > 0) {
    s = B[BCount-1]; // take from top of list of B like a stack
    BCount--; // since we're searching s, BCount removes one from list

    ConnectedNeighbors(s, T, img, width, height, M, c);
    // possible error: c is an array, so this might not work?
    // does c need to be passed, or can it be generated in lower func?
    // following the lab format but might not be necessary
    // perhaps make c a pointer to an array and pass pointer


    //----------------------------------------------------

     
    for(i=0; i<*M; i++) {
      if (seg[c[i].m-1][c[i].n-1] == 0) {

	seg[c[i].m-1][c[i].n-1] = 1;
	(*NumConPixels)++;
	B[BCount] = c[i];
	BCount++;
      }
    }

  }

  free(c);
  free(M);
  //***
  return; // Should this return number of connected pixels?****
}



// finds connected neighbors of pixel s

  void ConnectedNeighbors(struct pixel s, double T, double **img, int width, int height, int *M, struct pixel *c) {

  double Ts; // tested threshold value
  double Tabs; // absolute value of threshold test
  struct pixel r[4]; // Pixels for all neighbors, connected or not
  int i=0;

  *M = 0;

  r[0].m = s.m-1;
  r[0].n = s.n;

  r[1].m = s.m;
  r[1].n = s.n+1;

  r[2].m = s.m+1;
  r[2].n = s.n;

  r[3].m = s.m;
  r[3].n = s.n-1;

  for (i=0; i<4; i++) {
    
    if(r[i].m >= 0) {
      if (r[i].m <= width) {
	if (r[i].n >= 0) {
	  if (r[i].n <= height) {
	    
	    Ts = img[r[i].m][r[i].n] - img[s.m][s.n];
	    Tabs = fabs(Ts);
	    
	    if (Tabs <= T) {
	      c[*M].m = r[i].m;
	      c[*M].n = r[i].n;
	      (*M)++;
	    }
	    
	  }
	}
      }
    }
    
  }
    
  
  return;
}
