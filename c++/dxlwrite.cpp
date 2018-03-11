#include "Dxl.h"
#include <vector>
#include <iostream>

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
  int pos = 100;
  dxl.write(9, 100.0, 300);
  dxl.write(17, -100.0, 300);
  return 0;
}
