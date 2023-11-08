#include "bits.h"

Bits::Bits(const size_t& s)
{
  if (s < 1)
    throw std::invalid_argument("Size cannot be 0");
  size = s;
  bits = new bool(s);
}

Bits::Bits(const size_t& s, const std::string& b)
{
  if (s < 1)
    throw std::invalid_argument("Size cannot be 0");
  size = s;
  bits = new bool(s);

  size_t min_size = std::min(s, b.length());
  for (size_t i = 0; i < min_size; ++i)
  {
    if (b[b.length() - i - 1] == '1')
      bits[i] = 1;
    else if (b[b.length() - i - 1] == '0')
      bits[i] = 0;
    else
      throw std::invalid_argument("String must only contain 0 and 1");
  }
}

Bits::Bits(const size_t& s, const long& b)
{
  if (s < 1)
    throw std::invalid_argument("Size cannot be 0");
  size = s;
  bits = new bool(s);

  size_t min_size = std::min(s, sizeof(long) * 8);
  for (size_t i = 0; i < min_size; ++i)
    bits[i] = bool(b & (1 << i));
}

Bits::Bits(const Bits& b)
{
  size = b.size;

  bits = new bool(b.size);

  for (size_t i = 0; i < b.size; ++i)
    bits[i] = b.bits[i];
}

Bits& Bits::operator=(const Bits& r)
{
  if (this == &r)
    return *this;
  
  delete[] bits;
  size = r.size;
  bits = new bool(r.size);

  for (size_t i = 0; i < r.size; ++i)
    bits[i] = r.bits[i];
  
  return *this;
}

Bits& Bits::operator+=(const Bits& r)
{
  bool carry = 0;

  if (size != r.size)
    throw std::invalid_argument("Sizes of both Bits must be the same");
  for (size_t i = 0; i < size; ++i)
  {
    bits[i] = bits[i] != r.bits[i] != carry;

    carry =  bits[i]   && r.bits[i]
         ||  bits[i]   && carry
         ||  r.bits[i] && carry;
  }
  
  return *this;
}

Bits& Bits::operator-=(const Bits& r)
{
  Bits neg = ~r;
  *this += neg;
  return *this;
}

Bits& Bits::operator<<=(const unsigned char& n)
{
  if (size < 2)
  {
    bits[0] = 0;
    return *this;
  }
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = size-2; j >= 0; --j)
      bits[j+1] = bits[j];
    
    bits[0] = 0;
  }
  return *this;
}

Bits& Bits::operator>>=(const unsigned char& n)
{
  if (size < 2)
  {
    bits[0] = 0;
    return *this;
  }
  for (size_t i = 0; i < n; ++i)
  {
    for (size_t j = 1; j < size; ++j)
      bits[j-1] = bits[j];
    
    bits[size-1] = 0;
  }
  return *this;
}

Bits& Bits::operator++()
{
  *this += Bits(size, 1);
  return *this;
}

Bits Bits::operator++(int)
{
  Bits prev = *this;
  operator++();
  return prev;
}

Bits& Bits::operator--()
{
  *this -= Bits(size, 1);
  return *this;
}

Bits Bits::operator--(int)
{
  Bits prev = *this;
  operator--();
  return prev;
}

Bits operator+(const Bits& l, const Bits& r)
{
  Bits sum = l;
  sum += r;
  return sum;
}

Bits operator-(const Bits& l, const Bits& r)
{
  Bits diff = l;
  diff -= r;
  return diff;
}

Bits operator-(const Bits& b)
{
  Bits cmp = Bits(b.size);

  for (size_t i = 0; i < b.size; ++i)
    cmp.bits[i] = !b.bits[i];
  
  return cmp;
}

Bits operator~(const Bits& b)
{
  return ++(-b);
}

Bits operator&(const Bits& l, const Bits& r)
{
  Bits result(std::min(l.size, r.size));

  for (size_t i = 0; i < result.size; ++i)
    result.bits[i] = l.bits[i] && r.bits[i];

  return result;
}

Bits operator|(const Bits& l, const Bits& r)
{
  Bits result(std::min(l.size, r.size));

  for (size_t i = 0; i < result.size; ++i)
    result.bits[i] = l.bits[i] || r.bits[i];

  return result;
}

Bits operator^(const Bits& l, const Bits& r)
{
  Bits result(std::min(l.size, r.size));

  for (size_t i = 0; i < result.size; ++i)
    result.bits[i] = l.bits[i] != r.bits[i];

  return result;
}

Bits operator<<(const Bits& b, const unsigned char& n)
{
  Bits result = b;
  result <<= n;
  return result;
}

Bits operator>>(const Bits& b, const unsigned char& n)
{
  Bits result = b;
  result >>= n;
  return result;
}

bool operator==(const Bits& l, const Bits& r)
{
  if (l.size <= r.size)
  {
    for (size_t i = 0; i < l.size; ++i)
      if (l.bits[i] != r.bits[i])
        return false;
    
    for (size_t i = l.size; i < r.size; ++i)
      if (r.bits[i] != 0)
        return false;
  }
  else
  {
    for (size_t i = 0; i < r.size; ++i)
      if (l.bits[i] != r.bits[i])
        return false;
    
    for (size_t i = r.size; i < l.size; ++i)
      if (l.bits[i] != 0)
        return false;
  }
  return true;
}

bool operator<(const Bits& l, const Bits& r)
{
  if (l.size <= r.size)
  {
    for (size_t i = r.size-1; i >= l.size; --i)
      if (r.bits[i] == 1)
        return true; 

    for (size_t i = l.size-1; i >= 0; --i)
    {
      if (l.bits[i] && !r.bits[i])
        return false;
      
      if (!l.bits[i] && r.bits[i])
        return true;
    }
  }
  else
  {
    for (size_t i = l.size-1; i >= r.size; --i)
      if (l.bits[i] == 1)
        return false; 

    for (size_t i = r.size-1; i >= 0; --i)
    {
      if (l.bits[i] && !r.bits[i])
        return false;
      
      if (!l.bits[i] && r.bits[i])
        return true;
    }
  }
}

std::ostream& operator<<(std::ostream& os, const Bits& b)
{
  os << "0b";
  for (size_t i = b.size-1; i >= 0; --i)
    os << b.bits[i];
  
  return os;
}