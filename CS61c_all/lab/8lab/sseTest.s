	.file	"sseTest.c"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC7:
	.string	"|%g %g| * |%g %g| = |%g %g|\n"
.LC8:
	.string	"|%g %g|   |%g %g|   |%g %g|\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB531:
	.cfi_startproc
	subq	$104, %rsp
	.cfi_def_cfa_offset 112
	xorpd	%xmm6, %xmm6
	movsd	.LC1(%rip), %xmm7
	movabsq	$4616189618054758400, %rax
	movsd	.LC0(%rip), %xmm0
	movq	%rax, 56(%rsp)
	movsd	%xmm7, 40(%rsp)
	movl	$.LC7, %esi
	movl	$1, %edi
	movsd	%xmm0, 32(%rsp)
	movl	$6, %eax
	movsd	%xmm6, 64(%rsp)		//from stack to reg6
	movsd	%xmm6, 72(%rsp)
	movsd	%xmm6, 80(%rsp)
	movsd	%xmm6, 88(%rsp)
	movsd	.LC2(%rip), %xmm1
	movsd	%xmm6, (%rsp)
	movupd	32(%rsp), %xmm4		//
	movapd	.LC5(%rip), %xmm5
	mulpd	%xmm4, %xmm5		//a = a*b1
	movsd	%xmm1, 48(%rsp)
	movupd	64(%rsp), %xmm3		//load b1 into reg3
	movupd	80(%rsp), %xmm2		//load b2 into reg2
	addpd	%xmm5, %xmm3		//add
	movsd	%xmm7, 16(%rsp)
	xorpd	%xmm5, %xmm5		//zero
	mulpd	%xmm5, %xmm4		//zero
	addpd	%xmm4, %xmm2		//add
	movupd	48(%rsp), %xmm4
	mulpd	%xmm4, %xmm5		//zero
	mulpd	.LC6(%rip), %xmm4	//
	addpd	%xmm5, %xmm3		//c1 = c1 + a*b1
	addpd	%xmm4, %xmm2		//c2 = c2 + a*b2
	movupd	%xmm3, 64(%rsp)
	movapd	%xmm6, %xmm3
	movupd	%xmm2, 80(%rsp)
	movapd	%xmm1, %xmm2
	movsd	80(%rsp), %xmm5		//store c1 to C(0) array
	movsd	64(%rsp), %xmm4		//store c2 to C(2) array
	call	__printf_chk
	movsd	16(%rsp), %xmm7
	movl	$.LC8, %esi
	movsd	(%rsp), %xmm6
	movl	$1, %edi
	movsd	88(%rsp), %xmm5
	movl	$6, %eax
	movsd	72(%rsp), %xmm4
	movsd	56(%rsp), %xmm1
	movapd	%xmm7, %xmm3
	movsd	40(%rsp), %xmm0
	movapd	%xmm6, %xmm2
	call	__printf_chk
	xorl	%eax, %eax
	addq	$104, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE531:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1073741824
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC5:
	.long	0
	.long	1074266112
	.long	0
	.long	1074266112
	.align 16
.LC6:
	.long	0
	.long	1073741824
	.long	0
	.long	1073741824
	.ident	"GCC: (Ubuntu/Linaro 4.6.1-9ubuntu3) 4.6.1"
	.section	.note.GNU-stack,"",@progbits
