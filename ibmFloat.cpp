// CpE 3110 FS23 Honors Project
// File: ibmFloat.cpp
// Author: Nathan Mejia
#include "ibmFloat.h"

IBMFloat::IBMFloat(const std::string& b)
{
  for (size_t i = 0; i < b.size(); ++i)
  {
    if ((i+1)*4 - 1 >= IBM_LEN)
    {
      return;
    }

    auto assignBits = [this, &i](unsigned short h) {
      bits[i*4  ] = (h & 0b0001);
      bits[i*4+1] = (h & 0b0010);
      bits[i*4+2] = (h & 0b0100);
      bits[i*4+3] = (h & 0b1000);
    };
    
    switch (b[b.size() - i - 1])
    {
    case '0':
      assignBits(0x0);
      break;
    case '1':
      assignBits(0x1);
      break;
    case '2':
      assignBits(0x2);
      break;
    case '3':
      assignBits(0x3);
      break;
    case '4':
      assignBits(0x4);
      break;
    case '5':
      assignBits(0x5);
      break;
    case '6':
      assignBits(0x6);
      break;
    case '7':
      assignBits(0x7);
      break;
    case '8':
      assignBits(0x8);
      break;
    case '9':
      assignBits(0x9);
      break;
    case 'A':
    case 'a':
      assignBits(0xa);
      break;
    case 'B':
    case 'b':
      assignBits(0xb);
      break;
    case 'C':
    case 'c':
      assignBits(0xc);
      break;
    case 'D':
    case 'd':
      assignBits(0xd);
      break;
    case 'E':
    case 'e':
      assignBits(0xe);
      break;
    case 'F':
    case 'f':
      assignBits(0xf);
      break;
    default:
      return;
    }
  }
}

double IBMFloat::toNativeFloat() const
{
  auto mantToFlt = [this]() -> double {
    double mantissa = 0.0;
    for (size_t i = 0; i < MANT_LEN; ++i)
    {
      mantissa += this->mant()[i] / double(1 << (MANT_LEN-i));
    }
    return mantissa;
  };
  auto expToInt = [this]() -> short {
    short exponent = 0;
    for (size_t i = 0; i < EXP_LEN; ++i)
    {
      exponent += this->exp()[i] << i;
    }
    return exponent - EXP_BIAS;
  };

  return (sign()? -1: 1) * mantToFlt() * std::pow(EXP_BASE, expToInt());
}

ibmm IBMFloat::mant() const
{
  ibmm m;

  for (size_t i = 0; i < MANT_LEN; ++i)
    m[i] = this->bits[i+MANT_IDX];

  return m;
}

ibme IBMFloat::exp() const
{
  ibme e;

  for (size_t i = 0; i < EXP_LEN; ++i)
    e[i] = this->bits[i+EXP_IDX];

  return e;
}

void IBMFloat::setMant(const ibmm& m)
{
  for (size_t i = 0; i < MANT_LEN; ++i)
    this->bits[i+MANT_IDX] = m[i];
}

void IBMFloat::setExp(const ibme& e)
{
  for (size_t i = 0; i < EXP_LEN; ++i)
    this->bits[i+EXP_IDX] = e[i];
}

IBMFloat& IBMFloat::operator+=(const IBMFloat& b)
{
  if (this->iszero())
  {
    this->bits = b.bits;
    return *this;
  }
  if (b.iszero())
    return *this;

  ibmm this_mant = mant();
  ibmm b_mant = b.mant();
  ibme this_exp = exp();
  ibme b_exp = b.exp();

  // align the mantissas
  if (this_exp >= b_exp)
    b_mant >>= BASE_SZ * (this_exp - b_exp).to_ulong();
  else
  {
    this_mant >>= BASE_SZ * (b_exp - this_exp).to_ulong();
    this_exp = b_exp;
  }
  
  // perform addition on different signs
  if (sign() != b.sign())
  {
    // perform correct addition based on relative magnitude of mantissas
    if (this_mant > b_mant)
      this_mant = this_mant - b_mant;
    else if (this_mant < b_mant)
    {
      this_mant = b_mant - this_mant;
      setSign(b.sign());
    }
    // special case where mantissas are equal, reset all bits to have
    // a uniform 0 representation (no -0 or 0*16**5)
    else
    {
      bits.reset();
      return *this; // early exit because you cant normalize 0
    }
  }
  // perform addition on matching signs
  else
  {
    // if one mantissa's MSB is 1, then overflow is possible during addition
    bool poss_carry_out = this_mant[MANT_LEN-1] || b_mant[MANT_LEN-1];

    this_mant = this_mant + b_mant;

    // if one mantissa's MSB was 1, but the result's MSB is 0, then overflow
    if (poss_carry_out && !this_mant[MANT_LEN-1])
    {
      this_mant >>= BASE_SZ;
      this_mant.set(MANT_LEN-BASE_SZ);
      this_exp = this_exp + ibme(1);
    }
  }

  // normalize, while MSD is 0, left shift by num of bits 1 digit occupies
  // and increment exponent to maintain same value
  while (!(this_mant & MANT_MSD_MSK).any())
  {
    this_mant <<= BASE_SZ;
    this_exp = this_exp - ibme(1);
  }

  setMant(this_mant);
  setExp(this_exp);

  return *this;
}

std::ostream& operator<<(std::ostream& os, const IBMFloat& num)
{
  std::string hex;

  for (size_t i = 0; i < IBM_LEN; i+=BASE_SZ)
  {
    if (num.bits[i+3] == 0)
    {
      if (num.bits[i+2] == 0)
      {
        if (num.bits[i+1] == 0)
        {
          if (num.bits[i] == 0)
            hex = "0" + hex;
          else
            hex = "1" + hex;
        }
        else
        {
          if (num.bits[i] == 0)
            hex = "2" + hex;
          else
            hex = "3" + hex;
        }
      }
      else
      {
        if (num.bits[i+1] == 0)
        {
          if (num.bits[i] == 0)
            hex = "4" + hex;
          else
            hex = "5" + hex;
        }
        else
        {
          if (num.bits[i] == 0)
            hex = "6" + hex;
          else
            hex = "7" + hex;
        }
      }
    }
    else
    {
      if (num.bits[i+2] == 0)
      {
        if (num.bits[i+1] == 0)
        {
          if (num.bits[i] == 0)
            hex = "8" + hex;
          else
            hex = "9" + hex;
        }
        else
        {
          if (num.bits[i] == 0)
            hex = "A" + hex;
          else
            hex = "B" + hex;
        }
      }
      else
      {
        if (num.bits[i+1] == 0)
        {
          if (num.bits[i] == 0)
            hex = "C" + hex;
          else
            hex = "D" + hex;
        }
        else
        {
          if (num.bits[i] == 0)
            hex = "E" + hex;
          else
            hex = "F" + hex;
        }
      }
    }
  }
  
  os << hex;

  return os;
}

template<std::size_t N>
std::bitset<N> operator+(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{
  std::bitset<N> sum;
  bool carry = 0;

  for (size_t i = 0; i < N; ++i)
  {
    sum[i] = lhs[i] != rhs[i] != carry;
    carry = (lhs[i] && rhs[i]) || (lhs[i] && carry) || (rhs[i] && carry);
  }

  return sum;
}

template<std::size_t N>
bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{
  for (size_t i = 0; i < N; ++i)
  {
    if (lhs[N-i-1] && !rhs[N-i-1])
      return false;
    
    if (!lhs[N-i-1] && rhs[N-i-1])
      return true;
  }
  return false;
}