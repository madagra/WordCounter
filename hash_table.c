#include "hash_table.h"

#define HASH_STR djb2hash
#define HASH_INT hash64shift

/* use double hashing (first string, then integer) to decrease number
 * of collisions in the hash table */
#define HASH_FN(word,tsize) HASH_INT(HASH_STR(word),tsize)

/* local prototypes */
static uint64_t djb2hash(char *);
static uint64_t hash64shift(uint64_t, int);
static hash_node* set_ht_node(const char*);

int 
ht_init(hash_table *ht) 
{
    int i;
    nbuckets = NUM_BUCKET;
    /* keep track of the number of collisions in the hash table */
    ncollisions = 0;
    LOG_DEBUG("Total number of buckets in the hash table: %ld\n",nbuckets);
    *ht = malloc(nbuckets * sizeof(hash_node*));
    if( NULL == *ht ) {
        printf("ERROR: out of memory in hash table initialization\n");
        return OUT_OF_MEMORY;
    }

    for(i = 0; i < nbuckets; i++) {
         (*ht)[i] = NULL;
    }

    return SUCCESS;
}

int 
ht_add(hash_table ht,char *word) 
{
    uint64_t hash = HASH_FN(word,nbuckets);
    LOG_DEBUG("Adding word \"%s\" \t hash: %ld\n",word,hash);

    /* the bucket is still empty, therefore just add the supplied word 
     * This should be the most frequent branch of the IF in the case that 
     * the hash function is well defined and a sufficient number of buckets
     * is initialized */
    if ( LIKELY(NULL == ht[hash]) ) {
        ht[hash] = set_ht_node(word);
        if (NULL == ht[hash]) {
            return OUT_OF_MEMORY;
        }
        ht[hash]->next  = NULL; 

    }
    /* the bucket contains some elements, therefore one needs to 
     * search if the corresponding key is already present, otherwise
     * add a new node */
    else {
        ncollisions++;
        hash_node *htmp = NULL; 
        hash_node *hcur = ht[hash];
        while (NULL != hcur) {
            htmp = hcur;
            if( 0 == strcmp(hcur->word,word) ) {
                /* the word is already present, thus it is not a collision */
                ncollisions--;
                return SUCCESS;
            }
            hcur = hcur->next;
        }
        hcur = set_ht_node(word);
        if (NULL == hcur) {
            return OUT_OF_MEMORY;
        }
        /* if no match is found the variable hcur is at the end of the list */
        htmp->next = hcur;
    }
    return SUCCESS;
}

hash_node*
ht_find(hash_table ht,char *word)
{
    uint64_t hash = HASH_FN(word,nbuckets);
    
    if (NULL != ht[hash]) {
        /* again here it is assumed that there is most likely only 
         * one element in the current bucket, so searching the list can
         * be avoided */
        if ( LIKELY(NULL == ht[hash]->next) ) {
            return ht[hash];
        /* the bucket contains more than one element thus search for 
         * matching word */
        } else {
            hash_node *hcur;
            hcur = ht[hash];
            while (NULL != hcur) {
                if( 0 == strcmp(hcur->word,word) ) {
                    return hcur;
                }
                hcur = hcur->next;
            }
        }
    } 
    return NULL;
}

void
ht_count(hash_table ht,char *word)
{
    uint64_t hash = HASH_FN(word,nbuckets);
    
    if (NULL != ht[hash]) {
        /* again here it is assumed that there is most likely only 
         * one element in the current bucket, so searching the list can
         * be avoided */
        if ( LIKELY(NULL == ht[hash]->next) ) {
            ht[hash]->count++;
            return; 
        } else {
            hash_node *hcur;
            hcur = ht[hash];
            while (NULL != hcur) {
                if( 0 == strcmp(hcur->word,word) ) {
                    hcur->count++;
                    return;
                }
                hcur = hcur->next;
            }
        }
    } 
    return;
}

void 
ht_free(hash_table ht)
{
    int i;
    hash_node *htmp,*hcur;

    LOG_DEBUG("Number of collisions found: %ld\n",ncollisions);
    if( NULL != ht ) {
        for(i=0;i < nbuckets; i++) {
            if ( NULL != ht[i] ) {
                for (htmp = ht[i]->next, hcur = ht[i]; 
                     htmp != NULL; 
                     hcur = htmp, htmp = htmp->next) {
                    free(hcur->word);
                    free(hcur);
                }
            }
        }
    }
    free(ht);
}

/* allocate one one of the hash table */
static hash_node* set_ht_node(const char *word) 
{
    hash_node* htmp = NULL;
    
    htmp = malloc(sizeof(hash_node));
    if( NULL == htmp) {
        return NULL;
    }
    htmp->word = malloc( (strlen(word)+1) * sizeof(char));
    if (NULL == htmp->word) {
        return NULL;
    }
    strcpy(htmp->word,word);
    htmp->count = 0;
    return htmp;
}

/* djb2 hash function taken from http://www.cse.yorku.ca/~oz/hash.html */
static uint64_t 
djb2hash(char *str)
{
    uint64_t hash = 5381;
    int c;

    while ( (c = *str++) ) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return (uint64_t) hash;
}

/* 64bit mix hash function */
static uint64_t 
hash64shift(uint64_t hash, int tsize)
{
    hash = (~hash) + (hash << 21); // hash = (hash << 21) - hash - 1;
    hash = hash ^ (hash >> 24);
    hash = (hash + (hash << 3)) + (hash << 8); // hash * 265
    hash = hash ^ (hash >> 14);
    hash = (hash + (hash << 2)) + (hash << 4); // hash * 21
    hash = hash ^ (hash >> 28);
    hash = hash + (hash << 31);
    return hash % tsize;
}
