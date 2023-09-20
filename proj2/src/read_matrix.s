.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -36
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp) # store the file descriptor
    sw s4, 20(sp) # store the pointer to memory allocated by malloc
    sw s5, 24(sp) # store the total number of integers
    sw s6, 28(sp) # the loop counter
    sw s7, 32(sp) # movable pointer

    mv s0, a0
    mv s1, a1
    mv s2, a2

    # open the file
    mv a1, s0 # move the file name to a1
    li a2, 0 # read only
    jal fopen # now a0 has the file descriptor

    # fopen exception check
    li t0, -1
    beq a0, t0, fopen_exception

    mv s3, a0 # store file descriptor

    # read # of rows
    mv a1, s3 # move descriptor from s3 to a1
    mv a2, s1
    li a3, 4 # size of an integer
    jal fread

    # fread exception check
    li t0, 4
    bne a0, t0, fread_exception

    # read # of colums
    mv a1, s3
    mv a2, s2
    li a3, 4
    jal fread

    # fread exception check
    li t0, 4
    bne a0, t0, fread_exception

    # set to # of rows and columns
    lw s1, 0(s1)
    lw s2, 0(s2)

    # allocate heap memory
    mul a0, s1, s2 # total number of numbers in the matrix
    slli a0, a0, 2 # multiply by 4
    jal malloc
    
    # check for malloc exception
    beq zero, a0, malloc_exception
    mv s4, a0

    mul s5, s1, s2 # total number of integers
    li s6, 1 # loop counter
    mv s7, s4

loop_start:
    bgt s6, s5, loop_end
    # fread the rest integers
    mv a1, s3
    mv a2, s7
    li a3, 4 # 4 bytes an integer
    jal fread

    # fread exception check
    li t0, 4
    bne a0, t0, fread_exception

    addi s6, s6, 1 # increment counter by 1
    addi s7, s7, 4 # move the pointer by 4 bytes
    j loop_start # next round

loop_end:
    # close file
    mv a1, s3
    jal fclose

    # fclose exception check
    li t0, -1
    beq t0, a0, fclose_exception

    mv a0, s4
    # epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 4(sp)
    lw s2, 4(sp)
    lw s3, 4(sp)
    lw s4, 4(sp)
    lw s5, 4(sp)
    lw s6, 4(sp)
    lw s7, 4(sp)
    addi sp, sp, 36

    ret

fopen_exception:
    li a1, 90
    jal exit2
fread_exception:
    li a1, 91
    jal exit2
malloc_exception:
    li a1, 88
    jal exit2
fclose_exception:
    li a1, 92
    jal exit2
