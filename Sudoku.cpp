#include<iostream>
#include<ctime>

using namespace std;

int main() {
    int a[9][9];
    int c;
    int i, j;
    for(i=1; i<10; i++) {
        cout << "|" ;
        for(j=1; j<10; j++) {
            c = rand()%9 + 1;
            cout << c << " | ";
        }
        cout << endl;
    }

    return 0;
}