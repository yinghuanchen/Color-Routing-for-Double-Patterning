#include "Routing.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <queue>
using namespace std;

unordered_map<int,int> main_map;
int xpara(100000);
int ypara(10);
int alpha = 1; //parameter for hueristic

int MD(int x1,int x2,int y1,int y2){
    return abs(x1-x2)+abs(y1-y2);
}

point::point(){
    posx = 0;
    posy = 0;
    layer = 0;
    detour = 0;
    predx = 0;
    predy = 0;
    predl = 0;
}

point::point(int x,int y,int l){
    posx = x;
    posy = y;
    layer = l;
    detour = 0;
    predx = 0;
    predy = 0;
    predl = 0;
}

point::point(int x,int y,int l,int d,int px,int py,int pl){
    posx = x;
    posy = y;
    layer = l;
    detour = d;
    predx = px;
    predy = py;
    predl = pl;
}

void point::setselfx(int x){
    posx = x;
}

void point::setselfy(int y){
    posy = y;
}

int point::getx(){
    return posx;
}

int point::gety(){
    return posy;
}

int point::getlayer(){
    return layer;
}

void visualize(int x1,int x2,int y1,int y2,vector<int> remain,vector<int> layer){
    int tmp=max(x1,x2);
    if (x1==tmp){
        x1 = x2;
        x2 = tmp;
    }
    tmp = max(y1,y2);
    if (y1==tmp){
        y1 = y2;
        y2 = tmp;
    }
    for(int y=y2;y>=y1;y--){
        for(int x=x1;x<=x2;x++){
            bool record = false;
            char result = ' ';
            for(int lindex=0;lindex<layer.size();lindex++){
                if (!record && main_map.count(x*xpara+y*ypara+layer[lindex])){
                    if (main_map[x*xpara+y*ypara+layer[lindex]]==99999){
                        result = 'X';
                        record = true;
                    }
                    else{
                        for(int i=0;i<remain.size();i++){
                            if (main_map[x*xpara+y*ypara+layer[lindex]]==remain[i]){
                                record = true;
                                int tmp = remain[i]%10;
                                result = (char)(tmp+48);
                            }
                        }
                    }
                }
            }
            cout << result;
        }
        cout << '\n';
    }
}

void prepocessing(vector<point>& pin_record,vector<vector<int> >& net_record,vector<vector<int> >& block){
    main_map.clear();
    for(int i=0;i<block.size();i++){
        for(int x=block[i][0];x<=block[i][2];x++){
            for(int y=block[i][1];y<=block[i][3];y++){
                main_map[x*xpara+y*ypara+0] = 99999;
                main_map[x*xpara+y*ypara+1] = 99999;
                main_map[x*xpara+y*ypara+2] = 99999;
                main_map[x*xpara+y*ypara+3] = 99999;
                main_map[x*xpara+y*ypara+4] = 99999;
            }
        }
    }
    for(int i=0;i<net_record.size();i++){
        for(int j=1;j<net_record[i].size();j++){
            point current = pin_record[net_record[i][j]];
            main_map[current.getx()*xpara+current.gety()*ypara+current.getlayer()] = net_record[i][0];
        }
    }
    return;
}

struct cmpfororder{
    bool operator() (const pair<int,int>& lhs, const pair<int,int>& rhs){
        return lhs.second > rhs.second;
    }
};

int calH(int x1,int x2, int y1, int y2,int l1,int l2){
    int i,j,k,H=0,min=0,max=0,n=0,sum1,sum2,H1=0,H2=0,ll;
    if(x1==x2){
        if(y1>y2){
            min = y2;
            max = y1;
        } 
        else {
            min = y1;
            max = y2;
        }
        for(i=(min+1);i<max;i++){
            sum1 = 0;
            sum2 = 0;
            n = x1*xpara+i*ypara+1;
            if(main_map.count(n) && main_map[n]==99999){          
                    for(j=x1-1;j>=0;j--){
                        n = j*xpara+i*ypara+1;
                        if(main_map.count(n)) sum1+=1;
                        else break;
                    } 
                    sum2 = 0;
                    for(j=x1+1;j<=10000;j++){
                        n = j*xpara+i*ypara+1;
                        if(main_map.count(n)) sum2+=1;
                        else break;
                    }             
            }
            int a = (sum1>sum2)?sum2:sum1;
            if(a > H) H = a;
            if(sum1!=0 || sum2!=0) break;
        }
        return 2*H+abs(y1-y2);
    }
    else if(y1==y2){
        if(x1>x2){
            min = x2;
            max = x1;
        } 
        else {
            min = x1;
            max = x2;
        }
        for(i=(min+1);i<max;i++){
            sum1 = 0; 
            sum2 = 0;
            n = i*xpara+y1*ypara+1;
            if(main_map.count(n) && main_map[n]==99999){
                for(j=y1-1;j>=0;j--){
                    n = i*xpara+j*ypara+1;
                    if(main_map.count(n)) sum1+=1;
                    else break;
                } 
                for(j=y1+1;j<=10000;j++){
                    n = j*xpara+i*ypara+1;
                    if(main_map.count(n)) sum2+=1;
                    else break;
                }
            }
            int a = (sum1>sum2)?sum2:sum1;
            if(a > H) H = a;
            if(sum1!=0 || sum2!=0) break;
        }
        return 2*H+abs(x1-x2);
    }
    else{
        int a = 0;
        //upper L
        H1 = calH(x1,x2,y1,y1,l1,l2)+ calH(x2,x2,y1,y2,l1,l2);
        //Lower L
        H2 = calH(x1,x1,y1,y2,l1,l2)+ calH(x1,x2,y2,y2,l1,l2);
        return (int)(H1+H2)/2;
        // return (H1>H2)?H2:H1;
    }
}

vector<vector<int> > netordering(vector<point>& pin_record,vector<vector<int> >& net_record,vector<bool>& net_critical){
    vector<vector<int> > finalreturn;
    priority_queue<pair<int,int>,vector<pair<int,int> >,cmpfororder> netorderscore;
    int score,ct;
    for(int i=0;i<net_record.size();i++){
        score = 0;

        if(net_record[i].size()==3){
            point p1 = pin_record[net_record[i][1]];
            point p2 = pin_record[net_record[i][2]];
            score += MD(p1.getx(),p2.getx(),p1.gety(),p2.gety());
        }
        else{
            ct = 0;
            for(int j=1;j<net_record[i].size()-1;j++){
                for(int k=j+1;k<net_record[i].size();k++){
                    point p1 = pin_record[net_record[i][j]];
                    point p2 = pin_record[net_record[i][k]];
                    score += MD(p1.getx(),p2.getx(),p1.gety(),p2.gety());
                    ct+=1;
                }   
            }
            score = ((int)score/ct);
        }

        if (net_critical[i]) score >>= 1;
        pair<int,int> tmppair(i,score);
        netorderscore.push(tmppair);
    }
    while(!netorderscore.empty()){
        pair<int,int> current = netorderscore.top();
        netorderscore.pop();
        finalreturn.push_back(net_record[current.first]);
    }
    return finalreturn;
}

unordered_map<int, int> F;
unordered_map<int, int> G;
unordered_map<int ,int> color;

int Heuristic_function(point s, point t){
    int score = MD(s.getx(),t.getx(),s.gety(),t.gety());
    score += alpha*abs(s.getlayer()-t.getlayer());
    return (score/2);
    // return 0;
}

struct cmp_2p{             
    bool operator() (point& lhs, point& rhs)
    {
        return F[ lhs.getx()*xpara+lhs.gety()*ypara+lhs.getlayer() ] > F [rhs.getx()*xpara+rhs.gety()*ypara+rhs.getlayer()];
    }
};

bool Set_F(point pred, point current ,point terminal){
    int g , h;
    bool result = false;
    g = G[ pred.getx()*xpara + pred.gety()*ypara + pred.getlayer() ] + 1 ;
    h = Heuristic_function( current , terminal );
    if (color[current.getx()*xpara+current.gety()*ypara+current.getlayer()]==1){
        if (F[current.getx()*xpara+current.gety()*ypara+current.getlayer()]>(g+h)){
            F[current.getx()*xpara+current.gety()*ypara+current.getlayer()] = g+h;
            G[current.getx()*xpara+current.gety()*ypara+current.getlayer()] = g;
            result = true;
        }
    }
    else{
        F[current.getx()*xpara+current.gety()*ypara+current.getlayer()] = g+h;
        G[current.getx()*xpara+current.gety()*ypara+current.getlayer()] = g;
        result = true;
    }
    return result;
}

bool checkvalid(int x , int y ,int layer, int netorder,int mode){
    int boundary_max = 10000;
    bool valid = false;
    if (x<=boundary_max && y<=boundary_max && x>=0 && y>=0 && layer>=1 && layer<=4){
        if( !main_map.count(x*xpara + y*ypara + layer) ){
            valid = true;
        }
        else if (main_map[x*xpara + y*ypara + layer]==netorder){
            valid = true;
        }
        else if (mode==1 && main_map[x*xpara + y*ypara + layer]==99999){
            valid = true;
        }
    }
    return valid;
}

vector<point> twopin(int x1,int x2,int y1,int y2,int la1,int la2,int netorder,unordered_map<int,int> alreadyexist){
    F.clear();
    G.clear();
    color.clear();
    vector<point> record;
    int emergylock = 1000000;
    unordered_map<int, point> pred ;
    priority_queue<point,vector<point>,cmp_2p> unsearched_q;

    /////////start from the source (the first discovering step,6 directions)////
    point s(x1,y1,la1),t(x2,y2,la2);
    point track;
    unsearched_q.push(s);//pushing source
    color[s.getx()*xpara+s.gety()*ypara+s.getlayer()] = 1;//painting gray;
    pred[s.getx()*xpara+s.gety()*ypara+s.getlayer()] = s;

    bool success = false;
    int ct=0;
    while(!unsearched_q.empty() && ct < emergylock){

        point current = unsearched_q.top();
        unsearched_q.pop();
        color[ current.getx()*xpara+current.gety()*ypara+current.getlayer() ] = 2;
        if( color[x2*xpara+y2*ypara+la2] == 2 || alreadyexist.count(current.getx()*xpara+current.gety()*ypara+current.getlayer())){
            track = current;
            success = true;
            break;
        }
        //////////////////////////////check state///////////////////////////////////////////
        int judge = current.getlayer()%2;
        if (judge){
            if (checkvalid(current.getx()+1,current.gety(),current.getlayer(),netorder,0) && checkvalid(current.getx()+2,current.gety(),current.getlayer(),netorder,1) && (color[(current.getx()+1)*xpara+current.gety()*ypara+current.getlayer()]!=2)){
                point after(current.getx()+1,current.gety(),current.getlayer());
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
            if (checkvalid(current.getx()-1,current.gety(),current.getlayer(),netorder,0) && checkvalid(current.getx()-2,current.gety(),current.getlayer(),netorder,1) && (color[(current.getx()-1)*xpara+current.gety()*ypara+current.getlayer()]!=2)){
                point after(current.getx()-1,current.gety(),current.getlayer());
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
            if (checkvalid(current.getx(),current.gety(),current.getlayer()+1,netorder,0) && checkvalid(current.getx(),current.gety()+1,current.getlayer()+1,netorder,1) && checkvalid(current.getx(),current.gety()-1,current.getlayer()+1,netorder,1) && (color[current.getx()*xpara+current.gety()*ypara+current.getlayer()+1]!=2)){
                point after(current.getx(),current.gety(),current.getlayer()+1);
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
            if (checkvalid(current.getx(),current.gety(),current.getlayer()-1,netorder,0) && checkvalid(current.getx(),current.gety()+1,current.getlayer()-1,netorder,1) && checkvalid(current.getx(),current.gety()-1,current.getlayer()-1,netorder,1) && (color[current.getx()*xpara+current.gety()*ypara+current.getlayer()-1]!=2)){
                point after(current.getx(),current.gety(),current.getlayer()-1);
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
        }
        else{
            if (checkvalid(current.getx(),current.gety()+1,current.getlayer(),netorder,0) && checkvalid(current.getx(),current.gety()+2,current.getlayer(),netorder,1) && (color[current.getx()*xpara+(current.gety()+1)*ypara+current.getlayer()]!=2)){
                point after(current.getx(),current.gety()+1,current.getlayer());
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
            if (checkvalid(current.getx(),current.gety()-1,current.getlayer(),netorder,0) && checkvalid(current.getx(),current.gety()-2,current.getlayer(),netorder,1) && (color[current.getx()*xpara+(current.gety()-1)*ypara+current.getlayer()]!=2)){
                point after(current.getx(),current.gety()-1,current.getlayer());
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
            if (checkvalid(current.getx(),current.gety(),current.getlayer()+1,netorder,0) && checkvalid(current.getx()+1,current.gety(),current.getlayer()+1,netorder,1) && checkvalid(current.getx()-1,current.gety(),current.getlayer()+1,netorder,1) && (color[current.getx()*xpara+current.gety()*ypara+current.getlayer()+1]!=2)){
                point after(current.getx(),current.gety(),current.getlayer()+1);
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
            if (checkvalid(current.getx(),current.gety(),current.getlayer()-1,netorder,0) && checkvalid(current.getx()+1,current.gety(),current.getlayer()-1,netorder,1) && checkvalid(current.getx()-1,current.gety(),current.getlayer()-1,netorder,1) && (color[current.getx()*xpara+current.gety()*ypara+current.getlayer()-1]!=2)){
                point after(current.getx(),current.gety(),current.getlayer()-1);
                if (Set_F( current, after , t )){
                    pred[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ] = current;
                    color[ after.getx()*xpara + after.gety()*ypara + after.getlayer() ]=1;
                    unsearched_q.push(after);
                }
            }
        }
        
        ct++;
    }
    ///Back tracking
    
    ct = 0;
    while( success && (track.getx()!=s.getx() || track.gety()!=s.gety() || track.getlayer()!=s.getlayer()) &&  ct<emergylock){
        record.push_back(track);
        track = pred[track.getx()*xpara + track.gety()*ypara + track.getlayer()];
        ct++;
    }
    record.push_back(s);
    reverse(record.begin(),record.end());
    // for(int i=0;i<record.size();i++){
    //     cout << record[i].getx() << ' ' << record[i].gety() << ' ' << record[i].getlayer() << '\n';
    // }
    vector<point> fall;
    if (success){
        return record;
    }
    else{
        return fall;
    }
}

struct cmp2{
    bool operator() (const point& lhs, const point& rhs)
    {
        return lhs.detour > rhs.detour;
    }
};

vector<point> MST(int &d, vector<point> terminalpin, bool &judgeM, int netindex) {
    // cout << "start\n";
    unordered_map<int,int> tmap;
    unordered_map<int,int> partitionmap;
    judgeM=true;
    int x,y,n,l,n2;
    int m =  terminalpin.size();
    int ct = 1,totalcount = 0;
    priority_queue <point, vector<point>, struct cmp2> path;
    point start = terminalpin[0];
    int x0 = terminalpin[0].getx();
    int y0 = terminalpin[0].gety();
    int l0 = terminalpin[0].getlayer();
    tmap[x0*xpara+y0*ypara] = 1;

    for(int i=1;i<terminalpin.size();i++)
    {
        int x1 = terminalpin[i].getx();
        int y1 = terminalpin[i].gety();
        int l1 = terminalpin[i].getlayer();
        point tmpp(x1,y1,l1,0,x0,y0,l0);
        // tmpp.detour = MD(x0,x1,y0,y1);
        tmpp.detour = calH(x0,x1,y0,y1,l0,l1);
        path.push(tmpp);
    }
    vector<point> output;
    d = 0;
    bool judge;
    while(!path.empty() && ct<m && totalcount<1000)
    {
        point tmp = path.top();
        path.pop();
        int x1 = tmp.getx();
        int y1 = tmp.gety();
        int l1 = tmp.getlayer();
        n = x1*xpara+y1*ypara;
        if(!tmap[n])
        {
            x0 = tmp.predx;
            y0 = tmp.predy;
            l0 = tmp.predl;
            vector<point> record = twopin(x1,x0,y1,y0,l1,l0,netindex,partitionmap);
            // cout << x0 << ' ' << y0 << ' ' << x1 << ' ' << y1 << ' ' << l0 << ' ' << l1 << ' ' << record.size()  << '\n';

            judge = (record.size()!=0);
            if(judge)
            {
                //Calculate distance
                d += record.size();
                tmap[n] = 1;
                for(int i=0;i<record.size();i++) 
                {
                    point current = record[i];
                    partitionmap[current.getx()*xpara+current.gety()*ypara+current.getlayer()] = 1;
                    output.push_back(current);
                }
                // cout << "current map size: " << partitionmap.size() << '\n';
                // cout << "current output size: " << output.size() << '\n';
                ct+=1;

                for(int i=1;i<terminalpin.size();i++)
                {               
                    int x2 = terminalpin[i].getx();
                    int y2 = terminalpin[i].gety();
                    int l2 = terminalpin[i].getlayer();
                    n2 = x2*xpara+y2*ypara;
                    
                    if(!tmap[n2])
                    {
                        point tmpp(x2,y2,l2,0,x1,y1,l1);
                        // tmpp.detour =MD(x1,x2,y1,y2);
                        tmpp.detour = calH(x1,x2,y1,y2,l1,l2);
                        path.push(tmpp);
                    }
                }
            }
        }
        totalcount++;
    }

    for(int i=1;i<terminalpin.size();i++){
        x = terminalpin[i].getx();
        y = terminalpin[i].gety();
        n = x*xpara+y*ypara;
        if(!tmap[n]){
            judgeM = false;
            d = 2147483647;
        } 
    }
    // cout << "\n\n";
    return output;
}

bool compare(const pair<int,int> & l, const pair<int,int> & r){
    return l.second < r.second; //Large->
}

vector<point> multihandle0(vector<point>& pin_record,vector<int> net){
    bool judgeM;
    vector<point> terminalpin;
    unordered_map<int,int> tmap;
    int i,j,x,y,l,n,ct=0,df=0,d=0;
    int netindex = net[0];

    for(i=1;i<net.size();i++)
    {
        point tmp = pin_record[net[i]];
        terminalpin.push_back(tmp);
    }
    
    vector<point> finaloutput =  MST(df, terminalpin,judgeM,netindex);
    return finaloutput;
}

vector<point> multihandle3(vector<point>& pin_record,vector<int> net){
     bool judgeM;
    vector<point> terminalpin;
    unordered_map<int,int> tmap;
    vector <point> steinerpoint;
    int i,j,x,y,l,n,ct=0,df=0,d=0;
    int netindex = net[0];

    for(i=1;i<net.size();i++)
    {
        point tmp = pin_record[net[i]];
        terminalpin.push_back(tmp);
    }
    
    //Add steiner points 
    for(i=0;i<net.size()-1;i++)
    {
        for(j=i+1;j<net.size()-1;j++)
        {
            x = (int)(terminalpin[i].getx()+terminalpin[j].getx())/2;
            y = (int)(terminalpin[i].gety()+terminalpin[j].gety())/2;
            l = terminalpin[i].getlayer();
            n = x*xpara+y*ypara+l;
            if (!main_map.count(n)){
                point tmpp = point(x,y,l);
                steinerpoint.push_back(tmpp);
          
            }
        }
    }
    //Add steiner points (hanan grid)
    for(i=0;i<net.size()-1;i++)
    {
        for(j=0;j<net.size()-1;j++)
        {
            if(i!=j)
            {
                 x = (int)terminalpin[i].getx();
                 y = (int)terminalpin[j].gety();
                 l = terminalpin[i].getlayer();
                 n = x*xpara+y*ypara+l;
                if (!main_map.count(n)){
                    point tmpp = point(x,y,l);
                    steinerpoint.push_back(tmpp);
                
                }
            }
           
        }
    }
    int s = steinerpoint.size();
    vector<point> finaloutput = MST(df, terminalpin,judgeM,netindex);
    vector<point> out;
    vector<pair<int,int>> nondecT; //index, df-d
    // Choose the start point 
    for(i=0;i<s;i++)
    {
        terminalpin.push_back(steinerpoint[i]);
        out = MST(d, terminalpin,judgeM,netindex);
        if(df>d)
        {
            pair<int,int> p(i,d);
            nondecT.push_back(p);
        }  
        terminalpin.erase(terminalpin.end()-1,terminalpin.end()); 
    }
    sort(nondecT.begin(),nondecT.end(),compare);
    for(i=0;i<nondecT.size();i++)
    {
        terminalpin.push_back(steinerpoint[nondecT[i].first]);
    }
    finaloutput =  MST(df, terminalpin,judgeM,netindex);
    
    ct = nondecT.size();
    while(ct>0)
    {
        //Keep deleting
        terminalpin.erase(terminalpin.end()-1,terminalpin.end());
        out =  MST(d, terminalpin,judgeM,netindex);
        if(d<df)
        {
            finaloutput  = out;
            df = d;
        }
        ct--;
    }
    int ss = terminalpin.size();
    //Delete the useless points 
    for(i=net.size()-1;i<=(ss-1);i++)
    {
        terminalpin.erase(terminalpin.end()-1,terminalpin.end());
        out =  MST(d, terminalpin,judgeM,netindex);
        if(d<df)
        {
            finaloutput  = out;
            df = d;
        }
    }

    return finaloutput;
}

vector<wire> wiretransform(vector<point> origin, vector<unordered_map<int,int> > &vvvv, vector<int> &wirell){
    vector<wire> output;
    int state = 0,newstate=origin[0].getlayer()%2,onoff=0,size=origin.size(),thiscolor;
    if (size==0) return output;
    point start=origin[0],current=origin[0];
    for(int i=1;i<size;i++){
        if (!newstate && abs(current.gety()-origin[i].gety())>1) onoff = 1;
        if (!newstate && current.getx()!=origin[i].getx()) onoff = 1;
        if (newstate && abs(current.getx()-origin[i].getx())>1) onoff = 1;
        if (newstate && current.gety()!=origin[i].gety()) onoff = 1;
        if (current.getlayer()!=origin[i].getlayer()) onoff = 1;
        if (onoff){
            onoff = 0;
            thiscolor = 2;
            if (start.getlayer()%2 && start.gety()%2) thiscolor = 1;
            if (!(start.getlayer()%2) && start.getx()%2) thiscolor = 1;
            if (start.getx()!=origin[i-1].getx() || start.gety()!=origin[i-1].gety()){
                wire tmpout(start.getx(),origin[i-1].getx(),start.gety(),origin[i-1].gety(),origin[i-1].getlayer(),thiscolor);
                int l = origin[i-1].getlayer();
                if(l%2){
                    vvvv[l-1][origin[i-1].gety()] = 1;
                    wirell[l*2+thiscolor-3]+=abs(start.getx()-origin[i-1].getx());
                }
                else{
                    vvvv[l-1][origin[i-1].getx()] = 1;
                    wirell[l*2+thiscolor-3]+=abs(start.gety()-origin[i-1].gety());
                }
                output.push_back(tmpout);
            }
            start = origin[i];
        }
        current = origin[i];
        newstate = current.getlayer()%2;
    }
    if (start.getx()!=origin[size-1].getx() || start.gety()!=origin[size-1].gety()){
        thiscolor = 2;
        if (start.getlayer()%2 && start.gety()%2) thiscolor = 1;
        if (!(start.getlayer()%2) && start.getx()%2) thiscolor = 1;
        wire finalout(start.getx(),origin[size-1].getx(),start.gety(),origin[size-1].gety(),start.getlayer(),thiscolor);
        output.push_back(finalout);
        vvvv[start.getlayer()-1][origin[size-1].getx()] = 1;
        wirell[start.getlayer()*2+thiscolor-3]+=abs(start.gety()-origin[size-1].gety())+abs(start.getx()-origin[size-1].getx());
    }
    return output;
}

void markonmain(vector<point>& result,int num){
    for(int i=0;i<result.size();i++){
        main_map[result[i].getx()*xpara+result[i].gety()*ypara+result[i].getlayer()] = num;
    }
    return;
}

bool colorcompare(const wire &l, const wire &r){
    return l.wirelength > r.wirelength;
}

vector<vector<wire> > color_balancing(vector<vector<wire> > input,vector <unordered_map <int,int> > &vvvv,vector <int> &wirell){
    int a,b,c,x,y;
    vector<vector<wire> > vv;
    vv.resize(4);
    vector<vector<wire> > output(input);
    int eachindex = 0,l,c1,c2,thisc,wl,d;
    for(int i=0;i<output.size();i++){
        for(int j=0;j<output[i].size();j++){
            output[i][j].netnum = i;
            output[i][j].netind = j;
            vv[output[i][j].layer-1].push_back(output[i][j]);
        }
    }

    for(int i=0;i<4;i++){
        sort(vv[i].begin(),vv[i].end(),colorcompare);
    }

    wirell.clear();
    wirell.resize(8,0);

    for(int i=0;i<4;i++){
        if(!(i%2)){
            for(int j=0;j<vv[i].size();j++){
                y = vv[i][j].y1;
                if(vvvv[i].count(y+1) || vvvv[i].count(y-1)){
                    d = vv[i][j].color;
                    wirell[2*i+d-1]+=vv[i][j].wirelength;
                }
            }
        }
        else{
            for(int j=0;j<vv[i].size();j++){
                x = vv[i][j].x1;
                if(vvvv[i].count(x+1) || vvvv[i].count(x-1)){
                    d = vv[i][j].color;
                    wirell[2*i+d-1]+=vv[i][j].wirelength;
                }
            }
        }
    }

    for(int i=0;i<4;i++){
        if(wirell[2*i]>wirell[2*i+1]) c=2;
        else c = 1;
        if(!(i%2)){
            for(int j=0;j<vv[i].size();j++){
                a = vv[i][j].netnum;
                b = vv[i][j].netind;
                y = vv[i][j].y1;
                if(!vvvv[i].count(y+1) && !vvvv[i].count(y-1)){
                    output[vv[i][j].netnum][vv[i][j].netind].color = c;
                    if(c==1){
                        wirell[2*i]+=vv[i][j].wirelength;
                        if(wirell[2*i]<wirell[2*i+1]){
                            c = 1;
                        }
                        else{
                            c = 2;
                        } 
                    } 
                    else{
                        wirell[2*i+1]+=vv[i][j].wirelength;
                        if(wirell[2*i]>wirell[2*i+1]){
                            c = 2;
                        }
                        else{
                            c = 1;
                        } 
                    } 
                }              
            }
        }
        else{
            for(int j=0;j<vv[i].size();j++){
                a = vv[i][j].netnum;
                b = vv[i][j].netind;
                x = vv[i][j].x1;
                if(!vvvv[i].count(x+1) && !vvvv[i].count(x-1)){
                    output[vv[i][j].netnum][vv[i][j].netind].color = c;
                    if(c==1){
                        wirell[2*i]+=vv[i][j].wirelength;
                        if(wirell[2*i]<wirell[2*i+1]){
                            c = 1;
                        }
                        else{
                            c = 2;
                        }
                    } 
                    else{
                        wirell[2*i+1]+=vv[i][j].wirelength;
                        if(wirell[2*i]>wirell[2*i+1]){
                            c = 2;
                        }
                        else{
                            c = 1;
                        } 
                    } 
                }
            }
        }  
    }
    return output;
}

vector<vector<wire> > routing(vector<point>& pin_record,vector<vector<int> >& net_record){
    int sum = 0;
    int sumall = 0;
    vector<unordered_map<int,int> > color_distribution_map(4);
    vector<int> color_length_count(8,0);
    vector<wire> moduleforwire;
    int totalnetsize = net_record.size();
    vector<vector<wire> > transformed_result(totalnetsize,moduleforwire);
    for(int i=0;i<transformed_result.size();i++){
        int net_name = net_record[i][0];
        if (net_record[i].size() == 3){
            point start = pin_record[net_record[i][1]];
            point end = pin_record[net_record[i][2]];
            unordered_map<int,int> emptymap;
            vector<point> result = twopin(start.getx(),end.getx(),start.gety(),end.gety(),start.getlayer(),end.getlayer(),net_name,emptymap);
            sumall+= result.size();
            transformed_result[net_name-1] = wiretransform(result,color_distribution_map,color_length_count);
            markonmain(result,net_name);
        }
        else{
            vector<point> result;
            if (net_record[i].size()>20){
                result = multihandle0(pin_record,net_record[i]);
            }
            else{
                result = multihandle3(pin_record,net_record[i]);
            }
            transformed_result[net_name-1] = wiretransform(result,color_distribution_map,color_length_count);
            markonmain(result,net_name);
            sum+= result.size();
            sumall+= result.size();
        }
    }

    vector<vector<wire> > routingresult = color_balancing(transformed_result,color_distribution_map,color_length_count);
    return routingresult;
}