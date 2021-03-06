#include <stdio.h>
#include <stdlib.h>

struct item {
    int key;
    int data;
    struct item *next;
};

struct item *head = NULL;

int main()
{
    extern void insert(int key, int data), delete(int key), printall();
    extern int search(int key);

    insert(38, 3);
    insert(20, 2);
    insert(5, 0);
    insert(22, 6);
    insert(46, 18);

    printf("With all five items:\n");
    printall();

    printf("After delete(22):\n");
    delete(22);
    printall();
    printf("After delete(5):\n");
    delete(5);
    printall();
    printf("delete(30) shouldn't do anything:\n");
    delete(30);
    printall();

    return(0);
}


void insert(int key, int data)
{
    struct item **pp;
    pp = &head;
    struct item *new;

    /* create the new item */
    if ((new = malloc(sizeof(struct item))) == NULL) {
        fprintf(stderr, "out of memory!\n");  /* unlikely */
        exit(1);
    }
    new->key = key;
    new->data = data;

    /* find the node it goes after; NULL if it goes at the front */
    if (head != NULL && head->key < key) {
        for (pp = &head; (*pp)->next && (*pp)->next->key < key; pp = &(*pp)->next);
        pp = &(*pp)->next;
    }
    new->next = *pp;
    *pp = new;
}


void delete(int key)
{
    extern int search(int key);
    struct item** pp;
    struct item* next;
    if (search(key) != -1) {
        for (pp = &head; *pp && (*pp)->key != key; pp = &(*pp)->next);

        next = *pp;
        *pp = (*pp)->next;
        free(next);
    }
}


int search(int key) /* returns -1 if not found */
{
    struct item *p;

    for (p = head; p && p->key < key; p = p->next)
        ;
    if (p && p->key == key)
        return(p->data);
    else
        return(-1);
}


void printall()
{
    struct item *p;
    for (p = head; p; p = p->next)
        printf("%d: %d\n", p->key, p->data);
    printf("[end]\n");
}
