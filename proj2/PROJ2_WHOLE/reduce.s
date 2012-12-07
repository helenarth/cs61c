# Format of list links:
# first 4 bytes are value, second 4 bytes are pointer to next

	.data
spaceString:      .asciiz " "
newlineString:    .asciiz "\n"
testOneString:    .asciiz "Test one:\n"
testTwoString:    .asciiz "Test two:\n"
listString: .asciiz "List: "
resultString:  .asciiz "Result: "

	.text
main:
	jal     create_default_list
	move    $s0, $v0
	
	# print "test one"
	la $a0 testOneString
	li $v0 4
	syscall
	
	# print "list: "
	la      $a0, listString
	li      $v0, 4
	syscall
	
	# print the list
	move    $a0, $s0
	jal     print_list
	
	# print a newline
	jal     print_newline
	
	# issue the reduce call
	move    $a0, $s0
	li $a1 0
	# load the address of the add function into $a2 (check out la)
	la 	$a2, add
	
	jal     reduce
	move $t0 $v0
	
	# print "Result: "
	la      $a0, resultString
	li      $v0, 4
	syscall
	
	# print the result
	move    $a0, $t0
	li      $v0, 1
	syscall
	
	jal     print_newline
	jal     print_newline
	
	# print "test two"
	la $a0 testTwoString
	li $v0 4
	syscall
	
	# print "list before: "
	la      $a0, listString
	li      $v0, 4
	syscall
	
	# print the list
	move    $a0, $s0
	jal print_list
	
	# print a newline
	jal     print_newline
	
	# issue the reduce call
	move    $a0, $s0
	li $a1 1
	
	# load the address of the multiply function into $a2 (check out la)
	la  	$a2, multiply 

	jal     reduce
	move $t0 $v0

	# print "Result: "
	la      $a0, resultString
	li      $v0, 4
	syscall
	
	# print the result
	move    $a0, $t0
	li      $v0, 1
	syscall
	
	li      $v0, 10
	syscall

reduce:
	addiu   $sp, $sp, -12
	sw      $ra, 0($sp)
	sw      $s1, 4($sp)
	sw      $s0, 8($sp)

	beq     $a0, $zero, done        # if we were given a null pointer, we're done.
  
	move    $s0, $a0                # save address of this link in $s0
	move    $s1, $a2                # save address of function in $s1

	# remember that each link is 8 bytes long: 4 for the value followed by 4 for the pointer to next
	# load the value of the current link into $a0
	lw 	$a0, 0($s0)
	# Call the function
	jalr	$s1
	# load the address of the next link into $a0
	lw	$a0, 4($s0)
	# put the address of the function back into $a2 to prepare for the recursion
	move	$a2, $s1
	# put the updated total into $a1 to prepare for the recursion
	move 	$a1, $v0
	# recurse
	jal	reduce 	

restore:
	lw      $s0, 8($sp)
	lw      $s1, 4($sp)
	lw      $ra, 0($sp)
	addiu   $sp, $sp, 12
	jr      $ra
done:
	move $v0 $a1
	j restore

add:
	add     $v0, $a0, $a1
	jr      $ra

multiply:
	mul    $v0, $a0, $a1
	jr      $ra

create_default_list:
	addiu   $sp, $sp, -4
	sw      $ra, 0($sp)
	li      $s0, 0                  # pointer to the last link we handled
	li      $s1, 1                  # number of links handled
loop:                               # do...
	li      $a0, 8
	jal     malloc                  # get memory for the next link
	sw      $s1, 0($v0)             # node.value = i
	sw      $s0, 4($v0)             # node.next = last
	move    $s0, $v0                # last = node
	addiu   $s1, $s1, 1             # i++
	bne     $s1, 10, loop           # ... while i!= 10
	lw      $ra, 0($sp)
	addiu   $sp, $sp, 4
	jr      $ra

print_list:
	bne     $a0, $zero, printMeAndRecurse
	jr      $ra                     # nothing to print
printMeAndRecurse:
	move    $t0, $a0                # t0 gets current link
	lw      $a0, 0($t0)             # a0 gets value in current link
	li      $v0, 1                  # preparte for print integer syscall
	syscall
	la      $a0, spaceString        # a0 gets address of string containing space
	li      $v0, 4                  # prepare for print string syscall
	syscall
	lw      $a0, 4($t0)             # a0 gets address of next link
	j       print_list              # recurse. We don't have to use jal because we already have where we want to return to in $ra

print_newline:
	la      $a0, newlineString
	li      $v0, 4
	syscall
	jr      $ra

malloc:
	li      $v0, 9
	syscall
	jr      $ra
