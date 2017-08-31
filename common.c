#include "common.h"

/* print error messages */
void 
print_error(int ret) 
{
    switch(ret) {
        case OUT_OF_MEMORY:
        printf("The application consumed all the available memory!\n");
        break;
        case IO_ERROR:
        printf("An error occured reading or writing a file!\n");
        break;
        case FAIL:
        printf("A fatal error occured in the application!\n");
        break;
        default:
        printf("Error code %d not recognized\n",ret);
        break;
    }
    /* an error occured thus the application will abort immediately */
    printf("The application is going to abort...\n");
    abort();
}
