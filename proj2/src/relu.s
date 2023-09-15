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

    blt a1, t0, loop_end # if a1 < 1 loop_end because the length of the vector is less than 1

    addi a0, a0, -4 # a1 = a1 -4
    
loop_start:

    bgt t0, a1, done # if t0 >= a1 then goto loop_end because the iteration process is done

    addi a0, a0, 4 # a1 = a1 + 4

    j loop_continue  # jump to loop_continue

loop_continue:

    addi t0, t0, 1 # t0 = t0 + 1

    lw t2, 0(a0) # load the content in the address pointed by pointer a0

    bgt t2, zero ,loop_start # if t2 >zero then goto loop_start

    sw zero, 0(a0) # store zero to the address pointed
 
    j loop_start  # jump to loop_start
    
loop_end:

    li a1 78

    j exit2

    # Epilogue

done:    
	ret
