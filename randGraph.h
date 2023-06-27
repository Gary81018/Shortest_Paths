#ifndef RANDGRAPH_H
#define RANDGRAPH_H

#include <bits/stdc++.h>
using namespace std;
const int mod = 30;

map<int, int> f[14];
int n,m,cnt,fa[100015];
int zhao(int xx){
    if(fa[xx]==xx) return xx;
    else return zhao(fa[xx]);
}
void randGraph()
{
    freopen("data.in","w",stdout);
    srand(time(NULL));
    int n=rand()%4+3, m = min(n*(n-1)/2,n+rand()%3);
    cnt=0;
    printf("%d %d\n",n, m);
    for (int i=1;i<=10;i++) f[i].clear();
    for(int i=1;i<=n;i++){
        fa[i]=i;
    }
    while(cnt<n-1){
        int x=rand()%n+1,y=rand()%n+1;
        int x1=zhao(x),y1=zhao(y);
        if(x1!=y1){
            fa[x1]=y1,cnt++;
            int xx = rand() % mod + 1;
            printf("%d %d %d\n",x,y, xx);
            f[x][y] = 1; f[y][x] = 1;
        }
    }
    int cnt = 0;
    m -= (n - 1);
    while(cnt < m){
        int x = rand() % n + 1, y = rand() % n + 1;
        while(x == y || f[x][y] || f[y][x]){
            x = rand() % n + 1, y = rand() % n + 1;
        }
        f[x][y] = 1;
        f[y][x] = 1;
        int xx = rand() % mod + 1;
        cout << x << " " << y << " " << xx << endl;
        cnt++;
    }
    fclose(stdout);
    return;
}

#endif // RANDGRAPH_H
