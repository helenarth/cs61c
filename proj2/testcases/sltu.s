# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

sltu1:	addiu   $t0, $0, 0x1BCD
	addiu   $t1, $0, 0x2DEF
	sltu    $t2, $t0, $t1		# $t2 = 1
	
sltu2:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD	# 4294945741
	lui     $t1, 0x0BCD		
	ori     $t1, $t1, 0x1234	# 197988916
	sltu	$t2, $t1, $t0		# $t2 = 1
	
sltu3:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD	# 4294945741
	lui     $t1, 0xABCD		
	ori     $t1, $t1, 0x1234	# 2882343476
	sltu	$t2, $t1, $t0		# $t2 = 1
	
sltu4:	lui     $t0, 0xFFFF
	ori     $t0, $t0, 0xABCD
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0x1234
	sltu	$t2, $t0, $t1		# $t2 = 0

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start					

