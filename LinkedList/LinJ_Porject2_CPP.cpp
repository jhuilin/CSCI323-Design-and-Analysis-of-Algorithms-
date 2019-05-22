#include<iostream>
#include<fstream>
#include<string>
using namespace std;

class listNode
{
  public:
    string str;
    int f;
    listNode *next;
    listNode(string s, int i, listNode *n)
    {
      str = s;
      f = i;
      next = n;
    }
};
class linkedList
{
  public:
  linkedList()
  {}
  listNode *findSpot(listNode *head, listNode *newNode)
{
  listNode *spot = head;
  while(((spot->next)!=NULL) && (spot->next->f) < newNode->f)
    spot = spot->next;
  return spot;
}
void insert(listNode *head, listNode *newNode)
{
  listNode *spot = findSpot(head,newNode);
  newNode->next = spot->next;
  spot->next = newNode;
}
string print(listNode *head)
{
  listNode *current = head;
  string s = "listHead-->";
  while(current->next!=NULL)
  {
      s+="(\""+ current->str+"\",\"" + to_string(current->f) + "\", \""+ current->next->str+"\")-->";
      current=current->next;
  }
  s+="(\""+ current->str+"\",\"" + to_string(current->f) + "\", \""+"NULL\")-->NULL";
  return s;
  
}
};
int main(int argc, char* argv[])
{
  string str;
  string str_check;
  int i;
  ifstream input;
  linkedList *lists = new linkedList();
  listNode *head = new listNode("dammny",0,NULL);
  listNode *newNode;

  input.open(argv[1]);
  if(input.fail())
    {
      cout<<"Error opening"<<endl;
      exit(1);
    }
    if(!input.eof())
    {
      input>>str>>i;
      newNode = new listNode(str,i,NULL);
      lists->insert(head,newNode);
      str_check=str;
      while(!input.eof())
     { 
       input>>str>>i;
       if(str.compare(str_check)!=0)
       {   
           newNode = new listNode(str,i,NULL);
           lists->insert(head,newNode);
           str_check=str;
        }
      }
     }
     cout<<lists->print(head);

    input.close();
    ofstream output;
    output.open(argv[2]);
    output<<lists->print(head);
    output.close();
}
