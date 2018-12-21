#include<bits/stdc++.h>
using namespace std;
typedef long long int ll;
typedef bitset<100> bd;
typedef double db;
#define win first
#define los second 
unordered_map<bd,pair<db,db>> trans;
const db INF = 1e10;
bd mask1, mask2;
bd turn(bd h){
    return (h>>25ll)|(h<<75ll);
}
int end(bd h){
    if((h&mask1).none())return 2;
    if((h&mask2).none())return 1;
    if(h[24])return 1;
    if(h[49])return 2;
    if(h[74])return 1;
    if(h[99])return 2;
    return 0;
}
void move(bd h, bd* lst){
    if(end(h)){
        lst[0]=bd(0);
        return;
    }
    bd h1 = h & mask1;
    h1 = (h1 >> 25) | ((h1 << 75) >> 75);
    int idx = 0;
    for(int i=0;i<25;i++){
        if(!h[i])continue;
        if((i+1)%5)
            if(!h1[i+1]){
                lst[idx]=h;
                lst[idx][i]=0;
                lst[idx][i+1]=1;
                lst[idx][25+i+1]=0;
                lst[idx][75+i+1]=0;
                lst[idx]=turn(lst[idx]);
                idx++;
            }
        if(i<20)
            if(!(h1[i+5])){
                lst[idx]=h;
                lst[idx][i]=0;
                lst[idx][i+5]=1;
                lst[idx][25+i+5]=0;
                lst[idx][75+i+5]=0;
                lst[idx]=turn(lst[idx]);
                idx++;
            }
        if((i+1)%5 && i<20)
            if(!(h1[i+6])){
                lst[idx]=h;
                lst[idx][i]=0;
                lst[idx][i+6]=1;
                lst[idx][25+i+6]=0;
                lst[idx][75+i+6]=0;
                lst[idx]=turn(lst[idx]);
                idx++;
            }
    }
    lst[idx]=bd(0);
    return;
}
int mcts(bd h,db c){
    if(end(h)){
        if(end(h)==1){
            trans[h].first++;
            return 1;
        }
        else{
            trans[h].second++;
            return 2;
        }
    }
    bd lst[12];
    move(h,lst);
    int idx = 0;
    bd g = bd(0);
    db score = -INF;
    pair<int,int>s=trans[h];
    while(lst[idx].any()){
        pair<int,int>t=trans[lst[idx]];
        db rate = 0.0;
        if(t.win+t.los)
            rate = t.los/(t.win+t.los);
        db reg = 0.0;
        if(s.win+s.los){
            if(t.win+t.los)
                reg = c * sqrt(log(s.win+s.los)/(t.win+t.los));
            else reg = INF;
        }
        if(score<reg+rate){
            g=lst[idx];
            score = reg+rate;
        }
        idx++;
    }
    int res = 3-mcts(g,c);
    if(res==1)trans[h].first++;
    else trans[h].second++;
    return res;
}
bd dcs(bd h){
    bd lst[12];
    move(h,lst);
    int idx=0;
    bd g = bd(0);
    db rate = -INF;
    while(lst[idx].any()){
        if(rate<trans[lst[idx]].los/(trans[lst[idx]].win+trans[lst[idx]].los)){
            g=lst[idx];
            rate=trans[lst[idx]].los/(trans[lst[idx]].win+trans[lst[idx]].los);
        }
    }
    return g;
}
int init[6]={0,1,2,5,6,10}
int main(){
    char c;
    scanf("%c",&c);
    int board[25];
    char odr[10];
    scanf("%s",odr);
    for(int i=0;i<6;i++)board[init[i]]=ord[i]-'0';
    for(int i=0;i<6;i++)board[24-init[i]]=ord[i]-'0';
}
