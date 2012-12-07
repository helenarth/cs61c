# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

addu1:	lui     $t0, 0x0FFF
	ori     $t0, $t0, 0xABCD
	lui     $t0, 0x1FFF
	ori     $t0, $t0, 0xABCD
	addu    $t2, $t0, $t1		# $t2 = 0x1fffabcd
	
addu2:	lui     $t0, 0x7FFF
	ori     $t0, $t0, 0xABCD
	addu    $t2, $t0, $t0		# no overflow error

# End the program	
ori   $v0, $zero, 10
syscall
.end _start
