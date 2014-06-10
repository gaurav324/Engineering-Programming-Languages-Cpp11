#include <vector>
#include <iostream>

using namespace std;

void print(vector<int>& v) {
    for (int i=0; i<13; ++i) {
        cout << v[i] << endl;
    }
}

void bubble_sort(vector<int>& v) {
    for (int i = 0; i != v.size(); ++i) {
        auto max = v.begin();
        auto q = v.begin();
        for (q = v.begin(); q != v.end() - i; ++q) {
             if (*q > *max) {
                max = q;
             }
        }
        --q;
        int temp = *max;
        *max = *q;
        *q = temp;
    }
}

int main() {
    int arr[13] = {10, 34, 84, 52, 8, 99, 22, 29, 11, 35, 75, 44, 68};
    vector<int> v;

    for (int i=0; i<13; ++i) {
        v.push_back(arr[i]);
    }
    
    cout << "Pre-Sort values: " << endl;
    print(v);

    bubble_sort(v);

    cout << "Post-Sort values: " << endl;
    print(v);
}
