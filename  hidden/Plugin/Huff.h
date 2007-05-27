#pragma once
//alphabet
//a-z 0-9 
//space,dot,comma
//39 symbols
//less then 6 bits per symbol
//i hope this encoding compress some bits better
using namespace std;
typedef enum eMethod
{
	BIT8,
	BIT6,
	HUFFMAN
}eMethod;
class CHuff
{
	string m_sTbl[39];
	DWORD m_dwMethod;
public:
	CHuff(void);
	~CHuff(void);
	void SetMethod(eMethod method){m_dwMethod=method;}
	int CompressHuff(const string &input_str,PBYTE pData,int & length);
	int DeCompressHuff(string &output_str,PBYTE pData,int length);
private:
//convert string to array of numbers
	int Convert(const string &input,int * out);
	int DeConvert(string &output,int * in,int length);
	int findch(string &str);
	int bitstr2UCHAR(string &str,PBYTE  pData,int kbit);
	int UCHAR2bitstr(string &str,PBYTE pData,int length);
//my_table
//0-9 0-9
//a-z 10-35
//space 36
//dot 37
//comma 38
	int CharToInt(char ch);
	char IntToChar(int num);
};
