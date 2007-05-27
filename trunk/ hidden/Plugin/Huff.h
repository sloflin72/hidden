#pragma once
//alphabet
//a-z 0-9 
//space,dot,comma
//39 symbols
//less then 6 bits per symbol
//i hope this encoding compress some bits better
using namespace std;
class CHuff
{
	string m_sTbl[39];
	
public:
	CHuff(void);
	~CHuff(void);
	int CompressHuff(const string &input_str,PBYTE pData,int & length);
	int DeCompressHuff(string &output_str,PBYTE pData,int length);
private:
//convert string to array of numbers
	int Convert(const string &input,int * out);
	int DeConvert(string &output,int * in,int length);
	int findch(string &str);
//my_table
//0-9 0-9
//a-z 10-35
//space 36
//dot 37
//comma 38
	int CharToInt(char ch);
	char IntToChar(int num);
};
