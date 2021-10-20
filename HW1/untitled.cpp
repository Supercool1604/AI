#include<bits/stdc++.h>
using namespace std;

// bool sortByFinishTime(vector<int>& v1, vector<int>& v2)
// {
//     return v1[1]<v2[1];
// }
int solve(vector<vector<int> > &A) {
    int count=1;
    priority_queue<int, vector<int>, greater<int> > pq;
    int n = A.size();
    cout<<n<<endl;
    sort(A.begin(),A.end());
    pq.push(A[0][1]);
    int i=1;
    while(i<A.size())
    {
        // int j=i+1;

        if(A[i][0] < pq.top()) {
            pq.push(A[i][1]);
            count++;
        }
        else{
            pq.pop();
            pq.push(A[i][1]);
        }
        i++;
    }
    cout<<count<<endl;
    return pq.size();
}
int main()
{
	vector< vector<int> > vect{{1, 18},
            {18, 23},
            {15, 29},
            {4, 15},
            {2, 11},
            {5, 13}};
    cout<<solve(vect);
}