//                                                                     ⨯  ROW  SHIFT
const char dramRU5[] = { CMD_INIT_16_3|FL_DRAM, 0b11111111, 0b11011111, 0, 0x08, 1 };
const char dramRU6[] = { CMD_INIT_16_3|FL_DRAM, 0b11111111, 0b11011111, 0, 0x07, 1 };
const char dramRU7[] = { CMD_INIT_16_3|FL_DRAM, 0b11111111, 0b11011111, 0, 0x09, 1 };
const char dramRU9[] = { CMD_INIT_18_1|FL_DRAM, 0b11111111, 0b11111111, 0b00000010, 1, 0x0A, 1 };
const char dram4256[]= { CMD_INIT_20_2|FL_DRAM, 0b11111111, 0b11111111, 0b00001111, 2, 0x09, 4 };
const char dram464[] = { CMD_INIT_18_1|FL_DRAM, 0b11111111, 0b11111111, 0b00000011, 3, 0x08, 4 };
