/*
 * CMPT332 Group 20:
 * Liam Bindle       LRB175  11161519
 * Shahriar Rahman   SHR062  11153915
 */
 
#include "list.h"

/*
 * Below is the global error number variable which indicates what the
 * most recent error was. This is similar to the way errno.h handles
 * error codes.
 */
int list_errno = 0;

/*
 * Below a list of error messages is given which correspond to the
 * enumeration above.
 */
const char* LIST_ERROR_MSGS[6] = {
    "No errors.",
    "No memory left.",
    "A nullptr was given.",
    "Tried to go beyond the last item in a list.",
    "Tried to go before the first item in a list.",
    "A comparator function returned a value that was not 0 or 1."
};
