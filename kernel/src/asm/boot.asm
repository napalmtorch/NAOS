[BITS 32]
[GLOBAL _mboot]
[GLOBAL _start]
[GLOBAL _stack_top]
[GLOBAL _stack_bottom]
[EXTERN _bss_start]
[EXTERN _bss_end]
[EXTERN _kernel_start]
[EXTERN _kernel_end]
[EXTERN kernel_entry]

STACKSZ equ (128 * 1024)

MBOOT_PAGE_ALIGN    equ 1 << 0
MBOOT_MEM_INFO      equ 1 << 1 
MBOOT_HEADER_MAGIC  equ 0x1BADB002
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

section .multiboot.text
align 4
_mboot:
    dd MBOOT_HEADER_MAGIC
    dd MBOOT_HEADER_FLAGS
    dd MBOOT_CHECKSUM   
    dd _mboot
    dd 0
    dd 0
    dd 0
    dd _start

section .text
_start:
    cli
    mov  ebp, _stack_top            ; set stack base
    mov  esp, ebp                   ; set stack pointer
    push ebx                        ; push multiboot address to stack
    call kernel_entry                ; jump to kernel main function
    cli                             ; disable interrupts
    hlt                             ; halt the system

section .bss
align 4096
_stack_bottom:
    resb STACKSZ
_stack_top: