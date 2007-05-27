#include "consts.h"
class CSteganoDecoder
{

private:
	void decodeBytes(unsigned char  (*message)[DATA_SIZE], Code* code) {
     for(int i = 0; i < DATA_SIZE; i++) {
             (*message)[i]^= code[2*i].spacesParity;
             (*message)[i]^= code[2*i].returnsParity<<1;
             (*message)[i]^= code[2*i].pointsParity<<2;
             (*message)[i]^= code[2*i].spacesMod<<3;
             
             (*message)[i]^= code[2*i+1].spacesParity<<4;
             (*message)[i]^= code[2*i+1].returnsParity<<5;
             (*message)[i]^= code[2*i+1].pointsParity<<6;
             (*message)[i]^= code[2*i+1].spacesMod<<7;
             

     };
	}
public:

int decode(char *instr, unsigned char  (*data)[DATA_SIZE]) {
   char ** messages = NULL;
   int size = 0;
   memset(data, 0, DATA_SIZE);
   size =   parseString2(instr, messages);
   
   if(-1 == size) {
       return -1;
   };
       
   Code* code = (Code*) malloc(2*sizeof(Code)*DATA_SIZE);

   for(int i = 0; i < DATA_SIZE * 2; i++) {
           code[i].spacesParity = getOccurences(messages[i], " ") & 1;
           code[i].spacesMod = (getOccurences(messages[i], " ") >> 1 ) & 1;
           code[i].returnsParity = getOccurences(messages[i], "\n") % 2;
           code[i].pointsParity = getOccurences(messages[i], ".") % 2;
           

   };
   
   decodeBytes(data,code);
   
    
    return 0;
};
};