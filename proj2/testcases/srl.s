# _start is the entry point into any program.
.global _start
.ent    _start 
_start:


srl1:	addiu   $t0, $0, 0x1BCD
	srl	$t2, $t0, 2		# $t2 = 0x6F3
	
srl2:	lui     $t0, 0x5643
	ori     $t0, $t0, 0xCDAB
	srl	$t2, $t0, 0		# $t2 = 0x5643CDAB
	srl	$t2, $t0, 3		# $t2 = 0xAC879B5
	srl	$t2, $t0, 13		# $t2 = 0x2B21E
	
srl3:	lui     $t0, 0xF643
	ori     $t0, $t0, 0xCDAB
	srl	$t2, $t0, 0		# $t2 = 0xF643CDAB
	srl	$t2, $t0, 3		# $t2 = 0x1EC879B5
	srl	$t2, $t0, 13		# $t2 = 0x7B21E

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start		
