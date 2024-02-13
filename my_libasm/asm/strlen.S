            global  my_strlen

            section .text
my_strlen:
            push    rbp
            mov     rbp, rsp
            mov     rax, 0

            test    rdi, rdi        ; Check for NULL
            jz      null_input

loop:
            cmp     byte [rdi], 0   ; Reached null terminator?
            je      close
            inc     rax
            inc     rdi
            jmp     loop

null_input:
            mov     rax, 0          ; Return 0 for NULL pointer
            jmp     close

close:
            mov     rsp, rbp
            pop     rbp
            ret