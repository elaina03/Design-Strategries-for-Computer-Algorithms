// 0/1 Knapsack problem(branch and bound)
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>
using namespace std;

typedef struct{
    int value;
    int weight;
} Item;

// Compares two Items according to value/weight ratio.
// Decreasing order
bool CompareItem(Item i1, Item i2)
{
    double ValuePerWeight1 = (double)i1.value/(double)i1.weight;
    double ValuePerWeight2 = (double)i2.value/(double)i2.weight;
    return (ValuePerWeight1 > ValuePerWeight2);
}

class Node{
public:
    int UpperBound, CumulativeProfit, CumulativeWeight;
    int level;
    Node() : UpperBound(0), CumulativeProfit(0), CumulativeWeight(0), level(-1) {}
    Node(int UpperBound, int CumulativeProfit, int CumulativeWeight, int level)
        :  UpperBound(UpperBound), CumulativeProfit(CumulativeProfit),CumulativeWeight(CumulativeWeight), level(level)
    {}
};
// operator overloading
bool operator<(const Node& n1, const Node& n2){
    // return true when second node has greater UpperBound
    // max UpperBound will be at the top(max priority)
    return (n1.UpperBound < n2.UpperBound);

}

// Return maximum profit with constraint MaxWeight (Problem Solution)
int Knapsack(vector<Item> &vec, const int MaxWeight);
// Compute upper bound of profit in the subtree rooted at node
// Greedy method to calculate the upper bound of MaxProfit
int ComputeUpperBound(vector<Item> &vec, const int MaxWeight, Node node);

// MaxProfit: problem answer
int MaxProfit=-1;

int main(int argc, char** argv)
{
    int MaxWeight, ItemNum;
    vector<Item> Problem;

    // ifstream infile(argv[1]);
    // if( !infile){
    //     cerr << "Cannot open the File: "<< argv[1]  << endl;
    //     // return false;
    // }

    // Item temp;
    // if(infile >> MaxWeight >> ItemNum){
    //     for(int i = 0; i < ItemNum; i++){
    //         infile >> temp.value >> temp.weight;
    //         Problem.push_back(temp);
    //     }
    // }

    // read input
    scanf("%d %d", &MaxWeight, &ItemNum);
    Item temp;
    for( int i=0; i < ItemNum; i++){
        cin >> temp.value >> temp.weight;
        Problem.push_back(temp);
    }
    cout << Knapsack(Problem, MaxWeight);
    return 0;
}

int Knapsack(vector<Item> &vec, const int MaxWeight)
{
    // vec: a problem vector
    // MaxWeight: problem constraint
    // MaxProfit: problem answer
    // int MaxProfit=-1;
    int ItemNum = vec.size();

    // sort the Items in decreasing order of value/weight ratio
    sort(vec.begin(), vec.end(), CompareItem);

    //priority queue for traversal
    priority_queue<Node> QueueforVisit;
    // initialize the root
    Node parent,child;
    parent.UpperBound = ComputeUpperBound(vec, MaxWeight, parent);

    QueueforVisit.push(parent);

    while(QueueforVisit.size() > 0){
        // remove the front node from priority queue
        parent = QueueforVisit.top();
        QueueforVisit.pop();

        // If this is the last node( end of problem vector )
        if(parent.level == ItemNum-1) continue;
        // important
        // determine a node has become nonpromising after visiting the node
        if(parent.UpperBound <= MaxProfit) continue;

        // If this is not the last node,
        // consider two cases: adding new item or without new item
        // First case
        child.level = parent.level + 1;
        child.CumulativeProfit = parent.CumulativeProfit + vec[child.level].value;
        child.CumulativeWeight = parent.CumulativeWeight + vec[child.level].weight;

        if(child.CumulativeWeight <= MaxWeight && child.CumulativeProfit > MaxProfit){
            MaxProfit = child.CumulativeProfit;
        }
        child.UpperBound = ComputeUpperBound(vec, MaxWeight, child);

        // If upper bound is greater than MaxProfit
        // add the child node into FIFO queue for traversal
        if(child.UpperBound > MaxProfit) QueueforVisit.push(child);

        // Second case
        child.CumulativeProfit = parent.CumulativeProfit;
        child.CumulativeWeight = parent.CumulativeWeight;
        child.UpperBound = ComputeUpperBound(vec, MaxWeight, child);
        // If upper bound is greater than MaxProfit
        // add the child node into FIFO queue for traversal
        if(child.UpperBound > MaxProfit) QueueforVisit.push(child);
    } // end while
    return MaxProfit;
}

int ComputeUpperBound(vector<Item> &vec, const int MaxWeight, Node node)
{
    int ItemNum = vec.size();
    // If the CumulativeWeight exceeds MaxWeight, not a feasible solution
    if(node.CumulativeWeight > MaxWeight) return 0;
    double UpperBound = node.CumulativeProfit;
    int CumulativeWeight = node.CumulativeWeight;
    // NextLevel: index for next node in vec
    int NextLevel = node.level+1;

    while((NextLevel < ItemNum) && (CumulativeWeight + vec[NextLevel].weight <= MaxWeight)){
        UpperBound += vec[NextLevel].value;
        CumulativeWeight += vec[NextLevel].weight;
        NextLevel++;
    }
    // Update MaxProfit if it is a feasible solution
    if(UpperBound > MaxProfit) MaxProfit = UpperBound;

    if(NextLevel < ItemNum){
        double ValuePerWeight = (double)vec[NextLevel].value/(double)vec[NextLevel].weight;
        UpperBound += (MaxWeight- CumulativeWeight)*ValuePerWeight;
    }
    return (int)UpperBound;
}

