#include <iostream>
#include <hamming74cpp/hamming74.h>


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
      auto e = h74::encode(data[0]);
      std::cout.write((char*)&e, sizeof(e));
    }
  } else {
    char data[2];
    while (std::cin.read(data, 2)) {
      h74::halves e = {data[0], data[1]};
      auto d = h74::decode(e);
      std::cout.put(d);
    }
  }

  return 0;
}