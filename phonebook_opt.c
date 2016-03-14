#include <stdlib.h>
#include <string.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastname[], entry **pHead)
{
    unsigned int val = hash(lastname);
    entry *tmp = pHead[val];
    while(tmp != NULL) {
        if(strcasecmp(lastname, tmp->lastName) == 0)
            return tmp;
        tmp = tmp->pNext;
    }

    return NULL;
}

entry **append(char lastName[], entry **e)
{
    /*
     * append at beginning of the block
     * */
    unsigned int val = hash(lastName);
    entry *remainder = e[val]->pNext;
    entry *tmp = e[val];

    tmp->pNext = (entry *) malloc(sizeof(entry));
    tmp = tmp->pNext;
    strcpy(tmp->lastName, lastName);
    tmp->pNext = remainder;

    return e;
}

unsigned int hash(char key[])
{
    unsigned int hashValue = 0;
    while(*key != '\0') {
        hashValue = (hashValue << 5) + hashValue + *key++;
    }
    return hashValue % TABLESIZE;
}

