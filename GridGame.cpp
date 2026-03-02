#include <iostream>
#include <vector>
#include <queue>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Cell { int r, c; };

const int G = 10;
int board[G][G];
Cell player_pos{0,0};
Cell target_pos{9,9};

vector<Cell> shortest_path;
int path_index = 0;
vector<vector<int>> dist_glob(G, vector<int>(G,-1));

const int DR[4] = {-1,1,0,0};
const int DC[4] = {0,0,-1,1};

bool inside(int r,int c){ return r>=0 && r<G && c>=0 && c<G; }
bool is_obstacle(int r,int c){ return board[r][c]==1; }

// Very simple obstacles: a single vertical wall that still leaves a path
void init_fixed_obstacles() {
    for(int r=0;r<G;r++) for(int c=0;c<G;c++) board[r][c]=0;
    for(int r=2;r<=7;r++) board[r][5] = 1;     // wall in column 5
}

void compute_shortest_path() {
    vector<vector<int>> dist(G, vector<int>(G,-1));
    vector<vector<Cell>> parent(G, vector<Cell>(G,{-1,-1}));
    queue<Cell> q;
    dist[player_pos.r][player_pos.c]=0;
    q.push(player_pos);

    while(!q.empty()){
        Cell u=q.front(); q.pop();
        if(u.r==target_pos.r && u.c==target_pos.c) break;
        for(int k=0;k<4;k++){
            int nr=u.r+DR[k], nc=u.c+DC[k];
            if(!inside(nr,nc) || is_obstacle(nr,nc)) continue;
            if(dist[nr][nc]!=-1) continue;
            dist[nr][nc]=dist[u.r][u.c]+1;
            parent[nr][nc]=u;
            q.push({nr,nc});
        }
    }
    dist_glob=dist;
    shortest_path.clear();
    if(dist[target_pos.r][target_pos.c]==-1){ path_index=0; return; }
    Cell cur=target_pos;
    while(!(cur.r==player_pos.r && cur.c==player_pos.c)){
        shortest_path.push_back(cur);
        cur=parent[cur.r][cur.c];
    }
    shortest_path.push_back(player_pos);
    reverse(shortest_path.begin(),shortest_path.end());
    path_index=0;
}

void print_text(double x,double y,const string& s,double size=0.03){
    cout<<"newstring hjl vjc fontsize "<<size<<" x "<<x<<" y "<<y<<" : "<<s<<"\n";
}

void cell_to_coords(int r,int c,double& x1,double& y1,double& x2,double& y2){
    double margin=0.05;
    double w=1.0-2*margin, h=1.0-2*margin;
    double cw=w/G, ch=h/G;
    x1=margin+c*cw; x2=margin+(c+1)*cw;
    y2=1.0-(margin+r*ch); y1=1.0-(margin+(r+1)*ch);
}

void print_frame(int step){
    cout<<"newgraph\n";
    cout<<"newcurve marktype none linethickness 0 pts\n";
    cout<<"xaxis min 0 max 1 nodraw\n";
    cout<<"yaxis min 0 max 1 nodraw\n";

    stringstream ss; ss<<"Grid pathfinding - step "<<step;
    print_text(0.05,0.97,ss.str(),0.04);

    // grid
    for(int r=0;r<=G;r++){
        double x1,y1,x2,y2;
        cell_to_coords(r,0,x1,y1,x2,y2); double gx1=x1, gy=y1;
        cell_to_coords(r,G-1,x1,y1,x2,y2); double gx2=x2;
        cout<<"newcurve linetype solid linethickness 0.4 color 0.7 0.7 0.7\n";
        cout<<"pts "<<gx1<<" "<<gy<<" "<<gx2<<" "<<gy<<"\n";
    }
    for(int c=0;c<=G;c++){
        double x1,y1,x2,y2;
        cell_to_coords(0,c,x1,y1,x2,y2); double gy1=y2, gx=x1;
        cell_to_coords(G-1,c,x1,y1,x2,y2); double gy2=y1;
        cout<<"newcurve linetype solid linethickness 0.4 color 0.7 0.7 0.7\n";
        cout<<"pts "<<gx<<" "<<gy1<<" "<<gx<<" "<<gy2<<"\n";
    }

    // obstacles
    for(int r=0;r<G;r++) for(int c=0;c<G;c++) if(is_obstacle(r,c)){
        double x1,y1,x2,y2; cell_to_coords(r,c,x1,y1,x2,y2);
        cout<<"newcurve marktype box color 0 0 0\n";
        cout<<"pts "<<x1<<" "<<y1<<" "<<x2<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x1<<" "<<y2<<" "<<x1<<" "<<y1<<"\n";
    }

    // path so far (blue polyline)
    if(!shortest_path.empty()){
        cout<<"newcurve linetype solid linethickness 1.0 color 0 0 1\npts ";
        for(int i=0;i<=path_index && i<(int)shortest_path.size();++i){
            double x1,y1,x2,y2; cell_to_coords(shortest_path[i].r,shortest_path[i].c,x1,y1,x2,y2);
            double cx=0.5*(x1+x2), cy=0.5*(y1+y2);
            cout<<cx<<" "<<cy<<" ";
        }
        cout<<"\n";
    }

    // target (red)
    {
        double x1,y1,x2,y2; cell_to_coords(target_pos.r,target_pos.c,x1,y1,x2,y2);
        cout<<"newcurve marktype box color 1 0 0\n";
        cout<<"pts "<<x1<<" "<<y1<<" "<<x2<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x1<<" "<<y2<<" "<<x1<<" "<<y1<<"\n";
    }

    // player (green)
    {
        double x1,y1,x2,y2; cell_to_coords(player_pos.r,player_pos.c,x1,y1,x2,y2);
        cout<<"newcurve marktype box color 0 0.8 0\n";
        cout<<"pts "<<x1<<" "<<y1<<" "<<x2<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<x1<<" "<<y2<<" "<<x1<<" "<<y1<<"\n";
    }

    cout<<"\n";
}

int main(){
    player_pos={0,0};
    target_pos={9,9};
    init_fixed_obstacles();
    compute_shortest_path();

    if(shortest_path.size()<=1){
        cerr<<"No path found\n";
        return 0;
    }

    int max_steps = (int)shortest_path.size()-1;

    for(int step=0; step<=max_steps && step<19; ++step){
        path_index = step;
        player_pos = shortest_path[path_index];

        ostringstream fname;
        fname<<"step_"<<setw(2)<<setfill('0')<<step<<".jgr";
        ofstream out(fname.str());
        streambuf* backup = cout.rdbuf();
        cout.rdbuf(out.rdbuf());
        print_frame(step);
        cout.rdbuf(backup);
    }
    return 0;
}