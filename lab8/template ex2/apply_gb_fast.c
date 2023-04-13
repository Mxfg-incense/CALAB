#include "main.h"
Image transpose(Image a, size_t block_size){
    /* img_sc() will return a copy of the given image*/
    Image b = img_sc(a);
    b.dimX = a.dimY;
    b.dimY = a.dimX;

    for (int i = 0; i < (a.dimX / block_size) + 1; ++i)
        for (int j = 0; j < (a.dimY / block_size) + 1; ++j)
            for (int m = 0; m < block_size; ++m)
                for (int n = 0; n < block_size; ++n)
                {
                    // x = i * block_size + n 
                    // y = j * block_size + m
                    if (i * block_size + n >= a.dimX || j * block_size + m >= a.dimY)
                        continue;
                    b.data[a.dimY * (i * block_size + n) + j * block_size + m] = a.data[a.dimX * (j * block_size + m) + i * block_size + n];
                   
                }
    return b;
}

Image apply_gb(Image a, FVec gv)
{
    size_t block_size = 8;
    struct timeval start_time, stop_time, elapsed_time;
    gettimeofday(&start_time,NULL);
    
    Image b = gb_h(a, gv);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("horizontal gaussian blur time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);

    gettimeofday(&start_time,NULL);

    Image c = transpose(gb_h(transpose(b,block_size),gv),block_size);

    gettimeofday(&stop_time,NULL);
    timersub(&stop_time, &start_time, &elapsed_time); 
    printf("vertical gaussian blur time: %f \n", elapsed_time.tv_sec+elapsed_time.tv_usec/1000000.0);

    free(b.data);
    return c;
}
