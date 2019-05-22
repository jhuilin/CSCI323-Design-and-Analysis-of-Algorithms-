#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
#include<time.h>
using namespace std;
class kMeanClustering
{
    class point{
        public:
        int xcoord;
        int ycoord;
        int label;
        double distance=9999.0;
        point()
        {
            xcoord=0;
            ycoord=0;
            label=0;
            distance=9999.0;
        }
    };
    class xycoord{
        public: 
        int xcoord;
        int ycoord;
        int label;
        xycoord()
        {
            xcoord=0;
            ycoord=0;
            label=0;
        }
    };
    public:
    int rows=0;
    int cols=0;
    int max=0;
    int min=0;
    int k=0;
    int numPts=0;;
    int change =0;
    int **imageArray;
    point *pointSet;
    xycoord *kcentroids;
    kMeanClustering()
    {}
    void getHeader(ifstream &input)
    {
        if(!input.eof())
            input>>rows;     
        if(!input.eof())
            input>>cols;     
        if(!input.eof())
            input>>min;    
        if(!input.eof())
            input>>max;
    }
    void initializeArray()
    {
        imageArray = new int*[rows];
        for(int i=0;i<rows;i++)
        {
            imageArray[i]= new int[cols];  
            for(int j=0;j<cols;j++)
            {
                imageArray[i][j]=0;
             }
        }
    }
    void ask4k()
    {
        cout<<"Enter k:"<<endl;
        cin>>k;
        while(k<0)
        {
            cout<<"Enter value for k greater than or equal to 0"<<endl;
            cout<<"Enter k:"<<endl;
            cin>>k;
        }
        kcentroids = new xycoord[k+1];
    }
    void getPoint(ifstream &input, ofstream &output)
    {
        int n =0;
        int data;
        for(int i=0;i<rows;i++)
            for(int j=0;j<cols;j++){
                input>>data;
                if(data>0)
                    {
                        n++;
                        output<<i<<" "<<j<<endl;
                    }
            }
        numPts=n;
        pointSet = new point[numPts];
    }
    void loadPoint(ifstream& input)
    {
        int x,y;
        int c=0;
        while(!input.eof())
        {
            if(input.good()==true)
            {
                input>>x>>y;
                pointSet[c].xcoord=x;
                pointSet[c].ycoord=y;
                c++;
            }
        }
    }
    void assignLabel()
    {
        int top=0;
        int bottom=numPts-1;
        int index=0;
        int la=1;
        while(index<=numPts)
        {
            pointSet[top].label=la;
            la++;
            index++;
            if(index<=numPts)
            {
                pointSet[bottom].label=la;
                bottom--;
                top++;
                index++;
                la++;
                if(la>k)
                {
                    la=1;
                }
            }
        }
    }
    void computeCentroids()
    {
        int *sumY = new int[k+1];
        int *sumX = new int[k+1];
        int *total = new int[k+1];
        for(int i=0;i<=k;i++)
        {
            sumY[i]=0;
            sumX[i]=0;
            total[i]=0;
        }
        int index = 0;
        int label;
        while(index<numPts)
        {
            label = pointSet[index].label;
            sumX[label]+=pointSet[index].xcoord;
            sumY[label]+=pointSet[index].ycoord;
            total[label]++;
            index++;
        }
        label=1;
        while(label<=k)
        {
            if(total[label]!=0)
           { 
                kcentroids[label].xcoord = (sumX[label]/total[label]);
                kcentroids[label].ycoord = (sumY[label]/total[label]);
           }
            label++;
        }
    }
    double computeDist(int index, int label)
    {
        int x = pow((pointSet[index].xcoord-kcentroids[label].xcoord),2);
        int y = pow((pointSet[index].ycoord-kcentroids[label].ycoord),2);
        return pow((x+y),0.5);
    }
    void distanceMinlabel(int index)
    {
        double minDist = 9999.0;
        int minLabel =0;
        int label = 1;
        while(label<=k)
        {
            double dist = computeDist(index,label);
            if(dist<minDist)
            {
                minLabel = label;
                minDist = dist;
            }
            label++;
        }
        if(minLabel!=pointSet[index].label)
        {
            pointSet[index].distance=minDist;
            pointSet[index].label=minLabel;
            change++;
        }
    }
    void point2Image()
    {
        int r=0;
        int c=0;
        for(int i=0;i<numPts;i++)
        {
            r=pointSet[i].xcoord;
            c=pointSet[i].ycoord;
            imageArray[r][c]=pointSet[i].label;
        }
    }
    void printImage(ofstream &output, int i)
    {
        output<<"******************iteration"<<i<<"******************"<<endl;
        for(int a=0;a<rows;a++)
        {
            for(int b=0;b<cols;b++)
                {
                    if(imageArray[a][b]==0)
                        output<<"  ";
                    else
                        output<<imageArray[a][b]<<" ";
                }
            output<<endl;
        }    
    }
    void writeSet(ofstream &output)
    {
        output<<numPts<<endl;
        output<<rows<<" "<<cols<<endl;
        for(int i=0;i<rows;i++)
        {
            for(int j=0;j<cols;j++)
            {
                if(imageArray[i][j]!=0)
                {
                    output<<i<<" "<<j<<" "<<imageArray[i][j]<<endl;
                }
            }
        }
    }
    void clustering(ofstream &output, ofstream &output3)
    {
        int iteration=1;
        assignLabel();
        do
        {
            change=0;
            computeCentroids();
            point2Image();
            printImage(output,iteration);
            int index =0;
            while(index<numPts)
            {
                distanceMinlabel(index);
                index++;
            }
            iteration++;
        }while(change > 0);
    writeSet(output3);
    }
};
int main(int argc, char* argv[])
{
        ifstream input1;
        ifstream input2;
        ofstream output1;
        ofstream output2;
        ofstream output3;
        
        input1.open(argv[1]);  
        output1.open(argv[2]);
        if(input1.fail())
        {
            cout<<"file 1 Error opening"<<endl;
            exit(1);
        }  
        kMeanClustering *cluster = new kMeanClustering();
        cluster->getHeader(input1);
        cluster->ask4k();
        cluster->initializeArray();
        cluster->getPoint(input1,output1);
        output1.close();
        input1.close();
        input2.open(argv[2]);
        if(input2.fail())
        {
            cout<<"file 2 Error opening"<<endl;
            exit(1);
        }
        cluster->loadPoint(input2);
        output2.open(argv[3]);
        output3.open(argv[4]);
        cluster->clustering(output2,output3);
        input2.close();
        output2.close();
        output3.close();  
        return 0;
}