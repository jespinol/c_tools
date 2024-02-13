            global  my_memmove

            section .text
my_memmove:
            push    rbp
            mov     rbp, rsp

            cmp     rdi, 0                  ; Check if either string is NULL, exit if so
            je      close
            cmp     rsi, 0
            je      close

            cmp     rdi, rsi            ; Check for overlapping regions
            je      close               ; dest is src are the same, do nothing
            mov     rcx, rdi
            sub     rcx, rsi
            cmp     rcx, rdx
            jge     forward_copy        ; dest is after src+n

            add     rsi, rdx            ; Move pointers to src+n and dest+n to copy from end
            add     rdi, rdx

backward_copy:
            cmp     rdx, 0
            jle      close               ; n is 0

            dec     rdi
            dec     rsi

            mov     al, [rsi]           ; Store char from src in dest
            mov     [rdi], al

            dec     rdx
            jmp     backward_copy

forward_copy:
            cmp     rdx, 0
            jle      close               ; n is 0

            mov     al, [rsi]           ; Store char from src in dest
            mov     [rdi], al

            inc     rdi
            inc     rsi
            dec     rdx
            jmp     forward_copy

close:
            mov     rsp, rbp
            pop     rbp
            ret
