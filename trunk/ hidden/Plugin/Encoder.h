#include "consts.h"
#include <fstream>
#include <iostream>
#include <string>
#define MAX_STRINGS_TO_MUTATE 4

#define FILE_NAME "bash.txt"
#define MAX_STRING_SIZE 8192


using namespace std;

class  CSteganoEncoder{
private:
	
	
	char * stringsToMutate[MAX_STRINGS_TO_MUTATE];
	
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
	CSteganoEncoder() {

		char * EMPTY_STRING = "";

		for(int i =0; i < MAX_STRINGS_TO_MUTATE; i++) {
			
			stringsToMutate[i] = EMPTY_STRING;
		}
				
		srand(time(NULL));
		ifstream ifile(FILE_NAME);
		
		if(!ifile) {
			//cout << "no file";
			return;
		};
		int temp = 0;
		
		while((!ifile.eof()) && (temp < MAX_STRINGS_TO_MUTATE)){
			char * currentMessage= (char *)malloc(MAX_STRING_SIZE);
			ifile.getline(currentMessage, MAX_STRING_SIZE);
			stringsToMutate[temp] = currentMessage;
			temp++;
			//puts(currentMessage);
		}		
		ifile.close();
	};

//data двоичные данные
//instr строка которую нужно видоизменить.
//data всегда 64 бита = 8 unsigned chars
//instr - не менее 15 предложений(сообщений), разделенные тегом *enter*
//outst - выходная строка
// returns -1 if error, 0 otherwise

int encode(char *&outstr,char *instr, unsigned char *data) {

	Code code[TOTAL_MESSAGES] = {0,0,0,0,0,0,0,0};
	
	srand(time(NULL));
//	__asm int 3;
	int num = rand()%MAX_STRINGS_TO_MUTATE;
	instr = stringsToMutate[num];
//	instr = stringsToMutate[intRandom(MAX_STRINGS_TO_MUTATE)];

	char ** messages = NULL;
    int size = 0;
    //puts(instr);
    size = parseString2(instr,messages);

    if(NULL == messages) {
		//cout << "1";
        return -1;
    };    
    if(-1 == size) {
		//cout << "2";
        return -1;
    };    
    //will change only first TOTAL_MESSAGES messages)
    encodeBytes(data, code);
    
    for(int i = 0; i < TOTAL_MESSAGES; i++) {
            setPoints(code[i], messages[i]);
			setSpaces(code[i], messages[i]);
                          
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