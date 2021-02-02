// Implements a dictionary's functionality
#include <stdbool.h>
#include <string.h>
#include "dictionary.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

#define HASH_SIZE 65536

// Hash table
node *table[HASH_SIZE] = {0};
int wordcount = 0;

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    char tmp_word[LENGTH + 1] = {0};
    int hash_num;

    //open file to read dictionary
    FILE *read_file = fopen(dictionary, "r");
    if (read_file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    //Go through every word until end of doc
    while (fscanf(read_file, "%s", tmp_word) != EOF)
    {
        //create node to hold new word
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        //set value of word and next in new node
        strcpy(n-> word, tmp_word);
        n -> next = NULL;

        //get hash num of new word
        hash_num = hash(tmp_word);

        //case 1: no nodes just add to begining
        if (table[hash_num] == NULL)
        {
            table[hash_num] = n;
            wordcount++;
        }

        //case 2: move 1st in list to 2nd and add new node to front
        else
        {
            n -> next = table[hash_num];
            table[hash_num] = n;
            wordcount++;
        }
    }

    fclose(read_file);
    return true;
}


// Hashes word to a number
//https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/
unsigned int hash(const char *word)
{
    unsigned int hash = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        //add tolower so it lowers all words in texts so hash values match all dictionary words which are lower
        hash = (hash << 2) ^ tolower(word[i]);
    }
    return hash % HASH_SIZE;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return wordcount;
}


// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //get hash location
    int hash_num = hash(word);

    // for each node in array bucket
    for (node *cursor = table[hash_num]; cursor != NULL; cursor = cursor->next)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
    }

    return false;
}


// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{

    for (int i = 0; i < HASH_SIZE; i++)
    {
        //will list isnt null there are nodes
        while (table[i] != NULL)
        {
            // Point to the next node with tmp var
            node *tmp = table[i]->next;
            // Free header as tmp is pointing to next
            free(table[i]);
            // set header to next node
            table[i] = tmp;
        }
    }

    return true;
}
