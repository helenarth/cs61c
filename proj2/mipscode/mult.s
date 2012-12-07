.text
.set noat

.ent _start
.global _start
_start:
		
		addiu $s0 $0 100
        add $s1 $0 $s0
        addiu $t0 $0 99
        addiu $t1 $0 1
loop:   add $s0 $s0 $s1
        subu $t0 $t0 $t1
        bne $t0 $0 loop

.end _start	

