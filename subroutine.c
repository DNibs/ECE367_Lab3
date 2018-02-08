#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "subroutine.h"


// Define functions
int ConnectedSet(struct pixel s, double T, double **img, int width, int height, int ClassLabel, int **seg, int *NumConPixels) {
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
  BCount = 0;

  B[0] = s; // B is set to seed
  BCount ++;

  while (BCount > 0) {
    s = B[BCount-1]; // take from top of list of B like a stack
    BCount--; // since we're searching s, BCount removes one from list

    // find all connected neighbors
    ConnectedNeighbors(s, T, img, width, height, M, c);

    //
    for(i=0; i<*M; i++) {

      if (seg[c[i].m][c[i].n] == 0) {

	seg[c[i].m][c[i].n] = ClassLabel;
	(*NumConPixels)++;
	B[BCount] = c[i];
	BCount++;
      }
    }
  }
  
  free(c);
  free(M);
  //***
  return *NumConPixels; // Should this return number of connected pixels?****
}



// finds connected neighbors of pixel s

  void ConnectedNeighbors(struct pixel s, double T, double **img, int width, int height, int *M, struct pixel *c) {

  double Ts; // tested threshold value
  double Tabs; // absolute value of threshold test
  struct pixel r; // Pixels for all neighbors, connected or not

  *M = 0;

  // define neighbors
  // test boundary conditions to avoid segfault
  if(s.m > 0){
    r.m = s.m-1;
    r.n = s.n;

    Ts = img[r.m][r.n] - img[s.m][s.n];
    Tabs = fabs(Ts);
	    
    if (Tabs <= T) {
      c[*M].m = r.m;
      c[*M].n = r.n;
      (*M)++;
    }
  }
  
  if(s.n < height-1) {
    r.m = s.m;
    r.n = s.n+1;

    Ts = img[r.m][r.n] - img[s.m][s.n];
    Tabs = fabs(Ts);
	    
    if (Tabs <= T) {
      c[*M].m = r.m;
      c[*M].n = r.n;
      (*M)++;
    }
  }

  if(s.m < width-1) {
    r.m = s.m+1;
    r.n = s.n;

    Ts = img[r.m][r.n] - img[s.m][s.n];
    Tabs = fabs(Ts);
	    
    if (Tabs <= T) {
      c[*M].m = r.m;
      c[*M].n = r.n;
      (*M)++;
    }
  }

  if(s.n > 0) {
    r.m = s.m;
    r.n = s.n-1;

    Ts = img[r.m][r.n] - img[s.m][s.n];
    Tabs = fabs(Ts);
	    
    if (Tabs <= T) {
      c[*M].m = r.m;
      c[*M].n = r.n;
      (*M)++;
    }
  }


  return;
}
