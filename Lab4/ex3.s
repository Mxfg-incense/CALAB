.data
n: .word 3 # number of test data
data: # input, answer
.word 0x12345678, 0x1E6A2C48
.word 0x71C924BF, 0xFD24938E
.word 0x19260817, 0xE8106498

.text
# === main and helper functions ===
# You don't need to understand these, but reading them may be useful
main:
    la t0, n
    lw s0, 0(t0)
    la s1, data
    main_loop: # run each of the n tests
        beq s0, zero, main_loop_end
        lw a0, 0(s1)
        jal print_hex_and_space # print input
        lw a0, 4(s1)
        jal print_hex_and_space # print answer
        lw a0, 0(s1)
        jal bitrev1
        jal print_hex_and_space # print result of bitrev1
        lw a0, 0(s1)
        jal bitrev2
        jal print_hex_and_space # print result of bitrev2
        jal print_newline
        addi s0, s0, -1
        addi s1, s1, 8
        j main_loop
    main_loop_end:
    li a0, 10
    ecall # exit

print_hex_and_space:
    mv a1, a0
    li a0, 34
    ecall
    li a1, ' '
    li a0, 11
    ecall
    ret

print_newline:
    li a1, '\n'
    li a0, 11
    ecall
    ret

# === The first version ===
# Reverse the bits in a0 with a loop
bitrev1:
    ### TODO: YOUR CODE HERE ###
    li t0, 31 # t0 = 31
    li a1, 0
    Loop:
        blt t0, x0, exit
        # fetch the i^th bit
        srl t1, a0, t0 # t1 = a0>>=t0 
        andi t1, t1, 1 # t1 & 1
        addi t2, t0, -31 # t2 = t0 -31
        sub t2, x0, t2 # t2 = 31 -t1 
        sll t1, t1, t2 # t1 << = (31 -t0) 
        add a1, a1, t1 
        addi t0, t0, -1 # --t0
        j Loop
    exit:
        mv a0, a1 
        ret 

# === The second version ===
# Reverse the bits in a0. Only use li, and, or, slli, srli!
bitrev2:
    ### TODO: YOUR CODE HERE ###

    li t0, 0x55555555    # Load 0x55555555 into t0 (01010101010101010101010101010101)
    li t1, 0x33333333    # Load 0x33333333 into t1 (00110011001100110011001100110011)
    li t2, 0x0f0f0f0f    # Load 0x0f0f0f0f into t2 (00001111000011110000111100001111)

    # Swap adjacent pairs of bits
    and a0, t0, a0      # Extract even bits
    srl a0, a0, 1       # Shift even bits to odd positions
    and a1, t0, a0      # Extract odd bits
    sll a1, a1, 1       # Shift odd bits to even positions
    or a0, a0, a1       # Combine even and odd bits

    # Swap adjacent pairs of 2-bit groups
    and a1, t1, a0      # Extract groups of 2 bits
    srl a1, a1, 2       # Shift groups of 2 bits to opposite 2-bit positions
    and a2, t1, a1      # Extract the new 2-bit groups
    sll a2, a2, 2       # Shift the new 2-bit groups to their original positions
    or a1, a1, a2       # Combine the old and new 2-bit groups

    # Swap adjacent pairs of 4-bit groups
    and a2, t2, a1      # Extract groups of 4 bits
    srl a2, a2, 4       # Shift groups of 4 bits to opposite 4-bit positions
    and a3, t2, a2      # Extract the new 4-bit groups
    sll a3, a3, 4       # Shift the new 4-bit groups to their original positions
    or a2, a2, a3       # Combine the old and new 4-bit groups

    # Swap adjacent pairs of 8-bit groups
    slli a3, a2, 24     # Shift the upper 8 bits to the lower 8 bits
    srli a2, a2, 24     # Shift the lower 8 bits to the upper 8 bits
    or a0, a0, a2       # Combine the original lower 24 bits with the reversed upper 8 bits
    or a0, a0, a3       # Combine the original upper 8 bits with the reversed lower 24 bits


    ret
