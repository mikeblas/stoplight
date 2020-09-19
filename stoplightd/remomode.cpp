
#include "remomode.h"


void RemoMode::operator()()
{
   log << log.critical << "RemoMode starting" << std::endl;
   lights.AllOff();
}

