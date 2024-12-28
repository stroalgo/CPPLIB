#include <test.h>


namespace Utilities {


  
int multiply(int a, int b)
{
    return a*b;
}

float multiply(float a, float b)
{
    return a*b;
}


int*  leaker(void) {
  return new int();
}


}