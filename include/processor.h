#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();

  // TODO: Declare any necessary private members
 private:
  // Declaring the Previous total and Previous Idle times for calculation of CPU usage
  float PrevTotalTime;
  float PrevIdleTime;
};

#endif