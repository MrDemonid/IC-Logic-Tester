const char clab0000[] = { // ��1(HC4094)
CMD_INIT_16, b10000111, b11000000, 
CMD_SET_ALL, b00000000, b00000000, 
CMD_PULSE, 3, 
CMD_SET_ALL_AND_TEST, b00000001, b01000000, b00000000, b00000000, 
CMD_SET_TO_1, 2, 
CMD_LAST_PULSE_AND_TEST, b00001000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00011000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00111000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01111000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01111000, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01111000, b00110000, 
CMD_LAST_PULSE_AND_TEST, b01111000, b00111000, 
CMD_LAST_PULSE_AND_TEST, b01111000, b00111101, 
CMD_SET_TO_0, 3, 
CMD_TEST_ALL, b01111000, b00111111, 
CMD_SET_TO_0, 2, 
CMD_LAST_PULSE_AND_TEST, b01110000, b00111111, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00111111, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00111111, 
CMD_LAST_PULSE_AND_TEST, b00000000, b00111111, 
CMD_LAST_PULSE_AND_TEST, b00000000, b00011111, 
CMD_LAST_PULSE_AND_TEST, b00000000, b00001111, 
CMD_LAST_PULSE_AND_TEST, b00000000, b00000111, 
CMD_LAST_PULSE_AND_TEST, b00000000, b00000010, 
CMD_SET_TO_0, 3, 
CMD_TEST_ALL, b00000000, b00000000, 
CMD_SET_TO_0, 15, 
CMD_PULLUP,
CMD_TEST_ALL, b01111000, b00111100, 
CMD_END };
const char clab0001[] = { // ��1(CD4028)
CMD_INIT_14, b11100011, b00110001, 
CMD_SET_ALL_AND_TEST, b00000001, b00010000, b00000000, b00000000, 
CMD_SET_TO_0, 13, 
CMD_PULSE, -1, 
CMD_TEST_ALL, b00000100, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00001000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00001100, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00010000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00010100, b00000000, 
CMD_END };
const char clab0002[] = { // ��2(CD4015)
CMD_INIT_16, b11100001, b11100001, 
CMD_SET_ALL_AND_TEST, b01100000, b01100000, b00000000, b00000000, 
CMD_SET, b00100000, b00100000, b00000000, b00000000, 
CMD_PULSE, 9, 
CMD_TEST_ALL, b00010000, b00000000, 
CMD_PULSE, 1, 
CMD_TEST_ALL, b00010000, b00010000, 
CMD_PULSE, 9, 
CMD_TEST_ALL, b00011000, b00010000, 
CMD_PULSE, 1, 
CMD_TEST_ALL, b00011000, b00011000, 
CMD_PULSE, 9, 
CMD_TEST_ALL, b00011100, b00011000, 
CMD_PULSE, 1, 
CMD_TEST_ALL, b00011100, b00011100, 
CMD_PULSE, 9, 
CMD_TEST_ALL, b00011100, b00011110, 
CMD_PULSE, 1, 
CMD_TEST_ALL, b00011110, b00011110, 
CMD_END };
const char clab0003[] = { // ��1(CD4028)
CMD_INIT_16, b10000000, b10011110, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00000100, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000010, b00000000, b00100000, 
CMD_SET_ALL_AND_TEST, b00000000, b00010000, b00000010, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00010010, b00000000, b01000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001000, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001010, b00100000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00011000, b01000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00011010, b00001000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000100, b00000000, b00000001, 
CMD_SET_ALL_AND_TEST, b00000000, b00000110, b00010000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00010100, b00000000, b00000001, 
CMD_SET_ALL_AND_TEST, b00000000, b00010110, b00010000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001100, b00000000, b00000001, 
CMD_SET_ALL_AND_TEST, b00000000, b00001110, b00010000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00011100, b00000000, b00000001, 
CMD_SET_ALL_AND_TEST, b00000000, b00011110, b00010000, b00000000, 
CMD_END };
const char clab0004[] = { // ��2
CMD_INIT_16, b11111111, b10000000, 
CMD_SET_ALL_AND_TEST, b00100001, b00000000, b00000000, b00100000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b01011111, 
CMD_SET_ALL_AND_TEST, b00110001, b00000000, b00000000, b01111001, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b00000110, 
CMD_SET_ALL_AND_TEST, b00100101, b00000000, b00000000, b01000100, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b00111011, 
CMD_SET_ALL_AND_TEST, b00110101, b00000000, b00000000, b01010000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b00101111, 
CMD_SET_ALL_AND_TEST, b00100011, b00000000, b00000000, b00011001, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b01100110, 
CMD_SET_ALL_AND_TEST, b00110011, b00000000, b00000000, b00010010, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b01101101, 
CMD_SET_ALL_AND_TEST, b00100111, b00000000, b00000000, b00000010, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b01111101, 
CMD_SET_ALL_AND_TEST, b00110111, b00000000, b00000000, b01111000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b00000111, 
CMD_SET_ALL_AND_TEST, b00101001, b00000000, b00000000, b00000000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b01111111, 
CMD_SET_ALL_AND_TEST, b00111001, b00000000, b00000000, b00010000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000000, b01101111, 
CMD_SET_TO_0, 1, 
CMD_SET, b00011110, b00000000, b00000000, b00000000, 
CMD_TEST_ALL, b00000000, b01101111, 
CMD_SET_ALL_AND_TEST, b00001101, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00011101, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00001011, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00011011, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00001111, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00011111, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01000001, b00000000, b00000000, b00000000, 
CMD_END };
const char clab0005[] = { // ��1(CD4016), ��3(CD4066)
CMD_INIT_14, b11111001, b00111100, 
CMD_SET_ALL_AND_TEST, b00110000, b00011000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b10111001, b00011100, b00000110, b00000011, 
CMD_SET_ALL_AND_TEST, b00111001, b00011000, b00000110, b00000000, 
CMD_SET_ALL_AND_TEST, b10110000, b00011100, b00000000, b00000011, 
CMD_SET_ALL, b00000000, b00000000, 
CMD_PULLUP,
CMD_TEST_ALL, b00000110, b00000011, 
CMD_PULLDN,
CMD_CONFIG, b01110110, b00111011, 
CMD_SET_ALL_AND_TEST, b00110000, b00011000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00110110, b00011011, b10001001, b00000100, 
CMD_SET_ALL_AND_TEST, b00110110, b00011000, b00001001, b00000000, 
CMD_SET_ALL_AND_TEST, b00110000, b00011011, b10000000, b00000100, 
CMD_SET_ALL, b00000000, b00000000, 
CMD_PULLUP,
CMD_TEST_ALL, b10001001, b00000100, 
CMD_END };
const char clab0006[] = { // ��12
CMD_INIT_14, b01111110, b00101111, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b10000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00100000, b00000000, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00000010, b00000001, b10000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00100100, b00000010, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00001000, b00000100, b10000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00110000, b00001000, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00001010, b00000101, b10000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00110100, b00001010, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00111110, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001111, b10000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00111110, b00001111, b00000000, b00000000, 
CMD_END };
const char clab0007[] = { // ��2(CD4043)
CMD_INIT_16, b11111100, b11111100, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00101000, b00101000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01000100, b01000100, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01101100, b01101100, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00111000, b00101000, b00000011, b00000011, 
CMD_SET_ALL_AND_TEST, b01010100, b01000100, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01011000, b01001000, b00000010, b00000010, 
CMD_SET_ALL_AND_TEST, b00110100, b00100100, b00000001, b00000001, 
CMD_SET_ALL_AND_TEST, b01111100, b01101100, b00000011, b00000011, 
CMD_END };
const char clab0008[] = { // ��1(CD4027)
CMD_INIT_16, b11111100, b10011111, 
CMD_SET_ALL_AND_TEST, b00001000, b00001000, b00000010, b00100000, 
CMD_SET_ALL_AND_TEST, b01000000, b00000001, b00000001, b01000000, 
CMD_SET_ALL, b00010000, b00000100, 
CMD_PULSE, 3, 
CMD_TEST_ALL, b00000010, b01000000, 
CMD_PULSE, 13, 
CMD_TEST_ALL, b00000010, b00100000, 
CMD_SET_ALL, b00100000, b00000010, 
CMD_PULSE, 3, 
CMD_TEST_ALL, b00000001, b00100000, 
CMD_PULSE, 13, 
CMD_TEST_ALL, b00000001, b01000000, 
CMD_SET_ALL, b00110000, b00000110, 
CMD_PULSE, 3, 
CMD_TEST_ALL, b00000010, b01000000, 
CMD_PULSE, 13, 
CMD_TEST_ALL, b00000010, b00100000, 
CMD_PULSE, 3, 
CMD_TEST_ALL, b00000001, b00100000, 
CMD_PULSE, 13, 
CMD_TEST_ALL, b00000001, b01000000, 
CMD_END };
const char clab0009[] = { // ��3(CD4503)
CMD_INIT_16, b10101011, b11101010, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000010, b00000000, b00000100, b00000000, 
CMD_SET_ALL_AND_TEST, b00001000, b00000000, b00010000, b00000000, 
CMD_SET_ALL_AND_TEST, b00100000, b00100000, b01000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001010, b00000000, b00000101, 
CMD_SET_ALL_AND_TEST, b00100010, b00001000, b01000100, b00000100, 
CMD_SET_ALL_AND_TEST, b00001000, b00100010, b00010000, b00010001, 
CMD_SET_ALL_AND_TEST, b00101010, b00000010, b01010100, b00000001, 
CMD_SET_ALL_AND_TEST, b00000000, b00101000, b00000000, b00010100, 
CMD_SET_ALL_AND_TEST, b00101010, b00101010, b01010100, b00010101, 
CMD_SET_ALL, b00101011, b01101010, 
CMD_PULLUP,
CMD_TEST_ALL, b01010100, b00010101, 
CMD_END };
const char clab000A[] = { // ��6(CD4002)
CMD_INIT_14, b11111110, b00101111, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00000010, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00000100, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00001000, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00010000, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000001, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000010, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000100, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001000, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00001010, b00001010, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00010100, b00000101, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00011110, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001111, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00011110, b00001111, b00000000, b00000000, 
CMD_END };
const char clab000B[] = { // ��10(CD4025)
CMD_INIT_14, b11011111, b00111100, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00100000, b00000011, 
CMD_SET_ALL_AND_TEST, b00010001, b00001000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00001010, b00000100, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b10000100, b00010000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b10000011, b00000000, b00100000, b00000010, 
CMD_SET_ALL_AND_TEST, b00011100, b00000000, b00000000, b00000011, 
CMD_SET_ALL_AND_TEST, b00000000, b00011100, b00100000, b00000001, 
CMD_SET_ALL_AND_TEST, b10011111, b00000000, b00000000, b00000010, 
CMD_SET_ALL_AND_TEST, b00011100, b00011100, b00000000, b00000001, 
CMD_SET_ALL_AND_TEST, b10000011, b00011100, b00100000, b00000000, 
CMD_SET_ALL_AND_TEST, b10011111, b00011100, b00000000, b00000000, 
CMD_END };
const char clab000C[] = { // ��9(CD4023)
CMD_INIT_14, b11011111, b00111100, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00100000, b00000011, 
CMD_SET_ALL_AND_TEST, b00010001, b00001000, b00100000, b00000011, 
CMD_SET_ALL_AND_TEST, b00001010, b00000100, b00100000, b00000011, 
CMD_SET_ALL_AND_TEST, b10000100, b00010000, b00100000, b00000011, 
CMD_SET_ALL_AND_TEST, b10000011, b00000000, b00100000, b00000010, 
CMD_SET_ALL_AND_TEST, b00011100, b00000000, b00000000, b00000011, 
CMD_SET_ALL_AND_TEST, b00000000, b00011100, b00100000, b00000001, 
CMD_SET_ALL_AND_TEST, b10011111, b00000000, b00000000, b00000010, 
CMD_SET_ALL_AND_TEST, b00011100, b00011100, b00000000, b00000001, 
CMD_SET_ALL_AND_TEST, b10000011, b00011100, b00100000, b00000000, 
CMD_SET_ALL_AND_TEST, b10011111, b00011100, b00000000, b00000000, 
CMD_END };
const char clab000D[] = { // ��8(CD4012)
CMD_INIT_14, b11111110, b00101111, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00000010, b00000001, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00000100, b00000010, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00001000, b00000100, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00010000, b00001000, b00000001, b00010000, 
CMD_SET_ALL_AND_TEST, b00011110, b00000000, b00000000, b00010000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001111, b00000001, b00000000, 
CMD_SET_ALL_AND_TEST, b00011110, b00001111, b00000000, b00000000, 
CMD_END };
const char clab000E[] = { // ��2(CD4013)
CMD_INIT_14, b11111100, b00100111, 
CMD_SET_ALL_AND_TEST, b00001000, b00000010, b00000010, b00001000, 
CMD_SET_ALL_AND_TEST, b10100000, b00000000, b00000001, b00010000, 
CMD_SET, b10100000, b00000000, b00000000, b00000000, 
CMD_PULSE, 3, 
CMD_TEST_ALL, b00000010, b00010000, 
CMD_PULSE, 11, 
CMD_TEST_ALL, b00000010, b00001000, 
CMD_SET, b00000000, b00000000, b00010000, b00000001, 
CMD_PULSE, 3, 
CMD_TEST_ALL, b00000001, b00001000, 
CMD_PULSE, 11, 
CMD_TEST_ALL, b00000001, b00010000, 
CMD_END };
const char clab000F[] = { // ��2(CD4019)
CMD_INIT_16, b11111111, b11100001, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00100000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000001, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00100001, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01010101, b00000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01010101, b00100000, b00000000, b00011110, 
CMD_SET_ALL_AND_TEST, b01010101, b00000001, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01010101, b00100001, b00000000, b00011110, 
CMD_SET_ALL_AND_TEST, b00101010, b01000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00101010, b01100000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00101010, b01000001, b00000000, b00011110, 
CMD_SET_ALL_AND_TEST, b00101010, b01100001, b00000000, b00011110, 
CMD_SET_ALL_AND_TEST, b01111111, b01000000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b01111111, b01100000, b00000000, b00011110, 
CMD_SET_ALL_AND_TEST, b01111111, b01000001, b00000000, b00011110, 
CMD_SET_ALL_AND_TEST, b01111111, b01100001, b00000000, b00011110, 
CMD_END };
const char clab0010[] = { // ��3
CMD_INIT_14, b01111000, b00100000, 
CMD_SET_ALL_AND_TEST, b00011000, b00000000, b10000000, b00011111, 
CMD_SET_ALL_AND_TEST, b00111000, b00000000, b00000001, b00000000, 
CMD_SET, b00110000, b00000000, b00000000, b00000000, 
CMD_PULSE, -4, 
CMD_TEST_ALL, b00000000, b00000110, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b10000001, b00011001, 
CMD_LAST_PULSE_AND_TEST, b10000100, b00000100, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000101, b00011011, 
CMD_LAST_PULSE_AND_TEST, b00000101, b00001111, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b10000100, b00010000, 
CMD_LAST_PULSE_AND_TEST, b00000100, b00011001, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b10000101, b00000110, 
CMD_LAST_PULSE_AND_TEST, b10000111, b00001101, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b00000110, b00010010, 
CMD_LAST_PULSE_AND_TEST, b00000001, b00000000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b10000000, b00011111, 
CMD_END };
const char clab0011[] = { // ��4
CMD_INIT_14, b01111000, b00100000, 
CMD_SET_ALL_AND_TEST, b00011000, b00000000, b10000000, b00011111, 
CMD_SET_ALL_AND_TEST, b00111000, b00000000, b00000001, b00000000, 
CMD_SET, b00110000, b00000000, b00000000, b00000000, 
CMD_PULSE, -4, 
CMD_TEST_ALL, b00000000, b00000110, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b10000001, b00011001, 
CMD_LAST_PULSE_AND_TEST, b10000000, b00000100, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b00000001, b00011011, 
CMD_LAST_PULSE_AND_TEST, b00000001, b00001111, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b10000000, b00010000, 
CMD_LAST_PULSE_AND_TEST, b00000100, b00011001, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b10000101, b00000110, 
CMD_LAST_PULSE_AND_TEST, b10000111, b00001101, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b00000110, b00010010, 
CMD_LAST_PULSE_AND_TEST, b00000110, b00000010, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b10000111, b00011101, 
CMD_LAST_PULSE_AND_TEST, b00000110, b00000111, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b10000111, b00011000, 
CMD_LAST_PULSE_AND_TEST, b00000110, b00000000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b10000111, b00011111, 
CMD_LAST_PULSE_AND_TEST, b10000011, b00001111, 
CMD_SET_TO_1, 6, 
CMD_TEST_ALL, b00000010, b00010000, 
CMD_LAST_PULSE_AND_TEST, b00000001, b00000000, 
CMD_SET_TO_0, 6, 
CMD_TEST_ALL, b10000000, b00011111, 
CMD_END };
const char clab0012[] = { // ��11(CD4516)
CMD_INIT_16, b10011101, b11011011, 
CMD_SET_ALL, b00000100, b00001000, 
CMD_SET_TO_1, 1, 
CMD_SET_TO_0, 1, 
CMD_TEST_ALL, b01000010, b00000100, 
CMD_SET_ALL, b00001000, b00010000, 
CMD_SET_TO_1, 1, 
CMD_SET_TO_0, 1, 
CMD_TEST_ALL, b01100000, b00100000, 
CMD_SET_ALL, b00001100, b00011011, 
CMD_TEST, b00100010, b00100100, b00000000, b00000000, 
CMD_SET_TO_0, 9, 
CMD_PULSE, 15, 
CMD_TEST_ALL, b01100000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00100100, 
CMD_LAST_PULSE_AND_TEST, b00100010, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00000000, 
CMD_SET_TO_0, 10, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01100010, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01000010, b00000000, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00100100, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00100000, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00000100, 
CMD_LAST_PULSE_AND_TEST, b01100000, b00000000, 
CMD_LAST_PULSE_AND_TEST, b00000000, b00000000, 
CMD_SET_TO_1, 5, 
CMD_LAST_PULSE_AND_TEST, b01000000, b00000000, 
CMD_END };
const char clab0013[] = { // ��10(CD4520)
CMD_INIT_16, b11000011, b11000011, 
CMD_SET_ALL_AND_TEST, b01000010, b01000010, b00000000, b00000000, 
CMD_SET, b01000000, b01000000, b00000000, b00000000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00000100, b00000100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00001000, b00001000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00001100, b00001100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00010000, b00010000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00010100, b00010100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00011000, b00011000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00011100, b00011100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00100000, b00100000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00100100, b00100100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00101000, b00101000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00101100, b00101100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00110000, b00110000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00110100, b00110100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00111000, b00111000, 
CMD_PULSE, 1, 
CMD_SET_TO_0, 1, 
CMD_PULSE, -10, 
CMD_SET_TO_1, 10, 
CMD_TEST_ALL, b00111100, b00111100, 
CMD_PULSE, -2, 
CMD_SET_TO_1, 2, 
CMD_PULSE, 9, 
CMD_SET_TO_0, 9, 
CMD_TEST_ALL, b00000000, b00000000, 
CMD_SET_ALL, b01000000, b01000000, 
CMD_PULSE, 1, 
CMD_PULSE, 9, 
CMD_TEST_ALL, b00000000, b00000000, 
CMD_PULSE, -2, 
CMD_PULSE, -10, 
CMD_TEST_ALL, b00000000, b00000000, 
CMD_END };
const char clab0014[] = { // ��7(CD4011), ��1(CD4093)
CMD_INIT_14, b11110011, b00111001, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00001100, b00000110, 
CMD_SET_ALL_AND_TEST, b00000010, b00000001, b00001100, b00000110, 
CMD_SET_ALL_AND_TEST, b00100000, b00010000, b00001100, b00000110, 
CMD_SET_ALL_AND_TEST, b10010001, b00001000, b00001100, b00000110, 
CMD_SET_ALL_AND_TEST, b00100010, b00010001, b00001100, b00000110, 
CMD_SET_ALL_AND_TEST, b10000011, b00000001, b00001000, b00000100, 
CMD_SET_ALL_AND_TEST, b00110000, b00011000, b00000100, b00000010, 
CMD_SET_ALL_AND_TEST, b10110011, b00000001, b00000000, b00000100, 
CMD_SET_ALL_AND_TEST, b10110011, b00011001, b00000000, b00000000, 
CMD_END };
const char clab0015[] = { // ��5(CD4001)
CMD_INIT_14, b11110011, b00111001, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b00001100, b00000110, 
CMD_SET_ALL_AND_TEST, b00000010, b00000000, b00001000, b00000110, 
CMD_SET_ALL_AND_TEST, b00100000, b00000000, b00000100, b00000110, 
CMD_SET_ALL_AND_TEST, b00000000, b00000001, b00001100, b00000100, 
CMD_SET_ALL_AND_TEST, b00000000, b00010000, b00001100, b00000010, 
CMD_SET_ALL_AND_TEST, b00100001, b00000000, b00000000, b00000110, 
CMD_SET_ALL_AND_TEST, b10000000, b00010000, b00001100, b00000000, 
CMD_SET_ALL_AND_TEST, b00010010, b00001001, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b10110011, b00011001, b00000000, b00000000, 
CMD_END };
const char clab0016[] = { // ��2(4049), ��7(4069)
CMD_INIT_14, b01010101, b00110101, 
CMD_SET_ALL_AND_TEST, b00010101, b00010101, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b10101010, b00001010, 
CMD_SET_ALL_AND_TEST, b00000100, b00010001, b00100010, b00000010, 
CMD_SET_ALL_AND_TEST, b00010001, b00000100, b10001000, b00001000, 
CMD_END };
const char clab0017[] = { // ��1(CD4502)
CMD_INIT_16, b10101101, b11011010, 
CMD_SET_ALL_AND_TEST, b00000000, b00000000, b01010010, b00100101, 
CMD_SET_ALL_AND_TEST, b00000001, b00000000, b01010000, b00100101, 
CMD_SET_ALL_AND_TEST, b00000100, b00000000, b01000010, b00100101, 
CMD_SET_ALL_AND_TEST, b00100000, b00000000, b00010010, b00100101, 
CMD_SET_ALL_AND_TEST, b00000000, b00000010, b01010010, b00100100, 
CMD_SET_ALL_AND_TEST, b00000000, b00010000, b01010010, b00100001, 
CMD_SET_ALL_AND_TEST, b00000000, b01000000, b01010010, b00000101, 
CMD_SET_ALL_AND_TEST, b00100001, b00010000, b00010000, b00100001, 
CMD_SET_ALL_AND_TEST, b00000100, b01000010, b01000010, b00000100, 
CMD_SET_ALL_AND_TEST, b00100101, b01010010, b00000000, b00000000, 
CMD_PULLUP,
CMD_SET_ALL_AND_TEST, b00100101, b01011010, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000000, b00001000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00100001, b00011000, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00000100, b01001010, b00000000, b00000000, 
CMD_SET_ALL_AND_TEST, b00101101, b01010010, b01010010, b00100101, 
CMD_SET_ALL_AND_TEST, b00001000, b00000000, b01010010, b00100101, 
CMD_SET_ALL_AND_TEST, b00101001, b00010000, b01010010, b00100101, 
CMD_SET_ALL_AND_TEST, b00001100, b01000010, b01010010, b00100101, 
CMD_END };
