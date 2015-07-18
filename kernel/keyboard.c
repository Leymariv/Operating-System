#include "keyboard.h"
#include <stdio.h>
#include "cpu.h"
#include "kbd.h"
#include "string.h"
#include "message_file.h"
#include "semaphore.h"
#include "ecran.h"
#include "process.h"

#define ADDR_VALIDE(addr) (unsigned)addr>=0x40000000

int fid_keyboard;
int sem;

#define GB1 0x4000000

void init_keyboard()
{
	fid_keyboard=pcreate(20);
	sem=screate(1);
	echo_fct=echo_fct_standard;
}

void scan_keyboard()
{
	outb(0x20, 0x20);
	int scancode=inb(0x60);
	do_scancode(scancode);
}

int echo_on=1;

void cons_echo(int on)
{
	echo_on=on;
}

unsigned long icour;
unsigned long cons_read(char *string, unsigned long length)
{
	if (!(ADDR_VALIDE(string)))
		return -1;
	wait(sem);
	for(icour=0; icour<length; icour++){
		int msg;
		preceive(fid_keyboard,&msg,1);
		if (msg==13) {
			signal(sem);
			return icour;
		} else if (msg==127 && icour>0) {
			string[--icour]=0;
			icour--;
		} else if (msg!=127){
			string[icour]=msg;
		} else {
			icour--;
		}
	}
	signal(sem);
	return length;
}

int cons_write(const char *str, long size)
{
	if (!(ADDR_VALIDE(str)))
		return -1;
	for (int i=0; i<size; i++)
		printf("%c",str[i]);
	return 0;
}


void echo_fct_standard(char * str, int fid, int echo)
{
	for (unsigned i=0; i<strlen(str); i++){
		if (echo){
			if ((int)(str[i])==13)
				printf("%c",(char)10);
			else if ((int)(str[i])<32)
				printf("^%c",(char)(64+(int)(str[i])));
			else if ((int)(str[i])==127 && icour>0) 
//Legere protection qui permet d'eviter d'effacer le prompt
				printf("\b \b");
			else if ((int)(str[i])!=127)
				printf("%c",str[i]);
		} 
		psend(fid,(int)str[i]);
	}
}

void keyboard_data(char *str)
{
	if (table_file[fid_keyboard]->nb_message==
	    table_file[fid_keyboard]->tampon_length)
		return;
	echo_fct(str,fid_keyboard,echo_on);
}

void kbd_leds(unsigned char leds)
{
	outb(0x60,0xed);
	outb(0x60,leds);
}
