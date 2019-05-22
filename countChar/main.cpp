#include<iostream>
#include<fstream>
#include<string>
using namespace std;
void printAry(ofstream& output,int c[256])
{
    for(int i=0;i<256;i++)
    {
        if(c[i]!=0)
        {
            char ch = (char) i;
            output<<ch<<" # "<<c[i]<<endl;
        }
    }
}
int main(int argc, char* argv[])
{
  int counts[256];
  for(int i=0;i<256;i++)
  {
    counts[i]=0;
  }
  if(argc<2)
  {
      cout<<"Error"<<endl;
      exit(1);
  }
  ifstream input;
  input.open(argv[1]);
  if(input.fail())
    {
      cout<<"Error opening"<<endl;
      exit(1);
    }
  char c;
  while(!input.eof())
  {
    input.get(c);
    if((int)c<256)
    {
      int ascii = int(c);
      counts[ascii]++;
    }
  }
  input.close();
  for(int i=0;i<256;i++)
  {
    if(counts[i]!=0)
    {
      char ch = (char) i;
      cout<<ch<<" # "<<counts[i]<<endl;
    }
  }
  ofstream output;
  output.open(argv[2]);
  printAry(output,counts);
    return 0;
}
