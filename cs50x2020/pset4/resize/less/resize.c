// Resizes a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // check if second argument is valid integer or not
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (!isdigit(argv[1][i]))
        {
            fprintf(stderr, "n must be a valid int\n");
            return 5;
        }
    }
    // convert str to int
    int n = atoi(argv[1]);

    // check range
    if (n > 100 || n < 0)
    {
        fprintf(stderr, "0 <= n <= 100 required\n");
        return 6;
    }


    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf_in;
    fread(&bf_in, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi_in;
    fread(&bi_in, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf_in.bfType != 0x4d42 || bf_in.bfOffBits != 54 || bi_in.biSize != 40 ||
        bi_in.biBitCount != 24 || bi_in.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine padding for input scanlines
    int padding_in = (4 - (bi_in.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // new bitmap headers, same initially
    BITMAPFILEHEADER bf_out = bf_in;
    BITMAPINFOHEADER bi_out = bi_in;

    // new width and height values
    bi_out.biWidth = bi_in.biWidth * n;
    bi_out.biHeight = bi_in.biHeight * n;

    // determine padding for output scanlines
    int padding_out = (4 - (bi_out.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // new calculated bi and bf values for output file
    bi_out.biSizeImage =
        ((sizeof(RGBTRIPLE) * bi_out.biWidth) + padding_out) * abs(bi_out.biHeight);

    bf_out.bfSize =
        bi_out.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_out, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_out, sizeof(BITMAPINFOHEADER), 1, outptr);

    // row array, remembers pixels
    RGBTRIPLE resized[bi_out.biWidth + padding_out];
    // count
    int count = 0;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi_in.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < bi_in.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to resized array
            for (int k = 0; k < n; k++)
            {
                resized[count] = triple;
                count++;
            }
        }
        // skip over padding, if any
        fseek(inptr, padding_in, SEEK_CUR);

        // write resized row with padding, n times
        for (int t = 0; t < n; t++)
        {
            fwrite(resized, sizeof(RGBTRIPLE) * count, 1, outptr);

            // add padding
            for (int k = 0; k < padding_out; k++)
            {
                fputc(0x00, outptr);
            }
        }
        // reset count
        count = 0;
    }
    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
