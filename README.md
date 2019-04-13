# Social network implemented by AVL tree

School project (Data structures and algorithms) using AVL tree structure.

## Technology stack

- C language
- AVL inside in AVL

> using [AVL tree](https://cs.wikipedia.org/wiki/AVL-strom)

## Description

- We save people and his/her like on pages
- Like, Dislike, Init, Search in time
- Tree balancing after insert

## Code example

```c
/*AVL tree node*/
typedef struct page {
    char *name; //nazov stranky
    struct page *subname; // mena uzivatelov ktori likeli stranku, teda dalsi AVL strom 
    struct page *left;
    struct page *right;
    int height;
}page;
```

## Licence

MIT