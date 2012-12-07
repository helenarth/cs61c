# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

lui1:	lui     $t0, 0xCDEF
	srl     $t1, $t0, 16		# $t2 = 0x0000CDEF

lui2:	lui     $t1, 0x0DEF		# $t2 = 0x0DEF0000

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start					



