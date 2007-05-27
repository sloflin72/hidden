// spartcus.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <conio.h>
#include "encoder.h"
#include "decoder.h"

char testString[] = "*enter*\
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

char testString2[] = "\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\
\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*\n*enter*";
unsigned char message[]= {1,1,1,2,2,1,1,1};



int doTest1() {
char* encoded = NULL;

unsigned     char decoded[DATA_SIZE] = {0,0,0,0,0,0,0,0};
    CSteganoEncoder *encoder = new CSteganoEncoder;
	CSteganoDecoder *decoder = new CSteganoDecoder;
    
    if(0!=encoder->encode(encoded,testString, message)) {
        puts("encode error!");
        return -1;
    }
    
    puts(encoded);
    if(0!=decoder->decode(encoded, &decoded)) {
        puts("decode error!");
        return -1;
    };

    for(int i = 0; i < DATA_SIZE; i++) {
        printf("%i", decoded[i]);
        
        if(message[i]!=decoded[i]) {        
            puts("decoded message differs from source!");
            printf("message[%i]=%i, decoded[%i]=%i", i, message[i],  i, decoded[i]);
            return -1;
        };
    };
    return 0;
    
}
/*
int doTest2() {
    unsigned     char messageDecoded[]= {0,0,0,0,0,0,0,0};
    
    Code code[DATA_SIZE*2];
    encodeBytes(message,code, char ** messages, int size);
    
    decodeBytes(&messageDecoded, code);
    
    for(int i = 0; i < DATA_SIZE; i++) {
        //printf("messageDecoded[%i]=%i", i, messageDecoded[i]);
        if(message[i]!=messageDecoded[i]) {        
            puts("decoded byte_message differs from source!");
            printf("messageDecoded[%i]=%i, message[%i]=%i", i, messageDecoded[i], i , message[i]);
            return -1;
        };
    };
    return 0;
    
    
};
*/


int _tmain(int argc, _TCHAR* argv[])
{


   
	/*
    for(int i = 0; i < 10; i++) {
        printf("%u\n", intRandom(100));
    };
	*/
    if(0 == doTest1()) {
          puts("doTest1() success!");
   };

    
    getch();
	return 0;
}

