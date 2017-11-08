nvcc -ccbin clang-3.8 -arch=sm_50 main.c gpu.cu multi.c single.c -lm -o hellotraveler
