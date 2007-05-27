#include "stdafx.h"
#include "windows.h"
#include <string>
#include <fstream>
#include ".\stegano.h"
#include "huff.h"
#include "Encoder.h"
#include "Decoder.h"

using namespace std;

char testString1[] = "*enter*\
<MooseOnDaLoose> Hey \nMike\
*enter*<goatboy> \nwhat?\
*enter*<MooseOnDaLoose> \nPussy.\
*enter*<goatboy> \ner?\
*enter*<MooseOnDaLoose> \nPussy.\
*enter*<goatboy> \nand?\
*enter*<MooseOnDaLoose> \nPussy.\
*enter*<goatboy> .\n..\
*enter*<MooseOnDaLoose> Pu\nssy.\
*enter*<goatboy> i dont get\n it\
*enter*<MooseOnDaLoose> AND \nYOU NEVER WILL.\
*enter*<goatboy> \nbastard\
*enter*<goatboy> what\n a bastard\
*enter*<goatboy>life \nsucks\
*enter*<goatboy>so infosec \ndoes\
*enter*<goatboy>additional \nstring\
*enter*)))\
*enter*)))\
*enter*)))\
*enter*)))\
*enter*))))))!*&(*YHUHOIDHFSAOIWEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE*enter*";

//char testString2[] = "!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*enter*!*en\
//ter*!*enter*!*enter*!*enter*!*enter*";

//char testString2[] = "*enter**enter**enter**enter**enter**enter**enter**enter**enter**enter**enter**enter**enter**enter**e\
//nter**enter**enter*";


//char testString2[] = "hello there*enter*how r u?*enter*get up*enter*get on up*enter*get up*enter*get on up*enter*and DANCE\
//*enter*:D-<*enter*:D|-<*enter*:D/-<*enter*i'm going to become rich *enter*and famous after i *enter*\
//invent a device that allows you to*enter* stab people in the face *enter*over the internet*enter*tha\
//t's all i wanted to say*enter*hehe*enter*bye)*enter*bye...*enter*the end";

char testString2[] = "hello there*enter*how r u?*enter*get up*enter*get on up*enter*get up*enter*get on up*enter*and DANCE\
*enter*:D-<*enter*:D|-<*enter*:D/-<*enter*i'm going to become rich *enter*and famous after i *enter*\
invent a device that allows you to*enter* stab people in the face *enter*over the internet*enter*tha\
t's all i wanted to say*enter*hehe*enter*bye)*enter*bye...*enter*the end";


unsigned char gMessage[]  = {1, 0, 128, 250, 252, 253, 254, 255};

CStegano::CStegano(void)
{
	codec=new CHuff;
	Enc=new CSteganoEncoder;
	Dec=new CSteganoDecoder;
}

CStegano::~CStegano(void)
{
	delete codec;
	delete Enc;
	delete Dec;
}

bool Check(unsigned char *mess, unsigned char *decoded)
{
	for (int i = 0; i < 8; i++) {
		if (mess[i] != decoded[i]) {
			return false;
		}
	}
	return true;
}


void TestDecode(string &receive)
{
	CSteganoDecoder decoder;
	unsigned     char decoded[DATA_SIZE] = {1,1,1,1,1,1,1,1};
 	char* encoded = new char[receive.size()+1];
	ofstream ofile("c:\\TestDecode.txt");
	
	ofile << "begin " << receive << " end" << endl;
	ofile.close();

	memcpy(encoded,receive.c_str(),receive.size()+1);
	
	if(0!=decoder.decode(encoded, &decoded)) {
		   __asm int 3;
    }

	if (!Check(gMessage, decoded)) {
		__asm int 3;
	}

	delete[] encoded;
}
int CStegano::Encode(string & in,string & tosend)
{
	int len;
	char* encoded = NULL;

//	CHuff codec;
	
 	unsigned char message[8];
//	memcpy(message, in.c_str(), 8*sizeof(char));
	//__asm int 3
	if (codec->CompressHuff(in,message,len) == -1) {
		return -1;
	}

    if(0!=Enc->encode(encoded,testString2, message)) {
        return -1;
    }

	tosend=encoded;
   
	return 1;	
}

int CStegano::Decode(string & recieve,string & out)
{
	unsigned     char decoded[DATA_SIZE] = {1,1,1,1,1,1,1,1};
	char* encoded = new char[recieve.size()+1];
	memcpy(encoded,recieve.c_str(),recieve.size()+1);
	if(0!=Dec->decode(encoded, &decoded)) {
           return -1;
    }

//	if (!Check(gMessage, decoded)) {
//		__asm int 3;
//	}
	codec->DeCompressHuff(out,decoded,64);
	/*for (int i = 0; i < DATA_SIZE; i++) {
		out+= decoded[i];
	}
	out+= '\0';*/
	return 0;
}
