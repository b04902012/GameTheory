#include<bits/stdc++.h>
#include<signal.h>
#include<unistd.h>
using namespace std;
typedef long long int ll;
typedef bitset<100> bd;
typedef double db;
#define win first
#define los second 
unordered_map<bd,pair<db,db>> trans;
const db INF = 1e10;
bd mask;
int self;
int phase;
bool timeup;
void alarm_func(int sig){
    timeup=true;
}
bd encode(int* board){
    bd h = bd(0);
    for(int t=0;t<4;t++){
        int tmp_phase = (phase + t)%4;
        if(tmp_phase%2 == self){
            for(int i=0;i<25;i++){
                if(!board[i])continue;
                if(board[i]>10)continue;
                if(board[i]%2 == tmp_phase/2){
                    h[i+25*t]=1;
                }
            }
        }
        else{
            for(int i=0;i<25;i++){
                if(!board[24-i])continue;
                if(board[24-i]<10)continue;
                if(board[24-i]%2 == tmp_phase/2){
                    h[i+25*t]=1;
                }
            }
        }
    }
    return h;
}
bd turn(bd h){
    return (h>>25)|(h<<75);
};

int end(bd h){
    if((h&mask).none()){
        return 2;
    }
    if((h&(~mask)).none()){
        return 1;
    }
    if(h[24]||h[74]){
        return 1;
    }
    if(h[49]||h[99]){
        return 2;
    }
    return 0;
}
void move(bd h, bd* lst){
    if(end(h)){
        lst[0]=bd(0);
        return;
    }
    bd h1 = h;
    h1 = ((h1 << 75) >> 75) | ((h1 << 25) >> 75);
    int idx = 0;
    for(int i=0;i<25;i++){
        if(!h[i])continue;
        if((i+1)%5)
            if(!h1[i+1]){
                lst[idx]=h;
                lst[idx][i]=0;
                lst[idx][i+1]=1;
                lst[idx][25+(24-i-1)]=0;
                lst[idx][75+(24-i-1)]=0;
                lst[idx]=turn(lst[idx]);
                idx++;
            }
        if(i<20)
            if(!(h1[i+5])){
                lst[idx]=h;
                lst[idx][i]=0;
                lst[idx][i+5]=1;
                lst[idx][25+(24-i-5)]=0;
                lst[idx][75+(24-i-5)]=0;
                lst[idx]=turn(lst[idx]);
                idx++;
            }
        if((i+1)%5 && i<20)
            if(!(h1[i+6])){
                lst[idx]=h;
                lst[idx][i]=0;
                lst[idx][i+6]=1;
                lst[idx][25+(24-i-6)]=0;
                lst[idx][75+(24-i-6)]=0;
                lst[idx]=turn(lst[idx]);
                idx++;
            }
    }
    lst[idx]=bd(0);
    return;
}
int mcts(bd h,db c){
    if(trans.find(h)==trans.end())trans[h]=make_pair(0.0,0.0);
    if(end(h)){
        if(end(h)==1){
            trans[h].win+=1.0;
            return 1;
        }
        else{
            trans[h].los+=1.0;
            return 2;
        }
    }
    bd lst[12];
    move(h,lst);
    int idx = 0;
    bd g = turn(h);
    db score = -INF;
    pair<db,db>s=trans[h];
    while(lst[idx].any()){
        pair<db,db>t=trans[lst[idx]];
        db rate = 0.5;
        if(t.win+t.los>0.5)
            rate = t.los/(t.win+t.los);
        db reg = 0.0;
        if(s.win+s.los>0.5){
            if(t.win+t.los>0.5)
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
    if(res==1)trans[h].win+=1.0;
    else trans[h].los+=1.0;
    return res;
}
int move_by_encode(bd h, bd newh, int* board){
    newh = (newh<<25)|(newh>>75);
    if(newh==h)return 0;
    int ori_pos=0;
    int new_pos=0;
    for(int i=0;i<25;i++){
        if(h[i] & ~newh[i]){
            ori_pos=i;
        }
        if(~h[i] & newh[i]){
            new_pos=i;
        }
    }
    if(phase%2 != self){
        ori_pos = 24 - ori_pos;
        new_pos = 24 - new_pos;
    } 
    board[new_pos]=board[ori_pos];
    board[ori_pos]=0;
    int res = board[new_pos]*10;
    if(abs(new_pos-ori_pos)==1)res+=1;
    if(abs(new_pos-ori_pos)==5)res+=2;
    if(abs(new_pos-ori_pos)==6)res+=3;
    return res;
}
bd dcs(bd h,int* board){
    bd lst[12];
    move(h,lst);
    int idx=0;
    bd g = turn(h);
    db rate = -INF;
    while(lst[idx].any()){
        int b[25];
        for(int i=0;i<25;i++)b[i]=board[i];
        fprintf(stderr,"%d: ",move_by_encode(h,lst[idx],b));
        fprintf(stderr,"%f\%\n",trans[lst[idx]].los/(trans[lst[idx]].win+trans[lst[idx]].los)*100.0);
        if(rate<trans[lst[idx]].los/(trans[lst[idx]].win+trans[lst[idx]].los)){
            g=lst[idx];
            rate=trans[lst[idx]].los/(trans[lst[idx]].win+trans[lst[idx]].los);
            
        }
        idx++;
    }
    return g;
}
int init[6]={0,1,2,5,6,10};
int main(){
    signal(SIGALRM, alarm_func);
    for(int i=0;i<25;i++){
        mask[i]=mask[50+i]=1;
        mask[25+i]=mask[75+i]=0;
    }
    while(true){
        trans.clear();
        char c=' ';
        while(c!='e'&&c!='f'&&c!='s')
            scanf("%c",&c);
        fprintf(stderr,"%c\n",c);
        if(c=='e')return 0;
        if(c=='f')self=0;
        if(c=='s')self=1;
        phase = 2;
        int board[25]={0};
        char ord[10]={' '};
        for(int i=0;i<6;i++)
            while(ord[i]<'1'||ord[i]>'6')
                scanf("%c",&ord[i]);
        ord[6]='\0';
        fprintf(stderr,"%s\n",ord);
        for(int i=0;i<6;i++)board[init[i]]=ord[i]-'0';
        for(int i=0;i<6;i++)board[24-init[i]]=10+ord[i]-'0';
        bd h = bd(0);
        h=encode(board);
        bd list[12];
        move(h,list);
        while(true){
            if(phase%2==self){
                timeup=false;
                int num=0;
                alarm(3);
                while(!timeup){
                    mcts(h,1.18);
                    num++;
                }
                fprintf(stderr,"%d\n",num);
                fprintf(stderr,"%f\%\n",trans[h].win/(trans[h].win+trans[h].los)*100);
                cerr<<h<<endl;
                bd new_h = dcs(h,board);
                int res = move_by_encode(h,new_h,board);
                if(!res)printf("00");
                else
                    printf("%d",res);
                fflush(stdout);
            }
            else{
                char res[2]={' '};
                while(res[0]<'0'||res[0]>'z')
                    scanf("%c",&res[0]);
                while(res[1]<'0'||res[1]>'z')
                    scanf("%c",&res[1]);
                if(res[0]=='w'||res[0]=='l')
                    break;
                int n=res[0]-'0'+10,m=res[1]-'0';
                for(int i=0;i<25;i++){
                    if(board[i]==n){
                        board[i]=0;
                        if(m==1)board[i-1]=n;
                        if(m==2)board[i-5]=n;
                        if(m==3)board[i-6]=n;
                        break;
                    }
                }
            }
            phase++;
            phase%=4;
            fprintf(stderr,"\n");
            for(int i=0;i<5;i++){
                for(int j=0;j<5;j++){
                    fprintf(stderr,"%3d ",board[i*5+j]);
                }
                fprintf(stderr,"\n");
            }
            h=encode(board);
        }
    }
}
