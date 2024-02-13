			global  my_write

			section .text
my_write:
            push    rbp
            mov     rbp, rsp
            mov     rax, 1
            syscall

close:
            mov     rsp, rbp
            pop     rbp
            ret