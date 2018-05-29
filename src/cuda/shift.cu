__global__ void bgr_to_gray_kernel(unsigned char* input,
                     unsigned char* output,
                     int width,
                     int height,
                     int colorWidthStep,
                     int grayWidthStep)
{
    //2D Index of current thread
    const int xIndex = blockIdx.x * blockDim.x + threadIdx.x;
    const int yIndex = blockIdx.y * blockDim.y + threadIdx.y;

    //Only valid threads perform memory I/O
    if((xIndex < width) && (yIndex <  height))
    {
        /* Kernel Code Here */
    }
}