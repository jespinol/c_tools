            global  my_memcpy

            section .text
my_memcpy:
            push    rbp
            mov     rbp, rsp

            test    rdi, rdi            ; Check for null (dest and src)
            jz      close
            test    rsi, rsi
            jz      close

            mov     rcx, rdx            ; Copy n to rcx
            rep     movsb               ; Copy contents from src to dest

close:
            mov     rsp, rbp
            pop     rbp
            ret