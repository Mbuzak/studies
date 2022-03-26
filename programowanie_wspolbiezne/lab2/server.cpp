#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

bool file_exists(const string &name) {
  ifstream f(name.c_str());
  return f.good();
}

int main() {
  while (true) {
    if (file_exists("server_buffer")) {
      string input, data = "";
      ifstream buffer("server_buffer");
      getline(buffer, input);
      string client_filename = input;
      while (getline(buffer, input)) {
        data += input + "\n";
      }
      buffer.close();
      cout<<data<<endl<<endl;
      cout<<"Podaj odpowiedz, Q - quit"<<endl<<endl;

      string answer;
      data = "";
      while (true) {
        cin>>answer;
        if (answer == "Q")
          break;
        data += answer + "\n";
      }
      ofstream client_file(client_filename);
      client_file<<data;
      client_file.close();

      remove("lockfile");
      remove("server_buffer");
    }
    else {
      fflush(stdout);
      cout<<"Brak kolejki"<<endl;
      sleep(5);
    }
  }

  return 0;
}
