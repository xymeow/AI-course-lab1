#include <iostream>
// #include <queue>
// #include <map>
// #include <unordered_map>
#include <fstream>
#include <string>
#include <sstream>
// #include <stack>
#include <time.h>
#include <stdlib.h>
#include <cmath>
// #include <set>

using namespace std;

int *queen;
int *pdiag;
int *ndiag;
int attack;
int nattack;
int N;
int M;
int barrierx, barriery;

void swap_queens(int i, int j) {
    pdiag[i - queen[i] + N-1] --;
    ndiag[i + queen[i]] --;
    pdiag[j - queen[j] + N-1] --;
    ndiag[j + queen[j]] --;
    
    std::swap(queen[i], queen[j]);
    
    pdiag[i - queen[i] + N-1] ++;
    ndiag[i + queen[i]] ++;
    pdiag[j - queen[j] + N-1] ++;
    ndiag[j + queen[j]] ++;
    
    
}

int conflict() {
    if (N<100)
        return 0;
    else if (N<1000)
        return 30;
    else if (N<10000)
        return 50;
    else if (N<100000)
        return 80;
    else
        return 100;
}

void init_status()
{
    int i, j;
    j = 0;
    for (i = 0; i<N; i++)
     queen[i]=i;
    int rand_index, rand_index2;
    
    
    
    // if (randd)
    // for (i = 0; i < int(N); i++) {
    //     rand_index1 = rand()%N;
    //     rand_index2 = rand()%N;
    //     std::swap(queen[rand_index2],queen[rand_index1]);
    // }
    // cout<<M<<endl;
    M = N - conflict();
    memset(pdiag,0,(2*N-1)*sizeof(int));
    // cout<<"init"<<endl;
    memset(ndiag,0,(2*N-1)*sizeof(int));
    
    for (i = 0, j = N; i < M; i++, j--) {
        do {
            rand_index = i + rand()%j;
        } while (pdiag[i-queen[rand_index]+N-1]>0||ndiag[i+queen[rand_index]]>0);
        swap(queen[i], queen[rand_index]);
        pdiag[i-queen[i]+N-1] ++;
        ndiag[i+queen[i]] ++;
    }

    for (i = M, j = N-M; i < N; i++, j--) {
        rand_index = i + rand()%j;
        swap(queen[i], queen[rand_index]);
        pdiag[i-queen[i]+N-1] ++;
        ndiag[i+queen[i]] ++;
    }

    // for (int i = 0; i < N; i++) {
    //     pdiag[i - queen[i] + N -1]++;
    //     ndiag[i + queen[i]]++;
    // }
    attack = 0;
    for (int i = 0; i < 2*N-1; i++) {
        attack += pdiag[i] > 1 ? pdiag[i] - 1: 0;
        attack += ndiag[i] > 1 ? ndiag[i] - 1: 0;
    }
    // cout<<attack<<endl;
}

bool barrier_center() {
    if (N%2 == 0)
        return false;
    if (barrierx==barriery&&barriery == N/2&&barrierx==queen[barriery])
        return true;
    return false;
}

void print_solution(int N) {
    if (N<=2) {
        cout<<"no solution!"<<endl;
        return;
    }
    if (N == 3) {
        if (barriery==barrierx||barrierx+barriery==2) {
            cout<<"no solution!"<<endl;
            return;
        }
        if (barrierx==0&&barriery==1) {
            cout<<0<<endl;
            cout<<2<<endl;
            cout<<0<<endl;
            return;
        }
        if (barrierx == 1 && barriery == 0) {
            cout<<"0 2"<<endl;
            cout<<-1<<endl;
            cout<<1<<endl;
            return;
        }
        if (barrierx == 1 && barriery == 2) {
            cout<<1<<endl;
            cout<<-1<<endl;
            cout<<"0 2"<<endl;
            return;
        }
        if (barrierx == 2 && barriery == 1) {
            cout<<2<<endl;
            cout<<0<<endl;
            cout<<2<<endl;
            return;
        }
    }
    if (barriery == queen[barrierx])
        for (int i = 0; i < N; i++)
            cout<<N-1-queen[i]<<endl;
    else
    for (int i = 0; i < N; i++)
        cout<<queen[i]<<endl;
}

int evaluate(int i, int j)
{
    int ret = 0;

    // 两个解在同一个条正对角线上
    if (i - queen[i] == j - queen[j])
    {
        ret = (pdiag[i - queen[i] + N -1] > 2 ? 2: 1) + (ndiag[i + queen[i]] > 1 ? 1 : 0) +
               (ndiag[j + queen[j]] > 1 ? 1 : 0) -
               (ndiag[i + queen[j]] > 0 ? 2 : 1) -
               (pdiag[i - queen[j] + N -1] > 0 ? 1: 0) - (pdiag[j - queen[i] + N -1] > 0 ? 1: 0);            

        return ret;
    }

    // 两个解在同一条负对角线上
    if (i + queen[i] == j + queen[j])
    {
        ret = (ndiag[i + queen[i]] > 2 ? 2: 1) + (pdiag[i - queen[i] + N -1] > 1 ? 1 : 0) +
              (pdiag[j - queen[j] + N -1] > 1 ? 1 : 0) -
              (pdiag[i - queen[j] + N -1] > 0 ? 2 : 1) -
              (ndiag[i + queen[j]] > 0 ? 1: 0) - (ndiag[j + queen[i]] > 0 ? 1: 0);               

        return ret;
    }


    // 一般情况，即两个解在不同的对角线上
    ret = (pdiag[i - queen[i] + N -1] > 1 ? 1 : 0) + (ndiag[i + queen[i]] > 1 ? 1 : 0) + 
          (pdiag[j - queen[j] + N -1] > 1 ? 1 : 0) + (ndiag[j + queen[j]] > 1 ? 1 : 0) - 
          (pdiag[i - queen[j] + N -1] > 0 ? 1 : 0) - (ndiag[i + queen[j]] > 0 ? 1 : 0) - 
          (pdiag[j - queen[i] + N -1] > 0 ? 1 : 0) - (ndiag[j + queen[i]] > 0 ? 1 : 0);

    return ret; // ret > 0 表明冲突减少，ret < 0 表示冲突增加
}

void simulated_annealing(int N) {
    queen = new int[N];
    pdiag = new int[2*N-1];
    ndiag = new int[2*N-1];
    // cout<<"allloc"<<endl;
    // bool randd = false;
    // const double dt = 0.0005;
    const int T = 100;
    int time = 0;
    int i, j;
    if (N <= 3) 
        return;
    do {
        init_status();
        if (attack == 0)
            break;
        bool flag = true;
        // randd = true;
        int change;
        while (flag) {
            flag = false;
            for (int i = 0; i < N-1; i++){
                int x = pdiag[i - queen[i] + N -1];
                int y = ndiag[i + queen[i]];
                bool t = (x>1)||(y>1);
                if (t) {
                for (int j = i+1; j < N; j++) {
                        time ++;
                        change = evaluate(i, j);
                        if (change > 0) {
                            flag = true;
                            attack -= change;
                            swap_queens(i, j);
                        }
                        else {
                            double ran = rand()/double(RAND_MAX);
                            if (ran <= exp(change*time/T)) {
                                attack -= change;
                                swap_queens(i, j);
                            }
                        }
                        if (attack == 0&&!barrier_center())
                            return;
                    }
                }
                // cout<<attack<<endl;
            }
            
        }
        // printf("%d\n", attack);
    } while (attack > 0||barrier_center());
}

int main(int argc, char const *argv[])
{
    // int N;
    // int x, y;
    clock_t start, used;
   ifstream fin("input.txt");
   string s;
   getline(fin, s);
   istringstream ss(s);
   ss >> N;
   getline(fin, s);
   istringstream ss2(s);
   ss2 >> barrierx >> barriery;
     // N = 1000000;
    srand((unsigned)time(NULL));
    // N = 5;
    // Nqueen nqueen(N, x, y, 100);
    // cout<<N<<endl;
    start = clock();
    // nqueen.simulated_annealing();
    simulated_annealing(N);
    used = clock() - start;
    // nqueen.print_solution(nqueen.queen);
    freopen("output_simulated_annealing.txt", "w", stdout);
    print_solution(N);
    // cout<<attack<<endl;
    cout<<used/double(1000)<<endl;
    fclose(stdout);
    return 0;
}