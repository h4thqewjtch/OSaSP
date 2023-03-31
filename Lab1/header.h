#include <stdbool.h>

struct comm
{
    bool dir;
    bool file;
    bool link;
    bool sort;
};

void dirWalk(char *, struct comm);
int compare(const void *, const void *);
