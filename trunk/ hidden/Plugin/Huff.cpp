#include "stdafx.h"
#include <string>
#include "windows.h"
#include "freqtable.h"
#include ".\huff.h"
using namespace std;

CHuff::CHuff(void):m_dwMethod(BIT6)
{
	CFreqTable table;
	table.GenerateTable(m_sTbl);
}

CHuff::~CHuff(void)
{
}

int CHuff::CompressHuff(const string &input_str,PBYTE pData,int & length)
{
	int * iout;
	int temp;
	string bitstr;
	switch(m_dwMethod)
	{
	case BIT8:
		if(input_str.size()>8) return -1;
		memset(pData,0,8);
		memcpy(pData,input_str.c_str(),input_str.size());
	break;
	case BIT6:
		if(input_str.size()>10) return -1;
		iout=new int[11];
		Convert(input_str,iout);
		if(input_str.size()<10)
			iout[input_str.size()]=39;
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<6;j++)
			{
				if(0==(iout[i] & 32))
					bitstr.push_back('0');
				else 
					bitstr.push_back('1');
				iout[i]=iout[i]<<1;
			}
		}
		bitstr2UCHAR(bitstr,pData,8);
		delete iout;
		break;
	case HUFFMAN:
		int len=input_str.size();
		memset(pData,0,sizeof(BYTE)*8);
		int * temp=new int[len];
		Convert(input_str,temp);
		string str="";
		for(int i=0;i<len;i++)
		{
			string st=m_sTbl[temp[i]];
			str+=st;
		}
		while(str.size() % 8)
			str+='0';
		length=str.size();
		int ucharcount;
		if(length/8<=8)
			ucharcount=length/8;
		else
			return -1;
		for(int i=0;i<ucharcount;i++)
		{
			BYTE temp=0;
			for(int k=0;k<8;k++)
			{
				temp<<=1;
				temp+=((int)str[i*8+k])-0x30;
 			}
			pData[i]=temp;
		}
		break;
}
	return 0;
}
int CHuff::DeCompressHuff(string & output_str,PBYTE pData,int length)
{
	BYTE buf[10];
	int istr[10];

	string bitstr;
	switch(m_dwMethod)
	{

	case BIT8:
		for(int i=0;i<8;i++)
			output_str.push_back(pData[i]);
		break;
	case BIT6:
		UCHAR2bitstr(bitstr,pData,64);
		bitstr2UCHAR(bitstr,(PBYTE)buf,6);
		for(int i=0;i<10;i++)
			istr[i]=buf[i];
        DeConvert(output_str,istr,10);
		break;
	case HUFFMAN:
		int len=length/8;
		if (length % 8!=0) len++;
		string str="";
 		for(int i=0;i<len;i++)
		{
			BYTE temp=*pData++;
			//char chBuf[9];
			//chBuf[8]='\0';
			for(int k=0;k<8;k++)
			{
				if(temp & 128)
					//chBuf[k]='1';
					str.push_back('1');
				else
					str.push_back('0');
					//chBuf[k]='0';
				temp=temp<<1;
			}
		//	str+=chBuf;
		}
		int istr[100];
 		int count=0;
		
		while(str.size()>0)
		{
			istr[count++]=findch(str);
		}
		DeConvert(output_str,istr,count);
		break;
	}
	return 0;
}
int CHuff::findch(string &str)
{
	string::size_type loc=12;
	int i=0;
	while((loc!=0)&&(i<40))
	{
		loc=str.find(m_sTbl[i],0);
		i++;
	}
	i--;
	if(i==39) 
		str.clear();
	else
		str.erase(0,m_sTbl[i].size());
	return i;
}
int CHuff::DeConvert(string &out,int * in,int length)
	{
		for(int i=0;i<length;i++)
		{
			char ch=IntToChar(in[i]);
			if(ch=='\0') return 0;
			out.push_back(ch);
		}
		return 0;
	}
int CHuff::Convert(const string &in,int * out)
	{
		for(int i=0;i<in.size();i++)
			out[i]=CharToInt(in[i]);	
		return 0;
	}
//my_table
//0-9 0-9
//a-z 10-35
//space 36
//dot 37
//comma 38

int CHuff::CharToInt(char ch)
{

    //numbers
	if(('0'<=ch)&&(ch<='9'))
		return (ch-0x30);
	else if(('a'<=ch)&&(ch<='z'))
		return ((ch-0x61)+10);
	else if(ch==' ')
		return 36;
	else if(ch=='.')
		return 37;
	else if(ch==',')
		return 38;
	else if(ch=='\0')
		return 39;
	else
		return -1;
}
char CHuff::IntToChar(int ii)
{
	char ch;
	if(ii<10)
		ch=ii+0x30;
	else if(ii<36)
		ch=ii-10+0x61;
	else if(ii==36)
		ch=' ';
	else if(ii==37)
		ch='.';
	else if(ii==38)
		ch=',';
	else if(ii==39) 
		ch='\0';
	else 
		return '\0';
		
	return ch;
}

int CHuff::bitstr2UCHAR(string &str,PBYTE  pData,int kbit)
{
	while(str.size() % 8)
			str+='0';
		int length=str.size();
		int ucharcount=length/kbit;
		
		for(int i=0;i<ucharcount;i++)
		{
			BYTE temp=0;
			for(int k=0;k<kbit;k++)
			{
				temp<<=1;
				temp+=((int)str[i*kbit+k])-0x30;
 			}
			pData[i]=temp;
		}
	return 0;
}
int CHuff::UCHAR2bitstr(string &str,PBYTE pData,int length)
{
		int len=length/8;
		if (length % 8!=0) len++;
 		for(int i=0;i<len;i++)
		{
			BYTE temp=*pData++;
			//char chBuf[9];
			//chBuf[8]='\0';
			for(int k=0;k<8;k++)
			{
				if(temp & 128)
					//chBuf[k]='1';
					str.push_back('1');
				else
					str.push_back('0');
					//chBuf[k]='0';
				temp=temp<<1;
			}
		//	str+=chBuf;
		}
		return 0;
}