# _start is the entry point into any program.
.global _start
.ent    _start 
_start:

main:   addiu $sp, $zero, 1600 #default SP to 1600
        addiu   $sp,$sp,-24
	addiu   $s7,$0,0
        li	$a0,4
	li	$a1,0
	jal	nchoosek		# evaluate C(4,0); should be 1
	jal	printv0
	li	$a0,4
	li	$a1,1
	jal	nchoosek		# evaluate C(4,1); should be 4
	jal	printv0
	li	$a0,4
	li	$a1,2
	jal	nchoosek		# evaluate C(4,2); should be 6
	jal	printv0
	li	$a0,4
	li	$a1,3
	jal	nchoosek		# evaluate C(4,3); should be 4
	jal	printv0
	li	$a0,4
	li	$a1,4
	jal	nchoosek		# evaluate C(4,4); should be 1
	jal	printv0
	li	$a0,4
	li	$a1,5
	jal	nchoosek		# evaluate C(4,5); should be 0
	jal	printv0
        # End the program	
done: ori   $v0, $zero, 10
syscall


printv0:
 
 addiu	$sp,$sp,-4
	sw	$ra,0($sp)
	addu	$a0,$zero, $v0
	addiu	$v0,$zero,1
	syscall
	addiu	$a0,$zero, 10
	addiu	$v0,$zero, 11
	syscall
	lw	$ra,0($sp)
	addiu	$sp,$sp,4
	jr	$ra
	
        

nchoosek:	# you fill in the prologue
	addiu	$sp,$sp,-16
	sw	$s0,0($sp)
	sw	$s1,4($sp)
	sw	$s2,8($sp)
	sw	$ra,12($sp)

	beq	$a1,$0,return1
	beq	$a0,$a1,return1
	beq	$a0,$0,return0
	blt	$a0,$a1,return0

	addiu	$a0,$a0,-1		# C(n,k) = C(n-1,k) + C(n-1,k-1)
	addiu	$s0,$a0,0
	addiu	$s1,$a1,0
	jal	nchoosek
	addiu	$s2,$v0,0
	addiu	$a0,$s0,0
	addiu	$a1,$s1,-1
	jal	nchoosek
	addu	$v0,$v0,$s2
	j	return
return0:
	addiu	$v0,$0,0
	j	return
return1:
	addiu	$v0,$0,1
	
return:		# you fill in the epilogue
	lw	$s0,0($sp)
	lw	$s1,4($sp)
	lw	$s2,8($sp)
	lw	$ra,12($sp)
	addiu	$sp,$sp,16
	jr	$ra
	

.end _start	
