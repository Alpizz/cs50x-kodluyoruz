// Recovers deleted JPEG files
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    // Cm-line argument check
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }
    // Open card.raw into file ptr
    FILE *fileptr = fopen(argv[1], "r");

    // Null ptr check
    if (fileptr == NULL)
    {
        printf("Null file.\n");
        return 1;
    }

    // Init 512 byte sized buffer block
    unsigned char buffer[512];

    // Filename "###.jpg"
    char filename[8];

    // Eof flag
    bool end_of_file = false;

    // Amount of read blocks
    int n_of_read = 0;

    // Count of recovered jpeg, -1 for init
    int jpeg_count = -1;

    // File ptr for jpeg
    FILE *img;

    // Loops until end of file
    while (!end_of_file)
    {
        // Reads 512 byte block into buffer array
        n_of_read = fread(buffer, 512, 1, fileptr);

        // Checks if block is the beginning of a JPEG file
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            jpeg_count++;
            // Closes previously opened image if it's not the first one
            if (jpeg_count != 0)
            {
                fclose(img);
            }

            // Prints to filename string
            sprintf(filename, "%03i.jpg", jpeg_count);
            // Opens ###.jpg for writing
            img = fopen(filename, "w");
            // Writes the first block of JPEG file
            fwrite(buffer, 512, 1, img);
        }
        // Checks for reaching end of file
        else if (n_of_read == 0)
        {
            end_of_file = true;
        }
        // Writes if JPEG sequence has begun
        // Skips if hasn't
        else
        {
            if (jpeg_count >= 0)
            {
                fwrite(buffer, 512, 1, img);
            }
            continue;
        }
    }
    // Prints recovered image count to the screen
    printf("%i JPEG files have been recovered.\n", jpeg_count + 1);
    // Close remaining files
    fclose(img);
    fclose(fileptr);
    return 0;
}
