#include "Dxl.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;
int main(int argc, char *argv[])
{
  string file = "/dev/ttyUSB";
  vector<string> ports = get_available_ports(file);
  if(ports.size() == 0)
  {  
    std::cout << "No device connected\n";
    return 0;
  }
  Dxl dxl(ports[0]);
  vector<int> ids = dxl.scan(25);
  ifstream fd;
  fd.open("data.txt");
  map<int,float> inps;
  string inp;
  float temp;
  int i = 1;
  while(!fd.eof()){
    inps.clear();
    getline(fd, inp);
    stringstream s(inp);
    i = 1;
    while(s >> temp)
      inps.insert(pair<int, float> (i++, temp));
    for(auto &i : inps){
      cout << i.second << " ";
    }
    cout << endl;
    dxl.set_goal_position(inps);
  }
  
  return 0;
}
