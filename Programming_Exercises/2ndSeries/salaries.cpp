#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Global definition for constraint values
#define N 200001
#define K 1000000001

//Global initialization, no need to pass this variables into the functions.
int arr[N], modarr[N];
int n,k;

// Use this struct for having the lenght of each subsequence and it's last element
struct subsequence{
    int length;
    int last;
    subsequence() {};
    subsequence(int x, int y) : length(x), last(y){}
};

// Initialize four subsequence arrays which are explained below
subsequence lis[N], lislast[N], lds[N], ldslast[N];



// Function to create longest increasing subsequence struct till given element of
// arr and save it on the global array lis and lislast which contains the lis that
// contains the last element of the current element
// This function's algorithm is based from Geeks for Geeks:
// https://www.geeksforgeeks.org/longest-monotonically-increasing-subsequence-size-n-log-n/
void calculate_lis(){
    vector<int> tail(n,0);
    int length = 0;
    int len;
    int index;
    for(int i = 0; i < n; i++){
        // First we want to find we will use the iterator lower bound function
        // in order to return a iterator to the next >= element using binary search
        auto it = lower_bound(tail.begin(), tail.begin() + length, arr[i]);

        // If not present change the tail element to arr[i]
        if(it == tail.begin() + length){
            tail[length++] = arr[i];
            len = length;
        }
        else{
            *it = arr[i];
            len = distance(tail.begin(), it) + 1;
        }
        subsequence max(length, tail[length - 1]);
        subsequence last(len, arr[i]);
        lis[i] = max;
        lislast[i] = last;
    } 
}

// Similar function with the previous now calculating the lds(longest decreasing subsequence)
// till given element of moddarr for all the elements and save it on lds,also calculates lds 
// containing the last element
void calculate_lds(){
    vector<int> tail(n,0);
    int length = 0;
    int len;
    int index;
    for(int i = 0; i < n; i++){
        // First we want to find we will use the iterator lower bound function
        // in order to return a iterator to the next element using binary search
        // but we use lowerbound with greater because now we use the reversed modarr
        // so greater is essential to calculate the lds on this reversed decreasing array.
        auto it = lower_bound(tail.begin(), tail.begin() + length, modarr[i], greater<int>());

        // If not present change the tail element to arr[i]
        if(it == tail.begin() + length){
            tail[length++] = modarr[i];
            len = length;
        }
        else{
            *it = modarr[i];
            len = distance(tail.begin(), it) + 1;
        }
        subsequence max(length, tail[length - 1]);
        subsequence last(len, modarr[i]);
        lds[i] = max;
        ldslast[i] = last;
    } 
}
// This function provides the solution for our problem,for each element of our array
// it finds the maximum sum between lis or lislast and lds or ldslast, using lis of certain
// index and lds starting from the next index( (n - i - 2) in our case becausee lds is in reversed order)
int findMax(){
    int finalLength = 0;
    for(int i = 0; i < n - 1; i++){
        if(lis[i].last < lds[n - i - 2].last){
            finalLength = max(finalLength, lis[i].length + lds[n - i - 2].length);
        }
        if(lis[i].last < ldslast[n - i - 2].last){
            finalLength = max(finalLength, lis[i].length + ldslast[n - i - 2].length);
        }
        if(lislast[i].last < lds[n - i - 2].last){
            finalLength = max(finalLength, lislast[i].length + lds[n - i - 2].length);
        }
        if(lislast[i].last < ldslast[n - i - 2].last){
            finalLength = max(finalLength, lislast[i].length + ldslast[n - i - 2].length);
        }
    }
    return finalLength; 
}

int main(){
    // Read input
    cin >> n >> k;
    // Read arr
    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    // Modify arr in order to use it in calculate_lds
    for(int i = 0; i < n; i++){
        modarr[i] = arr[n - i - 1] + k;
    }
    // Calculating our subsequences arrays
    calculate_lis();
    calculate_lds();

    // Print the answer through solution function findMax
    cout<< findMax() <<endl;
}