#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  // define this fucntion to give out a format of HH:MIN:SEC
  
  int minutes = int((seconds/(60))%60);
  int sec = int(seconds%(60));
  int hours = int(minutes/(60*60));
  
  string formatted_time = std::to_string(hours)+":"+std::to_string(minutes)+":"+std::to_string(sec);
  
  return formatted_time; }