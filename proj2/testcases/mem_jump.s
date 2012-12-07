# _start is the entry point into any program.
.global _start
.ent    _start 
_start:


        addiu $s0, $0, 500
        addiu $s1, $0, 100
        # lui $t0, 0x0040
        addiu $t0, $t0 ,0x2000
        sw $s0, 0($t0)
        addiu $t0, $t0, 4
        sw $s1, 0($t0)
        j retr
        add $t0, $0, $0 #this line will cause problems, so that's why it's skipped.
retr:   lw $s1, 0($t0)
        addiu $t0, $t0, -4
        lw $s0, 0($t0)
        # zero everything
        and $s0, $s0, $0
        andi $s1, $s1, 0
		
# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start					

