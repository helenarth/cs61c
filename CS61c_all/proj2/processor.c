#include <stdio.h>
#include <stdlib.h>
#include "processor.h"
#include "disassemble.h"



int32_t sign_ext(int16_t imm)
{
    return (((int32_t)imm)<<16)>>16;
}


void execute_one_inst(processor_t* p, int prompt, int print_regs)
{
	inst_t inst;
	reg_t tmp;			//temporary storage value

	/* fetch an instruction */
	inst.bits = load_mem(p->pc, SIZE_WORD);

	/* interactive-mode prompt */
	if(prompt)
	{
		if (prompt == 1) {
		printf("simulator paused, enter to continue...");
		while(getchar() != '\n');
    }
    printf("%08x: ",p->pc);
    disassemble(inst);
	}

	switch (inst.rtype.opcode) /* could also use e.g. inst.itype.opcode */
	{
	case 0x0: // opcode == 0x0 (SPECIAL)

	    switch (inst.rtype.funct)
		{
		case 0x0: // fucnt=0x0 => (SLL)
			//R[rd] <- R[rt] << shamt
			p->R[inst.rtype.rd] = p->R[inst.rtype.rt] << inst.rtype.shamt;
			p->pc += 4;
			break;
		case 0x2: // funct=0x2 => (SRL)
			//R[rd] <- R[rt] >> shamt
			p->R[inst.rtype.rd] = p->R[inst.rtype.rt] >> inst.rtype.shamt;
			p->pc += 4;
			break;
		case 0x3: // (SRA)
			//R[rd] <- (signed)R[rt] >> shamt
		    p->R[inst.rtype.rd] = ((int32_t)(p->R[inst.rtype.rt])) >> inst.rtype.shamt;
			p->pc += 4;
			break;
		case 0x8: //(JR) = 
			//PC <- R[rs]
			p->pc = p->R[inst.rtype.rs];
			break;
		case 0x9: //09=> JALR
			tmp = (p->pc + 4);					//tmp <- PC + 4
			p->pc = p->R[inst.rtype.rs];		//PC <- R[rs]
			p->R[inst.rtype.rd] = tmp;			//R[rd] <- tmp
			break;
		//done
		case 0xc: // funct == 0xc (SYSCALL)
			handle_syscall(p);
			p->pc += 4;
			break;

//unsigned arithmetic 
		case 0x21: //0x21 => (ADDU)
			//R[rd] <- R[rs] + R[rt]
			p->R[inst.rtype.rd] = p->R[inst.rtype.rs] + p->R[inst.rtype.rt];
			p->pc += 4;
			break;			  
		case 0x23: // 0x23 => (SUBU)
			//R[rd] <- R[rs] - R[rt]
			p->R[inst.rtype.rd] = p->R[inst.rtype.rs] - p->R[inst.rtype.rt];
			p->pc += 4;
			break;		

//logicals
		//0RIGINAL
		case 0x24: // funct == 0x24 (AND)
			//R[rd] <- R[rs] & R[rt]
			p->R[inst.rtype.rd] = p->R[inst.rtype.rs] & p->R[inst.rtype.rt];
			p->pc += 4;
			break;
		case 0x25: // funct == 0x25 (OR)
			//R[rd] <- R[rs] | R[rt]
			p->R[inst.rtype.rd] = p->R[inst.rtype.rs] | p->R[inst.rtype.rt];
			p->pc += 4;
			break;
		case 0x26: // 0x26=>XOR
			//R[rd] <- R[rs] ^ R[rt]
			p->R[inst.rtype.rd] = p->R[inst.rtype.rs] ^ p->R[inst.rtype.rt];
			p->pc += 4;
			break;
		case 0x27: // 0x27=>NOR
			//R[rd] <- ~(R[rs] | R[rt])
			p->R[inst.rtype.rd] = ~(p->R[inst.rtype.rs] | p->R[inst.rtype.rt]);
			p->pc += 4;
			break;

		case 0x2a: // 0x2a=>SLT
		    if (((int32_t)p->R[inst.rtype.rs])  < ((int32_t)p->R[inst.rtype.rt]))
				p->R[inst.rtype.rd] = 1;
		    else
				p->R[inst.rtype.rd] = 0;
		    p->pc += 4;
		    break;
		case 0x2b: // 0x26=>SLTU
			//set rd to 1 if rs is less than rt else set to zero
			if (p->R[inst.rtype.rs] < p->R[inst.rtype.rt])
				p->R[inst.rtype.rd] = 1;
			else
				p->R[inst.rtype.rd] = 0;
			p->pc += 4;
			break;
		
		default: // undefined funct
			fprintf(stderr, "%s: pc=%08x, illegal instruction=%08x\n", __FUNCTION__, p->pc, inst.bits);
			exit(-1);
			break;
		}
    break;

	case 0x2: // opcode == 0x2 (J)
		p->pc = ((p->pc+4) & 0xf0000000) | (inst.jtype.addr << 2);
		break;
	case 0x3: // opcode == 0x3 (JAL)
		//R[31] <- PC + 4
		//PC <- {(upper 4 bits of PC+4), address*4}
		p->R[31] = p->pc + 4;
		p->pc = ((p->pc+4) & 0xf0000000) | (inst.jtype.addr << 2);
		break;
	case 0x4: //opcode == 0x4 (BEQ)
		//if(R[rs] == R[rt])
	    if (p->R[inst.itype.rs] == p->R[inst.itype.rt]) 
			//PC <- PC + 4 + signext(offset)*4
		p->pc += 4 + 4*(sign_ext(inst.itype.imm)); 
	    else
			p->pc+=4;
	    break;
	case 0x5: //opcode == 0x5 (BNE)
	    if (p->R[inst.rtype.rs] != p->R[inst.rtype.rt])
			//PC <- PC + 4 + signext(offset)*4
			p->pc += 4+(4*sign_ext(inst.itype.imm));
	    else
			p->pc+=4;
		break;
	case 0x9: // opcode == 0x9 (ADDIU)
		p->R[inst.itype.rt] = p->R[inst.itype.rs] + sign_ext(inst.itype.imm);
		p->pc+=4;
		break;
	case 0xA: // opcode == 0xA (SLTI)
		//R[rt] <- (signed)R[rs] < signext(imm)
		if (((int32_t)p->R[inst.itype.rs]) < sign_ext(inst.itype.imm))
			p->R[inst.itype.rt] = 1;
		else
			p->R[inst.itype.rt] = 0;
		p->pc+=4;
		break;
	case 0xB: // opcode == 0xd (SLTIU)
		//R[rt] <- R[rs] < signext(imm)
		if (p->R[inst.itype.rs] < sign_ext(inst.itype.imm)) 
			p->R[inst.itype.rt] = 1;
		else
			p->R[inst.itype.rt] = 0;
		p->pc+=4;
		break;
	
	//IMMEDIATE LOGICALS
	case 0xC: // opcode == 0xC (ANDI)
 		p->R[inst.itype.rt] = p->R[inst.itype.rs] & inst.itype.imm;
		p->pc += 4;
		break;
	//original
	case 0xD: // opcode == 0xd (ORI)
		p->R[inst.itype.rt] = p->R[inst.itype.rs] | inst.itype.imm;
		p->pc += 4;
		break;
	case 0xE: // opcode == 0xE (XORI)
 		p->R[inst.itype.rt] = p->R[inst.itype.rs] ^ inst.itype.imm;
		p->pc += 4;
		break;
	case 0xF: // opcode == 0xF (LUI)
		//R[rt] <- imm << 16
		p->R[inst.itype.rt] = (inst.itype.imm << 16);
		p->pc += 4;
		break;
		
/*INVOLVING OFFSET
==================*/
	case 0x20: // opcode == 0x20 (LB)
	//R[rt] <- signext(LoadMem(R[rs]+signext(offset), byte))
	    p->R[inst.itype.rt] = (int8_t)(load_mem(p->R[inst.itype.rs]+sign_ext(inst.itype.imm),SIZE_BYTE));
		p->pc += 4;
		break;	
	case 0x21: // opcode == 0x21 (LH)
	//R[rt] <- signext(LoadMem(R[rs]+signext(offset), half))
	    p->R[inst.itype.rt] = (int16_t)(load_mem(p->R[inst.itype.rs]+sign_ext(inst.itype.imm),SIZE_HALF_WORD));
		p->pc += 4;
		break;
	case 0x23: // opcode == 0x23 (LW)
	//R[rt] <- LoadMem(R[rs]+signext(offset), word)
	    p->R[inst.itype.rt] = load_mem(p->R[inst.itype.rs]+sign_ext(inst.itype.imm),SIZE_WORD);
		p->pc += 4;
		break;
	case 0x24: // opcode == 0x24 (LBU)
	//R[rt] <- zeroext(LoadMem(R[rs]+signext(offset), byte))
		p->R[inst.itype.rt] = load_mem(p->R[inst.itype.rs]+sign_ext(inst.itype.imm),SIZE_BYTE);
		p->pc += 4;
		break;
	case 0x25: // opcode == 0x25 (LHU)
	//R[rt] <-zeroext(LoadMem(R[rs]+signext(offset), half))
		p->R[inst.itype.rt] = load_mem(p->R[inst.itype.rs]+sign_ext(inst.itype.imm),SIZE_HALF_WORD);
		p->pc += 4;
		break;
	case 0x28: // opcode == 0x28 (SB)
	//StoreMem(R[rs]+signext(offset)], byte, R[rt])
		store_mem(p->R[inst.itype.rs] + sign_ext(inst.itype.imm), SIZE_BYTE, p->R[inst.itype.rt]);
		p->pc += 4;
		break;
	case 0x29: // opcode == 0x29 (SH)
	//StoreMem(R[rs]+signext(offset)], half, R[rt])
		store_mem(p->R[inst.itype.rs] + sign_ext(inst.itype.imm), SIZE_HALF_WORD, p->R[inst.itype.rt]);
		p->pc += 4;
		break;
	case 0x2B: // opcode == 0x2B (SW)
	//StoreMem(R[rs]+signext(offset)], word, R[rt])
	    store_mem(p->R[inst.itype.rs] + sign_ext(inst.itype.imm), SIZE_WORD, p->R[inst.itype.rt]);
		p->pc += 4;
		break;
	
	default: // undefined opcode
		fprintf(stderr, "%s: pc=%08x, illegal instruction: %08x\n", __FUNCTION__, p->pc, inst.bits);
		exit(-1);
		break;
	}

  // enforce $0 being hard-wired to 0
	p->R[0] = 0;

	if(print_regs)
		print_registers(p);
}


void init_processor(processor_t* p)
{
	int i;

	/* initialize pc to 0x1000 */
	p->pc = 0x1000;

	/* zero out all registers */
	for (i=0; i<32; i++)
	{
		p->R[i] = 0;
	}
}

void print_registers(processor_t* p)
{
	int i,j;
	for (i=0; i<8; i++)
	{
		for (j=0; j<4; j++)
			printf("r%2d=%08x ",i*4+j,p->R[i*4+j]);
			puts("");
	}
}

void handle_syscall(processor_t* p)
{
	reg_t i;

	switch (p->R[2]) // syscall number is given by $v0 ($2)
	{
	case 1: // print an integer
		printf("%d", p->R[4]);
		break;

	case 4: // print a string
		for(i = p->R[4]; i < MEM_SIZE && load_mem(i, SIZE_BYTE); i++) {
		printf("%c", load_mem(i, SIZE_BYTE)); }
		break;

	case 10: // exit
		printf("exiting the simulator\n");
		exit(0);
		break;

	case 11: // print a character
		printf("%c", p->R[4]);
		break;

	default: // undefined syscall
		fprintf(stderr, "%s: illegal syscall number %d\n", __FUNCTION__, p->R[2]);
		exit(-1);
		break;
	}
}
