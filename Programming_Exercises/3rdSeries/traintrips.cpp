#include <iostream>
#include <vector>
#include <set>
using namespace std;

int dk_max = 10001;
long int sj_max = 1000000001;
int N, dk = dk_max;
unsigned long Q;
vector<unsigned long> dst, temp(dk_max, -1);
set<pair<unsigned long,int>> lobby;

void Dijkstra() {
    while(!lobby.empty()) {
        pair<unsigned long,int> head = *lobby.begin();
        int sum = head.first;
        unsigned long cur = head.second;
        lobby.erase(head);

        for(auto di : dst) {
            int NewNode = (cur+di) % dk;
            int NewDistance = sum + di;
            if(NewDistance < temp[NewNode] || temp[NewNode] == -1) {
                lobby.erase( make_pair( temp[NewNode], NewNode ) );
                temp[NewNode] = NewDistance;
                lobby.insert( make_pair( temp[NewNode], NewNode ) );
            }
        }
    }
}

int main() {
    cin>>N>>Q;
    for(int i = 1; i <= N; i++) {
        unsigned long di;
        cin>>di;
        dst.push_back(di);
        if(di < dk)
            dk = di;
    }

    temp[0] = 0;
    lobby.insert( make_pair(temp[0],0) );
    Dijkstra();
    
    for(int i = 0; i < Q; i++) {
        unsigned long q;
        cin>>q;
        if(temp[q % dk] > q) {
            cout<<"NO\n";
        }
        else{
            cout<<"YES\n";
        }
    }
}