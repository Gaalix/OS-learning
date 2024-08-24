section .rodata
    dir db "SysDir", 0
    file db "PidTimeData.dat", 0

section .bss
    fd resb 4
    pid resb 4
    time resb 4

section .text
    global _start
    _start:
        ; mkdir
        mov eax, 39
        mov ebx, dir
        mov ecx, 0o777
        int 0x80
        cmp eax, 0
        jl error

        ; chdir
        mov eax, 12
        mov ebx, dir
        int 0x80
        cmp eax, 0
        jl error

        ; creat
        mov eax, 8
        mov ebx, file
        mov ecx, 0
        int 0x80
        cmp eax, 0
        jl error
        mov [fd], eax

        ; chmod
        mov eax, 15
        mov ebx, file
        mov ecx, 0o640
        int 0x80
        cmp eax, 0
        jl error

        ; getpid
        mov eax, 20
        int 0x80
        mov [pid], eax

        ; time
        mov eax, 13
        int 0x80
        mov [time], eax

        ; write
        mov eax, 4
        mov ebx, [fd]
        mov ecx, pid
        mov edx, 4
        int 0x80
        cmp eax, 0
        jl error

        ; write
        mov eax, 4
        mov ebx, [fd]
        mov ecx, time
        mov edx, 4
        int 0x80
        cmp eax, 0
        jl error

        ; close
        mov eax, 6
        mov ebx, [fd]
        int 0x80
        cmp eax, 0
        jl error

        ; exit
        mov eax, 1
        mov ebx, 0
        int 0x80

    error:
        mov eax, 1
        mov ebx, 1
        int 0x80
