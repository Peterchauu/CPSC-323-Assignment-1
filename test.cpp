#include <iostream>

using namespace std;

enum name {
    NUM1 = 100, NUM2 = 200, NUM3 = 300
};

struct test
{
    int value;
    std::string name;
};



int main() {

    test x[3];
    x[0].value = 100;
    x[0].name = NUM1;
    x[1].value = 200;
    x[1].name = NUM2;
    x[2].value = 300;
    x[2].name = NUM3;


    int testvalue;
    cout << "Enter: ";
    cin >> testvalue;

    switch(testvalue) {
        case NUM1:
            cout << "1\n";
            break;
        case NUM2:
            cout << "2\n";
            break;
        case NUM3:
            cout << "3\n";
    }
    return 0;
}