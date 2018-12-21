#include<stdio.h>
#include<bits/stdc++.h>
using namespace std;
#define _p 2147483647ll
#define _q 1000000007ll
#define _q1 37
#define _q2 31
#define _q3 19
#define PB push_back
#define MP make_pair

#define MAXL 15
typedef long long int ll;
char C[MAXL+1][MAXL+1];
bool B[MAXL+2][MAXL+2];
bool dead[MAXL+2][MAXL+2];
ll pow1[MAXL+2][MAXL+2];
ll pow2[MAXL+2][MAXL+2];
int n,m;
struct coord{
    int x,y;
    coord(){};
    coord(int _x,int _y){
        x=_x;y=_y;
    }
};
vector<coord>target;
coord dirs[4]={coord(1,0),coord(-1,0),coord(0,1),coord(0,-1)};
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
bool BC(coord c){
    return B[c.x][c.y];
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
    void print(){
        printf("%d %d\n",player.x,player.y);
        for(int i=0;i<box.size();i++){
            printf("%d: %d %d\n",i,box[i].x,box[i].y);
        }
    }
};
struct statusHasher{
    size_t operator()(const status& s)const{
        bool rc[MAXL+2][MAXL+2];
        bool bb[MAXL+2][MAXL+2];
        for(int i=0;i<n+2;i++){
            for(int j=0;j<m+2;j++){
                bb[i][j]=B[i][j];
                rc[i][j]=false;
            }
        }
        for(int i=0;i<s.box.size();i++)bb[s.box[i].x][s.box[i].y]=false;
        rc[s.player.x][s.player.y]=true;
        queue<coord>q;
        q.push(s.player);
        while(!q.empty()){
            coord p = q.front();
            q.pop();
            for(int d=0;d<4;d++){
                coord pd=p+dirs[d];
                if(!rc[pd.x][pd.y] && bb[pd.x][pd.y]){
                    rc[pd.x][pd.y]=true;
                    q.push(pd);
                }
            }
        }
        ll hs=0ll;
        coord p;
        for(int i=0;i<n+2;i++)for(int j=0;j<m+2;j++)
            if(rc[i][j]){
                p=coord(i,j);
                break;
            }
        p=s.player;
        hs+=pow1[p.x][p.y];
        for(int i=0;i<s.box.size();i++){
            hs+=pow2[s.box[i].x][s.box[i].y];
            hs%=_p;
        }
        return hs;
    }
};
bool operator < (const status s1, const status s2){
    return s1.player<s2.player;
}
bool operator == (const status& s1, const status &s2){
    for(int i=0;i<s1.box.size();i++)if(!(s1.box[i]==s2.box[i]))return false;
    return true;
}
status move(status s,int box, coord dir, int& step){
    status s1=s;
    int dist[MAXL+2][MAXL+2];
    bool bb[MAXL+2][MAXL+2];
    for(int i=0;i<n+2;i++){
        for(int j=0;j<m+2;j++){
            bb[i][j]=B[i][j];
            dist[i][j]=(int)1e9;
        }
    }
    for(int i=0;i<s.box.size();i++)bb[s.box[i].x][s.box[i].y]=false;
    dist[s.player.x][s.player.y]=0;
    queue<coord>q;
    q.push(s.player);
    while(!q.empty()){
        coord p = q.front();
        q.pop();
        for(int d=0;d<4;d++){
            coord pd=p+dirs[d];
            if(dist[pd.x][pd.y]>(int)1e8 && bb[pd.x][pd.y]){
                dist[pd.x][pd.y]=dist[p.x][p.y]+1;
                q.push(pd);
            }
        }
    }
    /*for(int i=0;i<n+2;i++){
        for(int j=0;j<n+2;j++){
            if(dist[i][j]>(int)1e8)printf("-1 ");
            else printf("%d  ",dist[i][j]);
        }
        puts("");
    }*/
    coord pb=s.box[box]-dir;
    if(dist[pb.x][pb.y]>(int)1e8)
        throw "";
    s1.player=s.box[box];
    s1.box[box]=s.box[box]+dir;
    if(dead[s1.box[box].x][s1.box[box].y])throw "";
    if(!bb[s1.box[box].x][s1.box[box].y])
        throw "";
    step=dist[pb.x][pb.y];
    return s1;
}
status unmove(status s,int box, coord dir,int& step){
    status s1=s;
    int dist[MAXL+2][MAXL+2];
    bool bb[MAXL+2][MAXL+2];
    for(int i=0;i<n+2;i++){
        for(int j=0;j<m+2;j++){
            bb[i][j]=B[i][j];
            dist[i][j]=(int)1e9;
        }
    }
    for(int i=0;i<s.box.size();i++)bb[s.box[i].x][s.box[i].y]=false;
    dist[s.player.x][s.player.y]=0;
    queue<coord>q;
    q.push(s.player);
    while(!q.empty()){
        coord p = q.front();
        q.pop();
        for(int d=0;d<4;d++){
            coord pd=p+dirs[d];
            if(dist[pd.x][pd.y]>(int)1e8 && bb[pd.x][pd.y]){
                dist[pd.x][pd.y]=min(dist[pd.x][pd.y],dist[p.x][p.y]+1);
                q.push(pd);
            }
        }
    }
    /*for(int i=0;i<n+2;i++){
        for(int j=0;j<n+2;j++){
            if(dist[i][j]>(int)1e8)printf("-1 ");
            else printf("%d  ",dist[i][j]);
        }
        puts("");
    }*/
    coord pb=s.box[box]-dir;
    if(dist[pb.x][pb.y]>(int)1e8)
        throw "";
    s1.player=s.box[box]-dir-dir;
    s1.box[box]=s.box[box]-dir;
    if(!bb[s1.player.x][s1.player.y])
        throw "";
    step=dist[pb.x][pb.y]+1;
    return s1;
}
bool solved(status s){
    ll h1=0ll,h2=0ll;
    for(int i=0;i<s.box.size();i++){
        h1+=pow1[s.box[i].x][s.box[i].y];
        h2+=pow1[target[i].x][target[i].y];
    }
    return h1==h2;
}

int main(){
    ll h1=_q1,h2=_q2;
    for(int i=0;i<MAXL+2;i++)for(int j=0;j<MAXL+2;j++){
        pow1[i][j]=h1;
        pow2[i][j]=h2;
        h1*=_q1;
        h1%=_p;
        h2*=_q2;
        h2%=_p;
    }
    while(scanf("%d%d",&n,&m)!=EOF){
        target.clear();
        status S;
        unordered_map<status, status, statusHasher>my_map;
        unordered_map<status, status, statusHasher>my_unmap;
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
        calcDead();
        priority_queue<pair<int,status>>Q;
        priority_queue<pair<int,status>>unQ;
        Q.push(MP(0,S));
        status unS;
        unS.box=target;
        for(int i=0;i<target.size();i++){
            for(int d=0;d<4;d++){
                unS.player=target[i]+dirs[d];
                if(!BC(unS.player))
                    continue;
                unQ.push(MP(0,unS));
            }
        }
        bool fin=false;
        status T;
        int size=1;
        while(true){
            //if(Q.size()<unQ.size()){
                int base_step=Q.top().first;
                status s1=Q.top().second;
                Q.pop();
                size--;
                for(int b=0;b<s1.box.size();b++){
                    for(int d=0;d<4;d++){
                        try{
                            int step;
                            status s2=move(s1,b,dirs[d],step);
                            step*=-1;
                            step+=base_step;
                            sort(s2.box.begin(),s2.box.end());
                            if(my_map.find(s2)==my_map.end()){
                                my_map[s2]=s1;
                                if(solved(s2)||my_unmap.find(s2)!=my_unmap.end()){
                                    printf("%d\n",step);
                                    fin=true;
                                    T=s2;
                                    break;
                                }
                                Q.push(MP(step,s2));
                                size++;
                            }
                        }
                        catch(...){}
                    }
                }
                if(fin)break;
            //}
            /*else{
                int base_step=unQ.top().first;
                status s1=unQ.top().second;
                unQ.pop();
                size--;
                for(int b=0;b<s1.box.size();b++){
                    for(int d=0;d<4;d++){
                        try{
                            int step;
                            status s2=unmove(s1,b,dirs[d],step);
                            step*=-1;
                            step+=base_step;
                            printf("%d\n",step);
                            sort(s2.box.begin(),s2.box.end());
                            if(my_unmap.find(s2)==my_unmap.end()){
                                my_map[s2]=s1;
                                if(my_map.find(s2)!=my_map.end()){
                                    fin=true;
                                    T=s2;
                                    break;
                                }
                                unQ.push(MP(step,s2));
                                size++;
                            }
                        }
                        catch(...){}
                    }
                }
                if(fin)break;
            }*/
        }
        
        /*status T1=T;
        T.print();
        while(!(T==S)){
            T.print();
            T=my_unmap[T];
        }
        puts("=======");
        while(!solved(T1)){
            T1.print();
            T1=my_map[T1];
        }*/
        puts("!");
    }
}
