#include "bits.h"

Bits::Bits(const size_t& s)
{
  if (s > sizeof(long) * 8 || s < 1)
    throw std::invalid_argument("Size not in range (1-64)");
  size = s;
  bits = new bool(s);
}

Bits::Bits(const size_t& s, const std::string& b)
{
  if (s > sizeof(long) * 8 || s < 1)
    throw std::invalid_argument("Size not in range (1-64)");
  size = s;
  bits = new bool(s);

  for (size_t i = 0; i < s; ++i)
  {
    if (b[i] == '1')
      bits[i] = 1;
    else if (b[i] == '0')
      bits[i] = 0;
    else
      throw std::invalid_argument("String must only contain 0 and 1");
  }
}

Bits::Bits(const size_t& s, const long& b)
{
  if (s > sizeof(long) * 8 || s < 1)
    throw std::invalid_argument("Size not in range (1-64)");
  size = s;
  bits = new bool(s);

  LongBits bBits = LongBits(b);

  for (size_t i = 0; i < s; ++i)
    bits[i] = bBits[i];
}

bool Bits::lsl(const unsigned short& n)
{
  bool out = bits[size-1];
  for (size_t i = 0; i < n-1; ++i)
  {
    bits[i+1] = bits[i];
  }
  bits[0] = 0;
}