#include <iostream>
#include <fstream>

using namespace std;

bool is_empty(ifstream& file) {
    return file.peek() == ifstream::traits_type::eof();
}

int main() {
    int x, y;

    cout<<"Input number: ";
    cin>>x;

    ofstream new_file;
    ifstream result_read;
    ofstream result_clear;

    new_file.open("dane.txt");
    new_file<<x;
    new_file.close();

    while(true) {
        result_read.open("wyniki.txt");
        if(!is_empty(result_read)) {
            result_read>>y;
            cout<<"Result: "<<y;

            result_clear.open("wyniki.txt", ios::out | ios::trunc);
            result_clear.close();

            break;
        }

        result_read.close();
    }

    return 0;
}
