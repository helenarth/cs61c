#include <stdio.h>
#include <stdlib.h>

#include "disassemble.h"

void disassemble(inst_t inst)
{
  /* you could use itype or type instead of rtype */
  /* remember that opcode field is at the same place for all types */
	switch (inst.rtype.opcode) //opcode testing
	{
	case 0x0: // opcode == 0x0 (SPECIAL)
		switch (inst.rtype.funct) //function testing
		{
			case 0xc: // funct == 0xc (SYSCALL)
				printf("syscall\n");
				break;
		//==============================================================================
		//shifting amount
			case 0x0: // fucnt=0x0 => (SLL)
				printf("sll\t$%d,$%d,%d\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
				break;
			case 0x2: // funct=0x2 => (SRL)
				printf("srl\t$%d,$%d,%d\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
				break;
			case 0x3: // funct=0x3 => (SRA)
				printf("sra\t$%d,$%d,%d\n", inst.rtype.rd, inst.rtype.rt, inst.rtype.shamt);
				break;

		//==============================================================================
		//section for rd,rs ,rtype
			case 0x21: //0x21 => (ADDU)
				printf("addu\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;			  
			case 0x23: // 0x23 => (SUBU)
				printf("subu\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;
			case 0x24: // funct == 0x24 (AND)
				printf("and\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;
		//ORIGINAL
			case 0x25: // funct == 0x25 (OR)
				printf("or\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;		
		//logicals
			case 0x26: // 0x26=>XOR
				printf("xor\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;
			case 0x27: // 0x27=>NOR
				printf("nor\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;
			case 0x2a: // 0x2a=>SLT
				printf("slt\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;
			case 0x2b: // 0x26=>SLTU
				printf("SLTU\t$%d,$%d,$%d\n", inst.rtype.rd, inst.rtype.rs, inst.rtype.rt);
				break;
		//==============================================================================
		//jumps
			case 0x8: //func=08 => jr, rs
				printf("jr\t$%d\n",inst.rtype.rs);
			case 0x9: //func = 09 => jalr
				printf("jalr\t$%d,$%d\n",inst.rtype.rd,inst.rtype.rs);

		//==============================================================================
			default: // undefined funct
				fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
				exit(-1);
				break;
		}
	break;

/*JUMPS
=========*/
	//ORIG
	case 0x2: // opcode == 0x2 (J)
		printf("j\t0x%x\n", inst.jtype.addr*4);
		break;
	case 0x3: // opcode == 0x3 (JAL)
		printf("jal\t0x%x\n", inst.jtype.addr*4);
		break;
	case 0x4: //opcode == 0x4 (BEQ)
	        printf("beq\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt, (4*inst.itype.imm));
		break;
	case 0x5: //opcode == 0x5 (BNE)
	        printf("bne\t$%d,$%d,%d\n", inst.itype.rs, inst.itype.rt,(4*inst.itype.imm));
		break;
/*ORS
=========*/	
	case 0x9: // opcode == 0x9 (ADDIU)
 		printf("addiu\t$%d,$%d,%d\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
		break;
	case 0xA: // opcode == 0xA (SLTI)
 		printf("slti\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
		break;
	case 0xB: // opcode == 0xd (SLTIU)
 		printf("sltiu\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
		break;
	//=========================
	case 0xC: // opcode == 0xC (ANDI)
 		printf("andi\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
		break;
	//ORIG
	case 0xd: // opcode == 0xd (ORI)
 		printf("ori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
		break;
	case 0xE: // opcode == 0xE (XORI)
 		printf("xori\t$%d,$%d,0x%x\n", inst.itype.rt, inst.itype.rs, inst.itype.imm);
		break;
	case 0xF: // opcode == 0xF (LUI)
 		printf("lui\t$%d,0x%x\n", inst.itype.rt, inst.itype.imm);
		break;	
	//=========================

/*INVOLVING OFFSET
==================*/
	case 0x20: // opcode == 0x20 (LB)
 		printf("lb\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;	
	case 0x21: // opcode == 0x21 (LH)
 		printf("lh\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;
	case 0x23: // opcode == 0x23 (LW)
	        printf("lw\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;
	case 0x24: // opcode == 0x24 (LBU)
 		printf("lbu\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;
	case 0x25: // opcode == 0x25 (LHU)
 		printf("lhu\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;
	case 0x28: // opcode == 0x28 (SB)
 	        printf("sb\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;
	case 0x29: // opcode == 0x29 (SH)
 		printf("sh\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;
	case 0x2B: // opcode == 0x2B (SW)
	    	printf("sw\t$%d,%d($%d)\n", inst.itype.rt, 4*inst.itype.imm, inst.itype.rs);
		break;

	default: // undefined opcode
		fprintf(stderr, "%s: illegal instruction: %08x\n", __FUNCTION__, inst.bits);
		exit(-1);
		break;
	}
}