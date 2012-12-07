StringLen:
	
	addi $r0, 0

	addi $r2, 0

	addi $r1, 0
	
loop:
	
	lw $r2, 0($r2)

	add	$r1, $r2, $r1
	
	srlv  $r2, 8($r2)

	sll	$r1, 8($r1)

	srlv	$r1, 8($r1)

	addi $r0, $r0, 1

	beq	$r2, 0,  end

	addi $r0, $r0, 1

	bne	$r1, 0, loop

end:

	disp $r0, 0

	jr    $r3
	                                                            