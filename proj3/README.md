# CS61CPU

Look ma, I made a CPU! Here's what I did:

## Part A

### ALU

- Create 13 subcircuits to perform all the possible operations.
- Store 13 possible results.
- Use 16-to-1 mux with `ALUSel` as selector to select the required result.

### Regfile

- Create 31 registers (no register needed for x0).
- Use two 32-to-1 muxes with `read_data1` and `read_data2` as selectors respectively to select two data output.
- Use 1-to-32 demux with `write_reg` as selectors to generate 32 write_enable inputs for 32 registers respectively (note: the WE for x0 is useless, and only 1 WE out of 32 is asserted with other 31 being deasserted).
- Every register is fed with the same `write_data`.

### Addi instruction

#### single-cycle

- The ID stage is implemented already: tunnel `instruction` contains the expected instruction.
- Use Splitter with 5 fans out to decode the instruction to get `rs1`, `rd`, `imm`, etc.
- Open a new window a open `imm_gen.circ` to implement, using Splitter and Bit Extender
- Save `imm_gen.circ`, re-open `cpu.circ` and feed the `imm_gen` with `instruction`.
- Feed what are needed to the regfile circuit (note: rs2 is useless in this case, and RegWEn should be hard-coded as asserted ).
- Feed the `read_data1` and the sign-extended immediate as 2 operands to ALU with 0 as ALUSel (hard-code the operation to be `add`)
- Use a mux to choose between operation result and `READ_DATA` with Sel hard-coded as 0 (intentionally choose the operation result).

#### pipeline

- Add `ID/rest pipeline register` that stores the instruction.
- replace the `instruction` tunnel in the Splitter and `imm_gen` circuit with the output from `ID/rest pipeline register`.
