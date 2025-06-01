#ifndef I8080_I8080_H_
#define I8080_I8080_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct i8080 {
  // memory + io interface
  uint8_t* mem;
  uint8_t (*port_in)(void*, uint8_t); // user function to read from port
  bool (*port_out)(void*, uint8_t, uint8_t); // same for writing to port
  void* userdata; // user custom pointer

  unsigned long cyc; // cycle count

  uint16_t pc, sp; // program counter, stack pointer
  uint8_t a, b, c, d, e, h, l, flags; // registers
  uint8_t interrupt_vector;
  uint8_t interrupt_delay;
  // flags: sign, zero, half-carry, parity, carry, interrupt flip-flop
  bool cf, iff;
  bool halted;
  bool interrupt_pending;
} i8080;

void i8080_init(i8080* const c);
long i8080_run(i8080* const c);
void i8080_interrupt(i8080* const c, uint8_t opcode);
void i8080_debug_output(i8080* const c, bool print_disassembly);

#endif // I8080_I8080_H_
