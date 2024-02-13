            global  my_strchr

            section .text
my_strchr:
            push    rbp
            mov     rbp, rsp
            mov     rax, 0

            test    rdi, rdi    ; Check for NULL
            jz      null_input

search_loop:
            cmp     [rdi], sil
            je      close

            inc     rdi
            cmp     [rdi], byte 0
            je      char_not_found

            jmp     search_loop

char_not_found:
            cmp     [rdi], sil
            je      close
            mov     rdi, 0

close:
            mov     rax, rdi
            mov     rsp, rbp
            pop     rbp
            ret

null_input:
            mov     rsp, rbp
            pop     rbp
            ret