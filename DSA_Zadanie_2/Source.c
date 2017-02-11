#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/*AVL tree node*/
typedef struct node {
    int key;
    struct node *left;
    struct node *right;
    int height;
}node;

int maxValue(int a, int b) {
    return (a > b) ? a : b;
}

node* minValueNode (node *root) {
    /***UKAZUJE NA ZACIATOK*/
    node*  act = root;

    /**A PRECHADZA AZ DO LAVEHO LISTU KDE JE NAJMENSIA HODNOTA*/
    while (act->left != NULL) {
        act = act->left;
    }
    return act;
}

int height (node *N) {
    if (N == NULL)
        return 0;
    else
        return N->height;
}

node* newNode(int key) {
    struct node *tmp =(node*)malloc(sizeof(node));
        tmp->key = key;
        tmp->left=NULL;
        tmp->right=NULL;
        tmp->height=1;
        return tmp;
}
/**ROTUJ PODSTROM do PRAVA */
 node* rightRotate(node *y) {
    node *x = y->left;
    node *tmp = x->right;

    //SAMOTNA ROTACIA
    x->right = y;
    y->left =tmp;

    /**UPRAVI VYSKA*/
    y->height = maxValue(height(y->left), height(y->right))+1;
    x->height = maxValue(height(x->left), height(x->right))+1;

    return x;
 }

node* leftRotate(node *x) {
    node *y = x->right;
    node *tmp =y->left;

    //SAMOTNA ROTACIA
    y->left = x;
    x->right= tmp;

    /**UPRAVI VYSKA*/
    x->height = maxValue(height(x->left), height(x->right)) + 1;
    y->height = maxValue(height(y->left), height(y->right)) + 1;

    return y;
}

int getBalance(node *N) {
    if (N == NULL)
        return 0;
    else
     return (height(N->left) - height(N->right));
}

node *insert(node *node, int key) {
    int balance=0;

    /**VYTVORI PRVY*/
    if(node == NULL) {
        return (newNode(key));
    }

    if (key < node->key)
        node->left = insert(node->left, key);
    else
        node->right= insert(node->right, key);

    /**UPRAVI VYSKU PREDCHODCU*/
    node->height = maxValue(height(node->left), height(node->right)) + 1;

    /**ZISTI AKO VELMI TREBA UPRAVOVAT PREDCHODCU*/
    balance = getBalance(node);

    /**LEFT LEFT ROTATION*/
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    /**RIGHT RIGHT ROTATION*/
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    /**LEFT RIGHT ROTATION*/
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    /**RIGHT LEFT ROTATION*/
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    /**VRACIA BEZ ZMENY, PRETOZE NIE NEBOLA POTREBNA*/
    return node;
}

node *deleteElement(node* root, int key) {
        int balance;
        node* tmp;
		
    if (root == NULL)
        return root;

   /**AK PRVOK, KTORY CHCEME VYMAZAT JE MENSI AKO HODNOTA KORENA*/
   /**V LAVOM PODSTROME SU VZDY MENSIE HODNOTY*/
    if (key < root->key)
        root->left = deleteElement(root->left, key);
    else
    /**AK PRVOK JE VACSI*/
        if (key > root->key) {
            root->right = deleteElement(root->right, key);
        }
    /**Ak hodnota prvku je rovnaka*/
    else {
            /*NEMA POTOMKOV*/
            if ((root->left == NULL) || (root->right == NULL)) {
                tmp = root->left ? root->left : root->right;

                if (tmp == NULL) {
                    tmp = root;
                    root = NULL;
                }
                else
                    *root = *tmp;
                free(tmp);
            }
            else { /*MA POTOMKOV*/
                    /**V tomto podstrome najde najlavejsi*/
                tmp = minValueNode(root->right);

                /*SKOPIRUJE */
                root->key = tmp->key;

                root->right = deleteElement(root->right, tmp->key);
            }
    }

    if (root == NULL)
        return root;

    root->height = maxValue(height(root->left), height(root->right)) + 1;

    balance = getBalance(root);

    /**KEDZE SME ODOBRALI PRVOK MUSIME ZVYSOK STROMU VYVAZIT*/
    // LEFT LEFT ROTATION
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // LEFT RIGHT ROTATION
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // RIGHT RIGHT ROTATION
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // RIGHT LEFT ROTATION
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

/**VYPIS*/
void preOrder (node *root) {

    if (root != NULL) {
        printf("%d ", root->key);
        preOrder(root->left);
        preOrder(root->right);
    }
}


void test_1() {
    node *root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    preOrder(root);
	printf("\n");
	root = deleteElement(root, 30);
    preOrder(root);
}


int main () {

    test_1();
    return 0;
}
