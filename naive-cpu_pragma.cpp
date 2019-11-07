#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <omp.h>
#include <bits/stdc++.h>

using namespace std;

const int KERNEL_X[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}};

const int KERNEL_Y[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}};

inline int getIndex(int row, int col, int width)
{
    return row * width + col;
}

int conv2d(int *image, int row, int col, int width, int height, const int kernel[3][3], int kernel_size)
{
    const int half_kernel_size = kernel_size / 2;
    int result = 0;
    for (int i = -half_kernel_size; i <= half_kernel_size; i++)
    {
        for (int j = -half_kernel_size; j <= half_kernel_size; j++)
        {
            int c_row = row + i;
            int c_col = col + j;
            if (c_row < 0 || c_row >= height)
            {
                continue;
            }
            if (c_col < 0 || c_col >= width)
            {
                continue;
            }
            int kernel_i = i + half_kernel_size;
            int kernel_j = j + half_kernel_size;
            int curPixel = image[getIndex(c_row, c_col, width)];
            result += kernel[kernel_i][kernel_j] * curPixel;
        }
    }
    return result;
}

void sobel_filters(int *src, int *dest, int width, int height)
{
    int hypot_max = -1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int cx = conv2d(src, i, j, width, height, KERNEL_X, 3);
            int cy = conv2d(src, i, j, width, height, KERNEL_Y, 3);
            int hypot = sqrt((cx * cx) + (cy * cy));
            if (hypot > hypot_max)
            {
                hypot_max = hypot;
            }

            int index = getIndex(i, j, width);
            dest[index] = hypot;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = getIndex(i, j, width);
            dest[index] = (dest[index] * 255) / hypot_max;
        }
    }
}

void threshold(int *src, int *dest, int width, int height, double lowThresholdRatio, double highThresholdRatio)
{
    int p_max = -1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = getIndex(i, j, width);
            int p = src[index];
            if (p > p_max)
            {
                p_max = p;
            }
        }
    }

    highThresholdRatio = p_max * highThresholdRatio;
    lowThresholdRatio = lowThresholdRatio * highThresholdRatio;

    const int WEAK = 25;
    const int STRONG = 255;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = getIndex(i, j, width);
            int p = src[index];
            if (p >= highThresholdRatio)
            {
                int index = getIndex(i, j, width);
                dest[index] = STRONG;
            }
            else if (p <= highThresholdRatio && p >= lowThresholdRatio)
            {
                int index = getIndex(i, j, width);
                dest[index] = WEAK;
            }
        }
    }
}

void hysteresis(int *src, int *dest, int width, int height, int weak, int strong)
{
#pragma omp parallel
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            int index = getIndex(i, j, width);
            if (src[index] == weak)
            {
                int iTL = getIndex(i - 1, j - 1, width);
                int iTM = getIndex(i - 1, j, width);
                int iTR = getIndex(i - 1, j + 1, width);
                int iL = getIndex(i, j - 1, width);
                int iR = getIndex(i, j + 1, width);
                int iBL = getIndex(i + 1, j - 1, width);
                int iBM = getIndex(i + 1, j, width);
                int iBR = getIndex(i + 1, j + 1, width);
                if (
                    src[iTL] == strong || src[iTM] == strong || src[iTR] == strong || src[iL] == strong || src[iR] == strong || src[iBL] == strong || src[iBM] == strong || src[iBR] == strong)
                {
                    dest[index] = strong;
                }
                else
                {
                    dest[index] = 0;
                }
            }
            else
            {
                dest[index] = src[index];
            }
        }
    }
}

int main()
{
    int width, height;
    scanf("%d %d", &width, &height);
    int *image = new int[width * height];
    int round = width * height;
    for (int i = 0; i < round; i++)
    {
        scanf("%d", &image[i]);
    }

    int *result = new int[width * height];

    clock_t start = clock();

    //sobel_filters(image, result, width, height);
    //threshold(image, result, width, height, 0.05, 0.09);
    hysteresis(image, result, width, height, 30, 255);
    clock_t end = clock();

    time(&end);

    printf("%lf\n", (end - start) / (double)CLOCKS_PER_SEC);

    // printf("%d %d\n", width, height);
    // for (int i = 0; i < width; i++) {
    //     for (int j = 0; j < height; j++) {
    //         int index = getIndex(i, j, width);
    //         printf("%d\n", result[index]);
    //     }
    // }
    return 0;
}