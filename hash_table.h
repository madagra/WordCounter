#include "common.h"

/* Implementation of a hash table using the standard chaining scheme 
 * The data structure used for storing the values of each bucket is a 
 * simple singly-linked list
 * The hash function used is the well known djb2 function */

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

/* initial number of buckets in the hash table
 * this correspond to a memory consumption of 4 MB */
#define NUM_BUCKET  (1 << 22) 

/* Maximum allowed size for a bucket
 *   TODO: implement a hash table rescaling for increasing the number of buckets
 *   in the case that one bucket reach its maximum size specified by the
 *   following macro */
#define MAX_SIZE_BUCKET (1 << 6) 

#define HT_ITER(ht,bucket) for(int i = 0; i < nbuckets; bucket = ht[i], i++)

/* Structure defining a node of a hash table bucket 
 * It contains the word, the number of occurrences and a pointer 
 * to the next node in the list */
struct hash_node {
    char *word;
    uint64_t count;
    struct hash_node *next;
};
typedef struct hash_node hash_node;

/* the hash table is defined as an array of pointers to 
 * pointers to hash_node structure, i.e. elements of the linked-list
 * contained in each bucket */
typedef hash_node** hash_table;

uint64_t nbuckets;
uint64_t ncollisions;

/* hash table public API */

int ht_init(hash_table*);

int ht_add(hash_table,char*);

hash_node* ht_find(hash_table,char*);

void ht_count(hash_table, char*);

void ht_free(hash_table);

#endif
