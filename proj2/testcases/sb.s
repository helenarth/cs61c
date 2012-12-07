# _start is the entry point into any program.
.global _start
.ent    _start 
_start:
     
	 addiu $sp, $zero, 1600 #init the SP
     addiu	$sp,$sp,-12
data:	lui     $t1, 0x9876
	ori     $t1, $t1, 0xABCD
	sw      $t1, 0($sp)		# 0x9876ABCD
	lui     $t1, 0x1234
	ori     $t1, $t1, 0x0405
	sw      $t1, 4($sp)		# 0x12340405
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0xEF01
	sw      $t1, 8($sp)		# 0xABCDEF01

sb1:	addiu   $t0, $0, 0x9
	sb	$t0, 3($sp)

sb2:	addiu   $t0, $0, 0x9
	sb	$t0, 4($sp)

sb3:	addiu   $t0, $0, 0x9
	sb	$t0, 5($sp)

sb4:	addiu   $t0, $0, 0x9
	sb	$t0, 6($sp)
	
# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start					

