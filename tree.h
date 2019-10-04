struct node {
    int n;
    struct node *firstKid;
    struct node *siblings;
};


struct node* newNode(int data);

void freeTree(struct node* node);
int removeNode(struct node* node);
void addSibling(struct node* node, struct node* newSibling);
void addChild(struct node* node, struct node* newChild);
void printTree(struct node* node);