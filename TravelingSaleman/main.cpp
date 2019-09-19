/*
	Travel Salesman Problem with A*	on undirected graph
*/
#include <fstream>
#include <iostream>
#include <climits>
using namespace std;

class AstarNode {
private:
	int ID;
	int gStar;
	double hStar;
	double fStar;
	int numEdgeFromStart;
	AstarNode *next;
	AstarNode *parent;
	friend class AstarSearch;
	friend class TSP;

public:
	AstarNode(int id) {
		ID = id;
		numEdgeFromStart = 0;
		next = NULL;
		parent = NULL;
		gStar = 0;
		hStar = 0;
		fStar = 0;
	}
};

class AstarSearch {
private:
	int numNodes;
	int start;
	int whichHFunction;
	int minCost;		//min edge cost
	int totalCost;		//total cost of all edge
	int numEdge;	//number of edge
	AstarNode *oldNode;
	AstarNode *OpenList;
	AstarNode *CloseList;
	int **costMatrix;
	int *costAry;	//for sorting all edge cost for h3 function 
	int *childAry;
	friend class TSP;

	~AstarSearch() {
		delete oldNode;
		AstarNode *spot = OpenList;
		deleteList(OpenList);
		deleteList(CloseList);
		for (int i = 0; i < numNodes; ++i) {
			delete[] costMatrix[i];
		}
		delete[] costMatrix;
		delete[] costAry;
	}

	void deleteList(AstarNode *node) {
		if (!node) { return; }
		deleteList(node->next);
		delete node;
	}

public:
	AstarSearch(int numN) {
		numNodes = numN;
		start = 0;
		whichHFunction = 0;
		minCost = INT_MAX;
		totalCost = 0;
		//numNotVisitedNode = numNodes;
		oldNode = NULL;
		numEdge = 0;
		OpenList = new AstarNode(0);
		CloseList = new AstarNode(0);
		costMatrix = new int*[numNodes];
		costAry = new int[(numNodes*(numNodes - 1)) / 2];  //maximum number of edges for undirected graph
		for (int i = 0; i < numNodes; ++i) {
			costMatrix[i] = new int[numNodes];
			for (int j = 0; j < numNodes; ++j) {
				costMatrix[i][j] = -1;
			}
		}
		for (int i = 0; i < (numNodes*(numNodes - 1)) / 2; ++i) {
			costAry[i] = 0;
		}
		//childAry = new int[numNodes];
		//for (int i = 0; i <= numNodes; ++i) {
		//	childAry[i] = -1;
		//}
	}
//-------------------------------------------------------------------------
	void findPath(ifstream &inFile, ofstream &outFile) {
		AstarNode *currentNode = NULL, *childNode = NULL;
		int matrixIndex = 0;

		loadMatrix(inFile, outFile);
		AstarNode *Snode = new AstarNode(start);
		Snode->parent = Snode;
		OpenInsert(Snode);
		printOpen(outFile);
		printClose(outFile);

		do {
			if (OpenList->next == NULL) { 
				cout << "ERROR: there is no path to complete Travel SalesMan Problem";
				outFile << "ERROR: there is no path to complete Travel SalesMan Problem";
				return;
			}
			currentNode = OpenRemove();
			matrixIndex = currentNode->ID - 1;
			//copyChildList (matrixIndex) -- copy costMatrix[matrixIndex] to childAry
			for (int cID = 0; cID < numNodes; ++cID) {
				if (costMatrix[matrixIndex][cID] > 0) {	//cost > 0 and is not visited
					childNode = new AstarNode(cID + 1);
					childNode->parent = currentNode;
					computeFunction(childNode);
					if (!isInCloseList(cID + 1)) {	//not visited
						childNode->numEdgeFromStart = childNode->parent->numEdgeFromStart + 1;
						OpenInsert(childNode);
						printOpen(outFile);
					}
					else {
						if (childNode->fStar < oldNode->fStar) {
							closeDelete(oldNode);
							OpenInsert(childNode);
							printOpen(outFile);
						}
					}
				}
			}
			if (hasEdge(currentNode)) {
				closePush(currentNode);
			}
			printClose(outFile);
		} while (checkPath(currentNode) <= 0);
	}
//-------------------------------------------------------------------------
	void loadMatrix(ifstream &inFile, ofstream &outFile) {
		int n1 = 0, n2 = 0, cost = -1;
		while (!inFile.eof()) {
			if (inFile.good()) {
				inFile >> n1;
				inFile >> n2;
				inFile >> cost;
				costMatrix[--n1][--n2] = cost;//node#1 is 0 in matrix and node#2 is 1 in matrix, etc.
				costMatrix[n2][n1] = cost;
				costMatrix[n1][n1] = 0; //goto itself cost 0
				costMatrix[n2][n2] = 0;
				costAry[numEdge++] = cost;
				minCost = (cost < minCost) ? cost : minCost;
				totalCost += cost;
			}
			while (inFile.peek() == '\n') { inFile.get(); }
		}
		if (whichHFunction == 3)
			sorting(0, numEdge - 1);
		for (int i = 0; i < numNodes; ++i) {
			for (int j = 0; j < numNodes; ++j) {
				outFile << costMatrix[i][j] << "\t";
			}
			outFile << endl;
		}
	}
//-------------------------------------------------------------------------
	void sorting(int low, int high) {	//quick sort
		if (low < high) {
			int par = partition(low, high);
			sorting(low, par - 1);
			sorting(par + 1, high);
		}
	}
	int partition(int low, int high) {
		int pivot = costAry[high];
		int i = low, temp = 0;
		for (int j = low; j <= high - 1; ++j) {
			if (costAry[j] <= pivot) {
				temp = costAry[j];
				costAry[j] = costAry[i];
				costAry[i++] = temp;
			}
		}
		temp = costAry[high];
		costAry[high] = costAry[i];
		costAry[i] = temp;
		return i;
	}
//-------------------------------------------------------------------------
	void computeFunction(AstarNode *node) {
		node->gStar = computeGstar(node);
		node->hStar = computeHstar(node);
		node->fStar = computeFstar(node);
	}
//-------------------------------------------------------------------------
	int computeGstar(AstarNode *node) {
		return node->parent->gStar + costMatrix[node->parent->ID - 1][node->ID - 1];
	}
//-------------------------------------------------------------------------
	double computeHstar(AstarNode *node) {
		switch (whichHFunction) {
		case 1:
			return h1function(node);
		case 2:
			return h2function(node);
		case 3:
			return h3function(node);
		default:
			return 0;
		}
		return 0;
	}
	double h1function(AstarNode *node) {	//(avg of all edges cost)*(number of not visited node)
		return (totalCost / numEdge) * numNodes - node->numEdgeFromStart;
	}
	double h2function(AstarNode *node) {	//(minimum of all edges' cost)*(number of not visited node)
		return minCost * numNodes - node->numEdgeFromStart;
	}
	double h3function(AstarNode *node) {	//(median of all edges' cost)*(number of not visited node)
		if (numEdge % 2 <= 0) {
			return ((costAry[(numEdge / 2) - 1] + costAry[numEdge / 2]) / 2) * numNodes - node->numEdgeFromStart;
		}
		return costAry[numEdge / 2] * numNodes - node->numEdgeFromStart;
	}
//-------------------------------------------------------------------------
	double computeFstar(AstarNode *node) {
		return node->gStar + node->hStar;
	}
//-------------------------------------------------------------------------
	void OpenInsert(AstarNode *node) {
		AstarNode *spot = findSpot(node);
		node->next = spot->next;
		spot->next = node;
	}
	AstarNode *findSpot(AstarNode *node) {
		AstarNode *spot = OpenList;
		while (spot->next && spot->next->fStar < node->fStar) {
			spot = spot->next;
		}
		return spot;
	}
//-------------------------------------------------------------------------
	void printOpen(ofstream &outFile) {
		outFile << "\n***OpenList***\n";
		AstarNode *spot = OpenList->next;
		while (spot) {
			outFile << "(" << spot->ID << ", " << spot->parent->ID << ", " << spot->fStar << ")->";
			spot = spot->next;
		}
		outFile << "NULL\n";
	}
	void printClose(ofstream &outFile) {
		outFile << "\n***CloseList***\n";
		AstarNode *spot = CloseList->next;
		while (spot) {
			outFile << "(" << spot->ID << ", " << spot->parent->ID << ", " << spot->fStar << ")->";
			spot = spot->next;
		}
		outFile << "NULL\n\n\n";
	}
//-------------------------------------------------------------------------
	AstarNode *OpenRemove() {
		AstarNode *node = OpenList->next;
		OpenList->next = node->next;
		return node;
	}
//-------------------------------------------------------------------------
	bool isInCloseList(int cID) {
		AstarNode *spot = CloseList->next;
		while (spot && spot->ID != cID) {
			spot = spot->next;
		}
		oldNode = spot;
		return spot != NULL;
	}
	bool hasEdge(AstarNode *node) {
		return CloseList->next == NULL || node->parent->ID == CloseList->next->ID;
	}
//-------------------------------------------------------------------------
	void closeDelete(AstarNode *node) {
		AstarNode *spot = CloseList;
		while (spot->next != node) {
			spot = spot->next;
		}
		spot->next = node->next;
	}
	void closePush(AstarNode *node) {
		node->next = CloseList->next;
		CloseList->next = node;
	}
//-------------------------------------------------------------------------
	int checkPath(AstarNode *node) {
		AstarNode *spot = CloseList->next->next;
		if (costMatrix[node->ID - 1][start - 1] > 0 && node->numEdgeFromStart == numNodes-1) {
			while (spot) {
				if (spot->ID == node->ID) { return 0; }
				spot = spot->next;
			}
			return 1;
		}
		return 0;
	}
//-------------------------------------------------------------------------
	void tracePath(ofstream &outFile) {
		outFile << "Start from node " << start << " using A* search\n";
		int tcost = 0;
		AstarNode *spot = CloseList->next;
		tcost += costMatrix[start - 1][spot->ID - 1];
		outFile << start << "\t" << spot->ID << "\t" << costMatrix[start - 1][spot->ID - 1] << endl;
		while (spot->next) {
			tcost += costMatrix[spot->ID - 1][spot->next->ID - 1];
			outFile << spot->ID << "\t" << spot->next->ID << "\t" << costMatrix[spot->ID - 1][spot->next->ID - 1] << endl;
			spot = spot->next;
		}
		outFile << "The total cost of the simple-path is " << tcost << endl;
	}
};

class TSP {
private:
	int numNodes;
	int startID;
	int whichHFunction;
	AstarSearch *Astar;

public:
	TSP(ifstream &inFile) {
		inFile >> numNodes;
		startID = 0;
		whichHFunction = 0;
		Astar = new AstarSearch(numNodes);
	}
//------------------------------------------------------------------------
	void findPath(ifstream &inFile, ofstream &outFile1, ofstream &outFile2) {
		Astar->findPath(inFile, outFile2);
		Astar->tracePath(outFile1);
	}
//------------------------------------------------------------------------
	void getStartNode() {
		cout << "Enter a Start Node ID: ";
		cin >> startID;
		while (startID <= 0 || startID > numNodes) {
			cout << "Enter a Start Node in range 1-" << numNodes << ": ";
			cin >> startID;
		}
		Astar->start = startID;
	}
//------------------------------------------------------------------------
	void getHFunctionNum() {
		cout << "Which H Function 1-3: ";
		cin >> whichHFunction;
		while (whichHFunction <= 0 || whichHFunction > 3) {
			cout << "Enter a H Function in range 1-3: ";
			cin >> whichHFunction;
		}
		Astar->whichHFunction = whichHFunction;
	}
};

int main(int args, char **argv) {
	ifstream inFile;
	ofstream outFile1, outFile2; //outFile1=result, outFile2=debugging
	TSP *tsp;
	if (args < 3) {
		cout << "ERROR: need 3 files.";
		exit(1);
	}
	inFile.open(argv[1]);
	if (inFile.fail()) {
		cout << "ERROR: cannot find \"" << argv[1] << "\"\n";
		exit(1);
	}
	outFile1.open(argv[2]);
	outFile2.open(argv[3]);

	tsp = new TSP(inFile);
	tsp->getStartNode();
	tsp->getHFunctionNum();
	tsp->findPath(inFile, outFile1, outFile2);

	inFile.close();
	outFile1.close();
	outFile2.close();
}
