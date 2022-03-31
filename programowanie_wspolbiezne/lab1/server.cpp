#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

bool is_empty(ifstream& file) {
    return file.peek() == ifstream::traits_type::eof();
}

int main() {
    int x, y;
    ifstream input_read;
    ofstream input_clear;
    ofstream output_write;

    while(true) {
        input_read.open("dane.txt");

        if (!is_empty(input_read)) {
            input_read>>x;
            // (2x - 10)^2
            y = 4*(pow(x, 2)) - (40*x) + 100;

            // clear file dane.txt
            input_clear.open("dane.txt", ios::out | ios::trunc);
            input_clear.close();

            // save result to wyniki.txt
            output_write.open("wyniki.txt");
            output_write<<y;
            output_write.close();
        }

        input_read.close();
    }

    return 0;
}
