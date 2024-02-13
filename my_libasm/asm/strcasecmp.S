            global  my_strcasecmp

            section .text
my_strcasecmp:
            push    rbp
            mov     rbp, rsp
            mov     rax, 0
            mov     rdx, 0
            mov     eax, 0
            mov     ecx, 0
            mov     edx, 0

            cmp     rdi, 0                  ; Check if either string is NULL, exit if so
            je      close
            cmp     rsi, 0
            je      close

loop:

            cmp     byte [rdi], 0           ; At end of string 1
            je      end_rdi

            cmp     byte [rsi], 0           ; At end of string 2
            je      end_rsi


            mov     al, [rdi]
            mov     cl, [rsi]

            inc     rdi
            inc     rsi

case_compare_al:
            cmp     al, byte 65             ; Check case for both chars
            jge     convert_al              ; If uppercase, convert to lowercase

case_compare_cl:
            cmp     cl,byte 65
            jge     convert_cl
            jmp     char_compare

convert_al:
            cmp     al, byte 90             ; Check case for both chars
            jg      case_compare_cl
            add     al, byte 32
            jmp     case_compare_cl

convert_cl:
            cmp     cl, byte 90             ; Check case for both chars
            jg      char_compare
            add     cl, byte 32
            jmp     char_compare

char_compare:
            cmp     al, cl                  ; Compare two chars, now both lowercase
            jne     not_equal
            jmp     loop

not_equal:
            sub     eax, ecx                ; Calculate case-insensitive ASCII difference
            mov     edx, eax
            jmp     close

end_rdi:
            cmp     byte [rsi], 0           ; Check if also at end of string 2
            je      close                   ; If so, end program
            mov     cl, [rsi]
            jmp     case_compare_cl         ; If not, check case of next char in string 2 and calculate difference

end_rsi:
            cmp     byte [rdi], 0           ; Similar for string 1
            je      close
            mov     al, [rdi]
            jmp     case_compare_al

close:
            mov     rax, rdx
            mov     rsp, rbp
            pop     rbp
            ret

