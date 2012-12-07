# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

addiu	$sp,$sp,500 # ghetto SP init
	
data:	lui     $t1, 0x9876
	ori     $t1, $t1, 0xABCD
	sw      $t1, 0($sp)		# 0x9876ABCD
	lui     $t1, 0x1234
	ori     $t1, $t1, 0x0405
	sw      $t1, 4($sp)		# 0x12340405
	lui     $t1, 0xABCD
	ori     $t1, $t1, 0xEF01
	sw      $t1, 8($sp)		# 0xABCDEF01

lw1:	lw	$t0, 0($sp)		# $t2 = 0x9876ABCD

lw2:	lw	$t1, 8($sp)		# $t2 = 0xABCDEF01

lw3:	lw	$t2, 2($sp)		# ERR

	#we don't need this because the last one is an ERR
# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start	
