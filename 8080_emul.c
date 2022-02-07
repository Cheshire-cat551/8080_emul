#include<stdc++.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>


//Stores condition flags
typedef struct ConditionCodes{
	uint8_t z:1;
	uint8_t s:1;
	uint8_t p:1;
	uint8_t cy:1;
	uint8_t ac:1;
	uint8_t padding:3;
}cc;

cc cc_zspac={1,1,1,0,1};

// Stores state of all 8080 registers 
typedef struct State{
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint8_t sp;
	uint8_t pc;
	uint8_t *mem;
	struct cc cc;
	uint8_t int_enable;
}State;

//disassemble helping function to convert objdump to instructions
int disassemble(unsigned char* buffer, int pc){
	
}


// emulating 8080
int emulate(state *st){
	int cycles=4;
	unsigned char *opcode=&st->memory[st->pc];
	disaasemble(st->memory, st->pc);
	
	st->pc+=1;
	swich(*opcode){
	}	
}




// Initializing State  structure and giving 16K memory;
state* Init(void){
	state* st=calloc(1,sizeof(state));
	state->memory=malloc(0x10000);
	return state;
}

 
// each instruction in 8080 is 1-3 bytes
// timing important factor
int main(int argc,char**argv){
	int finished=0;
	int blank_cycles=0;
	state *st=Init();
	
	//reading game files 
	
	
	
	//lopping through all instructions
	while(finished==0){
		finished=emulate(state);
	}
	
	return 0;
}
