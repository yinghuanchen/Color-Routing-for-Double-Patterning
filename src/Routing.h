#ifndef _ROUTING_H
#define _ROUTING_H

#include <vector>
#include <algorithm>
using namespace std;

class point{
public:
    point();
    point(int x,int y,int l);
    point(int x,int y,int l,int d,int px,int py,int pl);
    void setselfx(int x);
    void setselfy(int y);
    int getx();
    int gety();
    int getlayer();
    int detour;
    int predx;
    int predy;
    int predl;
    vector<point> path;
private:
    int posx;
    int posy;
    int layer;
};

class wire{
public:
    wire(int xpos1,int xpos2,int ypos1,int ypos2,int lindex,int c){
        x1=xpos1,x2=xpos2,y1=ypos1,y2=ypos2,layer=lindex,color=c, wirelength=abs(x1-x2)+abs(y1-y2);
    }
    wire();
    int x1;
    int x2;
    int y1;
    int y2;
    int layer;
    int color;
    int netnum;
    int netind;
    int wirelength;
};

vector<vector<int> > netordering(vector<point>&,vector<vector<int> >&,vector<bool>&);
void prepocessing(vector<point>&,vector<vector<int> >&,vector<vector<int> >&);
void visualize(int,int,int,int,vector<int>,vector<int>);
vector<vector<wire> > routing(vector<point>&,vector<vector<int> >&);

#endif
