/*
g++ test1717.cpp -o test1717.exe 후 F5 실행 
*/
#include <iostream>
using namespace std;

int parent[1000001];
int sz[1000001]; // 각 집합의 크기

int find(int x){
    if(parent[x]==x) return x;
    return parent[x]=find(parent[x]); // 경로 압축
}

void unite(int a,int b){
    a=find(a);
    b=find(b);
    if(a==b) return;

    // 항상 작은 트리를 큰 트리 밑에 붙임
    if(sz[a] < sz[b]) swap(a,b);

    parent[b]=a;
    sz[a]+=sz[b];
}

bool same(int a,int b){
    return find(a)==find(b);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);

    int n,m;
    cin >> n >> m;

    for(int i=0;i<=n;i++){
        parent[i]=i;
        sz[i]=1; // 초기 집합 크기 1
    }

    for(int i=0;i<m;i++){
        int op,a,b;
        cin >> op >> a >> b;
        if(op==0){
            unite(a,b);
        } else {
            cout << (same(a,b) ? "YES" : "NO") << '\n';
        }
    }

    return 0;
}
