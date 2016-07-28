.extern clock_state
.extern clock_count
.global _Atualiza_Clock_as
.type _Atualiza_Clock_as, @function
.section .text
	_Atualiza_Clock_as:
		pushq %rbp
		movq %rsp, %rbp

		movl clock_state, %eax
		cmpl $0, %eax
		jz clockiszero

		movl $0, clock_state
		movl clock_count, %eax
		addl $1, %eax
		movl %eax, clock_count
		jmp fim

	clockiszero:
		movl $1, clock_state
	fim:
		popq %rbp
		ret
