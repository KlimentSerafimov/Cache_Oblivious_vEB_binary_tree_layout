//
//  main.cpp
//  vEB_binary_tree_layout
//
//  Created by Kliment Serafimov on 10/17/17.
//  Copyright © 2017 Kliment Serafimov. All rights reserved.
//

#include <fstream>
#include <iomanip>
#include <iostream>

#include <map>
#include <set>
#include <cmath>
#include <queue>
#include <stack>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include <algorithm>

#define P push
#define f first
#define s second
#define pb push_back
#define mp make_pair
#define DEC 0.00000001
#define MAX 2139062143
#define MAX_63  1061109567
#define MAXll 9187201950435737471
#define bp(a) __builtin_popcount(a)
#define rand(a, b) ((rand()%(b-a+1))+a)
#define MEM(a, b) memset(a, b, sizeof(a))
#define sort_v(a) sort(a.begin(), a.end())
#define rev_v(a)  reverse(a.begin(), a.end())

//#define fin cin
//#define fout cout
using namespace std;
//ifstream fin(".in");
//ofstream fout(".out");

class element
{
public:
    int x, y;
    element(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    element()
    {
    }
    element(bool do_cin)
    {
        if(do_cin)
        {
            cin >> x >> y;
        }
    }
};

#define maxDepth 18
#define maxN (1<<maxDepth)

element points[maxN];

class node
{
    public:
    int left_bound;
    int right_bound;
    int node_id;
    int parent_id;
    int left_child_id;
    int right_child_id;
    node(int lb, int rb, int _id, int _parent_id)
    {
        left_bound = lb;
        right_bound = rb;
        node_id = _id;
        parent_id = _parent_id;
    }
    node(int lb, int rb)
    {
        left_bound = lb;
        right_bound = rb;
    }
    node(){}
    bool is_internal_node()
    {
        return left_bound != right_bound;
    }
};

node tree[4*maxN];


void buffer(int b, int width)
{
    for(int i = 0;i<b;i++)
    {
        if((i+1)%(width) != 0)
        {
            cout <<" ";
        }
        else
        {
            cout << "|";
        }
    }
}

string print_int(int n, int width)
{
    string ret;
    if(n == 0)
    {
        ret = "0";
    }
    else while(n>0)
    {
        ret+=(n%10)+'0';
        n/=10;
    }
    while(ret.size() < width)
    {
        ret+=' ';
    }
    if(ret.size()>width)
    {
        ret = "";
        while(ret.size() < width)
        {
            ret+="-";
        }
    }
    rev_v(ret);
    return ret;
}

int print_tree(int in, int delta_in, int id)
{
    int after_buffer = in+delta_in;
    if(tree[id].is_internal_node())
    {
        after_buffer = print_tree(in+delta_in, delta_in, tree[id].left_child_id);
    }
    buffer(in, delta_in);
    cout << print_int(id, delta_in-1) <<"|";
    buffer(after_buffer-in-delta_in+1, delta_in);
    cout << endl;
    if(tree[id].is_internal_node())
    {
        print_tree(in+delta_in, delta_in, tree[id].right_child_id);
    }
    return after_buffer;
}

queue<pair<int, pair<int, int> > > q[maxDepth];
int build_tree(int id, int parent_id, int lb, int rb, int local_root_depth, int local_half_depth, int local_max_depth)
{
    if(local_root_depth+1 == local_max_depth)
    {
        tree[id] = node(lb, rb, id, parent_id);
        if(lb != rb)q[local_max_depth].P(mp(id, mp(lb, rb)));
        return id+1;
    }
    else
    {
        int upper_half_half_depth = (local_root_depth+local_half_depth)/2;
        int upper_half_max_depth = local_half_depth;
        int next_id = build_tree(id, parent_id, lb, rb, local_root_depth, upper_half_half_depth, upper_half_max_depth);
        
        int lower_half_local_root = local_half_depth;
        int lower_half_half_depth = (local_half_depth+local_max_depth)/2;
        int current_q = lower_half_local_root;
        while(!q[current_q].empty())
        {
            int parent_id = q[current_q].front().f;
            int new_lb = q[current_q].front().s.f;
            int new_rb = q[current_q].front().s.s;
            int mid = (new_lb+new_rb)/2;
            q[current_q].pop();
            tree[parent_id].left_child_id = next_id;
            next_id = build_tree(next_id, parent_id, new_lb, mid, lower_half_local_root, lower_half_half_depth, local_max_depth);
            tree[parent_id].right_child_id = next_id;
            next_id = build_tree(next_id, parent_id, mid+1, new_rb, lower_half_local_root, lower_half_half_depth, local_max_depth);
        }
        return next_id;
    }
}


int main(int argc, const char * argv[]) {
    int n = 64;
    //cin >> n;
    assert(n<maxN);
    for(int i = 0;i<n;i++)
    {
        points[i] = element(i, 0);
    }
    int max_depth = log2(n)+2;
    build_tree(0, -1, 0, n-1, 0, max_depth/2, max_depth);
    print_tree(0, 4, 0);
    return 0;
}
