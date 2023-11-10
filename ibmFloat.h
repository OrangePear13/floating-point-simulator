// CpE 3110 FS23 Honors Project
// File: floating.h
// Author: Nathan Mejia
#ifndef IBM_FLOAT_H
#define IBM_FLOAT_H

#include <iostream>
#include <string>
#include <bitset>

constexpr unsigned short EXPONENT_LEN = 7                              ;
const unsigned short MANTISSA_LEN = 24                             ;
const unsigned short EXP_BIAS     = 64                             ;
const unsigned short EXP_BASE     = 16                             ;
const unsigned short IBM_LENGTH   = EXPONENT_LEN + MANTISSA_LEN + 1; // sign bit

class IBMFloat
{
  private:
  std::bitset<IBM_LENGTH> bits;
  
  public:
  constexpr IBMFloat() noexcept: bits() {}

  constexpr IBMFloat(std::bitset<IBM_LENGTH> b) noexcept: bits(b) {}


};

#endif