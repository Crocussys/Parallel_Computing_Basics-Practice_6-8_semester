#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

void frame(int size){
    for (int j = 0; j < size + 2; j++){
        if (j == 0 || j == size + 1){
            cout << "+";
        }else{
            cout << "--";
        }
    }
    cout << endl;
}

void print(vector<vector<int>> arr){
    cout << "\x1B[2J\x1B[H";
    frame(arr[0].size());
    for (int i = 0; i < arr.size(); i++){
        cout << "|";
        for (int j = 0; j < arr[i].size(); j++){
            if (arr[i][j] == 1){
                cout << "■ ";
            }else{
                cout << "  ";
            }
        }
        cout << "|" << endl;
    }
    frame(arr[arr.size() - 1].size());
}

int main()
{
    int n = 50, m = 100;
    vector<vector<int>> arr;
    for (int i = 0; i < n; i++){
        arr.push_back({});
        for (int j = 0; j < m; j++){
            arr[i].push_back(rand() % 2);
        }
    }

    print(arr);
    this_thread::sleep_for(chrono::seconds(3));

    while (true){
        auto begin_time = chrono::steady_clock::now();
        vector<vector<int>> new_arr = arr;
#pragma omp parallel for
        for (int i = 0; i < n; i++){
#pragma omp parallel for
            for (int j = 0; j < m; j++){
                int i_ = (i + 1) % n;
                int j_ = (j + 1) % m;
                int _i = i - 1;
                if (_i == -1) _i = n - 1;
                int _j = j - 1;
                if (_j == -1) _j = m - 1;
                int sum = arr[_i][_j] + arr[_i][j] + arr[_i][j_] + arr[i][_j] + arr[i][j_] + arr[i_][_j] + arr[i_][j] + arr[i_][j_];
                if (arr[i][j] == 1){
                    if (sum < 2 || sum > 3){
                        new_arr[i][j] = 0;
                    }else{
                        new_arr[i][j] = 1;
                    }
                }else{
                    if (sum == 3){
                        new_arr[i][j] = 1;
                    }else{
                        new_arr[i][j] = 0;
                    }
                }
            }
        }
        arr = new_arr;

        auto end_time = chrono::steady_clock::now();
        auto elapsed_ms = chrono::duration_cast<chrono::nanoseconds>(end_time - begin_time);
        print(arr);
        cout << elapsed_ms.count() << "ns" << endl;
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    return 0;
}
