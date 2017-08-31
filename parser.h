#include "common.h"
#include "hash_table.h"

#ifndef PARSER_H
#define PARSER_H

uint64_t nwords;

/* functions for parsing the dictionary and counting the words
 * in the input files */
int parse_dictionary(const char*, hash_table);
int parse_input_and_count(int, char**,hash_table);

#endif
