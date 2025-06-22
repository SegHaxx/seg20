// This file uses the 8080 emulator to run the test suite (roms in cpu_tests
// directory). It uses a simple array as memory.

#include <string.h>

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

static bool port_out(void* userdata, uint8_t port, uint8_t value) {
	i8080* const c = (i8080*) userdata;

	if(port==0){test_finished=1;return false;}
	if (port == 1) {
		uint8_t operation = c->c;

		if (operation == 2) { // print a character stored in E
			if(c->e=='\n'){print(NL);}
			else{printc(c->e);}
		} else if (operation == 9) { // print from memory at (DE) until '$' char
			uint16_t addr = (c->d << 8) | c->e;
			do {
				char out=rb(c, addr++);
				if(out=='\n'){print(NL);}
				else{printc(out);}
			} while (rb(c, addr) != '$');
		}
	}
	return true;
}

static inline int load_file(const char* filename, uint16_t addr) {
  FILE* f = fopen(filename, "rb");
  if (f == NULL) {
    //fprintf(stderr, "error: can't open file '%s'.\n", filename);
    return 1;
  }

  // file size check:
  fseek(f, 0, SEEK_END);
  size_t file_size = ftell(f);
  rewind(f);

  if (file_size + addr >= MEMORY_SIZE) {
    //fprintf(stderr, "error: file %s can't fit in memory.\n", filename);
    return 1;
  }

  // copying the bytes in memory:
  size_t result = fread(&memory[addr], sizeof(uint8_t), file_size, f);
  if (result != file_size) {
    //fprintf(stderr, "error: while reading file '%s'\n", filename);
    return 1;
  }

  fclose(f);
  return 0;
}

static void run_seg20(i8080* const c){
  i8080_init(c);
  c->userdata = c;
  c->mem=memory;
  c->port_in = port_in;
  c->port_out = port_out;
  //memset(memory, 0, MEMORY_SIZE);

  c->pc=0xC000;

  // inject "out 0,a" at 0x0000 (signal to stop the test)
  //memory[0x0000] = 0xD3;
  //memory[0x0001] = 0x00;

  // inject "out 1,a" at 0x0005 (signal to output some characters)
  //memory[0x0005] = 0xD3;
  //memory[0x0006] = 0x01;
  //memory[0x0007] = 0xC9;

  long long ticks=time_msec();
  test_finished = 0;

    // uncomment following line to have a debug output of machine state
    // warning: will output multiple GB of data for the whole test suite
  //i8080_debug_output(c, false);

	long nb_instructions=i8080_run(c,true);
  ticks=time_msec()-ticks;

  print(NL "*** ");
  printi(nb_instructions);
  print(" instructions executed in ");
  printi(c->cyc);
  print(" cycles " NL);
  print_u32_d((uint32_t)((double)ticks/10.0),3);
  print(" sec ");
  //print_u32_d((uint32_t)((double)c->cyc/((double)ticks/100.0)));
  //print(" khz" NL NL);
  print_u32_d((uint32_t)(0.1*((double)c->cyc/(double)ticks)),3);
  print(" mhz" NL NL);
}

int main(void) {
  i8080 cpu;
  memcpy(&memory[0xC000],&rom_solos,rom_solos_len);
  run_seg20(&cpu);

  return 0;
}
