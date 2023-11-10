#include "ibmFloat.h"

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
    this_mant >>= BASE_SZ * (b_exp - this_exp).to_ulong();
  
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
    }
  }

  // normalize, while MSD is 0, left shift by num of bits 1 digit occupies
  // and increment exponent to maintain same value
  while (!(this_mant & MANT_MSD_MSK).any())
  {
    this_mant <<= BASE_SZ;
    this_exp = this_exp + ibme(1);
  }

  setMant(this_mant);
  setExp(this_exp);

  return *this;
}

template<std::size_t N>
std::bitset<N> operator+(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{
  std::bitset<N> sum;
  bool carry = 0;

  for (size_t i = 0; i < lhs.size(); ++i)
  {
    sum[i] = lhs[i] != rhs[i] != carry;
    carry = (lhs[i] && rhs[i]) || (lhs[i] && carry) || (rhs[i] && carry);
  }

  return sum;
}

template<std::size_t N>
bool operator<(const std::bitset<N>& lhs, const std::bitset<N>& rhs)
{
  for (size_t i = 0; i < lhs.size(); ++i)
  {
    if (lhs[i] && !rhs[i])
      return false;
    
    if (!lhs[i] && rhs[i])
      return true;
  }
  return false;
}