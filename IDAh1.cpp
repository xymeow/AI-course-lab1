#include <iostream>
#include <queue>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <time.h>

using namespace std;

const char BLANK = 2;
const char BARRIER = 1;
const long int MAXSTEP = 10000000000;
const int LIMIT = 100;
const long int MAXVISIT0 = 100000;
long int visited = 0;
clock_t start_time, end_time;

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    FORWARD,
    BACK,
    NONE
};

typedef struct Point {
    int x;
    int y;
    int z;
}Point;

typedef struct Node {
    char status[28];
    Node *parent;
    int f;
    int g;
    int h;
    Point blank;
    direction movement;
    friend bool operator < (const Node a, const Node b) {
        return a.f >= b.f;
    }
}Node, *pNode;

struct cmp {
    bool operator()(const pNode &n1, const pNode &n2) const {
        return n1->f >= n2->f;
    }
};

int position(int x, int y, int z) {
    return x + y*3 + 9*z;
}

// priority_queue <Node*, vector<Node*>, cmp> OPEN;
// map<Node, int> CLOSE;
stack<Node*> IDAStack;

void print_status(Node *current_status) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cout<<int(current_status->status[position(k, j, i)])-2<<' ';
            }
            cout<<endl;
        }
        cout<<endl;
    }
}

int h1(Node *current_status, Node *target) {
    int count = 0;
    for (int i = 0; i < 27; i++)
        if (current_status->status[i] != target->status[i])
            count ++;
    return count;
}

void copy_status(const Node *from, Node *to){
    strcpy(to->status, from->status);
    return;
}

void move_blank(const Node *current_status, direction dire, Node *target, direction lst_move, Node VISIT[]) {
    Node *p = &VISIT[visited];
    //    p = new Node();
    visited ++;

    int x = current_status->blank.x;
    int y = current_status->blank.y;
    int z = current_status->blank.z;
    p->blank.x = x;
    p->blank.y = y;
    p->blank.z = z;
    copy_status(current_status, p);
    p->g = current_status->g;
    p->movement = dire;
    switch (dire){
        case DOWN:
            if (y >= 2 || lst_move == UP || p->status[position(x, y+1, z)] == BARRIER)
                return;
            else {
                p->status[position(x, y, z)] = p->status[position(x, y+1, z)];
                p->status[position(x, y+1, z)] = BLANK;
                p->blank.y = y+1;
            }
            break;
        case UP:
            if (y <= 0 || lst_move == DOWN || p->status[position(x, y-1, z)] == BARRIER)
                return;
            else {
                p->status[position(x, y, z)] = p->status[position(x, y-1, z)];
                p->status[position(x, y-1, z)] = BLANK;
                p->blank.y = y-1;
            }
            break;
        case LEFT:
            if (x <= 0 || lst_move == RIGHT || p->status[position(x-1, y, z)] == BARRIER)
                return;
            else {
                p->status[position(x, y, z)] = p->status[position(x-1, y, z)];
                p->status[position(x-1, y, z)] = BLANK;
                p->blank.x = x-1;
            }
            break;
        case RIGHT:
            if (x >= 2 || lst_move == LEFT || p->status[position(x+1, y, z)] == BARRIER)
                return;
            else {
                p->status[position(x, y, z)] = p->status[position(x+1, y, z)];
                p->status[position(x+1, y, z)] = BLANK;
                p->blank.x = x+1;
            }
            break;
        case FORWARD:
            if (z <= 0 || lst_move == BACK || p->status[position(x, y, z-1)] == BARRIER)
                return;
            else {
                p->status[position(x, y, z)] = p->status[position(x, y, z-1)];
                p->status[position(x, y, z-1)] = BLANK;
                p->blank.z = z-1;
            }
            break;
        case BACK:
            if (z >= 2 || lst_move == FORWARD || p->status[position(x, y, z+1)] == BARRIER)
                return;
            else {
                p->status[position(x, y, z)] = p->status[position(x, y, z+1)];
                p->status[position(x, y, z+1)] = BLANK;
                p->blank.z = z+1;
            }
            break;
        default: break;
    };
    p->h = h1(p, target);
    (p->g)++;
    p->f = p->h + p->g;
    
    // if (CLOSE.find(*p) == CLOSE.end()) {
    p->parent = (Node*)current_status;
    //     OPEN.push(p);
    // }
    IDAStack.push(p);
    return;
}

void print_movement (Node *node) {
    switch (node->movement) {
        case UP: cout << "U" << endl; break;
        case DOWN: cout << "D" << endl; break;
        case LEFT: cout << "L" << endl; break;
        case RIGHT: cout << "R" << endl; break;
        case FORWARD: cout << "F" <<endl; break;
        case BACK: cout << "B" << endl; break;
        case NONE: break;
        default: break;
    };
    return;
}

void print_path(Node *end){
    Node *p = end;
    Node *q;
    int step = 0;
    stack<Node*> Stack;
    while (p) {
        // cout << "step = " << ++step << endl;
        // cout << "g = " << p->g << " h = " << p->h << " f = " << p->f <<endl;
        // print_status(p);
        Stack.push(p);
        p = p->parent;
    }
    while (!Stack.empty()) {
        q = Stack.top();
        if (q->movement != NONE)
            cout << "step = " << ++step << endl;
        print_movement(q);
        print_status(q);
        Stack.pop();
    }
}

// bool dfs(Node * current_status);

void IDA(Node *start, Node *target) {
    start->g = 0;
    start->h = h1(start, target);
    start->f = start->g + start->h;
    start->parent = NULL;
    start->movement = NONE;
    direction lst_move = NONE;
    int limit = start->h;
    // stack<Node*> Stack;
    // int step = 0;
    Node *VISIT = new Node[MAXVISIT0];
    int MAXVISIT;
    MAXVISIT = MAXVISIT0;
    start_time = clock();
    while (limit < LIMIT) {
        int next_limit = LIMIT;
        // limit ++;
        IDAStack.push(start);
        while (!IDAStack.empty()) {

            Node *p = IDAStack.top();
        // cout << "visit nodes = " << step++ << endl;
        // cout << "g = " << p->g << " h = " << p->h << " f = " << p->f <<endl;
        // cout << "blank x = " << p->blank.x << " blank y = " << p->blank.y << " blank z = " << p->blank.z <<endl;
            IDAStack.pop();
            if (h1(p, target) == 0) {
                end_time = clock();
                cout << "done" << endl;
                print_path(p);
                cout << "time = " << double(end_time - start_time)/1000 << "ms" <<endl;
                return;
            }
            else {
                if (limit >= p->f) {
                    if (p)
                        lst_move = p->movement;
                    for (int i = 0; i < 6; i++) {
                        if (visited >= MAXVISIT) {
            //                delete [] VISIT;
                            MAXVISIT *= 2;
                            VISIT = new Node[MAXVISIT];
                        }
                        move_blank(p, (direction)i, target, lst_move, VISIT);
                    }
                }
                else
                    next_limit = min(p->f, next_limit);
            }
        }
        limit = next_limit;
    }
    cout << "no solution!" << endl;
    return;
}

void read_file2node(string fname, Node * node) {
    ifstream fin(fname);
    string s;
    int a, b, c;
    int y = 0;
    int z = 0;
    while (getline(fin, s)) {
        if (s != ""&&s != "\r") {
            istringstream ss(s);
            ss >> a>>b>>c;
            a +=2;b+=2;c+=2;
            node->status[position(0, y, z)] = a;
            node->status[position(1, y, z)] = b;
            node->status[position(2, y, z)] = c;
            y ++;
        }
        else {
            y = 0;
            z ++;
        }
    }
    print_status(node);
    return;
}

void find_blank(Node *node){
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                if (node->status[position(k, j, i)] == 2) {
                    node->blank.x = k;
                    node->blank.y = j;
                    node->blank.z = i;
                }
    return;
}

int main(int argc, char const *argv[]) {
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
    IDA(begin, target);  
    delete target;
    delete begin;
    return 0;
}
