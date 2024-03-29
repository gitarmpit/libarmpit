const uint8_t courBitmaps[] = {
  0xF9, 0xDC, 0xA4, 0x29, 0x55, 0xF5, 0x7D, 0x54, 0xA0, 0x27, 0x88, 0x79,
  0xE2, 0x20, 0x45, 0x10, 0x3E, 0x08, 0xA2, 0x32, 0x11, 0x59, 0x34, 0xF0,
  0x5A, 0xAA, 0x50, 0xA5, 0x55, 0xA0, 0x27, 0xC8, 0xA5, 0x00, 0x10, 0x20,
  0x47, 0xF1, 0x02, 0x04, 0x00, 0x6B, 0x40, 0xF8, 0xF0, 0x08, 0x44, 0x22,
  0x11, 0x08, 0x80, 0x74, 0x63, 0x18, 0xC6, 0x2E, 0x61, 0x08, 0x42, 0x10,
  0x9F, 0x74, 0x42, 0x22, 0x22, 0x3F, 0x74, 0x42, 0x60, 0x86, 0x2E, 0x18,
  0xA2, 0x92, 0x8B, 0xF0, 0x87, 0x7A, 0x10, 0xE0, 0x86, 0x2E, 0x3A, 0x21,
  0xE8, 0xC6, 0x2E, 0xFC, 0x42, 0x21, 0x08, 0x84, 0x74, 0x62, 0xE8, 0xC6,
  0x2E, 0x74, 0x63, 0x17, 0x84, 0x5C, 0xF0, 0xF0, 0x6C, 0x07, 0xA0, 0x0C,
  0x46, 0x20, 0x60, 0x40, 0xC0, 0xF8, 0x3E, 0xC0, 0x81, 0x81, 0x18, 0x8C,
  0x00, 0x69, 0x12, 0x40, 0xC0, 0x74, 0x63, 0x3A, 0xD6, 0x70, 0x8B, 0x80,
  0x30, 0x20, 0xA1, 0x42, 0x8F, 0x91, 0x77, 0xF9, 0x14, 0x5E, 0x45, 0x14,
  0x7E, 0x7C, 0x61, 0x08, 0x42, 0x2E, 0xF1, 0x24, 0x51, 0x45, 0x14, 0xBC,
  0xFD, 0x15, 0x1C, 0x51, 0x04, 0x7F, 0xFD, 0x15, 0x1C, 0x51, 0x04, 0x38,
  0x7A, 0x28, 0x20, 0x9E, 0x28, 0x9C, 0xEE, 0x89, 0x13, 0xE4, 0x48, 0x91,
  0x77, 0xF9, 0x08, 0x42, 0x10, 0x9F, 0x78, 0x84, 0x29, 0x4A, 0x4C, 0xEE,
  0x89, 0x22, 0x87, 0x09, 0x11, 0x73, 0xE2, 0x10, 0x84, 0x25, 0x3F, 0xEE,
  0xD9, 0xB2, 0xA5, 0x48, 0x91, 0x77, 0xEE, 0xC9, 0x92, 0xA5, 0x4A, 0x93,
  0x76, 0x74, 0x63, 0x18, 0xC6, 0x2E, 0xF2, 0x52, 0x97, 0x21, 0x1C, 0x74,
  0x63, 0x18, 0xC6, 0x2E, 0x38, 0xF8, 0x89, 0x12, 0x27, 0x89, 0x11, 0x71,
  0x6C, 0xE0, 0xE0, 0x87, 0x36, 0xFF, 0x24, 0x40, 0x81, 0x02, 0x04, 0x1C,
  0xEE, 0x89, 0x12, 0x24, 0x48, 0x91, 0x1C, 0xEE, 0x89, 0x11, 0x42, 0x85,
  0x04, 0x08, 0xEE, 0x89, 0x12, 0xA5, 0x4A, 0x95, 0x14, 0xC6, 0x88, 0xA0,
  0x81, 0x05, 0x11, 0x63, 0xEE, 0x88, 0xA1, 0x41, 0x02, 0x04, 0x1C, 0xFC,
  0x44, 0x42, 0x22, 0x3F, 0xF2, 0x49, 0x24, 0x9C, 0x82, 0x10, 0x82, 0x10,
  0x42, 0x00, 0xE4, 0x92, 0x49, 0x3C, 0x21, 0x15, 0x10, 0xFE, 0x90, 0x70,
  0x27, 0xA2, 0x89, 0xF0, 0xC1, 0x05, 0x99, 0x45, 0x14, 0x7E, 0x7C, 0x61,
  0x08, 0xB8, 0x18, 0x26, 0xA6, 0x8A, 0x28, 0x9F, 0x74, 0x7F, 0x08, 0x3C,
  0x3A, 0x3E, 0x84, 0x21, 0x1F, 0x6E, 0x68, 0xA2, 0x89, 0xE0, 0x9C, 0xC0,
  0x81, 0x63, 0x24, 0x48, 0x91, 0x77, 0x20, 0x38, 0x42, 0x10, 0x9F, 0x20,
  0xF1, 0x11, 0x11, 0x1E, 0xC1, 0x05, 0xD2, 0x71, 0x44, 0xB7, 0x61, 0x08,
  0x42, 0x10, 0x9F, 0xE8, 0xA9, 0x52, 0xA5, 0x5F, 0xC0, 0xD8, 0xC9, 0x12,
  0x24, 0x5D, 0xC0, 0x74, 0x63, 0x18, 0xB8, 0xD9, 0x94, 0x51, 0x45, 0xE4,
  0x38, 0x6E, 0x68, 0xA2, 0x89, 0xE0, 0x87, 0xDB, 0x10, 0x84, 0x7C, 0x7C,
  0x5C, 0x18, 0xF8, 0x43, 0xE4, 0x10, 0x41, 0x13, 0x80, 0xCC, 0x89, 0x12,
  0x24, 0xC6, 0xC0, 0xEE, 0x89, 0x11, 0x42, 0x82, 0x00, 0xEE, 0x89, 0x52,
  0xA5, 0x45, 0x00, 0xCD, 0x23, 0x0C, 0x4B, 0x30, 0xEE, 0x88, 0x91, 0x41,
  0x82, 0x04, 0x3C, 0xFC, 0x88, 0x88, 0xFC, 0x29, 0x25, 0x12, 0x44, 0xFF,
  0x80, 0x89, 0x24, 0x52, 0x50, 0x4D, 0x80 };

const GFXglyph courGlyphs[] = {
  {     0,   0,   0,   7,    0,    1 },   // 0x20 ' '
  {     0,   1,   8,   7,    3,   -7 },   // 0x21 '!'
  {     1,   5,   3,   7,    1,   -7 },   // 0x22 '"'
  {     3,   5,   9,   7,    1,   -7 },   // 0x23 '#'
  {     9,   4,   9,   7,    1,   -7 },   // 0x24 '$'
  {    14,   5,   8,   7,    1,   -7 },   // 0x25 '%'
  {    19,   5,   6,   7,    1,   -5 },   // 0x26 '&'
  {    23,   1,   4,   7,    3,   -7 },   // 0x27 '''
  {    24,   2,  10,   7,    3,   -7 },   // 0x28 '('
  {    27,   2,  10,   7,    2,   -7 },   // 0x29 ')'
  {    30,   5,   5,   7,    1,   -7 },   // 0x2A '*'
  {    34,   7,   7,   7,    0,   -6 },   // 0x2B '+'
  {    41,   3,   4,   7,    2,   -1 },   // 0x2C ','
  {    43,   5,   1,   7,    1,   -3 },   // 0x2D '-'
  {    44,   2,   2,   7,    2,   -1 },   // 0x2E '.'
  {    45,   5,   9,   7,    1,   -7 },   // 0x2F '/'
  {    51,   5,   8,   7,    1,   -7 },   // 0x30 '0'
  {    56,   5,   8,   7,    1,   -7 },   // 0x31 '1'
  {    61,   5,   8,   7,    1,   -7 },   // 0x32 '2'
  {    66,   5,   8,   7,    1,   -7 },   // 0x33 '3'
  {    71,   6,   8,   7,    1,   -7 },   // 0x34 '4'
  {    77,   5,   8,   7,    1,   -7 },   // 0x35 '5'
  {    82,   5,   8,   7,    1,   -7 },   // 0x36 '6'
  {    87,   5,   8,   7,    1,   -7 },   // 0x37 '7'
  {    92,   5,   8,   7,    1,   -7 },   // 0x38 '8'
  {    97,   5,   8,   7,    1,   -7 },   // 0x39 '9'
  {   102,   2,   6,   7,    2,   -5 },   // 0x3A ':'
  {   104,   3,   7,   7,    2,   -5 },   // 0x3B ';'
  {   107,   6,   7,   7,    0,   -6 },   // 0x3C '<'
  {   113,   5,   3,   7,    1,   -4 },   // 0x3D '='
  {   115,   6,   7,   7,    0,   -6 },   // 0x3E '>'
  {   121,   4,   7,   7,    2,   -6 },   // 0x3F '?'
  {   125,   5,  10,   7,    1,   -8 },   // 0x40 '@'
  {   132,   7,   8,   7,    0,   -7 },   // 0x41 'A'
  {   139,   6,   8,   7,    0,   -7 },   // 0x42 'B'
  {   145,   5,   8,   7,    1,   -7 },   // 0x43 'C'
  {   150,   6,   8,   7,    0,   -7 },   // 0x44 'D'
  {   156,   6,   8,   7,    0,   -7 },   // 0x45 'E'
  {   162,   6,   8,   7,    1,   -7 },   // 0x46 'F'
  {   168,   6,   8,   7,    1,   -7 },   // 0x47 'G'
  {   174,   7,   8,   7,    0,   -7 },   // 0x48 'H'
  {   181,   5,   8,   7,    1,   -7 },   // 0x49 'I'
  {   186,   5,   8,   7,    1,   -7 },   // 0x4A 'J'
  {   191,   7,   8,   7,    0,   -7 },   // 0x4B 'K'
  {   198,   5,   8,   7,    1,   -7 },   // 0x4C 'L'
  {   203,   7,   8,   7,    0,   -7 },   // 0x4D 'M'
  {   210,   7,   8,   7,    0,   -7 },   // 0x4E 'N'
  {   217,   5,   8,   7,    1,   -7 },   // 0x4F 'O'
  {   222,   5,   8,   7,    1,   -7 },   // 0x50 'P'
  {   227,   5,   9,   7,    1,   -7 },   // 0x51 'Q'
  {   233,   7,   8,   7,    0,   -7 },   // 0x52 'R'
  {   240,   5,   8,   7,    1,   -7 },   // 0x53 'S'
  {   245,   7,   8,   7,    0,   -7 },   // 0x54 'T'
  {   252,   7,   8,   7,    0,   -7 },   // 0x55 'U'
  {   259,   7,   8,   7,    0,   -7 },   // 0x56 'V'
  {   266,   7,   8,   7,    0,   -7 },   // 0x57 'W'
  {   273,   7,   8,   7,    0,   -7 },   // 0x58 'X'
  {   280,   7,   8,   7,    0,   -7 },   // 0x59 'Y'
  {   287,   5,   8,   7,    1,   -7 },   // 0x5A 'Z'
  {   292,   3,  10,   7,    2,   -7 },   // 0x5B '['
  {   296,   5,   9,   7,    1,   -7 },   // 0x5C '\'
  {   302,   3,  10,   7,    2,   -7 },   // 0x5D ']'
  {   306,   5,   4,   7,    1,   -7 },   // 0x5E '^'
  {   309,   7,   1,   7,    0,    3 },   // 0x5F '_'
  {   310,   2,   2,   7,    3,   -7 },   // 0x60 '`'
  {   311,   6,   6,   7,    1,   -5 },   // 0x61 'a'
  {   316,   6,   8,   7,    0,   -7 },   // 0x62 'b'
  {   322,   5,   6,   7,    1,   -5 },   // 0x63 'c'
  {   326,   6,   8,   7,    1,   -7 },   // 0x64 'd'
  {   332,   5,   6,   7,    1,   -5 },   // 0x65 'e'
  {   336,   5,   8,   7,    1,   -7 },   // 0x66 'f'
  {   341,   6,   8,   7,    1,   -5 },   // 0x67 'g'
  {   347,   7,   8,   7,    0,   -7 },   // 0x68 'h'
  {   354,   5,   8,   7,    1,   -7 },   // 0x69 'i'
  {   359,   4,  10,   7,    1,   -7 },   // 0x6A 'j'
  {   364,   6,   8,   7,    0,   -7 },   // 0x6B 'k'
  {   370,   5,   8,   7,    1,   -7 },   // 0x6C 'l'
  {   375,   7,   6,   7,    0,   -5 },   // 0x6D 'm'
  {   381,   7,   6,   7,    0,   -5 },   // 0x6E 'n'
  {   387,   5,   6,   7,    1,   -5 },   // 0x6F 'o'
  {   391,   6,   8,   7,    0,   -5 },   // 0x70 'p'
  {   397,   6,   8,   7,    1,   -5 },   // 0x71 'q'
  {   403,   5,   6,   7,    1,   -5 },   // 0x72 'r'
  {   407,   5,   6,   7,    1,   -5 },   // 0x73 's'
  {   411,   6,   7,   7,    1,   -6 },   // 0x74 't'
  {   417,   7,   6,   7,    0,   -5 },   // 0x75 'u'
  {   423,   7,   6,   7,    0,   -5 },   // 0x76 'v'
  {   429,   7,   6,   7,    0,   -5 },   // 0x77 'w'
  {   435,   6,   6,   7,    0,   -5 },   // 0x78 'x'
  {   440,   7,   8,   7,    0,   -5 },   // 0x79 'y'
  {   447,   5,   6,   7,    1,   -5 },   // 0x7A 'z'
  {   451,   3,  10,   7,    2,   -7 },   // 0x7B '{'
  {   455,   1,   9,   7,    3,   -7 },   // 0x7C '|'
  {   457,   3,  10,   7,    2,   -7 },   // 0x7D '}'
  {   461,   5,   2,   7,    1,   -3 } }; // 0x7E '~'

const GFXfont cour = 
{
  (uint8_t  *)courBitmaps,
  (GFXglyph *)courGlyphs,
  0x20, 0x7E, 14, 7
};

// Approx. 1135 bytes
