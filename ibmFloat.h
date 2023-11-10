// CpE 3110 FS23 Honors Project
// File: floating.h
// Author: Nathan Mejia
#ifndef IBM_FLOAT_H
#define IBM_FLOAT_H

#include <iostream>
#include <string>
#include <bitset>

const unsigned short EXP_LEN  = 7 ;
const unsigned short MANT_LEN = 24;
const unsigned short EXP_BIAS = 64;
const unsigned short EXP_BASE = 16;
const unsigned short BASE_SZ  = 4 ; // x * 16**n == x << 4*n
const unsigned short IBM_LEN  = 32; // EXP_LEN + MANT_LEN + 1 (+1 is sign bit)
const unsigned short SIGN_IDX = 31; // IBM_LEN - 1
const unsigned short EXP_IDX  = 24; // IBM_LEN - 1 - EXP_LEN
const unsigned short MANT_IDX = 0 ;

typedef std::bitset<IBM_LEN>  ibmb;
typedef std::bitset<MANT_LEN> ibmm;
typedef std::bitset<EXP_LEN>  ibme;

const ibmm MANT_MSD_MSK = ibmm(0xffffff << (MANT_LEN - BASE_SZ));

class IBMFloat
{
  private:
  ibmb bits;
  
  public:
  IBMFloat() noexcept: bits() {}

  IBMFloat(const ibmb& b) noexcept: bits(b) {}

  IBMFloat(const IBMFloat& f) noexcept: bits(f.bits) {}

  IBMFloat(const uint32_t& b) noexcept: bits(b) {}

  IBMFloat& operator=(const IBMFloat& b) {bits = b.bits; return *this;}

  IBMFloat operator-() const {return IBMFloat(ibmb(this->bits).flip(SIGN_IDX));}

  ibmb toBits() const {return bits;}

  bool iszero() const {return !bits.any();}

  bool sign() const {return bits[SIGN_IDX];}

  ibmm mant() const;

  ibme exp() const;

  void setSign(const bool& s) {bits[SIGN_IDX] = s;}

  void setMant(const ibmm& m);

  void setExp(const ibme& e);

  IBMFloat& operator+=(const IBMFloat& b);

  IBMFloat& operator-=(const IBMFloat& b) {*this += -b; return *this;}
};

IBMFloat operator+(IBMFloat lhs, const IBMFloat& rhs) {return lhs += rhs;}

IBMFloat operator-(IBMFloat lhs, const IBMFloat& rhs) {return lhs -= rhs;}

template<std::size_t N>
std::bitset<N> operator+(const std::bitset<N>& lhs, const std::bitset<N>& rhs);

template<std::size_t N>
std::bitset<N> operator-(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{return lhs + ~rhs + std::bitset<N>(1);}

// Performs unsigned numerical comparison
template<std::size_t N>
bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs);

template<std::size_t N>
bool operator>(const std::bitset<N>& lhs, const std::bitset<N>& rhs) {return rhs < lhs;}

template<std::size_t N>
bool operator<=(const std::bitset<N>& lhs, const std::bitset<N>& rhs) {return !(lhs > rhs);}

template<std::size_t N>
bool operator>=(const std::bitset<N>& lhs, const std::bitset<N>& rhs) {return !(lhs < rhs);}

#endif