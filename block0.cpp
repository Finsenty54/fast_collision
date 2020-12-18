/*

MD5 collision generator
=======================
Source code files:
  block0.cpp
  block1.cpp
  main.cpp
  main.hpp
  md5.cpp
  block1wang.cpp
  block1stevens00.cpp
  block1stevens01.cpp
  block1stevens10.cpp
  block1stevens11.cpp
Win32 executable:
  fastcoll_v1.0.0.5.exe

Version
=======
version 1.0.0.5-1, April 2006.

Copyright
=========
� M. Stevens, 2006. All rights reserved.

Disclaimer
==========
This software is provided as is. Use is at the user's risk.
No guarantee whatsoever is given on how it may function or malfunction.
Support cannot be expected.
This software is meant for scientific and educational purposes only.
It is forbidden to use it for other than scientific or educational purposes.
In particular, commercial and malicious use is not allowed.
Further distribution of this software, by whatever means, is not allowed
without our consent.
This includes publication of source code or executables in printed form,
on websites, newsgroups, CD-ROM's, etc.
Changing the (source) code without our consent is not allowed.
In all versions of the source code this disclaimer, the copyright
notice and the version number should be present.

*/

#include <iostream>
#include <vector>
#include "main.hpp"

//m 从0开始计数
//q 从-3（即1）开始计数
uint32 seed32_1, seed32_2;

void find_block0(uint32 block[], const uint32 IV[])
{
	//初始化
	uint32 Q[68] = { IV[0], IV[3], IV[2], IV[1] };

	std::vector<uint32> q4mask(1<<4); //1<<4 = 16
	for (unsigned k = 0; k < q4mask.size(); ++k)
		q4mask[k] = ((k<<2) ^ (k<<26)) & 0x38000004;  //^ 异或 & 与
		//0011 1000 0000 0000 0000 0000 0000 0100

	std::vector<uint32> q9q10mask(1<<3);
	for (unsigned k = 0; k < q9q10mask.size(); ++k)
		q9q10mask[k] = ((k<<13) ^ (k<<4)) & 0x2060;
		
	std::vector<uint32> q9mask(1<<16);
	for (unsigned k = 0; k < q9mask.size(); ++k)
		q9mask[k] = ((k<<1) ^ (k<<2) ^ (k<<5) ^ (k<<7) ^ (k<<8) ^ (k<<10) ^ (k<<11) ^ (k<<13)) & 0x0eb94f16;

	while (true)
	{
		//Qoff =3 , 随机赋值
		//Q68 , 那么 Q4‘ == Q1 实际上从Q1开始算
		Q[Qoff + 1] = xrng64();
		Q[Qoff + 3] = (xrng64() & 0xfe87bc3f) | 0x017841c0;
		//1111 1110 1000 0111 1011 1100 0011 1111  为0处的值都改为0，为1处的值都不变
		//0000 0001 0111 1000 0100 0001 1100 0000  都变为1，其他值不变
		//满足新的差分路径条件
		Q[Qoff + 4] = (xrng64() & 0x44000033) | 0x000002c0 | (Q[Qoff + 3] & 0x0287bc00);
		Q[Qoff + 5] = 0x41ffffc8 | (Q[Qoff + 4] & 0x04000033);
		Q[Qoff + 6] = 0xb84b82d6;
		//1011 1000 0100 1011 1000 0010 1101 0110 
		Q[Qoff + 7] = (xrng64() & 0x68000084) | 0x02401b43;
		Q[Qoff + 8] = (xrng64() & 0x2b8f6e04) | 0x005090d3 | (~Q[Qoff + 7] & 0x40000000);
		Q[Qoff + 9] = 0x20040068 | (Q[Qoff + 8] & 0x00020000) | (~Q[Qoff + 8] & 0x40000000);
		Q[Qoff + 10] = (xrng64() & 0x40000000) | 0x1040b089;
		Q[Qoff + 11] = (xrng64() & 0x10408008) | 0x0fbb7f16 | (~Q[Qoff + 10] & 0x40000000);
		Q[Qoff + 12] = (xrng64() & 0x1ed9df7f) | 0x00022080 | (~Q[Qoff + 11] & 0x40200000);
		Q[Qoff + 13] = (xrng64() & 0x5efb4f77) | 0x20049008;
		Q[Qoff + 14] = (xrng64() & 0x1fff5f77) | 0x0000a088 | (~Q[Qoff + 13] & 0x40000000);
		Q[Qoff + 15] = (xrng64() & 0x5efe7ff7) | 0x80008000 | (~Q[Qoff + 14] & 0x00010000);
		Q[Qoff + 16] = (xrng64() & 0x1ffdffff) | 0xa0000000 | (~Q[Qoff + 15] & 0x40020000);

		//已经赋值给 block[]
		MD5_REVERSE_STEP(0, 0xd76aa478, 7); //(t,ac,rc);q1-q0; result = m0
		MD5_REVERSE_STEP(6, 0xa8304613, 17); //result = m6
		MD5_REVERSE_STEP(7, 0xfd469501, 22); //m7
		MD5_REVERSE_STEP(11, 0x895cd7be, 22); //m11
		MD5_REVERSE_STEP(14, 0xa679438e, 17); //m14
		MD5_REVERSE_STEP(15, 0x49b40821, 22); //m15

		//没有加 m1
		const uint32 tt1 = FF(Q[Qoff + 1], Q[Qoff + 0], Q[Qoff - 1]) + Q[Qoff - 2] + 0xe8c7b756;	
		//没有加 m1
		const uint32 tt17 = GG(Q[Qoff + 16], Q[Qoff + 15], Q[Qoff + 14]) + Q[Qoff + 13] + 0xf61e2562;
		const uint32 tt18 = Q[Qoff + 14] + 0xc040b340 + block[6];
		const uint32 tt19 = Q[Qoff + 15] + 0x265e5a51 + block[11];
		const uint32 tt20 = Q[Qoff + 16] + 0xe9b6c7aa + block[0];
		const uint32 tt5 = RR(Q[Qoff + 6] - Q[Qoff + 5], 12) - FF(Q[Qoff + 5], Q[Qoff + 4], Q[Qoff + 3]) - 0x4787c62a;

		// change q17 until conditions are met on q18, q19 and q20
		unsigned counter = 0;
		// 失败查找128次
		while (counter < (1 << 7)) //七个条件
		{
			const uint32 q16 = Q[Qoff + 16];
			uint32 q17 = ((xrng64() & 0x3ffd7ff7) | (q16&0xc0008008)) ^ 0x40000000;
			++counter;

			uint32 q18 = GG(q17, q16, Q[Qoff + 15]) + tt18;
			//得到q18值
			q18 = RL(q18, 9); q18 += q17;
			if (0x00020000 != ((q18^q17)&0xa0020000)) //1010 0000 0000 00010 ...
				//这里的条件涉及backward
				//如果不相同，也就是条件不满足，就要重新计算
				//q18有三个条件
				continue;

			uint32 q19 = GG(q18, q17, q16) + tt19;
			q19 = RL(q19, 14); q19 += q18;
			if (0x80000000 != (q19 & 0x80020000))
				continue;
			
			uint32 q20 = GG(q19, q18, q17) + tt20;
			q20 = RL(q20, 20); q20 += q19;
			if (0x00040000 != ((q20^q19) & 0x80040000))
				continue;
			
			//计算得到m1
			block[1] = q17-q16; block[1] = RR(block[1], 5); block[1] -= tt17;

			//计算得到q2
			uint32 q2 = block[1] + tt1; q2 = RL(q2, 12); q2 += Q[Qoff + 1];

			//计算得到m5
			block[5] = tt5 - q2;

			Q[Qoff + 2] = q2; // q2刚开始没有算
			Q[Qoff + 17] = q17;
			Q[Qoff + 18] = q18;
			Q[Qoff + 19] = q19;
			Q[Qoff + 20] = q20;

			//计算m2
			MD5_REVERSE_STEP(2, 0x242070db, 17);

			counter = 0;
			break;
		}

		//counter不为0，失败，重新选择q1,q3,...,q16
		if (counter != 0)
			continue;

		const uint32 q4 = Q[Qoff + 4];
		const uint32 q9backup = Q[Qoff + 9];
		const uint32 tt21 = GG(Q[Qoff+20], Q[Qoff+19], Q[Qoff+18]) + Q[Qoff+17] + 0xd62f105d;

		// iterate over possible changes of q4 
		// 迭代q4的所有可能值
		// while keeping all conditions on q1-q20 intact
		// 但q1-q20的充分条件全部都保持，通过改变消息值 使得成立
		// this changes m3, m4, m5 and m7
		// 如果是klima的tunnel的话，m5应该不会变
		unsigned counter2 = 0;
		while (counter2 < (1<<4))
		{
			//tunnel??
			//有些值是要满足的，是要不变的
			Q[Qoff+4] = q4 ^ q4mask[counter2];
			++counter2;

			// 计算得到m5
			MD5_REVERSE_STEP(5, 0x4787c62a, 12);

			// 计算得到q21
			uint32 q21 = tt21 + block[5];
			q21 = RL(q21,5); q21 += Q[Qoff+20];
			if (0 != ((q21^Q[Qoff+20]) & 0x80020000))
				continue;

			Q[Qoff + 21] = q21;

			//q4 变了，计算出m3,m4,m7
			// q4 tunnel:q4-q8，不会影响q6,q7的等式,只要q6=0,q7=1
			// q4,q5,q8的等式，通过改变消息而使的等式仍然成立
			MD5_REVERSE_STEP(3, 0xc1bdceee, 22);
			MD5_REVERSE_STEP(4, 0xf57c0faf, 7);
			MD5_REVERSE_STEP(7, 0xfd469501, 22);
			
			const uint32 tt22 = GG(Q[Qoff + 21], Q[Qoff + 20], Q[Qoff + 19]) + Q[Qoff + 18] + 0x02441453;
			const uint32 tt23 = Q[Qoff + 19] + 0xd8a1e681 + block[15];
			const uint32 tt24 = Q[Qoff + 20] + 0xe7d3fbc8 + block[4];

			const uint32 tt9 = Q[Qoff + 6] + 0x8b44f7af;
			const uint32 tt10 = Q[Qoff + 7] + 0xffff5bb1;
			const uint32 tt8 = FF(Q[Qoff + 8], Q[Qoff + 7], Q[Qoff + 6]) + Q[Qoff + 5] + 0x698098d8;		
			const uint32 tt12 = RR(Q[Qoff+13]-Q[Qoff+12],7) - 0x6b901122;
			const uint32 tt13 = RR(Q[Qoff+14]-Q[Qoff+13],12) - FF(Q[Qoff+13],Q[Qoff+12],Q[Qoff+11]) - 0xfd987193;

			// iterate over possible changes of q9 and q10
			// 迭代q9,q10的所有可能值
			// while keeping conditions on q1-q21 intact
			// 保持q1-q21的条件不变
			// this changes m8, m9, m10, m12 and m13 (and not m11!)
			// the possible changes of q9 that also do not change m10 are used below
			for (unsigned counter3 = 0; counter3 < (1<<3);)
			{
				uint32 q10 = Q[Qoff+10] ^ (q9q10mask[counter3] & 0x60);
				Q[Qoff + 9] = q9backup ^ (q9q10mask[counter3] & 0x2000);
				++counter3;

				//计算得到m10
				uint32 m10 = RR(Q[Qoff+11]-q10,17);
				m10 -= FF(q10, Q[Qoff+9], Q[Qoff+8]) + tt10;

				//计算得到q22
				uint32 aa = Q[Qoff + 21];
				uint32 dd = tt22+m10; dd = RL(dd, 9) + aa;
				if (0x80000000 != (dd & 0x80000000)) continue; //这里有问题
				//0x00000000!=(dd & 0x80000000)

				//计算得到q23
				uint32 bb = Q[Qoff + 20];
				uint32 cc = tt23 + GG(dd, aa, bb); 
				if (0 != (cc & 0x20000)) continue;
				cc = RL(cc, 14) + dd;
				if (0 != (cc & 0x80000000)) continue;

				//计算得到q24
				bb = tt24 + GG(cc, dd, aa); bb = RL(bb, 20) + cc;
				//q24_32=1
				if (0 == (bb & 0x80000000)) continue;

				block[10] = m10;
				//计算新的m13
				block[13] = tt13 - q10;

				//其他消息没有在前几轮产生影响，所以没事，不用计算，下一个tunnel还是要改变的


				// iterate over possible changes of q9
				// while keeping intact conditions on q1-q24
				// this changes m8, m9 and m12 (but not m10!)
				for (unsigned counter4 = 0; counter4 < (1<<16); ++counter4)
				{
					//这里计算出 8，9，12
					uint32 q9 = Q[Qoff + 9] ^ q9mask[counter4];
					block[12] = tt12 - FF(Q[Qoff + 12], Q[Qoff + 11], q10) - q9;
					uint32 m8 = q9 - Q[Qoff + 8];
					block[8] = RR(m8, 7) - tt8; 
					uint32 m9 = q10 - q9;
					block[9] = RR(m9, 12) - FF(q9, Q[Qoff + 8], Q[Qoff + 7]) - tt9; 

					//aa=21,bb=24,cc=23,dd=22
					uint32 a = aa, b = bb, c = cc, d = dd;
					// get 25
					MD5_STEP(GG, a, b, c, d, block[9], 0x21e1cde6, 5); 
					MD5_STEP(GG, d, a, b, c, block[14], 0xc33707d6, 9);
					MD5_STEP(GG, c, d, a, b, block[3], 0xf4d50d87, 14);
					MD5_STEP(GG, b, c, d, a, block[8], 0x455a14ed, 20);
					MD5_STEP(GG, a, b, c, d, block[13], 0xa9e3e905, 5);
					MD5_STEP(GG, d, a, b, c, block[2], 0xfcefa3f8, 9);
					MD5_STEP(GG, c, d, a, b, block[7], 0x676f02d9, 14);
					MD5_STEP(GG, b, c, d, a, block[12], 0x8d2a4c8a, 20);
					MD5_STEP(HH, a, b, c, d, block[5], 0xfffa3942, 4);
					// get 34
					MD5_STEP(HH, d, a, b, c, block[8], 0x8771f681, 11);
					
					//测试q35是否满足条件，根据lila05
					c += HH(d, a, b) + block[11] + 0x6d9d6122;
					if (0 != (c & (1 << 15))) 
						continue;
					c = (c<<16 | c>>16) + d;
					
					MD5_STEP(HH, b, c, d, a, block[14], 0xfde5380c, 23);
					MD5_STEP(HH, a, b, c, d, block[1], 0xa4beea44, 4);
					MD5_STEP(HH, d, a, b, c, block[4], 0x4bdecfa9, 11);
					MD5_STEP(HH, c, d, a, b, block[7], 0xf6bb4b60, 16);
					MD5_STEP(HH, b, c, d, a, block[10], 0xbebfbc70, 23);
					MD5_STEP(HH, a, b, c, d, block[13], 0x289b7ec6, 4);
					MD5_STEP(HH, d, a, b, c, block[0], 0xeaa127fa, 11);
					MD5_STEP(HH, c, d, a, b, block[3], 0xd4ef3085, 16);
					MD5_STEP(HH, b, c, d, a, block[6], 0x04881d05, 23);
					MD5_STEP(HH, a, b, c, d, block[9], 0xd9d4d039, 4);
					MD5_STEP(HH, d, a, b, c, block[12], 0xe6db99e5, 11);
					MD5_STEP(HH, c, d, a, b, block[15], 0x1fa27cf8, 16);

					//get q48
					MD5_STEP(HH, b, c, d, a, block[2], 0xc4ac5665, 23);
					if (0 != ((b^d) & 0x80000000))
						continue;

					MD5_STEP(II, a, b, c, d, block[0], 0xf4292244, 6);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, d, a, b, c, block[7], 0x432aff97, 10);
					if (0 == ((b^d) >> 31)) continue;
					MD5_STEP(II, c, d, a, b, block[14], 0xab9423a7, 15);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, b, c, d, a, block[5], 0xfc93a039, 21);
					if (0 != ((b^d) >> 31)) continue;
					MD5_STEP(II, a, b, c, d, block[12], 0x655b59c3, 6);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, d, a, b, c, block[3], 0x8f0ccc92, 10);
					if (0 != ((b^d) >> 31)) continue;
					MD5_STEP(II, c, d, a, b, block[10], 0xffeff47d, 15);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, b, c, d, a, block[1], 0x85845dd1, 21);
					if (0 != ((b^d) >> 31)) continue;
					MD5_STEP(II, a, b, c, d, block[8], 0x6fa87e4f, 6);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, d, a, b, c, block[15], 0xfe2ce6e0, 10);
					if (0 != ((b^d) >> 31)) continue;
					MD5_STEP(II, c, d, a, b, block[6], 0xa3014314, 15);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, b, c, d, a, block[13], 0x4e0811a1, 21);
					if (0 == ((b^d) >> 31)) continue;
					MD5_STEP(II, a, b, c, d, block[4], 0xf7537e82, 6);
					if (0 != ((a^c) >> 31)) continue;
					MD5_STEP(II, d, a, b, c, block[11], 0xbd3af235, 10);
					if (0 != ((b^d) >> 31)) continue;
					MD5_STEP(II, c, d, a, b, block[2], 0x2ad7d2bb, 15);
					if (0 != ((a^c) >> 31)) continue;
					// get q64
					MD5_STEP(II, b, c, d, a, block[9], 0xeb86d391, 21);

					uint32 IHV1 = b + IV[1];
					uint32 IHV2 = c + IV[2];
					uint32 IHV3 = d + IV[3];

					bool wang = true;
					if (0x02000000 != ((IHV2^IHV1) & 0x86000000)) wang = false;
					if (0 != ((IHV1^IHV3) & 0x82000000)) wang = false;
					if (0 != (IHV1 & 0x06000020)) wang = false;
					
					bool stevens = true;
					if ( ((IHV1^IHV2)>>31)!=0 || ((IHV1^IHV3)>>31)!= 0 ) stevens = false;
					if ( (IHV3&(1<<25))!=0 || (IHV2&(1<<25))!=0 || (IHV1&(1<<25))!=0 
						|| ((IHV2^IHV1)&1)!=0) stevens = false;
										
					if (!(wang || stevens)) continue;

					std::cout << "." << std::flush;

					uint32 IV1[4], IV2[4];
					for (int t = 0; t < 4; ++t)
						IV2[t] = IV1[t] = IV[t];

					uint32 block2[16];
					for (int t = 0; t < 16; ++t)
						block2[t] = block[t];
					block2[4] += 1<<31;
					block2[11] += 1<<15;
					block2[14] += 1<<31;

					md5_compress(IV1, block);
					md5_compress(IV2, block2);
					if (	   (IV2[0] == IV1[0] + (1<<31))
							&& (IV2[1] == IV1[1] + (1<<31) + (1<<25))
							&& (IV2[2] == IV1[2] + (1<<31) + (1<<25))
							&& (IV2[3] == IV1[3] + (1<<31) + (1<<25)))
						return;

					if (IV2[0] != IV1[0] + (1<<31))
						std::cout << "!" << std::flush;
				}
			}
		}
	}
}
