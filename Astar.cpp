#include <iostream>
#include <queue>
#include <map>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const int BLANK = 0;
const int BARRIER = -1;

typedef struct Point
{
    int x;
    int y;
    int z;
}Point;

typedef struct Node
{
    int status[3][3][3];
    struct Node *parent;
    int f;
    int g;
    int h;
    Point blank;
    friend bool operator < (const Node a, const Node b){
        return a.f >= b.f;
    }
}Node;


enum direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACK
};

priority_queue <Node> OPEN;
map<Node, int> CLOSE;

void print_status(Node *current_status) {
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                cout<<current_status->status[i][j][k]<<' ';
            }      
            cout<<endl;  
        }
        cout<<endl;
    }
}

int h(Node *current_status, Node *target) {
    int count = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                if (current_status->status[i][j][k] != target->status[i][j][k])
                {
                    count++;
                }
            }
        }
    }
    return count;
}

void copy_status(Node *from, Node *to){
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                to->status[i][j][k] = from->status[i][j][k];
            }
        }
    }
}

void move_blank(Node *current_status, direction dire, Node *target) {
    Node *p;
    p = new Node();
    int x = current_status->blank.x;
    int y = current_status->blank.y;
    int z = current_status->blank.z;
    p->blank.x = x;
    p->blank.y = y;
    p->blank.z = z;
    copy_status(current_status, p);
    p->g = current_status->g;
    switch (dire){
        case UP:
            if (y >= 2)
            {
                return;
            }
            else {
                if (p->status[z][y+1][x] == BARRIER)
                {
                    return;
                }
                p->status[z][y][x] = p->status[z][y+1][x];
                p->status[z][y+1][x] = BLANK;
                p->blank.y = y+1;
            }
            break;
        case DOWN:
            if (y <= 0)
            {
                return;
            }
            else {
                if (p->status[z][y-1][x] == BARRIER)
                {
                    return;
                }
                p->status[z][y][x] = p->status[z][y-1][x];
                p->status[z][y-1][x] = BLANK;
                p->blank.y = y-1;
            }
            break;
        case LEFT:
            if (x <= 0)
            {
                return;
            }
            else {
                if (p->status[z][y][x-1] == BARRIER)
                {
                    return;
                }
                p->status[z][y][x] = p->status[z][y][x-1];
                p->status[z][y][x-1] = BLANK;
                p->blank.x = x-1;
            }
            break;
        case RIGHT:
            if (x >= 2)
            {
                return;
            }
            else {
                if (p->status[z][y][x+1] == BARRIER)
                {
                    return;
                }
                p->status[z][y][x] = p->status[z][y][x+1];
                p->status[z][y][x+1] = BLANK;
                p->blank.x = x+1;
            }
            break;
        case FORWARD:
            if (z <= 0)
            {
                return;
            }
            else {
                if (p->status[z-1][y][x] == BARRIER)
                {
                    return;
                }
                p->status[z][y][x] = p->status[z-1][y][x];
                p->status[z-1][y][x] = BLANK;
                p->blank.z = z-1;
            }
            break;
        case BACK:
            if (z >= 2)
            {
                return;
            }
            else {
                if (p->status[z+1][y][x] == BARRIER)
                {
                    return;
                }
                p->status[z][y][x] = p->status[z+1][y][x];
                p->status[z+1][y][x] = BLANK;
                p->blank.z = z+1;
            }
            break;
    };
    p->h = h(p, target);
    (p->g)++;
    p->f = p->h + p->g;
    p->parent = current_status;
    if (CLOSE.find(*p) == CLOSE.end())
    {
        OPEN.push(*p);
    }
    return;
}

int A_star(Node *start, Node *target) {
    start->g = 0;
    start->h = h(start, target);
    start->f = start->g + start->h;
    OPEN.push(*start);
    int step = 0;
    // cout << start->g << start->h << start->f <<endl;
    while (!OPEN.empty()&&step < 10) {
        Node p;
        p = OPEN.top();
        cout << "step = " << step++ << endl;
        cout << "g = " << p.g << " h = " << p.h << " f = " << p.f <<endl;
        cout << "blank x = " << p.blank.x << " blank y = " << p.blank.y << " blank z = " << p.blank.z <<endl;
        print_status(&p);
        OPEN.pop();
        CLOSE.insert(pair<Node, int>(p, p.f));
        if (h(&p, target) == 0)
        {
            cout << "done!" << endl;
            return 0;
        }
        for (int i = 0; i < 6; ++i)
        {
            move_blank(&p, (direction)i, target);
        }
    }
    cout << "no solution!" << endl;
    while (!OPEN.empty()) {
        Node p = OPEN.top();
        cout << "g = " << p.g << " h = " << p.h << " f = " << p.f <<endl;
        cout << "blank x = " << p.blank.x << " blank y = " << p.blank.y << " blank z = " << p.blank.z <<endl;
        OPEN.pop();
    }
    return 0;
}

void read_file2node(string fname, Node * node){
    ifstream fin(fname);
    string s;
    int x = 0;
    int y = 0;
    int z = 0;
    while (getline(fin, s)) {
        if (s != "\r")
        {
            istringstream ss(s);
            ss >> node->status[z][y][0] >> node->status[z][y][1] >> node->status[z][y][2];
            y ++;
        }
        else{
            y = 0;
            z ++;
        }
    }
    print_status(node);
    return;
}

void find_blank(Node *node){
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            for (int k = 0; k < 3; ++k)
            {
                if (node->status[i][j][k] == 0)
                {
                    node->blank.x = k;
                    node->blank.y = j;
                    node->blank.z = i;
                }
            }
        }
    }
    return;
}

int main(int argc, char const *argv[])
{
    Node *target, *begin;
    target = new Node();
    begin = new Node();
    string fname = "source.txt";
    cout << "source: " << endl;
    read_file2node(fname, begin);
    find_blank(begin);
    fname = "target.txt";
    cout << "target: " << endl;
    read_file2node(fname, target);
    find_blank(target);
    A_star(begin, target);
    return 0;
}


