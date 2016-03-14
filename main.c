#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }

    /* build the entry */
# if defined(OPT)
    entry **pHead, **e;
    pHead = (entry **) malloc(sizeof(entry*) * TABLESIZE);
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    for(i = 0; i < TABLESIZE; i++) {
        e[i] = (entry*) malloc(sizeof(entry));
        e[i]->pNext = NULL;
    }
    i = 0;
#else
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
#endif

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    e = pHead;

    /* the givn last name to find */
    char input[8][MAX_LAST_NAME_SIZE];
    strcpy(input[0], "zyxel");
    strcpy(input[1], "uninvolved");
    strcpy(input[2], "whiteshank");
    strcpy(input[3], "odontomous");
    strcpy(input[4], "pungoteague");
    strcpy(input[5], "reweighted");
    strcpy(input[6], "xiphisternal");
    strcpy(input[7], "yakattalo");

    e = pHead;

    assert(findName(input[0], e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input[0], e)->lastName, "zyxel"));

#if defined(__GNUC__)
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    for(i = 0; i < 8; i++) {
        findName(input[i], e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output;
#if defined(OPT)
    output = fopen("opt.txt", "a");
#else
    output = fopen("orig.txt", "a");
#endif
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
    printf("total time elapsed : %lf sec\n", cpu_time1 + cpu_time2);

#if defined(OPT)
    for(i = 0; i < TABLESIZE; i++) {
        if (pHead[i]->pNext) free(pHead[i]->pNext);
    }
#else
    if (pHead->pNext) free(pHead->pNext);
#endif
    free(pHead);

    return 0;
}
