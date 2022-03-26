#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

bool file_exists(const string &name) {
  ifstream f(name.c_str());
  return f.good();
}

int main(int argc, char **argv) {
  string input, data = "";

  while(open("lockfile", O_CREAT|O_EXCL, 0) == -1) {
    cout<<"Serwer zajety. Prosze czekac"<<endl;
    sleep(5);
  }

  cout<<"Podaj dane"<<endl<<"Q - quit"<<endl<<endl;

  while (true) {
    cin>>input;
    if (input == "Q")
      break;
    data += input + "\n";
  }

  ofstream buffer("server_buffer");
  buffer<<argv[1]<<endl;
  buffer<<data;
  buffer.close();
  cout<<endl<<endl;

  while(!file_exists(argv[1])) {
    fflush(stdout);
    cout<<"Czekam na odpowiedz serwera"<<endl;
    sleep(5);
  }

  data = "";
  ifstream client_file(argv[1]);
  while (getline(client_file, input)) {
    data += input + '\n';
  }
  client_file>>data;
  client_file.close();
  remove(argv[1]);
  cout<<data<<endl<<endl;

  return 0;
}
