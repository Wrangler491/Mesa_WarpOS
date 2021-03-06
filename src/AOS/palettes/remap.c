/* if we open a window on a public screen it is
 * better to randomize the order of allocation of 
 * pens as not all pens are exact, it prevents
 * of allocating only light colors, or only red ones
 */
static const GLshort remap[palNumColsMax] = {
  0x1c,0x39,0x12,0x37,0x38,0x25,0x0e,0x03,0x13,0x10,0x0a,0x30,0x3d,0x05,0x1a,0x0c,0x29,0x01,0x26,0x28,0x15,0x3e,0x33,0x00,0x20,0x2c,0x35,0x3b,0x18,0x32,0x17,0x2e,0x22,0x2b,0x08,0x07,0x34,0x1d,0x23,0x21,0x1e,0x3f,0x0d,0x16,0x2a,0x11,0x24,0x02,0x09,0x0f,0x3c,0x1b,0x14,0x3a,0x2d,0x0b,0x19,0x31,0x1f,0x06,0x27,0x04,0x36,0x2f,
  0x7b,0xb4,0x45,0x4e,0x79,0xac,0x7c,0x7e,0x97,0xa8,0x7a,0x43,0x52,0x80,0x8f,0x88,0x84,0x60,0x41,0xa5,0x5d,0x86,0x4b,0x5c,0xbc,0x93,0x53,0x92,0xba,0x4c,0xa1,0x56,0x6d,0x96,0x40,0xb0,0xaf,0x5a,0x50,0x6c,0x81,0x70,0x66,0x99,0x5e,0x82,0x91,0x51,0xab,0x68,0x63,0xa6,0x74,0x6e,0x7f,0x6a,0x83,0x65,0xa9,0x42,0xa3,0x76,0x8e,0x61,0x58,0xae,0x89,0x69,0x9e,0x5b,0xb6,0x7d,0x4a,0xaa,0x95,0x4d,0xb5,0x73,0x75,0x9c,0x46,0x6b,0x8c,0x4f,0x98,0x64,0xbb,0x59,0x55,0x78,0x44,0x9d,0xb7,0x67,0x90,0xa4,0xb1,0x85,0x54,0xad,0x49,0x8d,0x5f,0x87,0x9b,0xb8,0xb3,0x77,0x48,0x94,0x62,0x71,0x9a,0x6f,0xb2,0x57,0x72,0x9f,0x47,0xa2,0x8a,0xa0,0xa7,0xb9,0x8b,
  0xce,0xda,0xe6,0xe9,0xf4,0xe1,0xbf,0xd2,0xee,0xfd,0xd8,0xd7,0xe3,0xd9,0xfa,0xed,0xff,0xf9,0xc2,0xfb,0xef,0xe7,0xe2,0xf7,0xbd,0xfe,0xc0,0xdc,0xc7,0xca,0xd5,0xd0,0xdb,0xe0,0xf3,0xf5,0xf2,0xe8,0xde,0xc3,0xd3,0xcc,0xdf,0xc9,0xc6,0xc5,0xdd,0xe4,0xd6,0xf6,0xc4,0xd1,0xc1,0xe5,0xcf,0xf0,0xcb,0xcd,0xeb,0xf1,0xc8,0xbe,0xfc,0xec,0xd4,0xea,0xf8
};
