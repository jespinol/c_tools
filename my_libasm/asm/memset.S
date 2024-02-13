            global  my_memset

            section .text
my_memset:
            push    rbp
            mov     rbp, rsp

            test    rdi, rdi            ; Do nothing for (null)
            jz      close

            mov     rcx, rdx            ; Copy n (size) to rcx
            mov     al, sil             ; Copy c (fill character) to al

            rep     stosb               ; Repeat operation n times

close:
            mov     rsp, rbp
            pop     rbp
            ret