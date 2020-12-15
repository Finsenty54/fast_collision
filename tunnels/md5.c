#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "md5.h"

static void Process_One_Block_MD5 (unsigned long state[4], unsigned char block[64]);
static void long_to_byte(unsigned char *dst, unsigned long *src, unsigned long len);
static void byte_to_long(unsigned long *, unsigned char *, unsigned long);

static unsigned char doplnek[64] = {
128,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define F1(x, y, z) ( ((x) & (y)) | ((~x) & (z)) )
#define F2(x, y, z) ( ((x) & (z)) | ((y) & (~z)) )
#define F3(x, y, z) ( (x) ^ (y) ^ (z) )
#define F4(x, y, z) ( (y) ^ ((x) | (~z)) )

#define RL(x, n) ( ((x) << (n)) | ((x) >> (32-(n))) )

#define Sub1(a, b, c, d, e, f, g) { \
	(a) += F1((b), (c), (d)) + (e) + (unsigned long)(g); \
	(a) = RL((a), (f)); \
	(a) += (b); \
	}

#define Sub2(a, b, c, d, e, f, g) { \
	(a) += F2((b), (c), (d)) + (e) + (unsigned long)(g); \
	(a) = RL((a), (f)); \
	(a) += (b); \
	}

#define Sub3(a, b, c, d, e, f, g) { \
	(a) += F3((b), (c), (d)) + (e) + (unsigned long)(g); \
	(a) = RL((a), (f)); \
	(a) += (b); \
	}

#define Sub4(a, b, c, d, e, f, g) { \
	(a) += F4((b), (c), (d)) + (e) + (unsigned long)(g); \
	(a) = RL((a), (f)); \
	(a) += (b); \
	}
/*=============================================*/
int Init_MD5(MD5_CTX *MD5_ctx)
{
	MD5_ctx->count[0] = 0;
	MD5_ctx->count[1] = 0;
	MD5_ctx->state[0] = 0x67452301;
	MD5_ctx->state[1] = 0xefcdab89;
	MD5_ctx->state[2] = 0x98badcfe;
	MD5_ctx->state[3] = 0x10325476;
	
	return 0;
}

/*=============================================*/
int Update_MD5(MD5_CTX *MD5_ctx,unsigned char *buffer,unsigned long count)
{
	unsigned long i, rest, pad;

	if (count > 0xffffffc1) 
	{
		Update_MD5(MD5_ctx, buffer, 0xffffffc1);
		Update_MD5(MD5_ctx, buffer + 0xffffffc1, count - 0xffffffc1);
		return 0;
	}
	rest = (MD5_ctx->count[0] >> 3) & 0x3F;

	if ((MD5_ctx->count[0] += (count << 3)) < (count << 3))
	{
		MD5_ctx->count[1]++;
	}
	MD5_ctx->count[1] += (count >> 29);
	pad = 64 - rest;

	if (count >= pad) 
	{
		memcpy(&MD5_ctx->data[rest], buffer, pad);
		Process_One_Block_MD5 (MD5_ctx->state, MD5_ctx->data);
		for (i = pad; i + 63 < count; i += 64)
		{
			Process_One_Block_MD5(MD5_ctx->state, &buffer[i]);
		}
		rest = 0;
	}
	else
	{
		i = 0;
	}

	memcpy(&MD5_ctx->data[rest], &buffer[i], count-i);

	return 0;
}
/*=============================================*/
int Final_MD5(MD5_CTX *MD5_ctx)                                    
{
	unsigned char bits[8];
	unsigned short rest, padLen;

	long_to_byte(bits, MD5_ctx->count, 8);

	rest = (unsigned short)((MD5_ctx->count[0] >> 3) & 0x3f);
	padLen = (rest < 56) ? (56 - rest) : (120 - rest);
	Update_MD5(MD5_ctx, doplnek, padLen);

	Update_MD5(MD5_ctx, bits, 8);

	return 0;
}

/*=============================================*/
static void Process_One_Block_MD5(unsigned long state[4], unsigned char block[64])
{
	unsigned long a = state[0], b = state[1], c = state[2], d = state[3], e[16];

	byte_to_long(e, block, 64);

	Sub1(a, b, c, d, e[0], 7, 0xd76aa478);
	Sub1(d, a, b, c, e[1], 12, 0xe8c7b756);
	Sub1(c, d, a, b, e[2], 17, 0x242070db);
	Sub1(b, c, d, a, e[3], 22, 0xc1bdceee);
	Sub1(a, b, c, d, e[4], 7, 0xf57c0faf);
	Sub1(d, a, b, c, e[5], 12, 0x4787c62a);
	Sub1(c, d, a, b, e[6], 17, 0xa8304613);
	Sub1(b, c, d, a, e[7], 22, 0xfd469501);
	Sub1(a, b, c, d, e[8], 7, 0x698098d8);
	Sub1(d, a, b, c, e[9], 12, 0x8b44f7af);
	Sub1(c, d, a, b, e[10], 17, 0xffff5bb1);
	Sub1(b, c, d, a, e[11], 22, 0x895cd7be);
	Sub1(a, b, c, d, e[12], 7, 0x6b901122);
	Sub1(d, a, b, c, e[13], 12, 0xfd987193);
	Sub1(c, d, a, b, e[14], 17, 0xa679438e);
	Sub1(b, c, d, a, e[15], 22, 0x49b40821);

	Sub2(a, b, c, d, e[1], 5, 0xf61e2562);
	Sub2(d, a, b, c, e[6], 9, 0xc040b340);
	Sub2(c, d, a, b, e[11], 14, 0x265e5a51);
	Sub2(b, c, d, a, e[0], 20, 0xe9b6c7aa);
	Sub2(a, b, c, d, e[5], 5, 0xd62f105d);
	Sub2(d, a, b, c, e[10], 9,  0x2441453);
	Sub2(c, d, a, b, e[15], 14, 0xd8a1e681);
	Sub2(b, c, d, a, e[4], 20, 0xe7d3fbc8);
	Sub2(a, b, c, d, e[9], 5, 0x21e1cde6);
	Sub2(d, a, b, c, e[14], 9, 0xc33707d6);
	Sub2(c, d, a, b, e[3], 14, 0xf4d50d87);
	Sub2(b, c, d, a, e[8], 20, 0x455a14ed);
	Sub2(a, b, c, d, e[13], 5, 0xa9e3e905);
	Sub2(d, a, b, c, e[2], 9, 0xfcefa3f8);
	Sub2(c, d, a, b, e[7], 14, 0x676f02d9);
	Sub2(b, c, d, a, e[12], 20, 0x8d2a4c8a);

	Sub3(a, b, c, d, e[5], 4, 0xfffa3942);
	Sub3(d, a, b, c, e[8], 11, 0x8771f681);
	Sub3(c, d, a, b, e[11], 16, 0x6d9d6122);
	Sub3(b, c, d, a, e[14], 23, 0xfde5380c);
	Sub3(a, b, c, d, e[1], 4, 0xa4beea44);
	Sub3(d, a, b, c, e[4], 11, 0x4bdecfa9);
	Sub3(c, d, a, b, e[7], 16, 0xf6bb4b60);
	Sub3(b, c, d, a, e[10], 23, 0xbebfbc70);
	Sub3(a, b, c, d, e[13], 4, 0x289b7ec6);
	Sub3(d, a, b, c, e[0], 11, 0xeaa127fa);
	Sub3(c, d, a, b, e[3], 16, 0xd4ef3085);
	Sub3(b, c, d, a, e[6], 23,  0x4881d05);
	Sub3(a, b, c, d, e[9], 4, 0xd9d4d039);
	Sub3(d, a, b, c, e[12], 11, 0xe6db99e5);
	Sub3(c, d, a, b, e[15], 16, 0x1fa27cf8);
	Sub3(b, c, d, a, e[2], 23, 0xc4ac5665);

	Sub4(a, b, c, d, e[0], 6, 0xf4292244);
	Sub4(d, a, b, c, e[7], 10, 0x432aff97);
	Sub4(c, d, a, b, e[14], 15, 0xab9423a7);
	Sub4(b, c, d, a, e[5], 21, 0xfc93a039);
	Sub4(a, b, c, d, e[12], 6, 0x655b59c3);
	Sub4(d, a, b, c, e[3], 10, 0x8f0ccc92);
	Sub4(c, d, a, b, e[10], 15, 0xffeff47d);
	Sub4(b, c, d, a, e[1], 21, 0x85845dd1);
	Sub4(a, b, c, d, e[8], 6, 0x6fa87e4f);
	Sub4(d, a, b, c, e[15], 10, 0xfe2ce6e0);
	Sub4(c, d, a, b, e[6], 15, 0xa3014314);
	Sub4(b, c, d, a, e[13], 21, 0x4e0811a1);
	Sub4(a, b, c, d, e[4], 6, 0xf7537e82);
	Sub4(d, a, b, c, e[11], 10, 0xbd3af235);
	Sub4(c, d, a, b, e[2], 15, 0x2ad7d2bb);
	Sub4(b, c, d, a, e[9], 21, 0xeb86d391);

	state[0] += a;
	state[1] += b;
	state[2] += c;
	state[3] += d;

	memset(e, 0, sizeof(e));
}

/*=============================================*/
static void long_to_byte(unsigned char *dst, unsigned long *src, unsigned long size)
{
	unsigned long i, j;

	for (i = 0, j = 0; i < size; i += 4, j++) 
	{
		dst[i]   = (unsigned char)(src[j]);
		dst[i+1] = (unsigned char)(src[j] >> 8);
		dst[i+2] = (unsigned char)(src[j] >> 16);
		dst[i+3] = (unsigned char)(src[j] >> 24);
	}
}

/*=============================================*/
static void byte_to_long(unsigned long *dst, unsigned char *src, unsigned long size)
{
	unsigned long i, j;

	for (i = 0, j = 0; i < size; i += 4, j++) 
	{
		dst[j] = ((unsigned long)src[i]) | (((unsigned long)src[i+1]) << 8) |
			(((unsigned long)src[i+2]) << 16) | (((unsigned long)src[i+3]) << 24);
	}
}

/*=============================================*/