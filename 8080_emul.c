#include<stdio.h>
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
typedef struct state{
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;
	uint16_t sp;
	uint16_t pc;
	uint8_t *memory;
	struct ConditionCodes cc;
	uint8_t int_enable;
}state;

//disassemble helping function to convert objdump to instructions
int disassemble(unsigned char* buffer, int pc){
	unsigned char *code=&buffer[pc];
	switch(*code){
		case 0x00:printf("NOP");break;
		case 0x01:printf("LXI B, #$%02x%02x", code[2],code[1]);break;
		case 0x02:printf("STAX B");break;
		case 0x03:printf("INX B");break;
		case 0x04:printf("INR B");break;
		case 0x05:printf("DCR B");break;
		case 0x06:printf("MVI B, #$%02x", code[1]);break;
		case 0x07:printf("RLC");break;
		case 0x08:printf("No instruction");break;
		case 0x09:printf("DAD B");break;
		case 0x0A:printf("LDAX B")break;
		case 0x0B:printf("DCR B");break;
		case 0x0C:printf("INR C");break;
		case 0x0D:printf("DCR C");break;
		case 0x0E:printf("MVI C, #$%02x", code[1]);break;
		case 0x0F:printf("RRC");break;
		case 0x10:printf("No instruction");break;
		case 0x11:printf("LXI D, #$%02x%02x",code[2],code[1]);break;
		case 0x12:printf("STAX D");break;
		case 0x13:printf("INX D");break;
		case 0x14:printf("INR D");break;
		case 0x15:printf("DCR D");break;
		case 0x16:printf("MVI D, #$%02x", code[1]):break;
		case 0x17:printf("RAL");break;
		case 0x18:prinf("No instruction");break;
		case 0x19:printf("DAD D");break;
		case 0x1A:printf("LDAX D");break;
		case 0x1B:printf("DCX D");break;
		case 0x1C:printf("INR E");break;
		case 0x1D:printf("DCR E");break;
		case 0x1E:printf("MVI E, #$%02x", code[1]);break;
		case 0x1F:printf("RAR");break;
		case 0x20:printf("No instruction");break;
		case 0x21:printf("LXI H, #$%02x%02x",code[2],code[1]);break;
		case 0x22:		
	}	
}


// emulating 8080
int emulate(state *st){
}




// Initializing State  structure and giving 16K memory;
state* Init(void){
	state* st=calloc(1,sizeof(state));
	st->memory=malloc(0x10000);
	return st;
}


void ReadFileIntoMemoryAt(state* state, char* filename, uint32_t offset)
{
	FILE *f= fopen(filename, "rb");
	if (f==NULL)
	{
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}
	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);
	
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	//printf("%d",fsize);
	fclose(f);
}
 
// each instruction in 8080 is 1-3 bytes
// timing important factor
int main(void){
	int finished=0;
	int blank_cycles=0;
	state *st=Init();
	
	//reading game files 
	
	ReadFileIntoMemoryAt(st, "invaders.h", 0);
	
	//lopping through all instructions
	//while(finished==0){
	//	finished=emulate(state);
	//}
	while(*st->memory!=EOF){
		printf("%d",*st->memory);
			st->memory++;
	}
	return 0;
}

