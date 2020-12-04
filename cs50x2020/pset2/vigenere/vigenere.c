#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>

int shift(char c);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
    else
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            if (!isalpha(argv[1][i]))
            {
                printf("Usage: ./vigenere keyword\n");
                return 1;
            }
        }
        
    }
    int key = 0;
    int j = 0;
    int new_letter_mod = 0;
    string plain = get_string("plaintext: ");
    
    /*
    int direction = 0;
    
    do
    {
        direction = get_int("direction ('-1' or '1'): ");
    }
    while (direction == 0 || direction > 1 || direction < -1);
    */
    
    
    printf("ciphertext: ");
    for (int i = 0; i < strlen(plain); i++)
    {
        if (isalpha(plain[i]))
        {
            key = /*direction * */ shift(argv[1][j % strlen(argv[1])]);
            
            if (islower(plain[i]))
            {
                new_letter_mod = (plain[i] + key - 'a') % 26;

                // Since % operator returns remainder, to handle negative numbers
                // a check loop must be written (extra)
                for (j = 0; new_letter_mod < 0; j++)
                {
                    new_letter_mod += 26;
                }
                printf("%c", new_letter_mod + 'a');
                j++;
            }
            else
            {
                new_letter_mod = (plain[i] + key - 'a') % 26;

                // Since % operator returns remainder, to handle negative numbers
                // a check loop must be written (extra)
                for (j = 0; new_letter_mod < 0; j++)
                {
                    new_letter_mod += 26;
                }
                printf("%c", new_letter_mod + 'A');
                j++;
            }
        }
        else
        {
            printf("%c", plain[i]);
        }
    }
    printf("\n");
}

int shift(char c)
{
    if (isupper(c))
    {
        return c - 'A';
    }
    else
    {
        return c - 'a';
    }
}
