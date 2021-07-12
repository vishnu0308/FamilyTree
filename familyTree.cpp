/*
    Author:Karyampudi Venkata Vishnu Shravan
    Roll No: 1903124
    This program enables the user to create a family tree of numbers and prints the relationship
    between 2 numbers.
*/

#include <stdio.h>
class dlist;
class Node
{
    int ele;                //number to be stored
    int gen = 0;            //depth of the node, which helps to determine the generation of that no.
    Node *parent = NULL;    //pointer to it's parent
    dlist *children = NULL; //pointer to a doubly linked list of children of type Node
    Node *prev = NULL;      //Pointer to the node previous to it in the list
    Node *next = NULL;      //Pointer to the node next to it in the list
    //multiple constructors
    Node();
    Node(int k);
    Node(int k, int g);
    friend class dlist;
    friend class FamilyTree;
};

class dlist
{
    Node *head = NULL; //pointer to head node
    Node *tail = NULL; //pointer to tail node
    void add(Node *k); //function to add back a node to list
    void remove();     //function to remove front a node in the list
    void clear();      //clears the list. To use in destructor
    friend class Node;
    friend class FamilyTree;
};

Node::Node(){};
Node::Node(int k)
{
    ele = k;
}
Node ::Node(int k, int g)
{
    ele = k;
    gen = g;
}
/*
    void add(Node* k)
    Input: pointer to a node to add at the back.
*/
void dlist::add(Node *k)
{
    if (head == NULL)
    {
        head = k;
        tail = head;
    }
    else
    {
        tail->next = k;
        tail->next->prev = tail;
        tail = tail->next;
    }
}

void dlist::remove()
{
    Node *temp = head;
    head = head->next;
    delete temp;
}

void dlist::clear()
{
    while (head != NULL)
        remove();
}

//Tree ADT
class FamilyTree
{
    Node *root;
    Node *dumsearch(Node *start, int k);
    int commonancestor(Node *a, Node *b);
    bool childNochild(Node *jj);
    void tclear(Node *k);

public:
    FamilyTree(int k);
    Node *search(int k);
    void insertChild(int j, int i);
    void relationship(int i, int j);
    ~FamilyTree();
};
//constructor which creates the root node at the time of initialisation of tree
FamilyTree::FamilyTree(int k)
{
    root = new Node(k);
}
//takes an integer as input and returns a pointer to it's node in tree if present. If not there, NULL is returned.
Node *FamilyTree::search(int k)
{
    return dumsearch(root, k);
}
/*
   Node* dumsearch(Node* start,int k)
   Inputs: integer which is to be searched, pointer to a node(at first root is given in search() func.)
   Output: returns pointer to the found node or returns NULL
*/

Node *FamilyTree::dumsearch(Node *start, int k)
{
    Node *adr;
    if (start->ele == k)
        return start; //base step (if the current node itself has 'k' in it, then start is returned)
    if (start->children == NULL)
        return NULL;               //base step(if leaf node is reached NULL is returned)
    start = start->children->head; //start is made to point to the head of it's children list
    /*
        In the below while loop, for each child node in the children list, function is called.i.e; what the function
        does is 1.Current node is checked.
                2.All the subtrees of its children are checked recursively by giving as input to func.
                3.This process is continued until Node is found or whole tree is traversed.
    */
    while (start != NULL)
    {
        adr = dumsearch(start, k); //recursive step
        if (adr != NULL)
            return adr;      /*if found the pointer is returned at above line in the prev. step 
        where it is called and then comes to this line. As pointer is not NULL, it is returned and this process
        is continued until the pointer is returned from the function to search function.
        If not found, goes to the next line. */
        start = start->next; //iterating all children
    }
    return NULL;
}
/*
    void insertChild(int i,int j)
    Inputs: i: The parent to which child should be added
            j: Child to be added
    Output: None
*/
void FamilyTree::insertChild(int i, int j)
{
    Node *par = search(i); //searches for i in tree and returns a pointer to that address to 'par'.
    if (par == NULL)
        printf("No such member\n"); //if not found
    else
    {                                         //if found we need to add a j node
        Node *ch = new Node(j, par->gen + 1); //new node with value j and generation value 1 more than it's parent is created
        ch->parent = par;                     //Making the parent pointer in it to point to parent node
        if (par->children == NULL)
            par->children = new dlist(); //if no list before, it's created
        par->children->add(ch);          //using the add function of dlist adt.
    }
}
/*
    int commonancestor(Node* a,Node* b)
    inputs: pointers to two nodes of same generation
    output: returns a value which is helpful in relationship function, to print
            k cousin, aunt/uncle,etc;
            The value returned is the generation difference between one node and common ancestor of both-1.
*/
int FamilyTree::commonancestor(Node *a, Node *b)
{
    int gener = a->gen; //storing generation
    do
    {
        a = a->parent; //moving to parent of a
        b = b->parent; //moving to parent of b
    } while (a != b);  //until we find common ancestor
    return gener - a->gen - 1;
}

/*
    void relationship(int j,int i)
    inputs: integers j,i
    output:None
    Description: prints the relationship between j and i. Precisely, prints how j is related to i
*/

void FamilyTree::relationship(int j, int i)
{
    /*the terms in the following arrays are different from  from terms like kth great grand parent, kth aunt,etc;
      So,I used these arrays for those terms and used a seperate condition. For others I have directly printed using logic.
    */
    char lib1[5][25] = {"Parent", "Grand Parent", "Great Grand Parent", "Great Great Grand Parent", "3rd Great Grand Parent"};
    char lib2[6][30] = {"Sibling", "Aunt/Uncle", "Grand Aunt/Uncle", "Great Grand Aunt/Uncle", "Great Great Grand Aunt/Uncle", "3rd Great Grand Aunt/Uncle"};
    char lib3[3][4] = {"1st", "2nd", "3rd"};
    char lib4[3][7] = {"once", "twice", "thrice"};
    char lib6[5][32] = {"Niece/Nephew", "Grand Niece/Nephew", "Great Grand Niece/Nephew", "Great Great Grand Niece/Nephew", "3rd Great Grand Niece/Nephew"};
    char lib5[5][25] = {"Child", "Grand Child", "Great Grand Child", "Great Great Grand Child", "3rd Great Grand Child"};
    int gen1, gen2, pgd, cou = 0, cou2 = 0, kcous, pos_pgd;
    Node *a;
    Node *b;
    Node *same_gen_person;
    Node *c;
    Node *d;
    Node *r;
    //Finding those nodes in the tree
    if (i == j)
    {
        printf("Both are same\n");
        return;
    }
    Node *mem1 = search(i);
    Node *mem2 = search(j);
    if (mem1 == NULL || mem2 == NULL)
    {
        printf("%d is unrelated to %d\n", j, i);
        return;
    } //if any one is not in tree
    //storing their generation no's
    gen1 = mem1->gen;
    gen2 = mem2->gen;
    //creating required pointers to those for further use
    a = mem1;
    b = mem2;
    c = mem1, d = mem2;
    pgd = gen1 - gen2; //generation difference between those. Can be +ve / -ve
    if (pgd > 0)
    { //if +ve, we're sure that a is below b in tree, i,e; b is bigger than a
        do
        { //parents check
            a = a->parent;
            cou++;                        //this count gives the idea of which parent of 'a' is 'b'
        } while (a != b && !(a == NULL)); //traversing 'a' to the root and checking if 'b' is ancestor of 'a'
        if (cou <= gen1)
        { //i.e; 'b' is some parent of 'a'
            if (cou <= 5)
                printf("%d is %s of %d\n", j, lib1[cou - 1], i); //special terms: terms in lib1
            else
                printf("%d is %dth Great Grand Parent of %d\n", j, cou - 2, i); //printing kth great grand parents
                                                                                //by cou-2 from above while loop
        }
    }
    else
    { //b is not a parent or any kth grand parent of 'a'
        //similar to above reversing a and b and printing for child conditions
        do
        { //child check
            d = d->parent;
            cou2++;
        } while (d != c && !(d == NULL));
        if (cou2 <= gen2)
        {
            if (cou2 <= 5)
                printf("%d is %s of %d\n", j, lib5[cou2 - 1], i);
            else
                printf("%d is %dth Great Grand Child of %d\n", j, cou2 - 2, i);
        }
    } //not parents case or children case
    if (cou > gen1 || cou2 > gen2)
    {
        /*
          3 possible cases: a can be below b in tree
                            b can be below a in tree
                            both may belong to same generation
                In the 1st case, we'll find same_gen_person to b, who is some kth parent of a. Then we'll find the common
            ancestor of these two by putting these into the common_ancestor function which returns the generation 
            difference between these two which has a direct linear relation to their relationship(i.e; if the return 
            value is 0:aunt/uncle,1:1st cousin, 2:2nd cousin, so on...) and the generation difference between a and b 
            tells us whether it's once removed/twice removed.
                If b is below a, then the same code as above is used. This time b is moved up to find same_gen_person of a
            and we'll put these two in common_ancestor function and use it's returned value. the logic of k cousin will
            be same as up. In the above case, Aunt/uncle logic will be same as nephew/niece logic here.
                If both belongs to same generation then they both are given as input to common_ancestor and returned value 
            helps us to identify siblings or kcousin.

        */
        //pos_pgd is absolute value of generation difference. r is adjusted according to above explanation.
        if (pgd >= 0)
        {
            same_gen_person = mem1;
            pos_pgd = pgd;
            r = b;
        }
        else
        {
            same_gen_person = mem2;
            pos_pgd = 0 - pgd;
            r = a;
        }
        //finding same_gen_person
        for (int k = 0; k < pos_pgd; k++)
        {
            same_gen_person = same_gen_person->parent;
        }
        kcous = commonancestor(same_gen_person, r);
        if (kcous == 0 && pgd < 6 && pgd >= 0)
            printf("%d is %s of %d\n", j, lib2[pos_pgd], i);
        else if (kcous == 0 && pgd > -6 && pgd < 0)
            printf("%d is %s of %d\n", j, lib6[pos_pgd - 1], i);
        else if (kcous == 0 && pgd > 5)
            printf("%d is %dth Great Grand Aunt/Uncle of %d\n", j, pgd - 2, i);
        else if (kcous == 0 && pgd < -5)
            printf("%d is %dth Great Grand Nephew/Niece of %d\n", j, pos_pgd - 2, i);

        else if (kcous >= 1 && kcous <= 3)
        {
            if (pos_pgd == 0)
                printf("%d is %s cousin of %d\n", j, lib3[kcous - 1], i);
            else if (pos_pgd >= 1 && pos_pgd <= 3)
                printf("%d is %s cousin %s removed of %d\n", j, lib3[kcous - 1], lib4[pos_pgd - 1], i);
            else
                printf("%d is %s cousin %d times removed of %d\n", j, lib3[kcous - 1], pos_pgd, i);
        }
        else
        {
            if (pos_pgd == 0)
                printf("%d is %dth cousin of %d\n", j, kcous, i);
            if (pos_pgd >= 1 && pgd <= 3)
                printf("%d is %dth cousin %s removed of %d\n", j, kcous, lib4[pos_pgd - 1], i);
            else
                printf("%d is %dth cousin %d times removed of %d\n", j, kcous, pos_pgd, i);
        }
    }
}
/*
    bool childNochild(Node* jj)
    This function checks if the children nodes present in the linked list pointed by pointer in a given Node
    have no children and 
    returns true if the nodes in the list have no children
    else returns false.
    This function helps my destructor.
*/
bool FamilyTree::childNochild(Node *jj)
{
    Node *iter = jj->children->head;
    while (iter != NULL)
    {
        if (!(iter->children == NULL))
            return false;
        iter = iter->next;
    }
    return true;
}
/*
    void tclear(Node* k)
    Given a node clears whole subtree excluding root of that subtree.
    Used in deconstructor
*/
void FamilyTree::tclear(Node *k)
{
    Node *iter;
    if (k->children == NULL)
        return; //base case .If the node is a leaf return
    if (childNochild(k))
    {                         //true: as the children in the linked list have no children, the whole linked list can be deleted.
        k->children->clear(); //clear() function in dlist() adt deletes the list
        k->children = NULL;   //making the children pointer in node to point NULL.
        return;
    }
    iter = k->children->head;
    while (iter != NULL)
    {                 //iterating through all the children in the list
        tclear(iter); //recursive step
        if (childNochild(k))
        {
            k->children->clear();
            k->children = NULL;
            return;
        }
        iter = iter->next; //iterating
    }
}
FamilyTree::~FamilyTree()
{
    tclear(root);
    delete root;
}

int main()
{
    class FamilyTree ft(100);
}