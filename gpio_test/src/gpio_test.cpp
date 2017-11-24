//============================================================================
// Name        : gpio_test.cpp
// Author      : Yusuf Bülbül
// Version     :
// Copyright   : Your copyright notice
// Description : blinky in C++, Ansi-style
//============================================================================

#include <iostream>
#include <wiringPi.h>
using namespace std;

int main (void)
{

  wiringPiSetup () ;

  pinMode (0, OUTPUT) ;
  pinMode (1, OUTPUT) ;
  pinMode (2, OUTPUT) ;

  while(true)
  {

    digitalWrite (0, HIGH);
    digitalWrite (2,  LOW);
    delay (1000);

    digitalWrite (1, HIGH);
    digitalWrite (0,  LOW);
    delay (1000);

    digitalWrite (2, HIGH);
    digitalWrite (1,  LOW);
    delay (1000);

  }

  return 0 ;

}
