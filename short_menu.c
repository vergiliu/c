#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct block{
 char text[32]; 
 struct block *u, *d, *l, *r;
} block;

void setEmpty(block *b, char *text) {
 b->u = NULL; b->d = NULL;
 b->l = NULL; b->r = NULL;
 strcpy(b->text, text);
}

void printMenu(block *m) {
 if (NULL == m) return; 
 printf("%s ", m->text);
 if (m->r == NULL) printf("\n");
 printMenu(m->r);
 printMenu(m->d);
 /*also possible to use circular linked lists, but we need extra condition not to loop forever*/
}

block *addElem(block *c, char *text) {
 block *newEl = malloc(sizeof(block));
 setEmpty(newEl, text);
 c->r = newEl;
 newEl->l = c;
 return newEl;
}

block *addSubMenu(block *c, char *text) {
 block *newEl = malloc(sizeof(block));
 setEmpty(newEl, text);
 newEl->u = c;
 c->d = newEl;
 return newEl;
}

int main(int argc, char *argv[]) {
 block *root, *curent, *t;

 root = malloc(sizeof (block));
 setEmpty(root, "main");
 curent = addElem(root, "a");
 curent = addElem(curent,  "b");
 curent = addElem(curent, "c");

 curent = addSubMenu(curent, "ba");
 curent = addElem(curent, "bb");
 curent = addSubMenu(curent, "bba");

 printMenu(root);
}

