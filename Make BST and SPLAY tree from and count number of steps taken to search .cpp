/*
Read a large text file and store the unique words in a BST and a splay tree. Use a parent pointer in splay. Display the height of the two tree.

Read another text file to search for the words in first file. Count the number of comparisons used in both the trees.
*/

#include <iostream>
#include <cstring>

using namespace std;

//Binary Search Tree Structure
struct tree
{
	char word[30];
	struct tree *left, *right;
};
typedef struct tree *TREE;
TREE root=NULL;
int flag=1;

//Splay Tree Structure
struct SplayTree {
	char word[30];
	struct SplayTree *left;
	struct SplayTree *right;
	struct SplayTree *parent;
};
typedef struct SplayTree *SPLAYTREE;
SPLAYTREE splayroot=NULL;

//Binary Search Tree cell creation
struct tree* cell_creation(char *val)
{
	struct tree *p=(TREE)malloc(sizeof(struct tree));
	strcpy(p->word, val);
	p->left=NULL;
	p->right=NULL;
	return p;
}

//Splay Tree cell creation
struct SplayTree* splay_cell_creation(char *val)
{
	struct SplayTree *p=(SPLAYTREE)malloc(sizeof(struct SplayTree));
	strcpy(p->word, val);
	p->left=NULL;
	p->right=NULL;
	p->parent=NULL;
	return p;
}

struct tree* insertbst(tree *node, char *word)
{
	if(node==NULL)
	{
		if (flag == 1)
		{
			root = cell_creation(word);
			flag--;
		}
		else
		{
			return cell_creation(word);
		}	
	}
	else
	{
		if(strcmp(word, node->word)<0)
		{
			node->left  = insertbst(node->left, word);
		}	
		else if(strcmp(word, node->word)>0)
		{
	    		node->right = insertbst(node->right, word);
		}
	}
return node;
}

int bst_comparsion = 0; 
struct tree* searchbst(tree *node, char *word)
{
	if (node == NULL)
	{bst_comparsion = 0;return 0; }
	
	if(strcmp(word, node->word)==0)
	{
		bst_comparsion++;
		cout << word << " found and comparison made " << bst_comparsion << endl;
		bst_comparsion = 0;
		return 0;
	}
	else if(strcmp(word, node->word)<0)
	{	
		bst_comparsion++;
		return searchbst(node->left, word);
	}
	else if(strcmp(word, node->word)>0)
	{
		bst_comparsion++;
		return searchbst(node->right, word);
	}
}

void Zig(SplayTree *&t, SplayTree *&root)
{
	SplayTree *copy = t->right;
	t->right = t->parent;
	SplayTree *gp = t->parent->parent;

	if(gp) 
	{
		if (gp->right == t->parent)
		{
		    gp->right = t;
		}
		else if (gp->left == t->parent)
		{
		    gp->left = t;
		}
	}
	t->parent = gp;
	t->right->parent = t;
	t->right->left = copy;
	if(copy) 
	{
		t->right->left->parent = t->right;
	}
	if(!t->parent)
	{
		root = t;
	}
}

void Zag(SplayTree *&t, SplayTree *&root)
{
	SplayTree *copy = t->left;
	t->left = t->parent;
	SplayTree *gp = t->parent->parent;
	if(gp) 
	{
		if (gp->right == t->parent)
		{
		    gp->right = t;
		}
		else if (gp->left == t->parent) 
		{
		    gp->left = t;
		}
	}
	t->parent = gp;
	t->left->parent = t;
	t->left->right = copy;
	if(copy) 
	{
		t->left->right->parent = t->left;
	}
	if(!t->parent)
	{
		root = t;
	}
}

void ZigZig(SplayTree *&t, SplayTree *&root)
{
    Zig(t->parent, root);
    Zig(t, root);
}

void ZagZag(SplayTree *&t, SplayTree *&root)
{
    Zag(t->parent, root);
    Zag(t, root);
}

void ZigZag(SplayTree *&t, SplayTree *&root)
{
    Zag(t, root);
    Zig(t, root);
}
void ZagZig(SplayTree *&t, SplayTree *&root)
{
    Zig(t, root);
    Zag(t, root);
}

void Splay(SplayTree *&t, SplayTree *&root)
{
	if(!t->parent) 
	return;
	if(t->parent == root)
	{
		if(t == t->parent->left)
		{
		    Zig(t, root);
		} 
		else if(t == t->parent->right)
		{
		    Zag(t, root);
		}
	return;
	}
	else if(t->parent->left == t)
	{
		if(t->parent->parent->left == t->parent)
		{
		    ZigZig(t, root);
		}
		else if(t->parent->parent->right == t->parent)
		{
		    ZagZig(t, root);
		}
	}
	else if(t->parent->right == t)
	{
		if(t->parent->parent->left == t->parent)
		{
		    ZigZag(t, root);
		}
		else if(t->parent->parent->right == t->parent)
		{
		    ZagZag(t, root);
		}
	}
	Splay(t, root);
}

//Splay Insert
void insert(SplayTree *&root, char *value) 
{

	SplayTree *toAdd = splay_cell_creation(value);
	SplayTree *t = root;

	while (t) 
	{
		if (strcmp(value, t->word)<0) 
		{
			if(!t->left)
			{
				toAdd->parent = t;
				t->left = toAdd;
				//cout << value << " added at left";
				break;
			}
		t = t->left;
		}
		else if(strcmp(value, t->word)>0)
		{
			if(!t->right)
			{
				toAdd->parent = t;
				t->right = toAdd;
				//cout << value << " added at right";
				break;
			}
		t = t->right;
		}
		else
		{
			return;
		}
	}
	//cout << " and SPLAYED" << endl;
	Splay(toAdd, root);
}

int splay_count=0;
SplayTree* search(char *word, SplayTree *&root)
{
	SplayTree *t = root;
	SplayTree *result = NULL;

	while(t)
	{
		if(strcmp(word, t->word)==0)
		{
			splay_count++;
			cout << word << " found and comparison made " << splay_count << endl;
			splay_count = 0;
			result = t;
			Splay(t, root);
			return 0;
		}
		else if(strcmp(word, t->word)>0)
		{
			SplayTree *temp1 = t;
			//cout << "word is " << word << " and Right: " << t->word << endl;
			
			t = t->right;
			splay_count++;
			if(t==NULL)
			{
				splay_count = 0;
				Splay(temp1, root);
				//cout << "WORD not FOUND and Last node splayed" << endl;
				//cout << splayroot->word << endl;
				return 0;
			}
		}
		else if(strcmp(word, t->word)<0)
		{
			SplayTree *temp2 = t;
			//cout << "word is " << word << " and left: " << t->word << endl;
			
			t = t->left;
			splay_count++;
			if(t==NULL)
			{
				splay_count = 0;
				Splay(temp2, root);
				//cout << "WORD not FOUND and Last node splayed" << endl;
				//cout << splayroot->word << endl;
				return 0;
			}
		}		
	}
}

int main()
{
FILE *file1;
file1 = fopen("file.txt","r");
char word[30];
while(fscanf(file1,"%s",word)==1)
{
//	cout << word << endl;
	insertbst(root, word);		
}
fclose(file1);

file1 = fopen("file.txt","r");
char splayword[30];
int splayflag=1;
while(fscanf(file1,"%s",splayword)==1)
{
	if(splayflag==1)
	{
		splayroot = splay_cell_creation(splayword);
		splayflag--;
	}
	else
	{
		insert(splayroot, splayword);		
	}
}
fclose(file1);

cout << endl;
cout << "Comparison for Binary Search Tree if word is found: " << endl;

file1 = fopen("search_file.txt","r");
char word1[30];
while(fscanf(file1,"%s",word1)==1)
{
	searchbst(root, word1);		
}
fclose(file1);

cout << endl;
cout << "Comparison for Splay Tree if word is found: " << endl;

file1 = fopen("search_file.txt","r");
char word2[30];
while(fscanf(file1,"%s",word2)==1)
{
	search(word2, splayroot);		
}
fclose(file1);

return 0;
}
