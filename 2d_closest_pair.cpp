// 2-dimensional closet pair problem
#include <bits/stdc++.h>
#include <math.h>
using namespace std;

const double eps = 1e-9;
double MinDist= numeric_limits<double>::max();
int AnsCountIndex=0;
typedef pair<int, int> Index_pair;
typedef struct{
    int x,y;
    int index;
} Point;

// Problem set
vector<Point> PointVec;
// temp vector
vector<Point> tempVec(250000);
// candidates for merge
vector<Point> candVec;
// Record the pair of point indices that are closet
vector<Index_pair> Record(500000);

// Compares two Points according to their x coordinate.
// Ascending order
bool ComparePoint(Point p1, Point p2)
{
    if(p1.x != p2.x) return p1.x < p2.x;
    // if x are the same, compare y coordinate, ascending order
    if(p1.y != p2.y) return p1.y < p2.y;
    // same point
    return true;
}

// Compares two Points according to their y coordinate.
// Ascending order
bool ComparePoint_y(Point p1, Point p2)
{
    if(p1.y != p2.y) return p1.y < p2.y;
    // if y are the same, compare x coordinate, ascending order
    if(p1.x != p2.x) return p1.x < p2.x;
    // same point
    return true;
}

bool CompareIndex_pair(Index_pair p1, Index_pair p2)
{
    if(p1.first != p2.first) return p1.first < p2.first;
    if(p1.second != p2.second) return p1.second < p2.second;
    return false;
}

void Add_Record(Point p1, Point p2);

void Test_If_MinDist(Point p1, Point p2);

void Divide_and_Conquer(int left, int right);

int main(int argc, char** argv)
{
    int Num;
    Point point;
    cin >> Num;
    for(int i =0; i < Num; i++){
        cin >> point.x >> point.y;
        point.index = i+1;
        PointVec.push_back(point);
    }


    // ifstream infile(argv[1]);
    // if( !infile){
    //     cerr << "Cannot open the File: "<< argv[1]  << endl;
    //     // return false;
    // }
    // if( infile >> Num){
    //     for(int i=0; i < Num; i++){
    //         infile >> point.x >> point.y;
    //         point.index = i+1;
    //         PointVec.push_back(point);
    //     }
    // }

    // order the points in PointVec
    sort(PointVec.begin(), PointVec.end(), ComparePoint);
    // for(auto p: PointVec){
    //     cout << "["<<p.x << "," << p.y << "]";
    // }
    // cout << endl;

    Divide_and_Conquer(0,Num);
    unsigned long long MinDist_ans = pow(MinDist,2);
    vector<Index_pair> Answer(Record.begin(),Record.begin()+AnsCountIndex);

    sort(Answer.begin(),Answer.end());
    Answer.resize(distance(Answer.begin(),unique(Answer.begin(),Answer.end())));
    AnsCountIndex = Answer.size();
    cout << MinDist_ans << " " << AnsCountIndex;
    for(int i=0; i < AnsCountIndex; i++){
        cout << "\n";
        cout << Answer[i].first << " " << Answer[i].second;
    };
    return 0;
}


void Add_Record(Point p1, Point p2){
    // compare indices of two points(input order), smaller first
    int index1 = p1.index < p2.index ? p1.index : p2.index;
    int index2 = p1.index < p2.index ? p2.index : p1.index;
    Record.at(AnsCountIndex) = Index_pair(index1,index2);
    AnsCountIndex++;
} // end Add_Record

void Test_If_MinDist(Point p1, Point p2){
    // squared euclidean distance
    // double squared_dist = Squared_Euclidean_Distance(p1, p2);
    double squared_dist = pow(p2.x-p1.x, 2.0) + pow(p2.y-p1.y, 2.0);
    double dist = sqrt(squared_dist);
    if( dist < (MinDist - eps) ){
        MinDist = dist;
        // reassign the AnsCountIndex value to zero
        AnsCountIndex = 0;
        // cout << "Add index " << AnsCountIndex <<": " << squared_dist << endl;
        Add_Record(p1,p2);
        // for(int i=0; i < AnsCountIndex; i++){
        //     cout << Record[i].first << " " << Record[i].second << endl;
        // }
    }
    else if( abs(dist - MinDist) < eps ){
        // cout << "Add index " << AnsCountIndex <<": " << squared_dist << endl;
        Add_Record(p1,p2);
        // for(int i=0; i < AnsCountIndex; i++){
        //     cout << Record[i].first << " " << Record[i].second << endl;
        // }
    } // end else if
    return;
} // end Test_If_MinDist

// left : left boundary(include), right: right boundary(exclude)
void Divide_and_Conquer(int left, int right){
    // only less than or equal to 3 elements
    if((right-left) <= 3){
        if((right-left) == 3){
            Test_If_MinDist(PointVec[left], PointVec[left+1]);
            Test_If_MinDist(PointVec[left], PointVec[left+2]);
            Test_If_MinDist(PointVec[left+1], PointVec[left+2]);
        } // end 3 elements
        else if ((right-left) == 2){
            Test_If_MinDist(PointVec[left], PointVec[left+1]);
        }
        // sorting in y order
        sort(PointVec.begin()+left, PointVec.begin()+right, ComparePoint_y);
        return;
    } // end <= 3 elements

    int middle = (left + right)/2;
    double x_middle = PointVec[middle].x;

    Divide_and_Conquer(left,middle);
    Divide_and_Conquer(middle, right);

    double delta = MinDist;
    int left_idx = left;
    int right_idx = middle;
    int idx = left;

    candVec.clear();
    while( left_idx < middle || right_idx < right){
        // consider two boundary cases
        if(left_idx == middle){
            if(PointVec[right_idx].x >= (x_middle-delta-eps) && PointVec[right_idx].x <= (x_middle+delta+eps) ){
                candVec.push_back(PointVec[right_idx]);
            }
            tempVec[idx++] = PointVec[right_idx++];
        }else if(right_idx == right){
            if(PointVec[left_idx].x >= (x_middle-delta-eps) && PointVec[left_idx].x <= (x_middle+delta+eps) ){
                candVec.push_back(PointVec[left_idx]);
            }
            tempVec[idx++] = PointVec[left_idx++];
        }else{
            // general case
            // consider y values of left and right point
            if(PointVec[left_idx].y < PointVec[right_idx].y){
                // check left point
                if(PointVec[left_idx].x >= (x_middle-delta-eps) && PointVec[left_idx].x <= (x_middle+delta+eps) ){
                    candVec.push_back(PointVec[left_idx]);
                }
                tempVec[idx++] = PointVec[left_idx++];
            }else{
                // check right point
                if(PointVec[right_idx].x >= (x_middle-delta-eps) && PointVec[right_idx].x <= (x_middle+delta+eps) ){
                    candVec.push_back(PointVec[right_idx]);
                }
                tempVec[idx++] = PointVec[right_idx++];
            } // end else
        } // end general case
    } // end while

    for(int i = left; i < right; i++){
        PointVec[i] = tempVec[i];
    }
    for(int i=0; i < candVec.size(); i++){
        // check only 7 points
        for(int j = i+1; j < candVec.size()&& j < (i+8); j++){
            Test_If_MinDist(candVec[i],candVec[j]);
        } // end for
    }// end for
    return;
} // end Divide_and_Conquer