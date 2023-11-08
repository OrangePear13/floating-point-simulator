#ifndef BITS_H
#define BITS_H

#include <iostream>
#include <stdexcept>

class Bits
{
  private:
    size_t size;
    bool* bits;

  public:
    Bits(const size_t& s);

    Bits(const size_t& s, const std::string& b);

    Bits(const size_t& s, const long& b);

    Bits(const Bits& b);

    Bits& operator=(const Bits& r);

    Bits& operator+=(const Bits& r);

    Bits& operator-=(const Bits& r);

    Bits& operator<<=(const unsigned char& n);

    Bits& operator>>=(const unsigned char& n);

    Bits& operator++();

    Bits operator++(int);

    Bits& operator--();

    Bits operator--(int);

  private:

    friend Bits operator+(const Bits& l, const Bits& r);

    friend Bits operator-(const Bits& l, const Bits& r);

    friend Bits operator-(const Bits& b);

    friend Bits operator~(const Bits& b);

    friend Bits operator&(const Bits& l, const Bits& r);

    friend Bits operator|(const Bits& l, const Bits& r);

    friend Bits operator^(const Bits& l, const Bits& r);

    friend Bits operator<<(const Bits& b, const unsigned char& n);

    friend Bits operator>>(const Bits& b, const unsigned char& n);

    friend bool operator==(const Bits& l, const Bits& r);

    friend bool operator!=(const Bits& l, const Bits& r) {return !(l == r);}

    friend bool operator<(const Bits& l, const Bits& r);

    friend bool operator>(const Bits& l, const Bits& r) {return r < l;}

    friend bool operator<=(const Bits& l, const Bits& r) {return !(l > r);}

    friend bool operator>=(const Bits& l, const Bits& r) {return !(l < r);}

    friend std::ostream& operator<<(std::ostream& os, const Bits& b);
};

#endif