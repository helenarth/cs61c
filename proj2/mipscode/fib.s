.text
.set noat

.ent _start
.global _start
_start:

main:
	addiu	$s0	$0	0	#init $s0
	addiu	$a0	$0	10	#which fibonocci number to calc
	jal		fib
	addi		$0	$0	0
	addi		$s0	$v0	0
	li		$v0	10
	syscall
fib:
	addiu	$sp	$sp	-12
	sw		$ra	0($sp)
	sw		$s0	4($sp)
	sw		$s1	8($sp)
	add		$s0	$a0	$0
	addiu	$t0	$0	1
	beq		$a0	$0	return0
	beq		$a0	$t0	return1
	addiu	$a0	$s0	-1
	jal		fib
	add		$s1	$0	$v0
	addiu	$a0	$s0	-2
	jal 		fib
	add		$v0	$v0	$s1
	j		return
	
return1:
	li		$v0	1
	j		return
return0:
	li		$v0	0
	j		return
return:
	lw		$ra	0($sp)
	lw		$s0	4($sp)
	lw		$s1	8($sp)
	addiu	$sp	$sp	12
	jr		$ra
	
	
.end _start		
