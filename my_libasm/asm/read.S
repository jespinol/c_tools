			global  my_read

			section .text
my_read:
            push    rbp
            mov     rbp, rsp
            mov     rax, 0
            syscall

close:
            mov     rsp, rbp
            pop     rbp
            ret