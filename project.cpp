// CpE 3110 FS23 Honors Project
// File: project.cpp
// Author: Nathan Mejia
#include <iostream>
#include <string>
#include <bitset>
#include "ibmFloat.h"

int main()
{
  IBMFloat num1 = IBMFloat("0x40C00000");
  IBMFloat num2 = IBMFloat("0xC0400000");
  IBMFloat sum = num1 + num2;

  std::cout<<num1.toNativeFloat()<<std::endl;
  std::cout<<num1.sign()<<" "<<num1.exp()<<" "<<num1.mant()<<std::endl;

  std::cout<<std::endl<<num2.toNativeFloat()<<std::endl;
  std::cout<<num2.sign()<<" "<<num2.exp()<<" "<<num2.mant()<<std::endl;

  std::cout<<std::endl<<sum.toNativeFloat()<<std::endl;
  std::cout<<sum.sign()<<" "<<sum.exp()<<" "<<sum.mant()<<std::endl;
  return 0;
}
