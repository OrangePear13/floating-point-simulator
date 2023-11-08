#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <stdexcept>
#include <bitset>

typedef std::bitset<sizeof(long)> LongBits;

class Bits
{
  private:
    size_t size;
    bool* bits;
  
  public:
    Bits(const size_t& s);

    Bits(const size_t& s, const std::string& b);

    Bits(const size_t& s, const long& b);

    bool lsl(const unsigned short& n);

    bool lsr(const unsigned short& n);

    bool asl(const unsigned short& n);

    bool asr(const unsigned short& n);

    void bwAnd(const Bits& b);

    void bwOr(const Bits& b);

    void bwCmp();

    void bwXor(const Bits& b);

    bool add(const Bits& b);
};

#endif