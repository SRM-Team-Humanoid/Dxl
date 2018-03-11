#include "Dxl.h"
#include <string>
#include <iostream>
#include <unistd.h>
// Control table address
#define ADDR_MX_TORQUE_ENABLE           24                  // Control table address is different in Dynamixel model
#define ADDR_MX_GOAL_POSITION           30
#define ADDR_MX_PRESENT_POSITION        36
#define ADDR_MX_MOVING_SPEED            32
//Communication Macros
#define COMM_SUCCESS 0
#define COMM_TX_FAIL -1001
// Protocol version
#define PROTOCOL  1.0
#define TORQUE  1
#define DXL_MINIMUM_POSITION_VALUE  100
#define DXL_MAXIMUM_POSITION_VALUE  4000
#define DXL_MOVING_STATUS_THRESHOLD 10
#define OFFSET 2048
#define BAUDRATE 1000000
//Available Ports Works
std::vector<std::string> get_available_ports(std::string file){
  std::vector<std::string> temp;
  int fd;
  std::string port = "";
  for(int i = 0;i < 256;i++){
    port.clear();
    port.append(file);
    port.append(std::to_string(i));
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if(fd != -1)
      temp.push_back(port);
  }
  return temp;
}

Dxl::Dxl(std::string _port){
  const char* port = _port.c_str();
  portHandler = dynamixel::PortHandler::getPortHandler(port);
  packetHandler = dynamixel::PacketHandler::getPacketHandler(PROTOCOL);
  portHandler->openPort(); 
  portHandler->setBaudRate(BAUDRATE);
  dxl_comm_result = COMM_TX_FAIL;
  dxl_error = 0;
  dxl_present_pos = 0;
}
//Scan Works
std::vector<int> Dxl::scan(int range = 254){
  std::vector<int> res;
  uint16_t dxl_model;
  for(int i = 0;i <= range;++i){
    dxl_comm_result = packetHandler->ping(portHandler, i, &dxl_model, &dxl_error);
    if (dxl_comm_result != COMM_SUCCESS)
    {
      ;
    }
    else if (dxl_error != 0)
    {
      ;
    }
    else
      res.push_back(i);
  }
  return res;
}

//Write Works
int Dxl::_write(int DXL_ID, float POS){
  dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, DXL_ID, ADDR_MX_TORQUE_ENABLE, TORQUE, &dxl_error);
  if(dxl_comm_result != COMM_SUCCESS)
  {
    std::cout << packetHandler->getTxRxResult(dxl_comm_result) << std::endl;
    return -2;
  }
  if(dxl_error != 0)
  {
    std::cout << packetHandler->getRxPacketError(dxl_error) << std::endl;
    return -2;
  }
  dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_GOAL_POSITION, OFFSET + int(POS / 0.088), &dxl_error);
  if(dxl_comm_result != COMM_SUCCESS)
  {
    std::cout << packetHandler->getTxRxResult(dxl_comm_result) << std::endl;
    return -1;
  }
  if(dxl_error != 0)
  {
    std::cout << packetHandler->getRxPacketError(dxl_error) << std::endl;
    return -1;
  }
  return 0;
}
//Read Works
float Dxl::_read(int DXL_ID){
  dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, ADDR_MX_PRESENT_POSITION,&dxl_present_pos, &dxl_error);
  if(dxl_comm_result != COMM_SUCCESS)
  {
    std::cout << packetHandler->getTxRxResult(dxl_comm_result) << std::endl;
    return -1;
  }
  else if(dxl_error != 0)
  {
    std::cout << packetHandler->getRxPacketError(dxl_error) << std::endl;
    return -1;
  }
  return float(dxl_present_pos - 2048) * 0.088;
}

int Dxl::_get_present_speed(int DXL_ID){
  uint16_t dxl_speed;
  dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, DXL_ID, 32, &dxl_speed, &dxl_error);
  if(dxl_comm_result != COMM_SUCCESS)
  {
    std::cout << packetHandler->getTxRxResult(dxl_comm_result) << std::endl;
    return -1;
  }
  else if(dxl_error != 0)
  {
    std::cout << packetHandler->getRxPacketError(dxl_error);
    return -2;
  }
  return dxl_speed;
}

int Dxl::_set_moving_speed(int DXL_ID, int speed){
  dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, DXL_ID, ADDR_MX_MOVING_SPEED, int(float(speed) / 1000 * 1024), &dxl_error);
  if(dxl_comm_result != COMM_SUCCESS)
  {
    std::cout << packetHandler->getTxRxResult(dxl_comm_result) << std::endl;
    return -1;
  }
  else if(dxl_error != 0)
  {
    std::cout << packetHandler->getRxPacketError(dxl_error);
    return -2;
  }
  return 0;
}

int Dxl::set_goal_position(std::map<int, float> a){
  for( auto &i : a){
    this->_write(i.first, i.second);
  }
  return 0;
}

std::map<int, float> Dxl::get_present_position(int n){
  std::map<int, float> res;
  float pos;
  for(int i = 0;i <= n;i++)
  {
    pos = this->_read(i);
    if(pos >= 0)
      res.insert(std::pair<int, float> (i, pos));
  }
  return res;
}

int Dxl::set_moving_speed(std::map<int, int> m){
  for(auto &i : m){
    this->_set_moving_speed(i.first, i.second);
  }
  return 0;
}

std::map<int, int> Dxl::get_present_speed(int k){
  int sp;
  std::map<int, int> res;
  for(int i = 0;i <= k;i++){
    sp = this->_get_present_speed(i);
    if(sp >= 0)
      res.insert(std::pair<int, int>(i, sp));
  }
  return res;
}
