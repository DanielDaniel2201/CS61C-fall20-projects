.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:
    li t1, 1 # t1 = 1, acts as a counter for the loop

    ble t1, a1, no_exception # if a1 < t1 then goto exception

    li a1, 77 # a1 = 77
    
    j exit2

no_exception:
    # Prologue
    addi sp, sp, -8 # set stack pointerfor s1 and s2

    sw s1, 0(sp)

    sw s2, 4(sp)
    
    mv t1, zero

    li s1, 0 # s1 serves as a record of the index of the larges element in the array so far

    lw s2, 0(a0) # s2, initiated as the first element, serves as a record for the largest value of the array

loop_start:

    addi t1, t1, 1 # t1 += 1

    beq t1, a1, loop_end # goto loop_end when the iteration is done

    addi a0, a0, 4 # since the first element is alrealy stored at the beginning, no need to cater for it

loop_continue:

    lw t2, 0(a0) # t2 equals to the current value

    bge s2, t2, loop_start # if t2 > s2, update the index held

    mv s2, t2 # update the largest element so far

    mv s1, t1 # update the targeted index

    j loop_start

loop_end:

    mv a0, s1 # store the targeted index into a0 as the return register

    # Epilogue

    lw s1 0(sp)

    lw s2 4(sp)

    addi sp, sp, 8

    ret
