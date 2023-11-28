// CpE 3110 FS23 Honors Project
// File: ibmFloat.h
// Author: Nathan Mejia
#ifndef IBM_FLOAT_H
#define IBM_FLOAT_H

#include <iostream>
#include <string>
#include <bitset>
#include <math.h>

const unsigned short EXP_LEN  = 7 ;
const unsigned short MANT_LEN = 24;
const unsigned short EXP_BIAS = 64;
const unsigned short EXP_BASE = 16;
const unsigned short BASE_SZ  = 4 ; // x * 16**n == x << 1*n
const unsigned short IBM_LEN  = 32; // EXP_LEN + MANT_LEN + 1 (+1 is sign bit)
const unsigned short SIGN_IDX = 31; // IBM_LEN - 1
const unsigned short EXP_IDX  = 24; // IBM_LEN - 1 - EXP_LEN
const unsigned short MANT_IDX = 0 ;

typedef std::bitset<IBM_LEN>  ibmb; // full IBM HFP bitset
typedef std::bitset<MANT_LEN> ibmm; // IBM HFP mantissa bitset
typedef std::bitset<EXP_LEN>  ibme; // IBM HFP exponent bitset

const ibmm MANT_MSD_MSK = ibmm(0xffffffULL << (MANT_LEN - BASE_SZ));

class IBMFloat
{
  private:
  // All of the bits that make up an IBM HFP
  ibmb bits;
  
  public:
  // Default constructor that initializes all bits to 0
  IBMFloat() noexcept: bits() {}

  // Copies an existing std::bitset<IBM_LEN> into a new IBMFloat object
  IBMFloat(const ibmb& b) noexcept: bits(b) {}

  // Copy constructor
  IBMFloat(const IBMFloat& f) noexcept: bits(f.bits) {}

  // Uses std::bitset's bitwise constructor
  IBMFloat(const uint32_t& b) noexcept: bits(b) {}

  // Creates an IBMFloat from a string of hex digits
  // will only use the last 8 characters of the input string
  // will assign bits from right (LSB) to left (MSB), also parses string RTL
  // if a non hex digit is found or the string is exhausted, the remaining MSBs
  //  of the new IBMFloat will be 0's.
  IBMFloat(const std::string& b);

  // copy assignment
  IBMFloat& operator=(const IBMFloat& b) {bits = b.bits; return *this;}

  // Negates the IBMFloat number (returns a new negated IBMFloat)
  // Trivially flips sign bit because value stored in signed magnitude
  IBMFloat operator-() const {return IBMFloat(ibmb(this->bits).flip(SIGN_IDX));}

  // Gives the number represented by the object in native floating point
  double toNativeFloat() const;

  // returns a copy of the raw std::bitset
  ibmb toBits() const {return bits;}

  // True iff all bits are 0. Enforcing standard 0 (all bits =0)
  bool iszero() const {return !bits.any();}

  // Checks sign bit
  bool sign() const {return bits[SIGN_IDX];}

  // returns a copy of the mantissa bits
  ibmm mant() const;

  // returns a copy of the exponent bits
  ibme exp() const;

  // forcibly sets the sign bit to s
  void setSign(const bool& s) {bits[SIGN_IDX] = s;}

  // mantissa setter
  void setMant(const ibmm& m);

  // exponent setter
  void setExp(const ibme& e);

  // addition for IBM HFP numbers (addition assignment)
  // input does not need to be normalized, but output will be
  // if result is zero, all bits will be set to 0 (no -0 or 0 * 16^5)
  IBMFloat& operator+=(const IBMFloat& b);

  // trivially addition with the negation of the rhs
  IBMFloat& operator-=(const IBMFloat& b) {*this += -b; return *this;}
};

// non assignment version of addition (doesnt modify original object)
inline IBMFloat operator+(IBMFloat lhs, const IBMFloat& rhs) {return lhs += rhs;}

// non assignment version of subtraction (doesnt modify original object)
inline IBMFloat operator-(IBMFloat lhs, const IBMFloat& rhs) {return lhs -= rhs;}

// simple addition on 2 bitsets, no ovf checking, implemented as unsigned
//  also works on 2's complement numbers as the same implementation is used
template<std::size_t N>
std::bitset<N> operator+(const std::bitset<N>& lhs, const std::bitset<N>& rhs);

template<std::size_t N>
std::bitset<N> operator-(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{return lhs + (~rhs + std::bitset<N>(1));}

// lexicographic comparison between 2 bitsets (treated as unsigned)
template<std::size_t N>
bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs);

template<std::size_t N>
bool operator>(const std::bitset<N>& lhs, const std::bitset<N>& rhs) {return rhs < lhs;}

template<std::size_t N>
bool operator<=(const std::bitset<N>& lhs, const std::bitset<N>& rhs) {return !(lhs > rhs);}

template<std::size_t N>
bool operator>=(const std::bitset<N>& lhs, const std::bitset<N>& rhs) {return !(lhs < rhs);}

#endif