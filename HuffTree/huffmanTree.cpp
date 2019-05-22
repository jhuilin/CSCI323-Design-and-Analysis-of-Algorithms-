#include<iostream>
#include<fstream>
#include<string>
using namespace std;
class treeNode
{
    public:
    string str;
    int prob;
    treeNode *next;
    treeNode *left;
    treeNode *right;
    string code;
    treeNode()
    {
        str=" ";
        prob=0;
        next=NULL;
        left=NULL;
        right=NULL;
        code="";
    }
    treeNode(string s, int i, treeNode *n)
    {
        str=s;
        prob=i;
        next=n;
        left=NULL;
        right=NULL;
    }};
class linkedList
{
  public:
  treeNode *head;
  linkedList()
  {}
  treeNode *findSpot(treeNode *head, treeNode *newNode)
{
  treeNode *spot = head;
  while(((spot->next)!=NULL) && (spot->next->prob) < newNode->prob)
    spot = spot->next;
  return spot;
}
void insert(treeNode *head, treeNode *newNode)
{
  treeNode *spot = findSpot(head,newNode);
  newNode->next = spot->next;
  spot->next = newNode;
}
void print(treeNode *head, ofstream &output)
{
  treeNode *current = head;
  string s = "listHead-->";
  while(current->next!=NULL)
  {
      s+="(\""+ current->str+"\",\"" + to_string(current->prob) + "\", \""+ current->next->str+"\")-->";
      current=current->next;
  }
  s+="(\""+ current->str+"\",\"" + to_string(current->prob) + "\", \""+"NULL\")-->NULL\n";
  output<<s;
}};
class huffmanBinaryTree
{   
    public:
    treeNode *root;
    linkedList *lists = new linkedList();
    huffmanBinaryTree()
    {
        root = NULL;
    }
   void getCode(treeNode* t, string code,ofstream &output)
   {
      if(t->right==NULL && t->left==NULL)
       {
            t->code=code;
            output<<t->str<<" # "<<t->code+"\n";
       }
      else
      {
        getCode(t->left, code+"0",output);
        getCode(t->right, code+"1",output);
      }
   }
   treeNode *constructHuffmanLList(ifstream &input, ofstream &output)
   {
    string str;
    string str_check;
    int i;
    treeNode *head = new treeNode("dammny",0,NULL);
    treeNode *newNode;
    if(input.fail())
    {
      cout<<"Error opening"<<endl;
      exit(1);
    }
    if(!input.eof())
    {
      input>>str>>i;
      newNode = new treeNode(str,i,NULL);
      lists->insert(head,newNode);
      str_check=str;
      while(!input.eof())
     { 
       input>>str>>i;
       if(str.compare(str_check)!=0)
       {   
           newNode = new treeNode(str,i,NULL);
           lists->insert(head,newNode);
           str_check=str;
        }
      }
    }
    lists->print(head,output);
    return head;
   }
   void constructHuffmanBinTree(treeNode *h, ofstream &output)
    {
        treeNode *newNode;
        lists->head = h;
        while(lists->head->next->next!=NULL)
        {   
  			newNode = new treeNode();
            newNode->prob=lists->head->next->prob+lists->head->next->next->prob;
            newNode->str=lists->head->next->str+lists->head->next->next->str;
            newNode->left=lists->head->next;
            newNode->right=lists->head->next->next;
            lists->insert(lists->head,newNode);
            lists->head->next=lists->head->next->next->next;
            lists->print(lists->head->next,output);
            output<<"Node:"+newNode->str +", Node`s prob:"+to_string(newNode->prob)+", Node`s next:"+lists->head->next->str+", Node`s left:"+ newNode->left->str+", Node`s right:"+newNode->right->str+"\n\n"; 
        } 
        root = newNode;
    }
   void preOrderTraveral(treeNode* r, ofstream &output)
   {
        if(r==NULL)
            return;
        else
        {
            output<<r->str<<",";
            preOrderTraveral(r->left,output);
            preOrderTraveral(r->right,output);
        }
   }
   void postOrderTraveral(treeNode* r, ofstream &output)
   {
       if(r==NULL)
            return;
        else
        {
            postOrderTraveral(r->left,output);
            postOrderTraveral(r->right,output);
            output<<r->str<<",";        
        }
   }
   void inOrderTravversal(treeNode* r, ofstream &output)
   {
       if(r==NULL)
            return;
        else
        {
            inOrderTravversal(r->left,output);
            output<<r->str<<",";
            inOrderTravversal(r->right,output);
        }
   }};
int main(int argc, char** argv)
{
  huffmanBinaryTree *huff = new huffmanBinaryTree();
 treeNode *newNode;
  treeNode *root;
  treeNode *head;
  ifstream input;
  input.open(argv[1]);
  ofstream output;
  ofstream outputCode;
  ofstream outputPre;
  ofstream outputIn;
  ofstream outputPost;
  output.open(argv[6]);
  head=huff->constructHuffmanLList(input,output);
  huff->constructHuffmanBinTree(head,output);
  root=huff->root;
  outputCode.open(argv[2]);
  huff->getCode(root,"",outputCode);
  outputPre.open(argv[3]);
  huff->preOrderTraveral(root,outputPre);
  outputIn.open(argv[4]);
  huff->inOrderTravversal(root,outputIn);
  outputPost.open(argv[5]);
  huff->postOrderTraveral(root,outputPost);
  input.close();
  outputCode.close();
  output.close();
  outputIn.close();
  outputPre.close();
  outputPost.close();
}
