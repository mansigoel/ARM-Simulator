.equ SWI_Exit, 0x11

mov r5, #20
SUB r5,r5,#2
mov r2,#0
mov r3,#1
mov r4,#0
mov r1,r2
mov r1,r3
LOOP:
	CMP r4,r5
	BGE Exit
	ADD r1,r2,r3
	mov r2,r3
	mov r3,r1
	ADD r4,r4,#1
	B LOOP
Exit:
	swi SWI_Exit
