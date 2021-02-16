// Longest Common Subsequence
#include <bits/stdc++.h>
#define MaxLen 100
using namespace std;

typedef pair<int, int> Index_pair;
int LCSlen;
// Compute all possible edges at each match position
vector<Index_pair> MatchEdgeVec[MaxLen][MaxLen];
// LCS buffer
char tmp[MaxLen];
// key:LCS, value: number of occurance
map<string, unsigned int> LCS_Map;
void PrintLCS( Index_pair pos, int len, const string& s1);

int main()
{
    // string s1 = "AACDCCFSEDA";
    // string s2 = "ABCDEADFSA";
    string s1,s2;
    getline(cin, s1);
    getline(cin, s2);


    int len1 = s1.length();
    int len2 = s2.length();

    int **Record = new int *[len1+1];
    for (int i=0; i < len1+1; i++){
        Record[i] = new int[len2+1];
    }

    // int maxlen = max(len1,len2);
    // ex: MatchLenVec[1]: (9,6) (9,3) (7,8)
    // 1 represents len in Record, index pair represents match index location
    vector<Index_pair> MatchLenVec[MaxLen+1];
    // Compute all possible edges at each match position
    // vector<Index_pair> MatchEdgeVec[len1][len2];
    // Counting the number of combination to arrive at each match position
    // Create a vector containing len1 vectors of size len2.
    vector<vector<int> > MatchEdgeCount( len1 , vector<int> (len2, 0));

    // Dynamic programming
    for (int i= len1; i>= 0; i--)
    {
        for (int j=len2; j>= 0; j--)
        {
            if( i==len1 || j ==len2){
                // base case, initialize to 0
                Record[i][j] = 0;
            }
            else if (s1[i] == s2[j]){
                // match
                Record[i][j] = Record[i+1][j+1]+1;
                MatchLenVec[ Record[i][j] ].push_back(Index_pair(i,j));
                // MatchLenMap.insert(pair<int,Index_pair>(Record[i][j],Index_pair(i,j)));
            }
            else{
                // not match
                Record[i][j] = max(Record[i+1][j],Record[i][j+1]);
            } // end else

        } // end for
    } // end for

    LCSlen= Record[0][0];


    // Construct Edges(MatchEdgeVec)
    for (int i = 2; i <= LCSlen; i++) {
        for (auto TO: MatchLenVec[i]) {
            for (auto FROM: MatchLenVec[i-1]) {
                // ex: TO:(9,3) FROM: (10,9) (10,5) (10,0) (1,9) (0,9)
                // possible edges: (10,9) -> (9,3)  (10,5) -> (9,3)
                if (TO.first  < FROM.first &&
                    TO.second < FROM.second) {
                    MatchEdgeVec[TO.first][TO.second].push_back(FROM);
                } // end if
            } // end for
        } // end for
    } // end for

    // Counting combinations (MatchEdgeCount)
    // start from length 1 to LCSlen
    for (int len = 1; len <= LCSlen; len++) {
        // calculate combinations at each match position with len
        // ex: (5,2) at MatchLenVec[3]
        // (5,2) came from (8,4) (6,7)
        // (8,4) came from (9,6) (9,6) came from (10,9)
        // (6,7) came from (7,8) (7,8) came from (10,9)
        for (auto TO_loc: MatchLenVec[len]) {
            if (len == 1) {
                MatchEdgeCount[TO_loc.first][TO_loc.second] = 1;
                continue;
            }
            for (auto FROM_loc: MatchEdgeVec[TO_loc.first][TO_loc.second]) {
                MatchEdgeCount[TO_loc.first][TO_loc.second] += MatchEdgeCount[FROM_loc.first][FROM_loc.second];
            }
        }
    }


    int LCSnum = 0;
    for (auto pos: MatchLenVec[LCSlen]) {
        LCSnum += MatchEdgeCount[pos.first][pos.second];
    }
    // printf("%d %d\n", lcslen, lcscnt);
    cout << LCSlen << " " << LCSnum;



    for (auto pos: MatchLenVec[LCSlen]) {
        PrintLCS( pos, 1, s1);
    }

    for (auto key_value: LCS_Map) {
        string LCSstr = key_value.first;
        int count = key_value.second;
        while (count--)
            cout << "\n" << LCSstr;
    }
    return 0;
}


void PrintLCS( Index_pair pos, int len, const string& s1)
{
    if(len == LCSlen){
        string tmp_str(tmp);
        tmp_str.append(s1,pos.first,1);
        LCS_Map[tmp_str]++;
        return;
    }
    for( auto next_pos: MatchEdgeVec[pos.first][pos.second]){
        tmp[len-1] = s1[pos.first];
        PrintLCS(next_pos, len+1, s1);
    }
}