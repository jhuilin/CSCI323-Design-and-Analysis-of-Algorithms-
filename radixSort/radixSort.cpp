#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<stdio.h>
using namespace std;
class listNode
{
    public:
    int data;
    listNode *next;
    listNode()
    {
        data=0;
        next=NULL;
    }
    listNode(int d)
    {
        data=d;
    }
};
class linkedListStack
{
    public:
    listNode *top;
    linkedListStack()
    {
        top=NULL;
    }
    void push(int element)
    {
            listNode *newHead=new listNode();
            newHead->data = element;
            newHead->next = top;
            top=newHead;
    }
    listNode* pop()
    {
        if(isEmpty())
        {
            return NULL;
        }
        listNode *newTop = top;
        top=top->next;
        return newTop;
    }
    bool isEmpty()
    {
        return (top==NULL);
    }
   void printStack(listNode *t, ofstream &output)
    {
        output<<"Top->";
        while(t->next!=NULL)
        {
            output<<"("+to_string(t->data)+","+to_string(t->next->data)+")->";
            t=t->next;
        }
        output<<"("+to_string(t->data)+",NULL)->";
        output<<"NULL"<<endl;
    }
};
class linkedListQueue
{
    public:
    listNode *head;
    listNode *tail;
    linkedListQueue()
    {
        head=tail=NULL;
    }
    bool isEmpty()
    {
        return (head==NULL && tail==NULL);
    }
    void addTail(listNode *t)
    {
        t->next=NULL;
        if(isEmpty())
        {
            head=t;
            tail=t;
        }
        else
        {
            tail->next=t;
            tail=t;
        }
    }
    listNode *deleteFront()
    {
        if(isEmpty())
            return head;
        listNode *sb=head;
        head=head->next;
        if(head==NULL)
        {
            tail=NULL;
        }
        return sb;
    }
    void printQueue(int index,ofstream &output)
    {
        output<<"front("+to_string(index)+")->";
        listNode *spot=head;
        while(spot->next!=NULL)
        {
            output<<"("+to_string(spot->data)+","+to_string(spot->next->data)+")->";
            spot=spot->next;
        }
        output<<"("+to_string(spot->data)+",NULL)->NULL";
        output<<endl;
    }
};
class Radixsort
{
    public:
    int tableSize = 10;
    int data;
    linkedListQueue *hashTable[2][10];
    int currentTable=1;
    int previousTable=0;
    int currenQueue=0;
    int largest=0;
    int maxDigit;
    linkedListStack *stacks = new linkedListStack();
    linkedListQueue *queue;
    Radixsort()
    {
        for(int r=0; r<2; r++)
        {
            for(int c=0;c<10;c++)
            {
                queue = new linkedListQueue();
                hashTable[r][c] = queue;
            }
        }
    }
    void loadStack(ifstream &input)
    {
        while(!input.eof())
        {
            input>>data;
            if(input.good()==true)
            {
                if(data>largest)
                    largest=data;
                stacks->push(data);
            }
        }
    }
    int getLargestLength(int i)
    {
        int c=1;
        while(i/10>0)
        {
            c++;
            i/=10;
        }
        return c;
    }
    int getDigit(int n, int digit)
    {
        if(digit==0)
            return n%10;
        else
           {
                n=n/10;
               return getDigit(n,digit-1);
           }
    }
    void printTable(linkedListQueue **table, ofstream &output)
    {
      for(int i=0;i<10;i++)
        if(table[i]->head!=NULL)
            table[i]->printQueue(i,output);
    }
    void dumpStacks(int cDigit, int cTable, ofstream &output)
    {
        int digit=0;
        int hashIndex;
        listNode *node = stacks->pop();
        while(node!=NULL)
        {
            digit=getDigit(node->data,cDigit);
            hashTable[cTable][digit]->addTail(node);
            node=stacks->pop();
        }
    }
    void printSort(linkedListQueue **t, ofstream &output)
    {
        output<<"sorted Data:";
            for(int j=0;j<10;j++)
                while(t[j]->head!=NULL)
                    {  
                        output<<t[j]->head->data<<",";
                        t[j]->head=t[j]->head->next;
                    }
    }
    void sort(ofstream &output, ofstream &output2)
    {
        int value;
        int index;
        int currentDigit=0;
        int currentTable=0;
        listNode *node;
        dumpStacks(currentDigit,currentTable,output);
        printTable(hashTable[currentTable],output);
        currentDigit++;
        currentTable=1;
        previousTable=0;
        maxDigit=getLargestLength(largest);
        while(currentDigit<maxDigit){
            while(currenQueue<tableSize){
                while(!hashTable[previousTable][currenQueue]->isEmpty()){
                    node = hashTable[previousTable][currenQueue]->deleteFront();
                    value = node->data;
                    index = getDigit(value,currentDigit);
                    hashTable[currentTable][index]->addTail(node);
                }
                currenQueue++;
            }
            printTable(hashTable[currentTable],output);
            currenQueue=0;
            currentDigit++;
            int temp = currentTable;
            currentTable=previousTable;
            previousTable=temp;
    }
    printSort(hashTable[previousTable],output2);
    }
};
int main(int argc, char* argv[])
{
    ifstream input1;
    ofstream output1;
    ofstream output2;
    Radixsort *rs = new Radixsort();
    input1.open(argv[1]);
    output1.open(argv[2]);
    output2.open(argv[3]);
    if(input1.fail())
    {
         cout<<"file 1 Error opening"<<endl;
        exit(1);
    }

    rs->loadStack(input1);
    rs->sort(output2,output1);

    input1.close();
    output1.close();
    output2.close();
    return 0;
}
