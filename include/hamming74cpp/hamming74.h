namespace h74 {
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

  halves encode(const char);
  char decode (const halves);
}