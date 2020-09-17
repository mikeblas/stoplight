
#include "selectormode.h"


void SelectorMode::operator()()
{

   using namespace std::chrono_literals;

   mode = 0;

   while (!quitting)
   {
   }
}

void SelectorMode::UpdateLights()
{

   lights.SetGreen((mode & 1) >> 0);
   lights.SetYellow((mode & 2) >> 1);
   lights.SetRed((mode & 4) >> 2);
}

