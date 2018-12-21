#include<stdio.h>
#include<bits/stdc++.h>
using namespace std;
#define _p 2147483647ll
#define _q 1000000007ll
#define _q1 37ll
#define _q2 31ll
#define _q3 19ll
#define PB push_back
#define MP make_pair

#define MAXL 15
typedef long long int ll;
bool dead[MAXL+2][MAXL+2];
char C[MAXL+1][MAXL+1];
bool B[MAXL+2][MAXL+2];
ll pow1[MAXL+2][MAXL+2];
ll pow2[MAXL+2][MAXL+2];
char sol[(int)1e5];
int n,m;
struct coord{
    int x,y;
    coord(){};
    coord(int _x,int _y){
        x=_x;y=_y;
    }
};
bool BC(coord c){
    return B[c.x][c.y];
}
coord dirs[4]={coord(1,0),coord(-1,0),coord(0,1),coord(0,-1)};
char cirs[4]={'d','u','r','l'};
vector<coord>target;
coord operator + (const coord&c1, const coord &c2){
    return coord(c1.x+c2.x,c1.y+c2.y);
}
coord operator - (const coord&c1, const coord &c2){
    return coord(c1.x-c2.x,c1.y-c2.y);
}
int operator ^ (const coord&c1, const coord &c2){
    return abs(c1.x-c2.x)+abs(c1.y-c2.y);
}
bool operator < (const coord&c1, const coord &c2){
    if(c1.x!=c2.x)return c1.x<c2.x;
    return c1.y<c2.y;
}
bool operator == (const coord& c1, const coord &c2){
    return c1.x==c2.x&&c1.y==c2.y;
}
void calcDead(void){
    for(int i=0;i<MAXL+2;i++)for(int j=0;j<MAXL+2;j++)dead[i][j]=true;
    queue<coord>q;
    for(int i=0;i<target.size();i++){
        dead[target[i].x][target[i].y]=false;
        q.push(target[i]);
    }
    while(!q.empty()){
        coord p=q.front();
        q.pop();
        for(int d=0;d<4;d++){
            coord pd=p+dirs[d];
            coord pd1=pd+dirs[d];
            if(dead[pd.x][pd.y]&&BC(pd)&&BC(pd1)){
                dead[pd.x][pd.y]=false;
                q.push(pd);
            }
        }
    }
}
struct status{
    coord player;
    vector<coord>box;
    status(){};
    int heur()const{
        vector<coord>box1=box;
        int num=1;
        for(int i=1;i<=box1.size();i++)num*=i;
        int h=(int)1e9;
        while(num--){
            int hh=0;
            for(int i=0;i<box1.size();i++)
                hh+=box1[i]^target[i];
            h=min(h,hh);
            next_permutation(box1.begin(),box1.end());
        }
        return h;
    }
    void print(){
        printf("%d %d\n",player.x,player.y);
        for(int i=0;i<box.size();i++){
            printf("%d: %d %d\n",i,box[i].x,box[i].y);
        }
    }
};
struct statusHasher{
    size_t operator()(const status& s)const{
        ll hs=0ll;
        hs+=pow1[s.player.x][s.player.y];
        for(int i=0;i<s.box.size();i++){
            hs+=pow2[s.box[i].x][s.box[i].y];
            if(hs>=_p)
                hs-=_p;
        }
        return hs;
    }
};
bool operator < (const status &s1, const status &s2){
    return s1.heur()<s2.heur();
}
bool operator == (const status& s1, const status &s2){
    if(!(s1.player==s2.player))return false;
    for(int i=0;i<s1.box.size();i++)if(!(s1.box[i]==s2.box[i]))return false;
    return true;
}
bool lock(status &s){
    bool bb[MAXL+2][MAXL+2];
    for(int i=0;i<n+2;i++)for(int j=0;j<m+2;j++)bb[i][j]=B[i][j];
    for(int i=0;i<s.box.size();i++)
        bb[s.box[i].x][s.box[i].y]=false;
    int times=s.box.size();
    int num=s.box.size();
    while(times--){
        for(int i=0;i<s.box.size();i++){
            if(bb[s.box[i].x][s.box[i].y])
                continue;
            for(int d=0;d<4;d++){
                coord bs=s.box[i]+dirs[d];
                coord bs1=s.box[i]-dirs[d];
                if(dead[bs.x][bs.y])continue;
                if(bb[bs.x][bs.y]&&bb[bs1.x][bs1.y])bb[s.box[i].x][s.box[i].y]=true;
            }
        }
    }
    for(int i=0;i<target.size();i++)bb[target[i].x][target[i].y]=true;
    for(int i=0;i<s.box.size();i++)if(bb[s.box[i].x][s.box[i].y]==false)return true;
    return false;


}
status move(status &s,coord &dir){
    status s1=s;
    s1.player=s1.player+dir;
    if(!B[s1.player.x][s1.player.y])
        throw "";
    for(int i=0;i<s1.box.size();i++){
        if(s1.box[i]==s1.player){
            s1.box[i]=s1.box[i]+dir;
            if(!B[s1.box[i].x][s1.box[i].y])
                throw "";
            if(dead[s1.box[i].x][s1.box[i].y])
                throw "";
            for(int j=0;j<s.box.size();j++){
                if(i==j)continue;
                if(s1.box[i]==s1.box[j])
                    throw "";
            }
        }
    }
    if(lock(s1))throw "";
    return s1;
}
status unmove(status &s,coord &dir,bool box){
    status s1=s;
    if(box){
        for(int i=0;i<s1.box.size();i++)
            if(s1.box[i]-dir==s1.player){
                s1.box[i]=s1.box[i]-dir;
                box=false;
            }
    }
    if(box)throw "";
    s1.player=s1.player-dir;
    if(!B[s1.player.x][s1.player.y])
        throw "";
    for(int i=0;i<s1.box.size();i++){
        if(s1.box[i]==s1.player){
            throw "";
        }
    }
    return s1;
}
bool solved(status &s){
    ll h1=0ll,h2=0ll;
    for(int i=0;i<s.box.size();i++){
        h1+=pow1[s.box[i].x][s.box[i].y];
        h2+=pow1[target[i].x][target[i].y];
        if(h1>=_p)h1-=_p;
        if(h2>=_p)h2-=_p;
    }
    return h1==h2;
}

int main(){
    for(int i=0;i<MAXL+2;i++)for(int j=0;j<MAXL+2;j++)B[i][j]=true;
    ll h1=_q1,h2=_q2;
    for(int i=0;i<MAXL+2;i++)for(int j=0;j<MAXL+2;j++){
        pow1[i][j]=h1;
        pow2[i][j]=h2;
        h1*=_q1;
        h1%=_p;
        h2*=_q2;
        h2%=_p;
    }
    int cnt=0;
    while(scanf("%d%d",&n,&m)!=EOF){
        cnt++;
        if(cnt==10)return 0;
        target.clear();
        status S;
        unordered_map<status, pair<char,status>, statusHasher>my_map;
        unordered_map<status, pair<char,status>, statusHasher>my_unmap;
        for(int i=0;i<MAXL+2;i++)for(int j=0;j<MAXL+2;j++)B[i][j]=false;
        for(int i=0;i<n;i++){
            scanf("%s",C[i]);
            for(int j=0;j<m;j++){
                B[i+1][j+1]=true;
                switch(C[i][j]){
                    case '#':
                        B[i+1][j+1]=false;
                        break;
                    case '@':
                        S.player=coord(i+1,j+1);
                        break;
                    case '+':
                        S.player=coord(i+1,j+1);
                        target.PB(coord(i+1,j+1));
                        break;
                    case '$':
                        S.box.PB(coord(i+1,j+1));
                        break;
                    case '*':
                        S.box.PB(coord(i+1,j+1));
                        target.PB(coord(i+1,j+1));
                        break;
                    case '.':
                        target.PB(coord(i+1,j+1));
                        break;
                }
            }
        }
        sort(target.begin(),target.end());
        calcDead();
        queue<status>Q;
        queue<status>unQ;
        sort(S.box.begin(),S.box.end());
        Q.push(S);
        status unS;
        unS.box=target;
        for(int i=0;i<target.size();i++){
            for(int d=0;d<4;d++){
                unS.player=target[i]+dirs[d];
                if(!BC(unS.player))
                    continue;
                unQ.push(unS);
            }
        }
        bool fin=false;
        status T;
        int num_s=1;
        while(true){
            num_s++;
            if(num_s>(int)1e6)
                break;
                status s1=Q.front();
                Q.pop();
                for(int d=0;d<4;d++){
                    try{
                        status s2=move(s1,dirs[d]);
                        sort(s2.box.begin(),s2.box.end());
                        if(my_map.find(s2)==my_map.end()){
                            my_map[s2]=MP(cirs[d],s1);
                            Q.push(s2);
                            if(solved(s2) || my_unmap.find(s2)!=my_unmap.end()){
                                fin=true;
                                T=s2;
                                break;
                            }
                        }
                    }
                    catch(...){}
                }
                if(fin)break;
            }
        printf("%d\n",num_s);
        if(fin){
            int num=0;
            while(!(T==S)){
                sol[num]=my_map[T].first;
                T=my_map[T].second;
                num++;
            }
            printf("%d\n",num);
            sol[num]='\0';
            for(int i=num-1;i>=0;i--)printf("%c",sol[i]);
            puts("");
        }
        else{
            puts("Unsolved");
        }
    }
}
