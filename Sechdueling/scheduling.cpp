#include<iostream>
#include<fstream>
using namespace std;

class schedule
{
    public:
    int numNodes=0;
    int totalJobTime=0;
    int **matrix;
    int **table;
    int processors=0;
    int *processJob;
    int *processTime;
    int *parentCount;
    int *kidCount;
    int *jobDone;
    int *jobMarked;
    int avaliProc=0;
    int procUsed=0;
    int currenTime=0;
    class node
    {
        public:
        int jobId = 0;
        int jobTime = 0;
        node *next=NULL;
        node()
        {
            jobId = 0;
            jobTime = 0;
            next=NULL;
        }
    };
    node *open = new node();
    schedule()
    {}
    void initializeMatrix()
    {
        matrix = new int*[numNodes+1];
        for(int i=0;i<numNodes+1;++i)
        {
            matrix[i] = new int[numNodes+1];
            for(int j=0;j<=numNodes;++j)
            {
                matrix[i][j]=0;
            }
        }
    }
    void loadMatrix(ifstream &input)
    {
        if(!input.eof())
            input>>numNodes;
        initializeMatrix();
        initializeProcessJobTimeParentKidCountJobDone();
        int r=0;
        int c=0;
        while(!input.eof())
        {
            if(input.good()==true)
            {
                input>>r>>c;
                matrix[r][c]++;
            }
        }
    }
    void ask4processors(ofstream &output)
    {
        cout<<"Enter processors you want to use"<<endl;
        cin>>processors;
        while(processors<=0)
        {
            cout<<"Enter at least 1 processor"<<endl;
            cin>>processors;
        }
        if(processors>numNodes)
            processors=numNodes;
        output<<"given processors:"<<processors<<endl;
    }
    void initializeTable()
    {
        table = new int*[numNodes+1];
        for(int i=0;i<=numNodes;++i)
        {
            table[i] = new int[totalJobTime+1];
            for(int j=0;j<=totalJobTime;++j)
            {
                table[i][j]=0;
            }
        }
    }
    void initializeProcessJobTimeParentKidCountJobDone()
    {
        processTime = new int[numNodes+1];
        processJob = new int[numNodes+1];
        parentCount = new int[numNodes+1];
        kidCount = new int[numNodes+1];
        jobDone = new int[numNodes+1];
        jobMarked = new int[numNodes+1];
        for(int i=0;i<=numNodes;++i)
        { 
            processJob[i]=0;
            processTime[i]=0;
            jobDone[i]=0;
            jobMarked[i]=0;
            parentCount[i]=0;
            kidCount[i]=0;
        }
    }
    void setKidParentCount(ofstream &output)
    {
        for(int i=1;i<=numNodes;++i)
        {
            for(int j=1;j<=numNodes;++j)
            {
                if(matrix[j][i]!=0)
                    parentCount[i]++;
                if(matrix[i][j]!=0)
                    kidCount[i]++;
            }
        }
        output<<"kidcount:";
        for(int i=1;i<=numNodes;++i)
            output<<kidCount[i]<<" ";
        output<<endl;
        output<<"parentcount:";
        for(int i=1;i<=numNodes;++i)
            output<<parentCount[i]<<" ";
        output<<endl;
        output<<"matrix:"<<endl;
        for(int i=0;i<=numNodes;++i)
        {
            for(int j=0;j<=numNodes;++j)
            {
                output<<matrix[i][j]<<" ";
            }
            output<<endl;
        }
        output<<"^ this is jobTime array"<<endl<<endl;
    }
    void scheduleTable(ifstream &input, ofstream &output)
    {
        if(!input.eof())
            input>>numNodes;
        int time=0;
        int row=0;
        while(true)
        {
            input>>row>>time;
            matrix[row][0]=time;
            if(input.eof())
                break;
            totalJobTime+=time;
        }
        ask4processors(output);
        initializeTable();
        setKidParentCount(output);
    }
    void updateTable(int avaliblep)
    {
        for(int i=currenTime;i<currenTime+open->next->jobTime;++i)
            table[avaliblep][i] = open->next->jobId;
    }
    int getUnmarkOrphen(int i)
    {
        if(jobMarked[i]==0 && parentCount[i]==0)
            return i;
        else
            return -1;
    }
    int findAvaliProcessor()
    {
        for(int i=1;i<=processors;++i)
        {
            if(processTime[i]<=0)
                return i;
        }
        return -1;
    }
    bool isJobDone()
    {
        for(int i=1;i<=numNodes;++i)
        {
            if(jobDone[i]==0)
                return false;
        }
        return true;
    }
    bool findProcessor()
    {
        for(int i=1;i<=processors;++i)
        {
            if(processTime[i]>0)
                return false;
        }
        return true;
    }
    node *findSpot(node *open, node *newNode)
    {
        node* spot = open;
        while(((spot->next)!=NULL) && (kidCount[spot->next->jobId]>kidCount[newNode->jobId]))
            spot=spot->next;
        return spot;
    }
    void insert2open(node* open,node *newNode)
    {
        node* spot = findSpot(open,newNode);
        newNode->next = spot->next;
        spot->next = newNode;
    }
    void printOpen(ofstream &output)
    {
        output<<"open:";  
        node *spot = open;
        while(spot->next!=NULL)
        {
            output<<spot->next->jobId<<",";
            spot = spot->next;
        }
        output<<endl;
    }
    void printTable(ofstream &output)
    {
        output<<"     ";
        for(int i=0;i<=currenTime;++i)
        {
            if(i/10==0)
            {
                output<<"--"<<i<<"--";
            }
            else
            {
                output<<"-"<<i<<"--";
            }
        }
        output<<"  ... "<<endl;
        for(int i=1;i<=processors;++i)
        {
            output<<"p("<<i<<")";
            for(int j=0; j<=currenTime;++j)
            {
                if(table[i][j]==0)
                {
                    output<<"| -- ";
                }
                else
                {
                    if(table[i][j]/10==0)
                    {
                        output<<"| "<<table[i][j]<<"  ";
                    }
                    else
                    {
                        output<<"| "<<table[i][j]<<" ";
                    }
                }
            }
            output<<"  ... "<<endl<<"     ";
            for(int i=0;i<=currenTime;++i)
            {
                output<<"-----";
            }
            output<<endl;
        }
        output<<endl;
    }
    void printDebug(ofstream &output)
    {
        output<<"current time: "<<currenTime<<endl;
        output<<"jobMarked         processTime       processJob        jobDone"<<endl;
        for(int i=1;i<=numNodes;++i)
        {
            output<<i<<":"<<jobMarked[i];
            output<<"               "<<i<<":"<<processTime[i];
            output<<"               "<<i<<":"<<processJob[i];
            output<<"               "<<i<<":"<<jobDone[i]<<endl;
        }
        output<<endl;
    }
    void decreaseProcessTime()
    {
        for(int i=1; i<=numNodes;++i)
        {
            if(processTime[i]>0)
                processTime[i]-=1;
        }
    }
    void deleteFinishedJob()
    {
        int jobId;
        for(int i=1;i<=numNodes;++i)
        { 
            if(processTime[i]==0 && processJob[i]>0)
            {
                jobId=processJob[i];
                processJob[i]=0;
                jobDone[jobId]=1;
                --procUsed;
                for(int j=1;j<=numNodes;++j)
                {
                    if(matrix[jobId][j]>0)
                        parentCount[j]--;
                }
            }
        }
    }
    int checkCycle()
    {
        if(open->next==NULL && findProcessor() && !isJobDone())
            return 1;
        else
            return 0;
    }
    void scheduleSteps(ofstream &output1, ofstream &output2)
    {
        node *newNode;
        while(!isJobDone())
        {
            for(int i=1;i<=numNodes;++i)
                {
                    if(getUnmarkOrphen(i)!=-1)
                    {
                        newNode = new node();
                        newNode->jobId=i;
                        newNode->jobTime=matrix[i][0];
                        jobMarked[i]=1;
                        insert2open(open,newNode);
                    }
                }
                printOpen(output2);
                while((open->next!=NULL) && procUsed < processors)
                {
                    avaliProc = findAvaliProcessor();
                    if(avaliProc>0)
                    {
                        procUsed++;
                        processJob[avaliProc]=open->next->jobId;
                        processTime[avaliProc]=open->next->jobTime;
                        updateTable(avaliProc);
                        open->next=open->next->next;
                    }
                }
                if(checkCycle()>=1)   
                {
                    cout<<"error message its cycle"<<endl;
                    return;
                }
                printTable(output1); 
                printDebug(output2); 
                currenTime++;
                decreaseProcessTime();
                deleteFinishedJob();
                printDebug(output2);
            }
    }
};
int main(int argc, char* argv[])
{
    ifstream input1;
    ifstream input2;
    ofstream output1;
    ofstream output2;
    input1.open(argv[1]);
    if(input1.fail())
    {
        cout<<"file 1 Error opening"<<endl;
        exit(1);
    }
    input2.open(argv[2]);
    if(input2.fail())
    {
        cout<<"file 2 Error opening"<<endl;
        exit(1);
    }
    output1.open(argv[3]);
    output2.open(argv[4]);
    schedule *s = new schedule();
    s->loadMatrix(input1);
    s->scheduleTable(input2,output2);
    s->scheduleSteps(output1,output2);
    input1.close();
    input2.close();
    output1.close();
    output2.close();
    return 0;
}