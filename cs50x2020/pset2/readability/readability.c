// Readability
// index = 0.0588 * L - 0.296 * S - 15.8
// If index > 16 output: "16+"
// If index < 1, output: "Before grade 1"

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void check_grade(int ind);

int main(void)
{
    // Prompt user to input text to be graded
    string text = get_string("Text: ");
    
    // Init counts
    int count_letter = 0, count_word = 0, count_sent = 0;
    
    // Go through every char of the input str
    for (int i = 0; i < strlen(text) ; i++)
    {
        // Check and count if it's an alphabetic letter
        if (isalpha(text[i]))
        {
            count_letter++;
        }
        // Count spaces in order to determine word count (3 words has 2 spaces)
        if (text[i] == ' ')
        {
            count_word++;
        }
        // Count sentences
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count_sent++;
        }
    }
    // Average letter per 100 word
    float avg_L = 100 * ((float)count_letter / (count_word + 1));
    // Average sentence per 100 word
    float avg_S = 100 * ((float)count_sent / (count_word + 1));
    
    // Calculation of Coleman-Liau index
    float index = 0.0588 * avg_L - 0.296 * avg_S - 15.8;
    
    // Determine grade from index
    check_grade(round(index));

}

// Function for determining grade level
void check_grade(int ind)
{
    if (ind > 16)
    {
        printf("Grade 16+\n");
    }
    else if (ind < 16 && ind > 1)
    {
        printf("Grade %i\n", ind);
    }
    else
    {
        printf("Before Grade 1\n");
    }
}