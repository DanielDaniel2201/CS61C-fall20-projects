.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    # verify command line argument numbers
    li t0, 4
    bne t0, a0, exit_89

    # prologue
    addi sp, sp, -52
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)  # argv
    sw s2, 12(sp) # a2, whether to print
    sw s3, 16(sp) # pointer to malloc of 8 bytes
    sw s4, 20(sp) # pointer to m0 in memory
    sw s5, 24(sp) # pointer to malloc of 8 bytes
    sw s6, 28(sp) # pointer to m1 in memory
    sw s7, 32(sp) # pointer to malloc of 8 bytes
    sw s8, 36(sp) # pointer to input matrix in memory
    sw s9, 40(sp) # pointer of product matrix1
    sw s10, 44(sp) # pointer of product matrix2
    sw s11, 48(sp) # classification

    mv s0, a0
    mv s1, a1
    mv s2, a2

	# =====================================
    # LOAD MATRICES
    # =====================================
    # Load pretrained m0
    li a0, 8
    jal malloc
    beq a0, zero, exit_88
    mv s3, a0
    lw a0, 4(s1)
    addi a1, s3, 0
    addi a2, s3, 4
    jal read_matrix
    mv s4, a0

    # Load pretrained m1
    li a0, 8
    jal malloc
    beq a0, zero, exit_88
    mv s5, a0
    lw a0, 8(s1)
    addi a1, s5, 0
    addi a2, s5, 4
    jal read_matrix
    mv s6, a0

    # Load input matrix
    li a0, 8
    jal malloc
    beq a0, zero, exit_88
    mv s7, a0
    lw a0, 12(s1)
    addi a1, s7, 0
    addi a2, s7, 4
    jal read_matrix
    mv s8, a0

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)
    lw a1, 0(s3)
    lw a2, 4(s3)
    mv a3, s8
    lw a4, 0(s7)
    lw a5, 4(s7)

    mv t2, a1 # new row
    mv t3, a5 # new col

    mul t0, a1, a5 # total of items
    slli t1, t0, 2 # total of bytes
    mv a0, t1
    jal malloc
    beq a0, zero, exit_88
    mv a6, a0

    mv a0, s4
    jal matmul

    mv a0, a6 # pointer to 1-D array (matrix product)
    mv a1, t0 # total of items
    jal relu

    mv s9, a0 # pointer to product matrix1

    lw a1, 0(s5) # m1 row
    lw a2, 4(s5) # m1 col
    mv a3, s9 # pointer to product matrix1
    mv a4, t2 # row
    mv a5, t3 # col

    mul t0, a1, a5
    slli t1, t0, 2
    mv a0, t1
    jal malloc
    beq a0, zero, exit_88
    mv a6, a0

    mv a0, s6
    jal matmul
    mv s10, a6



    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix

    lw a0, 16(s1)
    mv a1, s10
    mv a2, t2
    mv a3, t3
    jal write_matrix

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax

    mv a0, s10
    li a1, 10
    jal argmax
    mv s11, a0 # classification

    # Print classification
    bne zero, s2, not_print
    mv a1, s11
    jal print_int

    # Print newline afterwards for clarity
    li a1, '\n'
    jal print_char

not_print:
    # free the space
    mv a0, s3
    jal free
    mv a0, s4
    jal free
    mv a0, s5
    jal free
    mv a0, s6
    jal free
    mv a0, s7
    jal free
    mv a0, s8
    jal free
    mv a0, s9
    jal free
    mv a0, s10
    jal free
    
    mv a0, s11 # return classification
    
    # epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    lw s8, 36(sp)
    lw s9, 40(sp)
    lw s10, 44(sp)
    lw s11, 48(sp)
    addi sp, sp, 52

    ret

exit_89:
    li a1, 89
    jal exit2
exit_88:
    li a1, 88
    jal exit2