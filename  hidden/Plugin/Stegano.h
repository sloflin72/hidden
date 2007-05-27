#pragma once

class CSteganoEncoder;
class CSteganoDecoder;
class CHuff;
using namespace std;
class CStegano
{
	CHuff  * codec;
	CSteganoEncoder * Enc;
	CSteganoDecoder * Dec;
public:
	CStegano(void);
	~CStegano(void);
	int Encode(string & in,string & tosend);
	int Decode(string & recieve,string & out);
};