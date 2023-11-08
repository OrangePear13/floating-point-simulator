# floating-point-simulator

This simulates some operations on floating point numbers.

This simulator also has a custom representation of a sequence of bits. Yes, things like std::bitset and std::vector<bool> exist, but I'm built different. Bitset's bit length has to be known at compile time, and I'm not doing anything crazy so why not use a dynamically allocated boolean array?