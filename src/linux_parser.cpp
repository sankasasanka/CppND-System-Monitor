#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  // /proc/meminfo is the location, it has multiple lines. only first 2 lines are needed
  float Mem,MemTotal,MemFree;

  string MemName;
  string line;

  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while(std::getline(stream ,line)){
      std::istringstream linestream(line);
      linestream>>MemName>>Mem;
      if (MemName=="MemTotal:"){
          MemTotal=Mem;
      }
      else if (MemName=="MemFree:"){
          MemFree=Mem;
      }

    }
  }


  return (MemTotal-MemFree)/MemTotal; }


long LinuxParser::UpTime() { 
  //located in kUptimeFilename
  std::string uptime,idletime;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()){
    while(std::getline(stream ,line)){
      std::istringstream linestream(line);
      linestream>>uptime>>idletime;

    }
  }

  return std::stol(uptime); }

// the following functions are not used
//long LinuxParser::Jiffies() { return 0; }
//long LinuxParser::ActiveJiffies(int pid) { return 0; }
//long LinuxParser::ActiveJiffies() { return 0; }
//long LinuxParser::IdleJiffies() { return 0; }


vector<string> LinuxParser::CpuUtilization() { 
   std::string key;
   std::string User,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice;
   // 
  // 0.   1.   2.     3.   4.     5.  6.      7.    8.    9.   
  
  vector<string> cpu_util;	
  std::string line;
  // readin from the file
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key>>User>>nice>>system>>idle>>iowait>>irq>>softirq>>steal>>guest>>guest_nice;
      if (key=="cpu"){
      	cpu_util = {User,nice,system,idle,iowait,irq,softirq,steal,guest,guest_nice};
        return cpu_util; 
      	}
      }
    }
  return cpu_util; }

// Overload CPUUtilization for returning the CPU utilization of each process
float LinuxParser::CpuUtilization(int pid) { 
  string val;
  
  // Convert Uptime in sec 
  long uptime = LinuxParser::UpTime();
  
  long utime,stime,start_time;
  
  float cpu_usage=0.0;
  
  std::string line;
  
  // readin from the file
  std::ifstream stream(kProcDirectory+"/"+std::to_string(pid)+"/"+kStatFilename);
  if(stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      for(int i=1;i<=22;i++){
      	linestream>>val;
        if (i==14){
        	utime = std::stol(val)/sysconf(_SC_CLK_TCK);
        }
        else if (i==15){
        	stime = std::stol(val)/sysconf(_SC_CLK_TCK);
        }
        else if (i==22){
        	start_time = std::stol(val)/sysconf(_SC_CLK_TCK);
        }
       }
      // CPU utilization Calculation
      float total_time = utime +stime;
      float seconds = uptime -(start_time);

      cpu_usage = (total_time/seconds);  
      }
  
  
  	}
  
  return cpu_usage; }


int LinuxParser::TotalProcesses() { 
  std::string key;
  int val;

  std::string line;
  // readin from the file
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key>>val;
      if (key=="processes")
      {
          return val;
      }
        
    }
  }
  return val; }


int LinuxParser::RunningProcesses() { 
  std::string key;
  int val;

  std::string line;
  // readin from the file
  std::ifstream stream(kProcDirectory+kStatFilename);
  if(stream.is_open()){
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key>>val;
      if (key=="procs_running")
      {
          return val;
      }
        
    }
  }
  return val; }


string LinuxParser::Command(int pid) {
  string cmd,line;
  std::ifstream stream(kProcDirectory+"/"+std::to_string(pid)+kCmdlineFilename);
  
  if(stream.is_open()){
    //while(std::getline(stream,line))
    stream>>cmd;
  }
  
  return cmd; }


string LinuxParser::Ram(int pid) { 
  
  string key;
  string line;
  string val;
  string Ram_MB{"0"};
  
  long Mem;
  
  std::ifstream stream(kProcDirectory+"/"+std::to_string(pid)+"/"+kStatusFilename);
  if (stream.is_open()){
  	while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key>>val;
      if (key=="VmSize:"){
        Mem =std::stol(val)/1024;
      	}
  	}
  }
  Ram_MB = std::to_string(Mem);
  
  return Ram_MB;  }

string LinuxParser::Uid(int pid) { 
  
  string key;
  string line;
  string val ="";
  
  std::ifstream stream(kProcDirectory+"/"+std::to_string(pid)+"/"+kStatusFilename);
  if (stream.is_open()){
  	while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream>>key>>val;
      if (key=="Uid:"){
      	return val;
      	}
  	}
  }
  
  
  return val; }

string LinuxParser::User(int pid) { 

  string pid_str = Uid(pid);
  string line, key, value{};
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == pid_str) {
          return value;
        }
      }
    }
  }
  return value;}


long LinuxParser::UpTime(int pid) { 
    
  string line;
  string value;
  long up_1;
  long uptime=0;
  
  long system_uptime = LinuxParser::UpTime();
  
  std::ifstream stream(kProcDirectory+"/"+std::to_string(pid)+"/"+kStatFilename);
  if (stream.is_open()){
  	while (std::getline(stream,line)){
      std::istringstream linestream(line);
      for (int i=0;i<=21;i++){
      	linestream>>value;
      }
     
     up_1 = std::stol(value);
      // Converts Clocks into Secondss
     uptime = up_1/sysconf(_SC_CLK_TCK); 
     return ( system_uptime-uptime);
  	}
    
  }
  return uptime; }
