.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue
    li t0, 1 # t0 = zero, t0 acts as a counter which decides to terminate the LOOP when t1 >= a1

    blt a1, t0, exception # if a1 < 1 loop_end because the length of the vector is less than 1

loop_start:

    bgt t0, a1, loop_end # if t0 >= a1 then goto loop_end because the iteration process is done

    li t3, 1 # t3 = 1

    bne t0, t3, update # if it's not the first element then update a0 now
    
    j loop_continue # if it's the first element then no need for update

update:

    addi a0, a0, 4 # a1 = a1 + 4

    j loop_continue  # jump to loop_continue

loop_continue:

    addi t0, t0, 1 # t0 = t0 + 1

    lw t2, 0(a0) # load the content in the address pointed by pointer a0

    bgt t2, zero ,loop_start # if t2 >zero then goto loop_start

    sw zero, 0(a0) # store zero to the address pointed
 
    j loop_start  # jump to loop_start
    
exception:

    li a1 78

    j exit2 # terminate with error code 78

    # Epilogue

loop_end:    
	ret
