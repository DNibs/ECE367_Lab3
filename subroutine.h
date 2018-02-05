#include <stdio.h>
#include <math.h>
#include <stdlib.h>

// define structure "pixel"
struct pixel {
  int m,n; // coordinate for pixel
};


// Declare functions
void ConnectedSet(struct pixel s, double T, double **img, int width, int height, int ClassLabel, int **seg, int *NumConPixels);

void ConnectedNeighbors(struct pixel s, double T, double **img, int width, int height, int *M, struct pixel *c);



