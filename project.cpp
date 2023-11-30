// CpE 3110 FS23 Honors Project
// File: project.cpp
// Author: Nathan Mejia
#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include "ibmFloat.h"

int main()
{
  IBMFloat num1, num2;
  bool subtraction;
  std::string input;
  std::stringstream native_flt;

  do
  {
    std::cin >> input;
    num1 = IBMFloat(input);

    std::cin >> input;
    if (input == "+")
      subtraction = 0;
    else if (input == "-")
      subtraction = 1;
    else
      break;

    std::cin >> input;
    num2 = IBMFloat(input);

    if (subtraction)
    {
      std::cout<<num1<<" - "<<num2<<" = "<<(num1-num2)<<"\n";
      native_flt<<num1.toNativeFloat()<<" - "
                <<num2.toNativeFloat()<<" = "
                <<(num1-num2).toNativeFloat()<<"\n";
    }
    else
    {
      std::cout<<num1<<" + "<<num2<<" = "<<(num1+num2)<<"\n";
      native_flt<<num1.toNativeFloat()<<" + "
                <<num2.toNativeFloat()<<" = "
                <<(num1+num2).toNativeFloat()<<"\n";
    }
  } while (!std::cin.eof());

  std::cout << native_flt.str();

  return 0;
}
