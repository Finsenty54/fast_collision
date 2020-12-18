#include <iostream>
#include <time.h>
#include "main.hpp"
#include <boost/timer/timer.hpp>

using namespace std;
using namespace boost::timer;

const uint32 MD5IV[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };
unsigned load_block(istream& i, uint32 block[]);
void save_block(ostream& o, const uint32 block[]);

//传的地址，跟引用差不多
void find_collision(const uint32 IV[], uint32 msg1block0[], uint32 msg1block1[], uint32 msg2block0[], uint32 msg2block1[], bool verbose = false);

int main()
{
    seed32_1=uint32(time(NULL));
    seed32_2=0x12345678;
    //MD5已经定义值
    uint32 IV[4] = { MD5IV[0], MD5IV[1], MD5IV[2], MD5IV[3] };
    //两个消息，每个有两个512bit块
	//16 * 32 = 512
	uint32 msg1block0[16]={0};
	uint32 msg1block1[16]={0};
	uint32 msg2block0[16]={0};
	uint32 msg2block1[16]={0};

	//计时器？？
	boost::timer::auto_cpu_timer runtime;
	find_collision(IV, msg1block0, msg1block1, msg2block0, msg2block1,true);

    return 0;
}

//C++ 传数组给一个函数，数组类型自动转换为指针类型，因而传的实际是地址。
void find_collision(const uint32 IV[], uint32 msg1block0[], uint32 msg1block1[], uint32 msg2block0[], uint32 msg2block1[], bool verbose)
{
	if (verbose)
		cout << "Generating first block: " << flush;
	find_block0(msg1block0, IV);

	uint32 IHV[4] = { IV[0], IV[1], IV[2], IV[3] };
    uint32 IHV_TE[4] = { IV[0], IV[1], IV[2], IV[3] };
    //计算得到新的IHV
	md5_compress(IHV, msg1block0);

	if (verbose)
		cout << endl << "Generating second block: " << flush;
	find_block1(msg1block1, IHV);

    md5_compress(IHV, msg1block1);

	for (int t = 0; t < 16; ++t)
	{
		msg2block0[t] = msg1block0[t];
		msg2block1[t] = msg1block1[t];
	}

	//加上差分，得到m2
	msg2block0[4] += 1 << 31; msg2block0[11] += 1 << 15; msg2block0[14] += 1 << 31;
	msg2block1[4] += 1 << 31; msg2block1[11] -= 1 << 15; msg2block1[14] += 1 << 31;
	
    md5_compress(IHV_TE,msg2block0);
    md5_compress(IHV_TE,msg2block1);

    cout<<"\n\nmsg1block0\n";
    for (size_t i = 0; i < 16; i++)
    {
        cout<<hex<<msg1block0[i]<<' ';
    }
    cout<<endl;
    cout<<"\nmsg1block1"<<endl;
    for (size_t i = 0; i < 16; i++)
    {
        cout<<hex<<msg1block1[i]<<' ';
    }
    cout<<endl;

    cout<<"\n\nmsg2block0\n";
    for (size_t i = 0; i < 16; i++)
    {
        cout<<hex<<msg2block0[i]<<' ';
    }
    cout<<endl;
    cout<<"\nmsg2block1"<<endl;
    for (size_t i = 0; i < 16; i++)
    {
        cout<<hex<<msg2block1[i]<<' ';
    }
    cout<<endl;

    //验证消息是否碰撞
    cout<<"\n碰撞hash值： ";
    for (size_t i = 0; i < 4; i++)
    {
        cout<<IHV[i];
        if (IHV[i]!=IHV_TE[i])
        {
            cout<<"not success\n";
        }
        
    }
    cout<<"\n\n时间：\n";
}