# _start is the entry point into any program.
.global _start
.ent    _start 
_start:


	addiu	$s5,$s5,500
data:	lui     $t1, 0x9876
	ori     $t1, $t1, 0xABCD
	sw      $t1, 0($s5)		# 0x9876ABCD
	lui	$t1,0x0123
	ori	$t1,$t1,0x4567
	sw      $t1, 4($s5)
lbu1:	lbu	$t0, 0($s5)		# $t2 = 0xCD
	
lbu2:	lbu	$t1, 1($s5)		# $t2 = 0xAB

lbu3:	lbu	$t2, 2($s5)		# $t2 = 0x76
	
lbu4:	lbu	$t3, 3($s5)		# $t2 = 0x98
	
	lbu	$t4, 4($s5)		# $t2 = 0x98
	
	lbu	$t5, 5($s5)		# $t2 = 0x98
	
	lbu	$t6, 6($s5)		# $t2 = 0x98
	
	lbu	$t7, 7($s5)		# $t2 = 0x98

	addiu	$s5, $s5, 12		# $s5 is out of bounds

	lbu	$t7, -6($s5)		# $t7 = Whatever $t6 was.

# End the program	
done: ori   $v0, $zero, 10
syscall
.end _start

