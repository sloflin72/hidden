#include "consts.h"
#include <fstream>
#include <iostream>
//#include <string>
//#define MAX_STRING_TO_MUTATE 1
//#define FILE_NAME "bash.txt"
using namespace std;

class  CSteganoEncoder{
private:
	
	/*
	char * stringsToMutate[MAX_STRING_TO_MUTATE];*/
	
	void encodeBytes(unsigned char* message, Code code[DATA_SIZE]) {
      
       Code temp;
       
       for(int i = 0; i < DATA_SIZE; i++) {
               temp.spacesParity = (message[i]>>0) & 1;
               temp.returnsParity = (message[i]>>1) & 1;
               temp.pointsParity = (message[i]>>2) & 1;
               temp.spacesMod = (message[i]>>3) & 1;
               code[2*i] = temp;

               temp.spacesParity = (message[i]>>4) & 1;
               temp.returnsParity = (message[i]>>5) & 1;
               temp.pointsParity = (message[i]>>6) & 1;
               temp.spacesMod = (message[i]>>7) & 1;
               code[2*i + 1] = temp;               

       };
       

}

public:
	//CSteganoEncoder() {
	//	
	//	
	//		for(int i =0; i < MAX_STRING_TO_MUTATE; i++) {
	//			cout << "no file";
	//			//to be null if no file exists
	//			stringsToMutate[i] = NULL;
	//		}

	//		string currentMonolog;
	//		string currentMessage;
	//	
	//		ifstream ifile(FILE_NAME);
	//	
	//		if(!ifile) {
	//			return;
	//		};
	//	
	//		while(!ifile.eof()){
	//			currentMessage+=ifile.get();
	//		}		

	//		cout << currentMessage;

	//};

//data двоичные данные
//instr строка которую нужно видоизменить.
//data всегда 64 бита = 8 unsigned chars
//instr - не менее 15 предложений(сообщений), разделенные тегом *enter*
//outst - выходная строка
// returns -1 if error, 0 otherwise

int encode(char *&outstr,char *instr, unsigned char *data) {

	Code code[TOTAL_MESSAGES] = {0,0,0,0,0,0,0,0};
    
    char ** messages = NULL;
    int size = 0;
    
    size = parseString2(instr,messages);

    if(NULL == messages) {
        return -1;
    };    
    if(-1 == size) {
        return -1;
    };    
    //will change only first TOTAL_MESSAGES messages)
    encodeBytes(data, code);
    
    for(int i = 0; i < TOTAL_MESSAGES; i++) {
            setSpaces(code[i], messages[i]);
            setPoints(code[i], messages[i]);              
            setReturns(code[i], messages[i]);
    };
    
    int futureStringLength = 0;
    
    for(int i =0; i < size; i++) {
        futureStringLength+=strlen(messages[i]);
    };    
    
    char* resultantMessage = (char *)malloc(futureStringLength + strlen(SEPARATOR) * size + 1);
    
    char * currentPosition = resultantMessage;

    for(int i = 0; i < size; i++) {
            int length = strlen(messages[i]);
            memcpy(currentPosition, messages[i],length);
            currentPosition+=length;
            memcpy(currentPosition, SEPARATOR, strlen(SEPARATOR));
            currentPosition+=strlen(SEPARATOR);
    };
    currentPosition[0] = 0;
    outstr = resultantMessage;
    return 0;
    
	}
};