            global  my_strcmp

            section .text
my_strcmp:
            push    rbp
            mov     rbp, rsp
            mov     rax, 0
            mov     rcx, 0

            cmp     rdi, 0                  ; Check if either string is NULL
            jz      check_null_input
            cmp     rsi, 0
            jz      check_null_input

loop:
            cmp     byte [rdi+rcx], 0       ; At end of string
            je      end_compare
            cmp     byte [rsi+rcx], 0
            je      end_compare

            mov     al, [rdi+rcx]           ; Compare two characters
            cmp     [rsi+rcx], al
            jne     end_compare

            inc     rcx
            jmp     loop                    ; Still equal, not at end. Continue loop

check_null_input:
            cmp     rdi, rsi
            je      ret_eq
            jl      ret_lt
            jmp     ret_gt

end_compare:
            mov     al, [rdi+rcx]
            sub     al, [rsi+rcx]
            cmp     al, 0
            jl      ret_lt
            jg      ret_gt

ret_eq:
            mov     rax, 0
            jmp     close

ret_lt:
            mov     rax, -1
            jmp     close

ret_gt:
            mov     rax, 1

close:
            mov     rsp, rbp
            pop     rbp
            ret