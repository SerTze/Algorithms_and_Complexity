#include <iostream>
#include <vector>

// define constraint values and a noselection used to indicate the case where
// no selection is used for N sets.
#define N_max 5001
#define M_max 1501
#define noselection 16000000000



using namespace std;
int N, M, A;
unsigned long long P;
vector <pair<int, unsigned long long>> offers[4][4]; //2d vector for offers
vector <unsigned long long> optimal_selection[4][4]; //2d vector for selection
unsigned long long int dp_arr[M_max+1][N_max+1];     //2d array for knapsack approach algorithm

//Knapsack minimum approach using dp to find the selection for N sets with minimum cost
void dp(int seller, int object) {
    int I = offers[seller][object].size();
    int J = N;
    for(int j = 1; j <= J; j++)
        dp_arr[0][j] = noselection;   
    for(int i = 0; i <= I; i++)
        dp_arr[i][0] = 0;
    for(int i = 1; i <= I; i++) {
        int quantity = offers[seller][object][i-1].first;
        int price    = offers[seller][object][i-1].second;
        for(int j = 1; j <= J; j++)
            dp_arr[i][j] = min(dp_arr[i-1][max(0,j-quantity)] + price, dp_arr[i-1][j]);
    }
    for(int j = 0; j <= J; j++)
        optimal_selection[seller][object].push_back(dp_arr[I][j]);
}

int main() {
    cin>>N>>M;
    for(int i = 0; i < M; i++) {
        int seller;
        cin>>seller;
        char equip = getchar();
        int object;
        switch(equip) {
            case 'A': object = 1; break;
            case 'B': object = 2; break;
            case 'C': object = 3; break;
        }
        cin>>A>>P;
        offers[seller][object].push_back(make_pair(A,P));
    }
    for(int i = 1; i <= 3; i++)
        for(int j = 1; j <= 3; j++)
            dp(i,j);
    for(int i = 1; i <= 3; i++)
        for(int j = 0; j <= N; j++) 
            optimal_selection[i][1][j] += optimal_selection[i][2][j] + optimal_selection[i][3][j];
    unsigned long long answer = noselection;
    for(int i = 0; i <= N; i++)
        for(int j = 0; j <= N; j++)
            if(N-i-j >= 0)
                answer = min(answer, optimal_selection[1][1][i] + optimal_selection[2][1][j] + optimal_selection[3][1][N-i-j]);
            else
                answer = min(answer, optimal_selection[1][1][i] + optimal_selection[2][1][j]);
    if(answer == noselection)
        cout<<-1<<endl;
    else
        cout<<answer<<endl;
}