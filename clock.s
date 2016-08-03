.extern clock_state
.extern clock_count
.global _Atualiza_Clock_as
.type _Atualiza_Clock_as, @function
.section .data	
.section .text
	_Atualiza_Clock_as:
		pushq %rbp
		movq %rsp, %rbp

		movb clock_state, %al
		cmpb $0, %al
		jz clockiszero

		movb $0, clock_state
		movl clock_count, %eax
		addl $1, %eax
		movl %eax, clock_count
		jmp fim

	clockiszero:
		movb $1, clock_state
	fim:
		popq %rbp
		ret
