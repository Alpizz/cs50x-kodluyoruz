#include "helpers.h"
#include <math.h>
#include <stdio.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize average value
    float avg = 0;
    // Go through every pixel in the bitmap file
    for (int i = 0; i < height ; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Get every pixel's RGB value and replace them by their average, in order to get
            // grayscale picture
            avg = round((float)(image[i][j].rgbtRed + image[i][j].rgbtBlue + image[i][j].rgbtGreen) / 3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Temporary image pixel placeholder
    RGBTRIPLE temp;
    // Go through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Break out of loop if index if halfway through i'th row's columns
            if (j > (width - 1 - j))
            {
                break;
            }
            // Reflects image with using middle temporary pixel
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Optional, magnitude of blur process
    // Searching box size
    int mag = 3;

    // Sub-array for neighbor pixels
    RGBTRIPLE sub_img[mag][mag];
    // Blurred image array
    RGBTRIPLE blur_img[height][width];

    // Initialize counter and sums
    float count = 0;
    float sum_red = 0, sum_blu = 0, sum_gr = 0;

    // Go through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Go through every pixel in sub-array (3x3 radius neighbor pixels)
            for (int k = 0; k < mag; k++)
            {
                // Skips if neighbor isn't available
                if ((i + (k - 1)) < 0 || (i + (k - 1)) > height - 1)
                {
                    continue;
                }

                for (int l = 0; l < mag; l++)
                {
                    // Skips if neighbor isn't available
                    if ((j + (l - 1)) < 0 || (j + (l - 1)) > width - 1)
                    {
                        continue;
                    }

                    // Adds valid neighbor pixels to sub-array
                    sub_img[k][l] = image[i + (k - 1)][j + (l - 1)];
                    // Counts for average calculation
                    count++;

                    // Sums all RGB values seperately for average calc
                    sum_red += (float)sub_img[k][l].rgbtRed;
                    sum_gr += (float)sub_img[k][l].rgbtGreen;
                    sum_blu += (float)sub_img[k][l].rgbtBlue;
                }
            }
            // Fills blur_img array with average values
            blur_img[i][j].rgbtRed = (int)round(sum_red / count);
            blur_img[i][j].rgbtGreen = (int)round(sum_gr / count);
            blur_img[i][j].rgbtBlue = (int)round(sum_blu / count);

            // Reset counter and sums
            sum_red = 0;
            sum_blu = 0;
            sum_gr = 0;
            count = 0;
        }
    }

    // Copies blurred image to the actual image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = blur_img[i][j];
        }
    }
    return;
}

// Returns square root of (a^2 + b^2)
float pythogoras(float a, float b)
{
    float w_sum = pow(a, 2) + pow(b, 2);
    return sqrt(w_sum);
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Edge detected image array
    RGBTRIPLE edge_img[height][width];
    // Sub array for neighbors
    RGBTRIPLE sub_img[3][3];

    // Sobel operator arrays
    int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Initial Gx and Gy values for all channels
    float Gx_red = 0, Gx_gr = 0, Gx_blu = 0;
    float Gy_red = 0, Gy_gr = 0, Gy_blu = 0;

    // Go through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Go through every pixel of sub array and Sobel operator
            for (int k = 0; k < 3; k++)
            {
                for (int l = 0; l < 3; l++)
                {
                    // Check for borders
                    if (((i + (k - 1)) < 0 || (i + (k - 1)) > height - 1) || ((j + (l - 1)) < 0 || (j + (l - 1)) > width - 1))
                    {
                        sub_img[k][l].rgbtRed = 0;
                        sub_img[k][l].rgbtGreen = 0;
                        sub_img[k][l].rgbtBlue = 0;
                        continue;
                    }

                    // Fill sub array with 3x3 neighbor channel values
                    sub_img[k][l] = image[i + (k - 1)][j + (l - 1)];

                    // Calculate Gx values
                    Gx_red += sub_img[k][l].rgbtRed * sobel_x[k][l];
                    Gx_gr += sub_img[k][l].rgbtGreen * sobel_x[k][l];
                    Gx_blu += sub_img[k][l].rgbtBlue * sobel_x[k][l];
                    // Calculate Gy values
                    Gy_red += sub_img[k][l].rgbtRed * sobel_y[k][l];
                    Gy_gr += sub_img[k][l].rgbtGreen * sobel_y[k][l];
                    Gy_blu += sub_img[k][l].rgbtBlue * sobel_y[k][l];
                }
            }

            // Cap all values to 255 if exceeds
            if (round(pythogoras(Gx_red, Gy_red)) > 255)
            {
                edge_img[i][j].rgbtRed = 255;
            }
            else
            {
                edge_img[i][j].rgbtRed = round(pythogoras(Gx_red, Gy_red));
            }

            if (round(pythogoras(Gx_gr, Gy_gr)) > 255)
            {
                edge_img[i][j].rgbtGreen = 255;
            }
            else
            {
                edge_img[i][j].rgbtGreen = round(pythogoras(Gx_gr, Gy_gr));
            }
            if (round(pythogoras(Gx_blu, Gy_blu)) > 255)
            {
                edge_img[i][j].rgbtBlue = 255;
            }
            else
            {
                edge_img[i][j].rgbtBlue = round(pythogoras(Gx_blu, Gy_blu));
            }
            // Reset Gx and Gy values for next iteration
            Gx_red = 0;
            Gx_gr = 0;
            Gx_blu = 0;

            Gy_red = 0;
            Gy_gr = 0;
            Gy_blu = 0;
        }
    }
    // Copy array
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = edge_img[i][j];
        }
    }
    return;
}
