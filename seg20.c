#include <string.h>

#ifdef PICO_RP2040
#include "pico/stdlib.h"
#endif

#include "portable/print.h"
#include "portable/conio.h"
#include "portable/timer.h"

#include "i8080.h"
#include "i8080.c"

#include "rom_solos.h"

#define MEMORY_SIZE 0x10000
static uint8_t memory[MEMORY_SIZE]={0};

#define SOL20_DOTCLOCK_HZ 14318180
#define SOL20_CPU_DIV     7
#define SOL20_CPU_HZ      SOL20_DOTCLOCK_HZ/SOL20_CPU_DIV

//static uint8_t rb(void* userdata, uint16_t addr) {
//  return memory[addr];
//}

//static void wb(void* userdata, uint16_t addr, uint8_t val) {
//  memory[addr] = val;
//}

uint8_t sol20_keybuf_rp=0;
uint8_t sol20_keybuf_wp=0;
uint8_t sol20_keybuf[0xFF];

static bool sol20_keybuf_status(void){
	return sol20_keybuf_rp==sol20_keybuf_wp;
}

}

static uint8_t port_in(void* userdata, uint8_t port) {
	switch(port){

		default:{
					  print("unknown port_in: port=");
					  print_hex_u8(port);
					  print(NL);
					  break;
				  }
	}
	return 0x00;
}

static uint8_t vdm_scr_pos=0;

static void vdm_render(){
	bool is_inverse=false;
	int offset=vdm_scr_pos&0xF;
	for(int row=0;row<16;++row){
#ifndef __DJGPP__
		gotoxy(1+((80-64)/2),2+row);
#endif
		for(int col=0;col<64;++col){
			uint8_t c=memory[0xCC00+(64*(0xF&(row+offset)))+col];
			bool inverse=c&0x80;
			c&=0x7F;
#ifndef __DJGPP__
			if(inverse&&!is_inverse){
				is_inverse=true;
				inverse_on();}
			if(!inverse&&is_inverse){
				is_inverse=false;
				inverse_off();}
			printc(c);
#else
			uint8_t attrib=inverse?WHITE<<4:WHITE;
			ScreenPutChar(c,attrib,col+((80-64)/2),1+row);
#endif
		}
	}
#ifndef __DJGPP__
	if(!is_inverse) inverse_off();
#endif
}

static bool sol20_port_out(void* userdata, uint8_t port, uint8_t val){
	//i8080* const c = (i8080*) userdata;

	switch(port){
		case 0xFA: break;
		// VDM Display Parameter
		case 0xFE: vdm_scr_pos=val; break;
		default:{
					  print("unknown port_out: port=");
					  print_hex_u8(port);
					  print(" value=");
					  print_hex_u8(val);
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

  c->pc=0xC000;

  while(1){
	  clock_t ticks=clock();

	  unsigned int tstates=
		  i8080_run(c,SOL20_CPU_HZ/60,false);
	  ticks=clock()-ticks;

	  vdm_render();

	  char buf[0xFF];
	  gotoxy(1,19);
	  str_u(buf,c->count);
	  cputs(buf);
	  cputs(" instructions executed in ");
	  str_u(buf,tstates);
	  cputs(buf);
	  cputs(" cycles \r\n");
	  str_u32(buf,ticks);
	  cputs(buf);
	  cputs(" ticks ");
	  double khz=(double)tstates/((double)ticks*(1.0/((double)CLOCKS_PER_SEC/1000.0)));
	  if(khz<1000.0){
		  str_u32_d(buf,(uint32_t)(1000.0*khz),3);
		  cputs(buf);
		  cputs(" khz");
	  }else{
		  str_u32_d(buf,(uint32_t)khz,3);
		  cputs(buf);
		  cputs(" mhz");
	  }
	  clreol();
	  cputs("\r\n");
#ifdef __DJGPP__
	  delay(100);
#else
#ifndef __MINT__
	  usleep(100000);
#endif
#endif
  }
}

int main(void) {
#ifndef __DJGPP__
	uint16_t txt_w,txt_h;
#else
	uint8_t txt_w,txt_h;
	textmode(C80);
	_setcursortype(_NOCURSOR);
#endif
	screensize(&txt_w,&txt_h);
#ifdef __MINT__
	Cursconf(0,0);
	Cconws("\33v");	
	for(int i=0;i<16;++i){
		Cconws("\33b");
		Cconout(15);
		Cconws("\33c");
		Cconout(i);
		if(i<10) printc(' ');
		printi(i);
	}
	fgetc_cons();
	Cconws("\33b");
	Cconout(0);
	Cconws("\33c");
	Cconout(15);
#endif
	clrscr();
	inverse_on();
	gotoxy(8,1);
	cputs("SEG20 Emulator 2025 by Seg <seg@haxxed.com> http://www.haxxed.com/");
	inverse_off();

	i8080 cpu;
	//memset(memory, 0, MEMORY_SIZE);
	memcpy(&memory[0xC000],&rom_solos,rom_solos_len);
	//while(1){
		run_seg20(&cpu);
	//}

	return 0;
}
