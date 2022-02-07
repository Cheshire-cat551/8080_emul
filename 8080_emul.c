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
		case 0x22:printf("SHLD $%02x%02x",code[2],code[1]);break;
		case 0x23:printf("INX H");break;
		case 0x24:printf("INR H");break;
		case 0x25:printf("DCR H"):break;
		case 0x26:printf("MVI H, #$%02x", code[1]);break;
		case 0x27:printf("DAA"):break;
		case 0x28:printf("No instruction");break;
		case 0x29:printf("DAD H");break;
		case 0x2A:printf("LHLD $%02x%02x",code[2],code[1]);break;
		case 0x2B:printf("DCX H");break;
		case 0x2C:printf("INR L");break;
		case 0x2D:printf("DCR L");break;
		case 0x2E:printf("MVI L, #$%02x", code[1]);break;
		case 0x2F:printf("CMA");break;
		case 0x30:printf("No instruction");break;
		case 0x31:printf("LXI SP, #$%02x%02x",code[2],code[1]);break;
		case 0x32:printf("STA $%02x%02x",code[2],code[1]);break;
		case 0x33:printf("INX SP");break;
		case 0x34:printf("INR M");break;
		case 0x35:printf("DCR M");break;
		case 0x36:printf("MVI M, #$%02x", code[1]);break;
		case 0x37:printf("STC");break;
		case 0x38:printf("No instruction");break;
		case 0x39:printf("DAD SP");break;
		case 0x3A:printf("LDA $%02x%02x",code[2],code[1]);break;
		case 0x3B:printf("DCX SP");break;
		case 0x3C:printf("INR A");break;
		case 0x3D:printf("DCR A");break;
		case 0x3E:printf("MVI A, #$%02x", code[1]);break;
		case 0x3F:printf("CMC");break;
		case 0x40:printf("MOV B,B");break;
		case 0x41:printf("MOV B,C");break;
		case 0x42:printf("MOV B,D");break;
		case 0x43:printf("MOV B,E");break
		case 0x44:printf("MOV B,H");break;
		case 0x45:printf("MOV B,L");break;
		case 0x46:printf("MOV B,M");break;
		case 0x47:printf("MOV B,A");break;
		case 0x48:printf("MOV c,B");break;
		case 0x49:printf("MOV C,C");break;
		case 0x4A:printf("MOV C,D");break;
		case 0x4B:printf("MOV C,E");break;
		case 0x4C:printf("MOV C,H");break;
		case 0x4D:printf("MOv C,L");break;
		case 0x4E:printf("MOV C,M");break;
		case 0x4f:printf("MOV C,A");break;
				
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


