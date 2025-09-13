// This file uses the 8080 emulator to run the test suite (roms in cpu_tests
// directory). It uses a simple array as memory.

#include <string.h>

#ifdef PICO_RP2040
#include "pico/stdlib.h"
#endif

#include "portable/print.h"
#include "portable/timer.h"

#include "i8080.h"
#include "i8080.c"

#include "rom_solos.h"

// memory callbacks
#define MEMORY_SIZE 0x10000
static uint8_t memory[MEMORY_SIZE]={0};
static bool test_finished = 0;

static uint8_t rb(void* userdata, uint16_t addr) {
  return memory[addr];
}

static void wb(void* userdata, uint16_t addr, uint8_t val) {
  memory[addr] = val;
}

static uint8_t port_in(void* userdata, uint8_t port) {
  return 0x00;
}

static bool sol20_port_out(void* userdata, uint8_t port, uint8_t value) {
	i8080* const c = (i8080*) userdata;

	switch(port){
		case 0xFA: break;
		case 0xFE: break;
		default:{
					  print("unknown port_out: port=");
					  print_hex_u8(port);
					  print(" value=");
					  print_hex_u8(value);
					  print(NL);
					  break;
				  }
	}
	return true;
}

static void run_seg20(i8080* const c){
  i8080_init(c);
  c->userdata = c;
  c->mem=memory;
  c->port_in = port_in;
  c->port_out = sol20_port_out;
  //memset(memory, 0, MEMORY_SIZE);

  c->pc=0xC000;

  // inject "out 0,a" at 0x0000 (signal to stop the test)
  //memory[0x0000] = 0xD3;
  //memory[0x0001] = 0x00;

  // inject "out 1,a" at 0x0005 (signal to output some characters)
  //memory[0x0005] = 0xD3;
  //memory[0x0006] = 0x01;
  //memory[0x0007] = 0xC9;

  uint64_t ticks=time_msec();
  test_finished = 0;

    // uncomment following line to have a debug output of machine state
    // warning: will output multiple GB of data for the whole test suite
  //i8080_debug_output(c, false);

  unsigned int tstates=i8080_run(c,0,false);
  ticks=time_msec()-ticks;

  print(NL "*** ");
  printu(c->count);
  print(" instructions executed in ");
  printu(tstates);
  print(" cycles " NL);
  print_u32_d((uint32_t)((double)ticks/10.0),3);
  print(" sec ");
  double khz=(double)tstates/(double)ticks;
  if(khz<1000.0){
	  print_u32_d((uint32_t)(1000.0*khz),3);
	  print(" khz" NL NL);
  }else{
	  print_u32_d((uint32_t)khz,3);
	  print(" mhz" NL NL);
  }
}

int main(void) {
#ifdef PICO_RP2040
	stdio_init_all();
#endif
	i8080 cpu;
	memcpy(&memory[0xC000],&rom_solos,rom_solos_len);
	while(1){
		run_seg20(&cpu);
	}

	return 0;
}
