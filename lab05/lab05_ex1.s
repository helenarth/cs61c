	.data
n:	.word 9


	.text
main:
	li      $t0, 0
	li      $t1, 1
	la      $t3, n
	lw      $t3, 0($t3)

fib:
	beq     $t3, $zero, finish
	addu    $t2, $t1, $t0
	move    $t0, $t1
	move    $t1, $t2
	addiu   $t3, $t3, -1
	j       fib

finish:
	move    $a0, $t0
	li      $v0, 1
	syscall

	li      $v0, 10
	syscall

