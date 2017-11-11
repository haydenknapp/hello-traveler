nvcc -ccbin clang++-3.8 -arch=sm_50 main.cu gpu.cu multi.cu single.cu -lm -o hellotraveler
