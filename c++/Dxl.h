#ifndef DXL_H
#define DXL_H
#include "dynamixel_sdk/dynamixel_sdk.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <iterator>
#include <fcntl.h>
class Dxl{
  dynamixel::PortHandler *portHandler;
  dynamixel::PacketHandler *packetHandler;
  int dxl_comm_result;
  uint8_t dxl_error;
  uint16_t dxl_present_pos;

  public:
    Dxl(std::string);
    std::vector<int> scan(int);
    int _write(int, float);
    int set_goal_position(std::map<int, float>);
    float _read(int);
    std::map<int, float> get_present_position(int);
    int _get_present_speed(int);
    std::map<int, int> get_present_speed(int);
    int _set_moving_speed(int, int);
    int set_moving_speed(std::map<int, int>);
};


std::vector<std::string> get_available_ports(std::string);

#endif
