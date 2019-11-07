#include <cstdio>
#include <cstdlib>
#include <cmath>

using namespace std;

struct Pixel
{
    int R;
    int G;
    int B;
};

const int KERNEL_X[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}};

const int KERNEL_Y[3][3] = {
    {1, 2, 1},
    {0, 0, 0},
    {-1, -2, -1}};

int getIndex(int row, int col, int width)
{
    return row * width + col;
}

Pixel conv2d(Pixel *image, int row, int col, int width, int height, const int kernel[3][3], int kernel_size)
{
    const int half_kernel_size = kernel_size / 2;
    Pixel result = {0, 0, 0};
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
            Pixel curPixel = image[getIndex(c_row, c_col, width)];
            result.R += kernel[kernel_i][kernel_j] * curPixel.R;
            result.G += kernel[kernel_i][kernel_j] * curPixel.G;
            result.B += kernel[kernel_i][kernel_j] * curPixel.B;
        }
    }

    return result;
}

int max(int a, int b)
{
    return a > b ? a : b;
}

void sobel_filters(Pixel *src, Pixel *dest, int width, int height)
{
    int hypot_max = -1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Pixel cx = conv2d(src, i, j, width, height, KERNEL_X, 3);
            // printf("%d %d %d\n", cx.R, cx.G, cx.B);
            Pixel cy = conv2d(src, i, j, width, height, KERNEL_Y, 3);
            // printf("%d %d %d\n", cy.R, cy.G, cy.B);
            int hypotR = sqrt((cx.R * cx.R) + (cy.R * cy.R));
            int hypotG = sqrt((cx.G * cx.G) + (cy.G * cy.G));
            int hypotB = sqrt((cx.B * cx.B) + (cy.B * cy.B));
            if (hypotR > hypot_max)
            {
                hypot_max = hypotR;
            }
            if (hypotG > hypot_max)
            {
                hypot_max = hypotG;
            }
            if (hypotB > hypot_max)
            {
                hypot_max = hypotB;
            }
            // printf("%d %d %d\n", hypotR, hypotG, hypotB);

            Pixel result = {hypotR, hypotG, hypotB};
            int index = getIndex(i, j, width);
            dest[index] = result;
        }
    }

    // printf("MAX: %d\n", hypot_max);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int index = getIndex(i, j, width);
            Pixel cal = dest[index];
            cal.R = (cal.R * 255) / hypot_max;
            cal.G = (cal.G * 255) / hypot_max;
            cal.B = (cal.B * 255) / hypot_max;
            // printf("(%d/%d):%d %d %d\n", cal.R, hypot_max, cal.R / hypot_max, cal.G / hypot_max, cal.B / hypot_max);
            dest[index] = cal;
        }
    }
}

int main()
{
    int width, height;
    scanf("%d %d", &width, &height);
    Pixel *image = new Pixel[width * height];
    int round = width * height;
    for (int i = 0; i < round; i++)
    {
        scanf("%d %d %d", &image[i].R, &image[i].G, &image[i].B);
    }

    Pixel *result = new Pixel[width * height];

    sobel_filters(image, result, width, height);

    printf("%d %d\n", width, height);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int index = getIndex(i, j, width);
            Pixel p = result[index];
            printf("%d %d %d\n", p.R, p.G, p.B);
        }
    }
    return 0;
}