#ifndef I8080_I8080_H_
#define I8080_I8080_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct i8080 {
	uint8_t a;
	uint8_t flags;
	uint8_t b,c, d,e;
	uint8_t h,l; // registers
	uint16_t pc, sp; // program counter, stack pointer
  // memory + io interface
  uint8_t* mem;
  uint8_t (*port_in)(void*, uint8_t); // user function to read from port
  bool (*port_out)(void*, uint8_t, uint8_t); // same for writing to port
  void* userdata; // user custom pointer

  unsigned int count; // instruction count

  uint8_t interrupt_vector;
  uint8_t interrupt_delay;
  // flags: sign, zero, half-carry, parity, carry, interrupt flip-flop
  bool cf, iff;
  bool halted;
  bool interrupt_pending;
} i8080;

void i8080_init(i8080* const c);
//unsigned int i8080_run(i8080* const c,unsigned int,bool);
void i8080_interrupt(i8080* const c, uint8_t opcode);
void i8080_debug_output(i8080* const c,unsigned int,bool print_disassembly);

#endif // I8080_I8080_H_
