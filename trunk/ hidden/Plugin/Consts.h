#ifndef __CONSTS_SPART__
#define __CONSTS_SPART__
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define DATA_SIZE  8
#define SEPARATOR "*enter*"
#define TOTAL_MESSAGES 2 * DATA_SIZE





typedef struct Code{
       unsigned char spacesParity;
       unsigned       char returnsParity;
       unsigned       char pointsParity;
       unsigned       char spacesMod;
} Code;
int intRandom(int ceiling) {
	srand(time(NULL));
    return (int)(ceiling * rand()/ (double)RAND_MAX);
};
void insert(char * &string, char * where, char * what) {
    char* temp = (char *)malloc(8192);//strlen(where) + 1);
    //save tail of the string
    memcpy(temp, where,strlen(where));
    temp[strlen(where)] = 0;
    strcpy(where, what);
    //term zero
//	puts(temp);
    memcpy(where + strlen(what), temp, strlen(temp) + 1);
    free(temp);
};  
//remove one char at position where
void remove(char * &string, char * where, int count) {
    char* temp = (char *)malloc(strlen(where+count) + 1);
    //save tail of the string
    memcpy(temp, where + count,strlen(where+count));
    temp[strlen(where+count)] = 0;
    strcpy(where, temp);
    free(temp);
}; 

int getOccurences(char * string, char * stringToFind) {

    char * nextOccurence = strstr(string, stringToFind);
    char * previousOccurence = string;
    
    int temp = 0;
    int length = strlen(stringToFind);
    while(NULL != nextOccurence) {
               temp++;
               previousOccurence  = nextOccurence + length;
               nextOccurence = strstr(previousOccurence, stringToFind);
   };
   
   return temp;
};
int addSpace(char * &instr) {

int count = getOccurences(instr, " ");
if(0 == count) {
	memcpy(instr+strlen(instr), " ", 2);
	//instr[strlen(instr) + 1] = ' ';
	//instr[strlen(instr) + 2] = 0;
} else {
	int 		pos = 1 + intRandom(count-1);
	char * currPos = instr;
	
	for(int i = 0; i < pos; i++) {
		currPos = strstr(currPos+1, " ");
	};
	insert(instr, currPos, " ");
};

return 0;
}

int parseString2(char *instr, char** &messages) {

    int messagesCount = getOccurences(instr, SEPARATOR);
    
    if(messagesCount < TOTAL_MESSAGES) {
        puts("a");
		return -1;
    };    
    
    int temp = 0;
    char * message;
    
    messages = (char**) malloc(sizeof(char*) * messagesCount);
        
    char* previousMessage = instr;
    char* nextMessage = NULL;
    
    nextMessage = strstr(instr, SEPARATOR);
    
    while(NULL != nextMessage) {
        //+16 due to string will be changed
        message = (char *) malloc((nextMessage - previousMessage) * sizeof(char) + 32);
        messages[temp] = message;
        memcpy(message, previousMessage, nextMessage - previousMessage);
        message[nextMessage - previousMessage] = 0;
        
        previousMessage = nextMessage+sizeof(SEPARATOR) - 1;
        nextMessage = strstr(previousMessage, SEPARATOR);
        temp++;                 
    };
    //last one
        message = (char *) malloc(strlen(previousMessage) * sizeof(char) + 32);
        messages[temp] = message;
        memcpy(message, previousMessage, strlen(previousMessage));
        message[strlen(previousMessage)] = 0;
        temp++;
        
        return temp;
    
};    


int setSpaces(Code code, char * message) {
    char* singleSpace = " ";
    char * doubleSpaces = "  ";

    int length = strlen(message);
    int count = getOccurences(message, singleSpace);

    if( (count & 1) != code.spacesParity) {
        addSpace(message);     
		//memcpy(message+length,singleSpace, 2);

    };
     length = strlen(message);
     count = getOccurences(message, singleSpace);


    if( ((count>>1) & 1) != code.spacesMod) {
        addSpace(message);     
		addSpace(message);     
		//memcpy(message+length,doubleSpaces, 3);
             length+=2;
             count+=2;
    };
    

    return 0;

};
//message should be long enough!
int setReturns(Code code, char * message){
    int length = strlen(message);
    char * addOn = "\n";
    if(getOccurences(message, addOn) % 2 != code.returnsParity) {
        if(NULL != (strstr(message, "\n"))) {
                remove(message, strstr(message, "\n"), strlen("\n"));
                return 1;
        };    
        memcpy(&message[length], addOn, 3);
    };
    return 1;
};

int setPoints(Code code, char * message) {
    int length = strlen(message);
    char * addOn = ".";

    if( getOccurences(message, addOn) % 2 != code.pointsParity) {
        memcpy(message+length, addOn, 2);
    };
	return 0;
}


#endif