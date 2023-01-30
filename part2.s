# Using bubble sort to order a list of numbers
.global _start
.text
_start:
	la s0, LIST         # Load memory in address into s0 (will behave as a pointer)
	lw s1, 0(s0)        # Assigning the first element of the list (number of items) to s1
	addi s1, s1, 0     # Number of iterations
	addi s2, s2, 2

MAIN_LOOP:                  # This is the worst case scenario
	beq s2, s1, END        # loops through the list s1-1 number of times
	addi s2, s0, 4      # Increasing pointer 1 element in the stack each loop, we do it before so that it avoids element #1
	mv t0, s1           # Assigning s1 to t0
	
	jal PUSH    # 
	addi s2, s2, 1     # Decrease the number of loops each time
	j MAIN_LOOP
PUSH:
	addi sp, sp, -4     # Allocating memory in the stack
	sw ra, 0(sp)        # Setting the ra memory, previous register to go back to

COMPARE_LOOP:
	beqz t0, GO_BACK
	mv a0, s2           # Assigning pointer s2 (the place on the original list) to a0
	jal SWAP
	addi s2, s2, 4      # Next item in the list
	addi t0, t0, -1
	j COMPARE_LOOP      # 

GO_BACK:
	lw ra, 0(sp)
	addi sp, sp, 4      # Deallocating memory
	jr ra

SWAP: # To compare: compare two list entries and if needed, swap them
	lw t1, 0(a0)
	lw t2, 4(a0)
	bge t1, t2, CHANGE   # Function to acc swap
	mv a0, zero          # Returning 0 since no SWAP
	j NEXT

CHANGE: 
	sw t2, 0(a0)         # Storing next value into previous memory adress
	sw t1, 4(a0)         # Storinng previous value into next memory adress
	addi a0, zero, 1     # Returning 1 since SWAP
NEXT: 
	jr ra
END:
    ebreak

.global LIST
.data
LIST:
.word 4, 2, 1, 5, 3