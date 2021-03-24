#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid):pid_(pid){};


int Process::Pid() { return  pid_; }


float Process::CpuUtilization() { 
  cpu_utilization_ = LinuxParser::CpuUtilization(pid_);
  return cpu_utilization_; }

string Process::Command() { return LinuxParser::Command(pid_) ; }


string Process::Ram() { return LinuxParser::Ram(pid_) ; }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_) ; }

bool Process::operator<(Process const& a) const { 
  
  return (this->cpu_utilization_>a.cpu_utilization_) ? true :false; }