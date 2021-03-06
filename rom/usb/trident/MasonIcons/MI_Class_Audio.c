#ifdef USE_CLASS_AUDIO_COLORS
const ULONG Class_Audio_colors[96] =
{
	0x96969696,0x96969696,0x96969696,
	0x2d2d2d2d,0x28282828,0x9e9e9e9e,
	0x00000000,0x65656565,0x9a9a9a9a,
	0x35353535,0x75757575,0xaaaaaaaa,
	0x65656565,0x8a8a8a8a,0xbabababa,
	0x0c0c0c0c,0x61616161,0xffffffff,
	0x24242424,0x5d5d5d5d,0x24242424,
	0x35353535,0x8a8a8a8a,0x35353535,
	0x86868686,0xb2b2b2b2,0x3d3d3d3d,
	0x0c0c0c0c,0xe3e3e3e3,0x00000000,
	0x4d4d4d4d,0x9e9e9e9e,0x8e8e8e8e,
	0x82828282,0x00000000,0x00000000,
	0xdfdfdfdf,0x35353535,0x35353535,
	0xdbdbdbdb,0x65656565,0x39393939,
	0xdbdbdbdb,0x8e8e8e8e,0x41414141,
	0xdfdfdfdf,0xbabababa,0x45454545,
	0xefefefef,0xe7e7e7e7,0x14141414,
	0x82828282,0x61616161,0x4d4d4d4d,
	0xa6a6a6a6,0x7e7e7e7e,0x61616161,
	0xcacacaca,0x9a9a9a9a,0x75757575,
	0x9a9a9a9a,0x55555555,0xaaaaaaaa,
	0xffffffff,0x00000000,0xffffffff,
	0xffffffff,0xffffffff,0xffffffff,
	0xdfdfdfdf,0xdfdfdfdf,0xdfdfdfdf,
	0xcacacaca,0xcacacaca,0xcacacaca,
	0xbabababa,0xbabababa,0xbabababa,
	0xaaaaaaaa,0xaaaaaaaa,0xaaaaaaaa,
	0x8a8a8a8a,0x8a8a8a8a,0x8a8a8a8a,
	0x65656565,0x65656565,0x65656565,
	0x4d4d4d4d,0x4d4d4d4d,0x4d4d4d4d,
	0x3c3c3c3c,0x3c3c3c3c,0x3b3b3b3b,
	0x00000000,0x00000000,0x00000000,
};
#endif

#define CLASS_AUDIO_WIDTH        16
#define CLASS_AUDIO_HEIGHT       16
#define CLASS_AUDIO_DEPTH         5
#define CLASS_AUDIO_COMPRESSION   1
#define CLASS_AUDIO_MASKING       2

#ifdef USE_CLASS_AUDIO_HEADER
const struct BitMapHeader Class_Audio_header =
{ 16,16,88,26,5,2,1,0,0,1,1,800,600 };
#endif

#ifdef USE_CLASS_AUDIO_BODY
const UBYTE Class_Audio_body[233] = {
0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0xff,0x00,0x01,0x00,0xa0,0x01,0x00,
0x90,0xff,0x00,0x01,0x00,0xf0,0x01,0x00,0xf0,0x01,0x01,0xb0,0x01,0x01,0xe8,
0x01,0x00,0xf0,0x01,0x01,0x78,0x01,0x01,0xf8,0x01,0x03,0xf8,0x01,0x02,0xfc,
0x01,0x00,0x70,0x01,0x03,0xfc,0x01,0x03,0xfc,0x01,0x06,0x68,0x01,0x07,0xfe,
0x01,0x00,0xf8,0x01,0x07,0xfe,0x01,0x07,0xfe,0x01,0x09,0xf4,0x01,0x0a,0xfe,
0x01,0x05,0xf8,0x01,0x0f,0xfe,0x01,0x0f,0xfe,0x01,0x35,0x00,0x01,0x3d,0xda,
0x01,0x01,0x3c,0x01,0x2f,0xfe,0x01,0x2f,0xfe,0x01,0x7a,0xe0,0x01,0x7b,0xb8,
0x01,0x06,0x9c,0x01,0x4d,0x7e,0x01,0x4f,0xfe,0x01,0x45,0xa8,0x01,0x69,0x70,
0x01,0x14,0x1c,0x01,0x57,0xfe,0x01,0x57,0xfe,0x01,0x77,0xe6,0x01,0x49,0xe4,
0x01,0x3d,0x18,0x01,0x7f,0xfe,0x01,0x7f,0xfe,0x01,0x21,0x4c,0x01,0x3e,0x9e,
0x01,0x1d,0xe0,0x01,0x3f,0xfe,0x01,0x3f,0xfe,0x01,0x06,0xbc,0x01,0x07,0xda,
0x01,0x00,0xe0,0x01,0x07,0xfe,0x01,0x07,0xfe,0x01,0x1b,0x18,0x01,0x1a,0x6c,
0x01,0x00,0xf0,0x01,0x1b,0xfc,0x01,0x1b,0xfc,0x01,0x75,0x90,0x01,0x75,0x68,
0x01,0x0a,0x70,0x01,0x7f,0xf8,0x01,0x7f,0xf8,0x01,0x0a,0x80,0x01,0x0a,0xf0,
0x01,0x01,0x00,0x01,0x0b,0xf0,0x01,0x0b,0xf0,0x01,0x01,0xc0,0x01,0x01,0xc0,
0xff,0x00,0x01,0x01,0xc0,0x01,0x01,0xc0, };
#endif
