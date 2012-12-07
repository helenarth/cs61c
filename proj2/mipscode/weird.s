.text
.set noat

.global _start
.ent    _start 
_start:

weird:	addiu $s0 $0 5
	add $s1 $0 $0	
loop:	addiu $t0 $0 0
	beq $t0 $s0 done
	lui $t1 0x0040
	ori $t1 $t1 8
	lw $t2 0($t1)
	addiu $t2 $t2 1
	sw $t2 0($t1)
	add $s1 $s1 $s0
	j loop
done:	addi $0 $0 0

.end _start