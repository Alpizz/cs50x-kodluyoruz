// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 65536;

// Hash table
node *table[N];

// Number of words in dictionary
int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Gets index value from hash for table
    unsigned int index = hash(word);
    // New node to check
    node *dict = table[index];
    
    // Returns false if nothing declared for that index
    if (dict == NULL)
    {
        return false;
    }
    
    // Repeats until end of the linked chain
    while (dict->next != NULL)
    {
        // Returns true if a word in chain matches checking word
        if (strcasecmp(dict->word, word) == 0)
        {
            return true;
        }
        // Checks next word
        else
        {
            dict = dict->next;
        }
    }
    // Returns true if first node word is checking word
    if (dict->next == NULL && strcasecmp(dict->word, word) == 0)
    {
        return true;
    }
    // Returns false if word isn't in dictionary
    return false;
}
// Hashes word to a number
// Source: djib2 by Dan Bernstein (http://www.cse.yorku.ca/~oz/hash.html)
// Adapted from:
// https://www.reddit.com/r/cs50/comments/eo4zro/good_hash_function_for_speller/
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);
    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c;
        c = *word++;
        c = tolower(c);
    }
    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Opens dictionary file for reading
    FILE *fp = fopen(dictionary, "r");

    // Null ptr check
    if (fp == NULL)
    {
        printf("Error opening %s\n", dictionary);
        return false;
    }
    // Temporary word for copy
    char temp_word[LENGTH];

    // Scans file line by line into temp_word until EOF
    while (fscanf(fp, "%s", temp_word) != EOF)
    {
        // Allocates memory for new node
        node *n = malloc(sizeof(node));
        
        // Null ptr check
        if (n == NULL)
        {
            fclose(fp);
            printf("Malloc failed\n");
            return false;
        }
        // Copies new word into node
        strcpy(n->word, temp_word);
        
        // Sets next ptr to NULL
        n->next = NULL;
        
        // Gets index from hash for word
        unsigned int index = hash(temp_word);
        
        // Adds new nodes to the table
        // Counts words for size()
        if (table[index] == NULL)
        {
            table[index] = n;
            word_count++;
        }
        else
        {
            n->next = table[index];
            table[index] = n;
            word_count++;
        }
        
        // Resets temp_word
        temp_word[0] = '\0';
    }
    // Closes file ptr    
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // printf("wordct: %i\n", word_count); 
    return word_count;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // Goes through every head in the table
    for (int i = 0; i < N; i++)
    {
        // Init cursor to check
        node *cursor = table[i];
        // Skips if there isn't any list in index i
        if (cursor == NULL)
        {
            continue;
        }
        
        // Frees the chain until end
        while (cursor->next != NULL)
        {
            // Temporary node for not getting lost
            node *tmp = cursor;
            // Goes to the next node
            cursor = cursor->next;
            // Frees previous
            free(tmp);
        }
        // Frees the remainder
        free(cursor);
    }
    return true;
}
