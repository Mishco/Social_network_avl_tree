#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/*strom AVL*/
typedef struct page {
	char *name; //nazov stranky
	struct page *subname; // mena uzivatelov ktori likeli stranku, teda dalsi AVL strom 
	struct page *left;
	struct page *right;
	int height;
}page;

struct page *web; //globalna premenna
char *fullName;
int NumberPeopleInSubname;

//AVL funkcie
int height(page *T){
	int lh,rh;
	if(T==NULL)
		return(0);
	if(T->left==NULL)
		lh=0;
	else
		lh=1+T->left->height;
	if(T->right==NULL)
		rh=0;
	else
		rh=1+T->right->height;
	if(lh>rh)
		return(lh);
	return(rh);
}

// jednotlive rotacie AVl stromu
page *rotateLeft(page *current) {
	page *tmp;

	tmp=current->right;
	current->right=tmp->left;
	tmp->left=current;
	current->height = height(current);
	tmp->height = height(tmp);
	
	return(tmp);
}

page *rotateRight(page *current) {
	page *tmp;

	tmp=current->left;
	current->left=tmp->right;
	tmp->right=current;
	current->height =height(current);
	tmp->height = height(tmp);

	return(tmp);
}
page* RR(page *T)
{
	T=rotateLeft(T);
	return(T);
}  
page* LL(page *T)
{
	T=rotateRight(T);
	return(T);
}
page* LR(page *T)
{
	T->left=rotateLeft(T->left);
	T=rotateRight(T);
	return(T);
}
page* RL(page *T)
{
	T->right=rotateRight(T->right);
	T=rotateLeft(T);
	return(T);
}

int BF (page *root) {
	int leftHeight, rightHeight;
	if(root==NULL)
		return(0);

	if(root->left==NULL)
		leftHeight = 0;
	else
		leftHeight = 1 + root->left->height;

	if(root->right==NULL)
		rightHeight = 0;
	else
		rightHeight = 1 + root->right->height;

	return(leftHeight - rightHeight);
}

/*vytvor novy uzol pre specificke meno*/
page *create_page(char *name) {
	//vloz hodnoty
	page *newPage = (page*)malloc(sizeof(page));
	memset (newPage, 0, sizeof(*newPage));

	newPage->name  = name;
	return newPage;
}

/* JEDNA FUNKCIA ktora riesi aj stranky aj mena ktore su v nej ulozene */
page *addNameToTree(page **root, char *name) {
	//vytvori novu stranku
	if (*root == NULL) 
	{
		*root = create_page(name);
	}
	
	//1 .stranku ktoru chcem vytvorit ukladam podla abecednoho 
	// poradia bud vlavo alebo vpravo
	//
	//2. ked skontrolujem ci stranka bola vytvorena vkladam do nej 
	//meno cloveka	
	
	if (strcmp(name, (*root)->name) < 0) 
	{
		(*root)->left = addNameToTree(&(*root)->left, name);
		if (BF(*root) == 2) //zistuje ci su potrebne rotacie
		{
			if (strcmp (name, (*root)->left->name) < 0)
				(*root) = LL(*root);
			else
				(*root) = LR(*root);
		}
	}
	else if (strcmp(name,(*root)->name) > 0) 
	{
		(*root)->right = addNameToTree(&(*root)->right, name);
		if (BF(*root) == -2) //zistuje ci su potrebne rotacie
		{
			if (strcmp (name, (*root)->right->name) > 0)
				(*root) = RR(*root);
			else
				(*root) = RL(*root);
		}
	}
	//zisti aktualnu vysku a prirad ju aktualnemu uzlu
	(*root)->height = height(*root);
	
	//stranka uz existuje nepotrebujem ju vytvarat
	return (*root);
}

page *addName(page *root, char *nameOfPage, char *nameOfPeople) {
	
	//najskor vytvor strom je samotnu stranku 
	page *lastNode = addNameToTree(&root,nameOfPage);
	
	//vyhladana novo vytvorenu stranku a pracuje dalej s nou
	while (strcmp (nameOfPage, lastNode->name) != 0) {
		  if (strcmp (nameOfPage, lastNode->name) > 0)
				lastNode = lastNode->right;
		  else if (strcmp (nameOfPage, lastNode->name) < 0)
				lastNode = lastNode->left;
	}

	//vytvori strom pre uzivatela ktory likelol danu stranku
	addNameToTree(&lastNode->subname, nameOfPeople);
	
	return root;
}

		//current riesi aktualne mena v danej stranek
page* deletePeople(page *current, char *name) {

	page *tmp;
	// nie je to strom 
	if (current == NULL ) {
		return NULL;
	}
	else if (strcmp(name, current->name) < 0) {
		current->left  = deletePeople(current->left, name);
		if (BF(current) == -2) 
			if (BF(current ->right ) <= 0)
					current = RR(current);
			else
					current = RL(current);
	}
	else if (strcmp(name, current->name) > 0) {
		//v pravom podstrome
		current->right = deletePeople(current->right, name);
		if (BF(current) == -2)
			if (BF(current->left ) >= 0)
					current = LL(current);
			else
					current = LR(current);
	}	
	else {
		// data na vymazanie boli najdene
		if (current->right != NULL) {
			tmp = current->right;
			while(tmp->left != NULL)
				tmp = tmp->left;

			current->name = tmp->name;
			current->right = deletePeople(current->right, tmp->name);
			if (BF(current)==2)
				if (BF(current->left) >= 0)
					current = LL(current);
				else
					current = LR(current);
		}
		else
			return (current->left);
	}
	current->height = height(current);
	return current;		
}

page* Find (page *current, char *nameOfpage) {

	//strom je prazdny
	if (current == NULL ){
		return NULL;
	}

	if ((strcmp(nameOfpage, current->name) > 0))
	{
		//hladaj v pravom podstrome
		return Find(current->right, nameOfpage);
	}
	else if (strcmp(nameOfpage, current->name) < 0) 
	{
		//hladaj v lavom podstrome
		return Find(current->left, nameOfpage);
	}
	else 
	{	//nasiel hladany
		return current;
	}
}


//prechadza stromov inorder a pripocitava index i, akonahle sa
// k == i vrati meno do globalnej premennej
void PrintInorder(page *node, int *i, int k)
{
	if(node==NULL) {
		return;
	}
	PrintInorder(node->left,i, k);
	/*Pocita kolko ludi uz presiel*/
	(*i)++;		
	if ( *(int*)i == k) 
		fullName = node->name; // funkcia pokracuje dalej aj ked by nemusela, pretoze uz nasiel daneho cloveka

	PrintInorder(node->right,i, k);
}

// vracia pocet ludi v danej stranke
void NumberOfPeople(struct page *root) {
	if(root==NULL) {
		return;
	}  
	NumberOfPeople(root->left);
	NumberPeopleInSubname++;	//uklada do globalnej premennej	
	NumberOfPeople(root->right);	 
}



void init() {
	web = NULL;
	NumberPeopleInSubname = 0;
}

void like(char *page, char *user) {
	char *helpUser, *helpPage;
	helpUser = _strdup(user);
	helpPage = _strdup(page);
	web = addName(web, helpPage , helpUser);
}

void unlike(char *page, char *user) {
	struct page *tmp=NULL;
	struct page *root=NULL;

	char *helpPage = _strdup(page);
	char *helpUser = _strdup(user);

	if (web == NULL)
		return;

	root = web;

	// najdeme danu stranku, ktorej chcem zobrat svoj like
	if ((tmp = Find(root , helpPage)) == NULL) 
		return;

	// pracuje uz len v vnutornom strome najdenej stranke
	tmp->subname = deletePeople(tmp->subname, helpUser);

}

//Zaujíma nás meno k-teho používate¾a v abecednom poradí, ktorý lajkuje stránku page. 
//Ak je týchto používate¾ov menej ako k, tak funkcia vráti NULL
char *getuser(char *page, int k) { 
	int i=0;
	struct page *current; 
	char *helpPage = _strdup(page);
	fullName = NULL;
	NumberPeopleInSubname = 0;

	// ci nieco vlozil
	if(web == NULL)
		return NULL;

	// hlada ci dana stranka existuje
	if ((current = Find(web, helpPage)) == NULL )
		return NULL;

	//tato funkcia vlozi do globalnej premennej pocet ludi v aktualnom podstrome
	// NumberPeopleInSubname 
	NumberOfPeople(current->subname);


	// zistujeme pocet ludi , ci nie je vacsi ako 
	if ( NumberPeopleInSubname  < k) 
		return NULL;

	i=0;
	// v danej stranke najdi k-teho uzivatela
	PrintInorder(current->subname,&i,k);

	return fullName;
}


/*TESTOVAC*/
char *generateString() {
	int length = 15; //pevna pocet znakov
	char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";        
	char *randomString = NULL;
	int n = 0;

	if (length) {
		randomString = (char* )malloc(sizeof(char) * (length +1));

		if (randomString) {            
			for (n = 0;n < length;n++) {            
				int key = rand() % (int)(sizeof(charset) -1);
				randomString[n] = charset[key];
			}

			randomString[length] = '\0';
		}
	}

	return randomString;
}

void StringToFile(int P, int L) {
	FILE *fr;
	char *stringPage;
	char *stringName;
	int i=0,j=0;
	int randomNumber = 0;

	fr = fopen ("test.txt", "w");
	printf("\ngenerating pages and names\n");
	for (i=0; i <= P; i++) {
		stringPage = generateString();
		for (j=0; j <= L ; j++) {
			randomNumber = rand()%L + 1;
			stringName = generateString();
			fprintf(fr, "%s %s %d\n" , stringPage, stringName, randomNumber); // vyplni stringami a cislo na konci  
		}
	}
	printf("generating completed\n");
	fclose(fr);
}

void TestFromFile() {
	FILE *fileP, *fileN, *file, *fileNUM;
	int c, d, e ;
	char *stringPage = (char*) malloc (100*sizeof(char));
	char *stringName = (char*) malloc (100*sizeof(char));
	int number= 0, numberLine=0;

	//file  = fopen ("test.txt", "w");
	fileP = fopen ("test_PAGE.txt", "r");
	fileN = fopen ("test_NAME.txt", "r");
	fileNUM = fopen ("test_NUMBER.txt","r");
	printf("\ntesting ");

	while ((c = getc(fileP) != EOF)) {
		numberLine++;
	}
	printf ("%d\n", numberLine);

	printf("like test: ");
	while ((c = getc(fileP) != EOF) || (d = getc(fileN) != EOF)) 
	{
		//fscanf(file, "%s%s%d", *stringPage, *stringName, &number);
		fgets(stringPage, 50, fileP);
		fgets(stringName, 50, fileN);
	 	
		like(stringPage, stringName);
	}
	printf("OK\n");
	
	printf("unlike test: ");
	rewind(fileP); // vrati sa na zaciatok suboru
	rewind(fileN);
	while (((c = getc(fileP)) != EOF) || (d = getc(fileN) != EOF)) 	
	{
		fgets(stringPage, 20, fileP);
		fgets(stringName, 20, fileN);	

		unlike(stringPage, stringName);
	}
	printf("OK\n");


	while (((c = getc(fileP)) != EOF) || (e = getc(fileNUM) != EOF)) 	
	{
		fgets(stringPage, 20, fileP);
		fscanf(fileNUM, "%d\n",&number);	
		getuser(stringPage, number);
	}
}

/*jedna funkcia na vypis*/
void print_names(page *names, int level) {
	int indent = 10;
	if (names == NULL) {
		printf("\n");
	}
	if (names->left) 
		print_names(names->left, level);
	if (names->subname) {
		printf("%*c %s \n", (indent * level), ' ', names->name);
		print_names(names->subname, level + 1);
		printf("\n");
	}
	else
		printf("%*c %-*s \n", (indent * level), ' ', indent, names->name);
	if (names->right) {
		print_names(names->right,level);	
	}
}

int main() {
	char *str[]={"/","/hello.html","/"};		//3
	char *mena[]={"Jozko","Peto","Juro","Mato","Adam","Rado","Miso","Aron"};
	char *user;
	int i=0;
	int n,l,u,g;

	clock_t t; 
	
	t = clock();

	init(); // vzdy vynuluje

	//printf("Zadaj pocet stranok n, pocet like l, pocet unlike u a pocet getuser g\n");
	//scanf ("%d %d %d %d", &n, &l, &u, &g);

	

	printf("calculating....\n");

 	//StringToFile(n,l);

	TestFromFile();


	for(i=0;i<8;i++){
		like(str[0], mena[i]);
	}
	print_names(web, 0);

	printf("%s\n", getuser(str[0],5));
	printf("%s\n", getuser(str[0],4));
	printf("%s\n", getuser(str[0],3));
	printf("%s\n", getuser(str[0],2));
	printf("%s\n", getuser(str[0],1));
	printf("%s\n", getuser(str[0],0));
	printf("%s\n", getuser(str[0],15));
	
	unlike(str[0], mena[0]);
	unlike(str[0], mena[1]);
	unlike(str[0], mena[2]);
	unlike(str[0], mena[3]);
	unlike(str[0], mena[4]);

	unlike(str[0], mena[5]);
	unlike(str[0], mena[6]);
	unlike(str[0], mena[7]);

	print_names(web, 0);

	like(str[0],mena[1]);
	
	like(str[1],mena[1]);
	
	unlike(str[1],mena[1]);



	t = clock() - t;

	printf("\nIt took me %d clicks (%f seconds).\n", t,((float)t)/CLOCKS_PER_SEC);

	return 0;
}


