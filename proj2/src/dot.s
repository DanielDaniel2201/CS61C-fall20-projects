.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:
    # length validation
    li t0, 1
    ble t0, a2, 2nd_exception
    li a1, 75
    j exit2

2nd_exception:
    bgt t0, a3, exception
    bgt t0, a4, exception
    j no_exception

exception:
    li a1, 76
    j exit2

no_exception:
    # Prologue
    # set stack pointer for saved registers
    addi sp, sp, -16 # set sp for 3 s registers
    sw s0, 12(sp) # store word for s0, the 1st multiplicand
    sw s1, 8(sp) # store word for s1, the 2nd multiplicand
    sw s2, 4(sp) # store word for s2, each dot product, to be updated every loop
    sw s3, 0(sp) # store word for s3, the ultimate dot product
    mv s2, zero
    mv s3, zero
    
    # temporary registers
    li t0, 1 # t0, the general counter for the general loop
    li t1, 1 # t1, the 1st subcounter for the 1st subloop for the 1st byte-stride
    li t2, 1 # t2, the 2nd subcounter for the 2nd subloop fot the 2nd byte-stride
    li t3, 4 # t3, the 1st byte-stride (4 * a3)
    li t4, 4 # t4, the 2nd byte-stride (4 * a4)

    #calculation of byte-strides by t1, t2, to be stored in t3, t4
subloop1:
    bge t1, a3, subloop2
    addi t1, t1, 1
    addi t3, t3, 4
    j subloop1
subloop2:
    bge t2, a4, loop_start
    addi t2, t2, 1
    addi t4, t4, 4
    j subloop2
    
loop_start:
    bgt t0, a2, loop_end # goto loop_end when the interaion is done
    lw s0, 0(a0)
    lw s1, 0(a1)
    li t5, 1 # counter for multiplication
    mv s2, zero # reset s2 to 0
mul_loop:
    bgt t5, s1, mul_loop_end
    addi t5, t5, 1
    add s2, s2, s0
    j mul_loop
mul_loop_end:
    add s3, s3, s2
    addi t0, t0, 1
    add a0, a0, t3
    add a1, a1, t4
    j loop_start

loop_end:
    # set return register
    mv a0, s3

    # Epilogue, load words back and reset stack pointer
    lw s0, 12(sp)
    lw s1, 8(sp)
    lw s2, 4(sp)
    lw s3, 0(sp)
    addi sp, sp, 16

    ret