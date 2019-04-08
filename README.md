# Social network implemented by AVL tree

DSA project for school

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
typedef struct node {
    int key;
    struct node *left;
    struct node *right;
    int height;
}node;
```


