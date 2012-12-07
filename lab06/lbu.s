start:	addiu	$sp,$sp,-8
data:	lui     $t1, 0x9876
	ori     $t1, $t1, 0xABCD
	sw      $t1, 0($sp)		# 0x9876ABCD
	lui	$t1,0x0123
	ori	$t1,$t1,0x4567
	sw      $t1, 4($sp)
lbu1:	lbu	$t0, 0($sp)		# $t2 = 0xCD
	
lbu2:	lbu	$t1, 1($sp)		# $t2 = 0xAB

lbu3:	lbu	$t2, 2($sp)		# $t2 = 0x76
	
lbu4:	lbu	$t3, 3($sp)		# $t2 = 0x98
	
	lbu	$t4, 4($sp)		# $t2 = 0x98
	
	lbu	$t5, 5($sp)		# $t2 = 0x98
	
	lbu	$t6, 6($sp)		# $t2 = 0x98
	
	lbu	$t7, 7($sp)		# $t2 = 0x98

	addiu	$sp, $sp, 12		# $sp is out of bounds

	lbu	$t7, -6($sp)		# $t7 = Whatever $t6 was.

done:	addi 	$0 $0 0			#unsupported by simulator, terminates
