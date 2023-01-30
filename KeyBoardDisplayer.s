# This program reads characters from the keyboard and writes them to the ASCII display. Special characters as included such as whitespace
# and newline. For this, I used the Keyboard and Display MMIO Simulator. A polling approach was used. This means that the program is constantly 
# looping over the keyboard inputs and if there is one, it is immediately displayed.

# loops through 3 messages and displays each with a 5 second interval in between
.global _start
.text 
_start:
	li a0, 0xffff0004  # address of the register where KEY is
	li a1, 0xffff0008  # transmitter control
	li a2, 0xffff0000  # receiveing beat
	li a3, 0xffff000c  # transmitter data register
	addi s4, s4, 1     # comparing register
	
POLL:
	lw s2, 0(a2)       # s2 now was the Ready bit
	beqz s2, POLL      # if it is not on, POLL
	
	lw s0, 0(a0)       # loading Word 1
	lw s1, 4(a0)       # loading Word 2

POLL_2:
	lw s3, 0(a1)       # load transmitter ready bit
	bne s3, s4, POLL_2 # if key is not released, do not display
	
DISPLAY: 
	sw s0, 0(a3)       # storing Word 1
	sw s1, 4(a3)       # storing Word 2
	
	lw s5, 0(a1)       # load transmitter ready bit
	beq s5, s4, POLL
	
	j DISPLAY

.global CHARACTER
.data
CHARACTER:
.word 








