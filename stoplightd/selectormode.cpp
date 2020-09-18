
#include "selectormode.h"


void SelectorMode::operator()()
{

   // update lights for the first time,
   // and after that we have no work to actively do
   mode = 0;
   UpdateLights();
}

void SelectorMode::UpdateLights()
{

   lights.SetGreen((mode & 1) >> 0);
   lights.SetYellow((mode & 2) >> 1);
   lights.SetRed((mode & 4) >> 2);
}

