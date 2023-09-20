.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:
    # Error checks
    ble a1, zero, m0_exception
    ble a2, zero, m0_exception
    ble a4, zero, m1_exception
    ble a5, zero, m1_exception
    bne a1, a5, match_exception
    bne a2, a4, match_exception
    j no_exception
m0_exception:
    li a1, 72
    j exit2
m1_exception:
    li a1, 73
    j exit2
match_exception:
    li a1, 74
    j exit2
no_exception:
    # Prologue
    addi sp, sp -32
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)

    # empty a-registers for future dot calls
    mv s0, a0
    mv s1, a1
    mv s2, a2
    mv s3, a3
    mv s4, a4
    mv s5, a5
    mv s6, a6
    mv s7, a3

    # counters
    li t0, 1 # counter for outer loop
    li t1, 1 # counter for inner loop
    li t2, 1 # counter for subloop for A pointer shift calculatoin

    li t3, 0 # A-pointer shift amount

    # calculation of A pointer shift
subloop_A_shift:
    bgt t2, s1, outer_loop_start
    addi t2, t2, 1
    addi, t3, t3, 4
    j subloop_A_shift

outer_loop_start:
    bgt t0, s1, outer_loop_end
    mv s3, s7 # reset s3 to the origional matrix B pointer every outer loop
    li t1, 1
    j inner_loop_start

inner_loop_start:
    bgt t1, s1, inner_loop_end

    # store t-registers for dot product
    addi sp, sp, -16
    sw t0, 0(sp)
    sw t1, 4(sp)
    sw t2, 8(sp)
    sw t3, 12(sp)

    # load arguments into a-registers for dot product procedure
    mv a0, s0 # pointer to matrix A
    mv a1, s3 # pointer to matrix B
    mv a2, s1 # number of rows of matrix A i.e. length of the vector
    li a3, 1 # stride of matrix A is 1
    mv a4, s4 # number of rows of matrix B as stride

    addi sp, sp, -4
    sw ra, 0(sp)
    jal dot  # jump to dot and save position to ra
             # the product is stored in a0 register
    lw ra, 0(sp)
    addi sp, sp, 4

    sw a0, 0(s6) # store each product according to matrix C pointer

    # load t-registers' value back
    lw t0, 0(sp)
    lw t1, 4(sp)
    lw t2, 8(sp)
    lw t3, 12(sp)
    addi sp, sp, 16

    addi t1, t1, 1 # update the counter
    addi s3, s3, 4 # move matrix B pointer to the next column
    addi s6, s6, 4 # move the target pointer 1 word further

    j inner_loop_start

inner_loop_end:
    addi t0, t0, 1 # update outer counter every time an inner loop ends 
                   # i.e. when matrix A pointer steps one stop forward
    add s0, s0, t3 # update matrix A pointer by t3, the shift amount (in bytes)
    j outer_loop_start
outer_loop_end:
    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    addi sp, sp, 32

    ret
