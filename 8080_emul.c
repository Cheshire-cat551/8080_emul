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
		case 0x43:printf("MOV B,E");break;
		case 0x44:printf("MOV B,H");break;
		case 0x45:printf("MOV B,L");break;
		case 0x46:printf("MOV B,M");break;
		case 0x47:printf("MOV B,A");break;
		case 0x48:printf("MOV C,B");break;
		case 0x49:printf("MOV C,C");break;
		case 0x4A:printf("MOV C,D");break;
		case 0x4B:printf("MOV C,E");break;
		case 0x4C:printf("MOV C,H");break;
		case 0x4D:printf("MOv C,L");break;
		case 0x4E:printf("MOV C,M");break;
		case 0x4F:printf("MOV C,A");break;
		case 0x50:printf("MOV D,B");break;
		case 0x51:printf("MOV D,C");break;
		case 0x52:printf("MOV D,D");break;
		case 0x53:printf("MOV D,E");break;
		case 0x54:printf("MOV D,H");break;
		case 0x55:printf("MOV D,L");break;
		case 0x56:printf("MOV D,M");break;
		case 0x57:printf("MOV D,A");break;
		case 0x58:printf("MOV E,B");break;
		case 0x59:printf("MOV E,C");break;
		case 0x5A:printf("MOV E,D");break;
		case 0x5B:printf("MOV E,E");break;
		case 0x5C:printf("MOV E,H");break;
		case 0x5D:printf("MOv E,L");break;
		case 0x5E:printf("MOV E,M");break;
		case 0x5F:printf("MOV E,A");break;
		case 0x60:printf("MOV H,B");break;
		case 0x61:printf("MOV H,C");break;
		case 0x62:printf("MOV H,D");break;
		case 0x63:printf("MOV H,E");break;
		case 0x64:printf("MOV H,H");break;
		case 0x65:printf("MOV H,L");break;
		case 0x66:printf("MOV H,M");break;
		case 0x67:printf("MOV H,A");break;
		case 0x68:printf("MOV L,B");break;
		case 0x69:printf("MOV L,C");break;
		case 0x6A:printf("MOV L,D");break;
		case 0x6B:printf("MOV L,E");break;
		case 0x6C:printf("MOV L,H");break;
		case 0x6D:printf("MOv L,L");break;
		case 0x6E:printf("MOV L,M");break;
		case 0x6F:printf("MOV L,A");break;
		case 0x70:printf("MOV M,B");break;
		case 0x71:printf("MOV M,C");break;
		case 0x72:printf("MOV M,D");break;
		case 0x73:printf("MOV M,E");break;
		case 0x74:printf("MOV M,H");break;
		case 0x75:printf("MOV M,L");break;
		case 0x76:printf("HLT");break;
		case 0x77:printf("MOV M,A");break;
		case 0x78:printf("MOV A,B");break;
		case 0x79:printf("MOV A,C");break;
		case 0x7A:printf("MOV A,D");break;
		case 0x7B:printf("MOV A,E");break;
		case 0x7C:printf("MOV A,H");break;
		case 0x7D:printf("MOv A,L");break;
		case 0x7E:printf("MOV A,M");break;
		case 0x7F:printf("MOV A,A");break;
		case 0x80:printf("ADD B");break;
		case 0x81:printf("ADD C");break;
		case 0x82:printf("ADD D");break;
		case 0x83:printf("ADD E");break;
		case 0x84:printf("ADD H");break;
		case 0x85:printf("ADD L");break;
		case 0x86:printf("ADD M");break;
		case 0x87:printf("ADD A");break;
		case 0x88:printf("ADC B");break;
		case 0x89:printf("ADC C");break;
		case 0x8A:printf("ADC D");break;
		case 0x8B:printf("ADC E");break;
		case 0x8C:printf("ADC H");break;
		case 0x8D:printf("ADC L");break;
		case 0x8E:printf("ADC M");break;
		case 0x8F:printf("ADC A");break;
		case 0x90:printf("SUB B");break;
		case 0x91:printf("SUB C");break;
		case 0x92:printf("SUB D");break;
		case 0x93:printf("SUB E");break;
		case 0x94:printf("SUB H");break;
		case 0x95:printf("SUB L");break;
		case 0x96:printf("SUB M");break;
		case 0x97:printf("SUB A");break;
		case 0x98:printf("SBB B");break;
		case 0x99:printf("SBB C");break;
		case 0x9A:printf("SBB D");break;
		case 0x9B:printf("SBB E");break;
		case 0x9C:printf("SBB H");break;
		case 0x9D:printf("SBB L");break;
		case 0x9E:printf("SBB M");break;
		case 0x9F:printf("SBB A");break;
		case 0xA0:printf("ANA B");break;
		case 0xA1:printf("ANA C");break;
		case 0xA2:printf("ANA D");break;
		case 0xA3:printf("ANA E");break;
		case 0xA4:printf("ANA H");break;
		case 0xA5:printf("ANA L");break;
		case 0xA6:printf("ANA M");break;
		case 0xA7:printf("ANA A");break;
		case 0xA8:printf("XRA B");break;
		case 0xA9:printf("XRA C");break;
		case 0xAA:printf("XRA D");break;
		case 0xAB:printf("XRA E");break;
		case 0xAC:printf("XRA H");break;
		case 0xAD:printf("XRA L");break;
		case 0xAE:printf("XRA M");break;
		case 0xAF:printf("XRA A");break;
		case 0xB0:printf("ORA B");break;
		case 0xB1:printf("ORA C");break;
		case 0xB2:printf("ORA D");break;
		case 0xB3:printf("ORA E");break;
		case 0xB4:printf("ORA H");break;
		case 0xB5:printf("ORA L");break;
		case 0xB6:printf("ORA M");break;
		case 0xB7:printf("ORA A");break;
		case 0xB8:printf("CMP B");break;
		case 0xB9:printf("CMP C");break;
		case 0xBA:printf("CMP D");break;
		case 0xBB:printf("CMP E");break;
		case 0xBC:printf("CMP H");break;
		case 0xBD:printf("CMP L");break;
		case 0xBE:printf("CMP M");break;
		case 0xBF:printf("CMP A");break;
		
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


