#include "Dxl.h"
#include <vector>
#include <iostream>

using namespace std;
int main(int argc, char *argv[])
{
  string file = "/dev/ttyUSB";
  vector<string> ports = get_available_ports(file);
  Dxl dxl(ports[0]);
  vector<int> ids = dxl.scan(25);
  for(auto &i : ids){
    dxl.write(i, 0.0, 100);
    std::cout << dxl.get_present_speed(i) << endl;
  }
  return 0;
}
