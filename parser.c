#include "parser.h"

/* local variables */

/* total dictionary size */
static uint64_t dict_size = 0;

/* common delimiters which are used for full word matching in the 
 * input file. The same delimiters are also used for reading the 
 * dictionary words */
char *delimiters = ".,;:-!?()[]{}*#$\"\'\n\t\r\f ";

/* local function prototypes */
static int count_dict_words(FILE*, hash_table);

int 
parse_dictionary(const char* filename, hash_table ht) 
{
    
    FILE    *filep;
    char    *dict_line = NULL, *token;
    size_t  length = 0;
    ssize_t nchar;

    filep = fopen(filename,"r");
    if ( NULL == filep ) {
        printf("ERROR: cannot open dictionary file %s\n",filename);
        return IO_ERROR;
    }

    /* Read the dictionary file and add the words to the hash table:
     * - if a line in the dictionary begins with # or it is empty then it is skipped
     * - if a line contains more than one space-separated word then only the first word is selected
     */
    while( -1 != ( nchar = getline(&dict_line,&length,filep) ) ) {
        /* remove the CR character kept by the getline function */
        memset(&dict_line[strlen(dict_line)-1],'\0',1);
        if( ('#' != dict_line[0]) && (strlen(dict_line) != strspn(dict_line," ")) ) {
            token = strtok(dict_line,delimiters);
            if ( (NULL != token) && (0 < strlen(token)) ) {
                dict_size++;
                ht_add(ht,token);
            }
        }
    }
    free(dict_line);
    fclose(filep);
    
    if( 0 == dict_size ) {
        printf("ERROR: empty dictionary file %s\n",filename);
        return IO_ERROR;
    }
    LOG_DEBUG("The dictionary contains %ld words\n",dict_size);
    return SUCCESS;
}


int 
parse_input_and_count(int argc, char **argv, hash_table ht) 
{
    nwords =0;
    /* take concatenation of all files given as command line parameters */
    if (argc >= 2) {
        int i;
        FILE *filep;
        for(i = 1; i < argc; i++) {
            filep = fopen(argv[i],"r");
            if( NULL == filep) {
                printf("ERROR: cannot open file %s\n",argv[i]);
                return IO_ERROR;
            }
            count_dict_words(filep,ht);
            fclose(filep);
        }
    } 
    /* otherwise use standard input */
    else {
        printf("Insert a text (end with newline followed by CTRL+D):\n\n");
        count_dict_words(stdin,ht);
    }

    return SUCCESS;
}


static int
count_dict_words(FILE *filep, hash_table ht) 
{
    char *line = NULL;
    size_t length = 0;
    ssize_t nchar;
    char *token;

    while ( -1 != (nchar = getline(&line, &length, filep)) ) {
        token = strtok(line,delimiters);
        while(NULL != token) {
            if(0 < strlen(token)) {
                nwords++;
                ht_count(ht,token);
            }
            token = strtok(NULL,delimiters);
        }
    }
    free(line);
    return SUCCESS;
}
