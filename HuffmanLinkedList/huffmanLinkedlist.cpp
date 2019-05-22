//author: JianHui Lin
//purpose: to encode a data 

#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<stdio.h>
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
        str="";
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
    }
        treeNode(unsigned char s, int i, treeNode *n)
    {
        str=string(1,s);
        prob=i;
        next=n;
        left=NULL;
        right=NULL;
    }
    bool isleaf()
    {
        if(left==NULL && right==NULL)
            return true;
        else
            return false;
    }
    };
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
  output<< "listHead-->";
  while(current->next!=NULL)
  {
      output<<"(\""+ current->str+"\",\"" + to_string(current->prob) + "\", \""+ current->next->str+"\")-->";
      current=current->next;
  }
  output<<"(\""+ current->str+"\",\"" + to_string(current->prob) + "\", \""+"NULL\")-->NULL\n";
}};

class huffmanBinaryTree
{   
    public:
    string charCode[256]={""};
    treeNode *root;
    linkedList *lists = new linkedList();

    huffmanBinaryTree()
    {
        root = NULL;
    }
    treeNode *constructHuffmanLList(int charCount[256], ofstream &output)
    {
        treeNode *head = new treeNode("dammny",0,NULL);
        treeNode *newNode;
        for(int index =0;index<256;index++)
        {
            if(charCount[index]>0)
            {
               unsigned char c=(unsigned char) index;
                int prob = charCount[index];
                newNode = new treeNode(c,prob,NULL);
                lists->insert(head,newNode);
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
    void getCode(treeNode* t, string code, ofstream& output)
    {
      if(t->isleaf())
       {
            t->code=code;
            string s = t->str;
            unsigned char c=s.at(0);
           int i = (int) c;
            charCode[i]=code;
            output<<c<<" "<<code<<endl;

        }
      else
      {
        getCode(t->left, code+"0",output);
        getCode(t->right, code+"1",output);
      }
    }

void enCode(ifstream& input, ofstream& output)
{
    unsigned char ch;
     int number;
     string code;
    while(true)
    {
        ch=input.get();
        if(input.eof())
            break;
        if((int)ch<256)
        { 
            int number = int(ch);
            code=charCode[number];
            output<<code;
        }
    }
}
    void deCode(ifstream& input, ofstream& output, treeNode* he)
    {  
        if(he->isleaf())
        {
            output<<he->str;
            deCode(input,output,root);
        }else
        {
            if(input.eof() && he!=root)
                cout<<"the encode file is a corrupted file"<<endl;
            else
            {
                unsigned char nextBit=input.get();
                if(nextBit=='0')
                    deCode(input,output,he->left);
                if(nextBit=='1')
                    deCode(input,output,he->right);
            }
        }
    }
    
};
void printAry(ofstream& output,int c[256])
{
    for(int i=0;i<256;i++)
    {
        if(c[i]!=0)
        {
            unsigned char ch = (unsigned char) i;
            output<<ch<<" # "<<c[i]<<endl;
        }
    }
}

int main(int argc, char* argv[])
{
    ifstream input1;
    ifstream input2;
    ifstream input3;

    ofstream output1;
    ofstream output2;
    ofstream output3;
    ofstream output4;

    output1.open(argv[3]);
    output2.open(argv[4]);
    output3.open(argv[5]);
    output4.open(argv[6]);
    int counts[256] ={0};
    unsigned char c;
    huffmanBinaryTree *huff = new huffmanBinaryTree();
    treeNode *root;
    treeNode *head;

    input1.open(argv[1]);
    if(input1.fail())
    {
         cout<<"file 1 Error opening"<<endl;
        exit(1);
    }
    while(true)
    {
        c=input1.get();
        if(input1.eof())
            break;
        if((int)c<256)
        {
            int ascii = int(c);
            counts[ascii]++;
        }
    }
  printAry(output4,counts);
  head=huff->constructHuffmanLList(counts,output4);
  huff->constructHuffmanBinTree(head,output4);
  root=huff->root;
  huff->getCode(root,"",output1);
   input2.open(argv[2]);
   if(input2.fail())
    {
        cout<<"file2 Error opening"<<endl;
        exit(1);
    }
  huff->enCode(input2,output2);

    input1.close();
    input2.close();
    output1.close();
    output2.close();
    output4.close();


  string name;
  cout<<"enter file you want to get decoded"<<endl;
  cin>>name;
  input3.open(name);
   if(input3.fail())
    {
        cout<<"file3 Error opening"<<endl;
        exit(1);
    }
huff->deCode(input3,output3,root);
 input3.close();
 output3.close();
 return 0;
}