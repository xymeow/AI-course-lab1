#include <iostream>
#include <queue>
#include <map>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>
#include <time.h>

using namespace std;

const int BLANK = 0;
const int BARRIER = -1;
const long int MAXSTEP = 10000000000;
const int MAXLEN = 200;

clock_t start_time, finish_time;

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
    int status[3][3][3];
    Node *parent;
    Node *next;
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

// priority_queue <Node*, vector<Node*>, cmp> OPEN;
Node OPEN[MAXLEN+1];
map<Node, int> CLOSE;
// map<int, Point> target_position;
Point target_position[27];
int currentf = 0;

void insert (Node *node) {
    Node *tmp = &OPEN[node->f];
    node->next = tmp->next;
    tmp->next = node;
    if (node->f < currentf)
        currentf = node->f;
}

Node *top() {
    while  (OPEN[currentf].next == NULL && currentf < MAXLEN)
        currentf ++;
    return OPEN[currentf].next;
}

void pop() {
    Node *p = OPEN[currentf].next;
    if (p)
        OPEN[currentf].next = p->next;
        p = NULL;
        // free(p);
    return;
}

void print_status(Node *current_status) {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                cout<<current_status->status[i][j][k]<<' ';
            }      
            cout<<endl;  
        }
        cout<<endl;
    }
}

int h2(Node *current_status, Node *target) {
    int dist = 0;
    for (int i = 0; i < 3; ++i) 
        for (int j = 0; j < 3; ++j) 
            for (int k = 0; k < 3; ++k) {
                int num = current_status->status[i][j][k];
                if (num == BARRIER)
                    continue;
                // map<int, Point>::iterator iter;
                // iter = target_position.find(num);
                Point p = target_position[num];
                dist += abs(p.x-k) + abs(p.y-j) + abs(p.z-i);
            }
    return dist;
}

void copy_status(const Node *from, Node *to){
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k)
                to->status[i][j][k] = from->status[i][j][k];
}

void move_blank(const Node *current_status, direction dire, Node *target, direction lst_move) {
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
    p->movement = dire;
    int h = current_status->h;
    Point tp, tb;
    p->next = NULL;
    // int dist;
    switch (dire){
        case DOWN: 
            if (y >= 2 || lst_move == UP||p->status[z][y+1][x] == BARRIER)
                return;
            else {
                p->status[z][y][x] = p->status[z][y+1][x];
                p->status[z][y+1][x] = BLANK;
                p->blank.y = y+1; 
                tp = target_position[p->status[z][y][x]];
                tb = target_position[0];
                if (y+1 <=tp.y) 
                    h ++;
                else
                    h --;
                if (y >= tb.y)
                    h ++;
                else
                    h --;
            }
            break;
        case UP:
            if (y <= 0 || lst_move == DOWN || p->status[z][y-1][x] == BARRIER)
                return;
            else {
                p->status[z][y][x] = p->status[z][y-1][x];
                p->status[z][y-1][x] = BLANK;
                p->blank.y = y-1;
                tp = target_position[p->status[z][y][x]];
                tb = target_position[0];
                if (y-1 >=tp.y) 
                    h ++;
                else
                    h --;
                if (y <= tb.y)
                    h ++;
                else
                    h --;
            }
            break;
        case LEFT:
            if (x <= 0 || lst_move == RIGHT || p->status[z][y][x-1] == BARRIER)
                return;
            else {
                p->status[z][y][x] = p->status[z][y][x-1];
                p->status[z][y][x-1] = BLANK;
                p->blank.x = x-1;
                tp = target_position[p->status[z][y][x]];
                tb = target_position[0];
                if (x-1 >=tp.x) 
                    h ++;
                else
                    h --;
                if (x <= tb.x)
                    h ++;
                else
                    h --;
            }
            break;
        case RIGHT:
            if (x >= 2 || lst_move == LEFT || p->status[z][y][x+1] == BARRIER)
                return;
            else {
                p->status[z][y][x] = p->status[z][y][x+1];
                p->status[z][y][x+1] = BLANK;
                p->blank.x = x+1;
                tp = target_position[p->status[z][y][x]];
                tb = target_position[0];
                if (x+1 <=tp.x) 
                    h ++;
                else
                    h --;
                if (x >= tb.x)
                    h ++;
                else
                    h --;
            }
            break;
        case FORWARD:
            if (z <= 0 || lst_move == BACK || p->status[z-1][y][x] == BARRIER)
                return;
            else {
                p->status[z][y][x] = p->status[z-1][y][x];
                p->status[z-1][y][x] = BLANK;
                p->blank.z = z-1;
                tp = target_position[p->status[z][y][x]];
                tb = target_position[0];
                if (z-1 >=tp.z) 
                    h ++;
                else
                    h --;
                if (z <= tb.z)
                    h ++;
                else
                    h --;
            }
            break;
        case BACK:
            if (z >= 2 || lst_move == FORWARD || p->status[z+1][y][x] == BARRIER)
                return;
            else {
                p->status[z][y][x] = p->status[z+1][y][x];
                p->status[z+1][y][x] = BLANK;
                p->blank.z = z+1;
                tp = target_position[p->status[z][y][x]];
                tb = target_position[0];
                if (z+1 <=tp.z) 
                    h ++;
                else
                    h --;
                if (z >= tb.z)
                    h ++;
                else
                    h --;
            }
            break;
        default: break;
    };
    (p->g)++;
    p->h = h;
    p->f = p->h + p->g;
    
    if (CLOSE.find(*p) == CLOSE.end()) {
        p->parent = (Node*)current_status;
        // OPEN.push(p);
        insert(p);
    }
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

void A_star(Node *start, Node *target) {
    start->g = 0;
    start->h = h2(start, target);
    start->f = start->g + start->h;
    start->parent = NULL;
    start->movement = NONE;
    start->next = NULL;
    currentf = start->f;
    // OPEN.push(start);
    insert(start);
    long int step = 0;
    direction lst_move = NONE;
    // cout << start->g << start->h << start->f <<endl;
    start_time = clock();
    while (top()&&step < MAXSTEP) {
        Node *p;
        p = top();
        // cout << "visit nodes = " << step++ << endl;
        // cout << "g = " << p->g << " h = " << p->h << " f = " << p->f <<endl;
        // cout << "blank x = " << p->blank.x << " blank y = " << p->blank.y << " blank z = " << p->blank.z <<endl;
        pop();
        // OPEN.pop();
        CLOSE.insert(pair<Node, int>(*p, p->f));
        if (h2(p, target) == 0) {
            finish_time = clock();
            cout << "done!" << endl;
            print_path(p);
            cout << "time = " << (double)(finish_time - start_time) / 1000 << "ms" <<endl;
            return;
        }
        if (p)
            lst_move = p->movement;
        for (int i = 0; i < 6; ++i)
            move_blank(p, (direction)i, target, lst_move);
    }
    cout << "no solution!" << endl;
    return;
}

void read_file2node(string fname, Node * node) {
    ifstream fin(fname);
    string s;
    int x = 0;
    int y = 0;
    int z = 0;
    while (getline(fin, s)) {
        if (s != ""&&s != "\r") {
            istringstream ss(s);
            ss >> node->status[z][y][0] >> node->status[z][y][1] >> node->status[z][y][2];
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
                if (node->status[i][j][k] == 0) {
                    node->blank.x = k;
                    node->blank.y = j;
                    node->blank.z = i;
                }
    return;
}

void creat_position(Node *node){
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            for (int k = 0; k < 3; ++k) {
                Point p = {k, j, i};
                // target_position.insert(pair<int, Point>(node->status[i][j][k], p));
                int num = node->status[i][j][k];
                if (num == BARRIER)
                    continue;
                target_position[num] = p;
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
    creat_position(target);
    // map<int, Point>::iterator iter;
    // for (iter = target_position.begin(); iter != target_position.end(); ++iter)
    // {
    //     cout<<"key " <<iter->first << " value:" <<iter->second.x<<iter->second.y<<iter->second.z <<endl;
    // }
    A_star(begin, target);  
    delete target;
    delete begin;
    return 0;
}


