            global  my_index

            section .text
my_index:
            push    rbp
            mov     rbp, rsp
            mov     rax, 0

            test    rdi, rdi        ; Check for NULL
            jz      not_found

search_loop:
            mov     al, byte [rdi]
            test    al, al          ; Check for the end of the string
            jz      not_found

            cmp     al, sil
            je      found

            inc     rdi
            jmp     search_loop

found:
            mov     rax, rdi        ; Pointer to the substring
            jmp     close

not_found:
            mov     rax, 0          ; NULL pointer

close:
            mov     rsp, rbp
            pop     rbp
            ret