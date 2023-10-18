	.section	__TEXT,__text,regular,pure_instructions
	.build_version macos, 14, 0
	.globl	_f                              ## -- Begin function f
	.p2align	4, 0x90
_f:                                     ## @f
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -12(%rbp)                 ## 4-byte Spill
	xorl	%eax, %eax
	movl	%edi, -8(%rbp)                  ## 4-byte Spill
	movl	%eax, -4(%rbp)                  ## 4-byte Spill
	jmp	LBB0_1
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	-12(%rbp), %edx                 ## 4-byte Reload
	movl	-8(%rbp), %eax                  ## 4-byte Reload
	movl	-4(%rbp), %ecx                  ## 4-byte Reload
	movl	%ecx, -20(%rbp)                 ## 4-byte Spill
	cmpl	%edx, %ecx
	movl	%eax, -16(%rbp)                 ## 4-byte Spill
	jge	LBB0_4
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	jmp	LBB0_3
LBB0_3:                                 ##   in Loop: Header=BB0_1 Depth=1
	movl	-20(%rbp), %eax                 ## 4-byte Reload
	addl	$1, %eax
	movl	$8, %ecx
	movl	%ecx, -8(%rbp)                  ## 4-byte Spill
	movl	%eax, -4(%rbp)                  ## 4-byte Spill
	jmp	LBB0_1
LBB0_4:
	movl	-16(%rbp), %eax                 ## 4-byte Reload
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                           ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movl	$29, %ecx
	xorl	%eax, %eax
	movl	%ecx, -8(%rbp)                  ## 4-byte Spill
	movl	%eax, -4(%rbp)                  ## 4-byte Spill
	jmp	LBB1_1
LBB1_1:                                 ## =>This Inner Loop Header: Depth=1
	movl	-8(%rbp), %eax                  ## 4-byte Reload
	movl	-4(%rbp), %ecx                  ## 4-byte Reload
	movl	%ecx, -16(%rbp)                 ## 4-byte Spill
	cmpl	$10, %ecx
	movl	%eax, -12(%rbp)                 ## 4-byte Spill
	jge	LBB1_4
## %bb.2:                               ##   in Loop: Header=BB1_1 Depth=1
	jmp	LBB1_3
LBB1_3:                                 ##   in Loop: Header=BB1_1 Depth=1
	movl	-16(%rbp), %eax                 ## 4-byte Reload
	addl	$1, %eax
	movl	$8, %ecx
	movl	%ecx, -8(%rbp)                  ## 4-byte Spill
	movl	%eax, -4(%rbp)                  ## 4-byte Spill
	jmp	LBB1_1
LBB1_4:
	movl	-12(%rbp), %esi                 ## 4-byte Reload
	movl	%esi, -20(%rbp)                 ## 4-byte Spill
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	-20(%rbp), %edi                 ## 4-byte Reload
	callq	_f
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"The value of the number is: %d\n"

.subsections_via_symbols
