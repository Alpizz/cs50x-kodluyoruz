// Caesar
// Encrypts plain text by shifting letter
// Also decrypts encrypted message by taking input of neg. numbers (extra to pset2)
// by Alpizz
#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <ctype.h>
#include <stdlib.h>

// Program takes shift amount as argument (ie. ./caesar 2)
int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else
    {
        for (int i = 0; i < strlen(argv[1]); i++)
        {
            // The argument must not contain non digit characters (except '-' sign)
            if (!isdigit(argv[1][i]) && argv[1][i] != '-')
            {
                printf("Usage: ./caesar key\n");
                return 1;
            }
        }
    }
    // Conversion of input string to int
    int key = atoi(argv[1]);
    // User prompt to input text to be ciphered
    string plain = get_string("plaintext: ");

    // Init empty char and int
    // This must be done in order to maintain negative shifts (extra)
    char new_letter = ' ';
    int new_letter_mod;
    printf("ciphertext: ");

    for (int i = 0; i < strlen(plain); i++)
    {
        // Print non-alphabetic chars as they are
        if (!isalpha(plain[i]))
        {
            printf("%c", plain[i]);
        }
        // Check for ASCII [a, z] [97, 122] lower letters
        else if (islower(plain[i]))
        {
            // Calculation of caesar shift
            // new_letter_mod gives how many letters are going to be shifted
            // from 'a'
            new_letter_mod = (plain[i] + key - 'a') % 26;

            // Since % operator returns remainder, to handle negative numbers
            // a check loop must be written (extra)
            for (int j = 0; new_letter_mod < 0; j++)
            {
                new_letter_mod += 26;
            }
            // Caesar shift from 'a' and print
            printf("%c", new_letter_mod + 'a');

        }
        else
        {
            // Same process for ASCII [A, Z] [65, 90] upper letters
            new_letter_mod = (plain[i] + key - 'A') % 26;
            for (int j = 0; new_letter_mod < 0; j++)
            {
                new_letter_mod += 26;
            }
            // Caesar shift from 'A' and print
            printf("%c", new_letter_mod + 'A');
        }
    }
    // Print newline character
    printf("\n");

}