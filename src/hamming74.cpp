#include <hamming74cpp/hamming74.h>
using namespace h74;

halves split_byte(const char b) {
  char upper = (b & 0xf0) >> 4;
  char lower = b & 0xf;
  return {upper, lower};
}

char correct(const char b, const char p1, const char p2) {
  auto diff = p1 ^ p2;
  return b ^ ((0x01 << (diff)) >> 1);
}

char extract_parity(const char b) {
  auto p2 = (b & 0x08) >> 1;
  auto p1 = b & 0x02;
  auto p0 = b & 0x01;

  return p2 | p1 | p0;
}

char extract_data(const char b) {
  auto d3 = (b & 0x40) >> 3;
  auto d2 = (b & 0x20) >> 3;
  auto d1 = (b & 0x10) >> 3;
  auto d0 = (b & 0x04) >> 2;

  return d3 | d2 | d1 | d0;
}

namespace h74 {
  halves encode(const char b) {
    auto split = split_byte(b);
    return halves{TRANSMIT[split.upper], TRANSMIT[split.lower]};
  }

  char decode(const halves h) {
    // got and expected parity for the upper half
    auto got_u = extract_parity(h.upper);
    auto exp_u = PARITY[extract_data(h.upper)];
    // got and expected parity for the lower half
    auto got_l = extract_parity(h.lower);
    auto exp_l = PARITY[extract_data(h.lower)];
    
    // correct and extract the data from upper and lower
    auto upper = extract_data(correct(h.upper, got_u, exp_u));
    auto lower = extract_data(correct(h.lower, got_l, exp_l));

    // recombine them and return it
    return (upper << 4) | lower;
  }
}