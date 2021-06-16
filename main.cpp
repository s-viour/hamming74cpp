#include <iostream>
#include <string>


struct halves {
  char upper;
  char lower;
};

const char PARITY[] = {
  0x0, 0x3, 0x5, 0x6,
  0x6, 0x5, 0x3, 0x0,
  0x7, 0x4, 0x2, 0x1,
  0x1, 0x2, 0x4, 0x7
};

const char TRANSMIT[] = {
  0x00, 0x07, 0x19, 0x1e,
  0x2a, 0x2d, 0x33, 0x34,
  0x4b, 0x4c, 0x52, 0x55,
  0x61, 0x66, 0x78, 0x7f
};

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


int main(int argc, char* argv[]) {
  std::string progname(argv[0]);
  std::string usage("usage: " + progname + " encode|decode");

  if (argc != 2) {
    std::cout << "invalid number of arguments\n"
      << usage << '\n';
    return 0;
  }

  std::string modestr(argv[1]);
  enum {ENCODE, DECODE} mode;
  if (modestr == "encode") {
    mode = ENCODE;
  } else if (modestr == "decode") {
    mode = DECODE;
  } else {
    std::cout << "invalid option: " << modestr << '\n'
      << usage << '\n';
    return 0;
  }

  if (mode == ENCODE) {
    char data[1];
    while (std::cin.read(data, 1)) {
      auto e = encode(data[0]);
      std::cout.write((char*)&e, sizeof(e));
    }
  } else {
    char data[2];
    while (std::cin.read(data, 2)) {
      halves e = {data[0], data[1]};
      auto d = decode(e);
      std::cout.put(d);
    }
  }

  return 0;
}