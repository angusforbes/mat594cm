#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "main.h"

float vs[] = {-1, -1, 0, 1, -1, 0, 1, 1, 0, -1, 1, 0};

void normalizeNormal(float n[]) {
  float len = sqrt(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
  n[0] /= len;
  n[1] /= len;
  n[2] /= len;
}

void computeNormal(float v1[], float v2[], float n[]) {
  n[0] = v1[1] * v2[2] - v2[1] * v1[2];
  n[1] = v1[2] * v2[0] - v2[2] * v1[0];
  n[2] = v1[0] * v2[1] - v2[0] * v1[1];
  normalizeNormal(n);
} 

void init(void) {
  int p0off = 0; 
  int p1off = 3; 
  int p3off = 10;

  float v1[] = {vs[p1off + 0] - vs[p0off + 0], vs[p1off + 1] - vs[p0off + 1], vs[p1off + 2] - vs[p0off + 2]};
  float v2[] = {vs[p3off + 0] - vs[p0off + 0], vs[p3off + 1] - vs[p0off + 1], vs[p3off + 2] - vs[p0off + 2]};

  float* n = (float*)malloc(3 * sizeof(float));
  computeNormal(v1, v2, n);
  
  printf("%f/%f/%f \n", n[0], n[1], n[2]);

}

int main(int argc, char** argv) {
  init();
}


