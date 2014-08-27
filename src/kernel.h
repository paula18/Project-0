/* CIS565 CUDA Checker: A simple CUDA hello-world style program for 
   Patrick Cozzi's CIS565: GPU Computing at the University of Pennsylvania
   Written by Yining Karl Li, Liam Boone, and Harmony Li.
   Copyright (c) 2014 University of Pennsylvania */

#ifndef KERNEL_H
#define KERNEL_H

#include <cuda.h>
#include <vector_types.h>

void CudaKernel(uchar4* pos, int width, int height, int major, int minor);

#endif
