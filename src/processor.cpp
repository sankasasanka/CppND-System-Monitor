#include "processor.h"
#include "linux_parser.h"
#include<iostream>

// TODO: Return the aggregate CPU utilization

// Constructor to initialize the Previous util
Processor::Processor(): PrevTotalTime(0.0),PrevIdleTime(0.0){};

float Processor::Utilization() { 
  // Get all the times 
  std::vector<std::string>Values = LinuxParser::CpuUtilization(); // Will be converting the integer location to the assigned CPU timers 
    
  // User nice system idle iowait irq softirq steal guest guest_nice
  // 0.   1.   2.     3.   4.     5.  6.      7.    8.    9.          
   
  float Idle = std::stof(Values[3])+std::stof(Values[4]);
  float NonIdle = std::stof(Values[0])+std::stof(Values[0])+std::stof(Values[5])+std::stof(Values[6])+std::stof(Values[7]);
  
  float Total = Idle+ NonIdle;
  
  // Difference with the previous values 
  float Total_diff = Total - PrevTotalTime;
  float Idle_diff = Idle - PrevIdleTime;
  
  float CPU_use = (Total_diff-Idle_diff)/Total_diff;
    
  // replace the Previous Values
  PrevTotalTime= Total;
  PrevIdleTime = Idle;
  
  return CPU_use; }