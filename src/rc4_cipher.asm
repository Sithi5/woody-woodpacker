.section .text
global rc4_cipher

rc4_cipher:
	/* 
	 * Storage usage:
	 *   Bytes  Location  Description
	 *       1  al        Temporary s[i] per round (zero-extended to rax)
	 *       1  bl        Temporary s[j] per round (zero-extended to rbx)
	 *       1  cl        RC4 state variable i (zero-extended to rcx)
	 *       1  dl        RC4 state variable j (zero-extended to rdx)
	 *       4  rdi       Base address of RC4 state array of 256 bytes
	 *       4  rsi       Address of current message byte to encrypt
	 *       4  rbp       End address of message array (msg + len)
	 *       4  rsp       x86 stack pointer
	 *       4  [rsp+ 0]  Caller's value of rbx
	 *       4  [rsp+ 4]  Caller's value of rdi
	 *       4  [rsp+ 8]  Caller's value of rsi
	 *       4  [rsp+12]  Caller's value of rbp
	 */
	
	/* Preserve callee-save registers */
	subl    $16, %rsp
	movl    %rbx,  0(%rsp)
	movl    %rdi,  4(%rsp)
	movl    %rsi,  8(%rsp)
	movl    %rbp, 12(%rsp)
	
	/* Load arguments */
	movl    20(%rsp), %rdi   /* Address of woody struct */
	movl    24(%rsp), %rsi   /* Address of message array */
	movl    28(%rsp), %rbp   /* Length of message array */
	addl    %rsi, %rbp       /* End of message array */
	
	/* Load state variables */
	movzbl  0(%rdi), %rcx  /* state->i */
	movzbl  1(%rdi), %rdx  /* state->j */
	addl    $2, %rdi       /* state->s */
	
	/* Skip loop if len=0 */
	cmpl    %rsi, %rbp
	je      .end
	
.loop:
	/* Increment i mod 256 */
	incl    %rcx
	movzbl  %cl, %rcx  /* Clear upper 24 bits */
	
	/* Add s[i] to j mod 256 */
	movzbl  (%rdi,%rcx), %rax  /* Temporary s[i] */
	addb    %al, %dl
	
	/* Swap bytes s[i] and s[j] */
	movzbl  (%rdi,%rdx), %rbx  /* Temporary s[j] */
	movb    %bl, (%rdi,%rcx)
	movb    %al, (%rdi,%rdx)
	
	/* Compute key stream byte */
	addl    %rbx, %rax  /* AL = s[i] + s[j] mod 256*/
	movzbl  %al, %rax   /* Clear upper 24 bits */
	movb    (%rdi,%rax), %al
	
	/* XOR with message */
	xorb    %al, (%rsi)
	
	/* Increment and loop */
	incl    %rsi
	cmpl    %rsi, %rbp
	jne     .loop
	
.end:
	/* Store state variables */
	movb    %cl, -2(%rdi)  /* Save i */
	movb    %dl, -1(%rdi)  /* Save j */
	
	/* Restore registers */
	movl     0(%rsp), %rbx
	movl     4(%rsp), %rdi
	movl     8(%rsp), %rsi
	movl    12(%rsp), %rbp
	addl    $16, %rsp
	retl