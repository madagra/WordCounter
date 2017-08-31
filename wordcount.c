#include "common.h"
#include "parser.h"
#include "hash_table.h"

void print_result(hash_table);

int 
main(int argc, char **argv) 
{  

    double t1,t2;
    hash_table ht;

    t1 = get_time();
    CHECK_ERROR(ht_init(&ht));

    CHECK_ERROR(parse_dictionary("./dictfile",ht));

    CHECK_ERROR(parse_input_and_count(argc,argv,ht));
    t2 = get_time();

    print_result(ht);
    printf("Total time (parsing + counting): %f s\n",t2-t1);
    ht_free(ht);

    return SUCCESS;
}

void 
print_result(hash_table ht) 
{
    hash_node *htmp = NULL;
    uint64_t nwords_dict = 0;
    HT_ITER(ht,htmp) {
        while(NULL != htmp) {
            if(0 < htmp->count) {
                printf("%ld \t %s\n",htmp->count,htmp->word);
                nwords_dict += htmp->count;
            }
            htmp = htmp->next;
        }
    }
    printf("%ld \t total dictionary words\n",nwords_dict);
    printf("%ld \t total words in the text\n",nwords);
}
