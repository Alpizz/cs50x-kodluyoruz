// Substitution
// Ciphers text according to given key
#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

// Gets key string as commandline argument
int main(int argc, string argv[])
{
    // Check if required number of argument has been entered
    if (argc != 2)
    {
        printf("Usage: ./substitution KEY\n");
        return 1;
    }
    // Define key
    string key = argv[1];
    // Init counter
    int count = 0;
    // Check key length
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // For every letter, check how many times this letter in string
    for (int i = 0 ; i < strlen(key) ; i++)
    {
        // Make the key all uppercase
        key[i] = toupper(key[i]);
        
        // Check every letter is alphabetic
        if (!isalpha(key[i]))
        {
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
        else
        {
            // Loop for checking every letter is different from each other
            for (int j = 0; j < strlen(key); j++)
            {
                
                if (key[j] == key[i])
                {
                    count++;
                }
            }
            
            if (count != 1)
            {
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
            else
            {
                count = 0;
            }
        }
    }
    
    // Init real alphabet all uppercase
    string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    // User prompt to enter plain text
    string plain = get_string("plaintext: ");
    
    // Init difference (which will be used for ciphering)
    int difference;
    
    printf("ciphertext: ");
    
    // Go through every letter in plain text
    for (int i = 0; i < strlen(plain); i++)
    {
        // Go through every letter in real alphabet
        for (int j = 0; j < strlen(alpha); j++)
        {
            // Calculate difference between ASCII values
            // ie: if first letter of key is C, the difference is C - A which is
            // 67 - 65 = 2
            difference = key[j] - alpha[j];
            
            // Determine ciphered char by looking real alphabet and plain text
            if (alpha[j] == plain[i])
            {
                printf("%c", plain[i] + difference);
            }
            // Check for lowercase in order to cipher correctly
            else if (tolower(alpha[j]) == plain[i])
            {
                printf("%c", tolower(plain[i] + difference));
            }
            
        }
        // Don't cipher non-alphabetic chars
        if (!isalpha(plain[i]))
        {
            printf("%c", plain[i]);
        }
    }
    // Newline character
    printf("\n");
    
    // Exit successfully
    return 0;
}