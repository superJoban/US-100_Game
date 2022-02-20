
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <bcm_host.h>
#include <time.h>

#define BLOCK_SIZE 4096
#define GPIO_OFFSET 0x200000

volatile unsigned int *gpio;
void *gpioMap;
int fdGPIO;

#define TX 9
#define RX 10

#define GPFSEL0 0
#define GPFSEL1 1

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD 37
#define GPPUDCLK0 38

void initSensor() {
	unsigned peripheralBase = bcm_host_get_peripheral_address();
	fprintf( stderr, "%08x %08x\n", peripheralBase, peripheralBase + GPIO_OFFSET);
	fdGPIO = open("/dev/mem", O_RDWR|O_SYNC);
	gpioMap = (unsigned int *)mmap(
		NULL,
		BLOCK_SIZE,
		PROT_READ|PROT_WRITE,
		MAP_SHARED,
		fdGPIO,
		peripheralBase + GPIO_OFFSET
	);
	
	if(gpioMap == MAP_FAILED) {
		perror("mmap");
		return;
	}
	
	gpio = (volatile unsigned int *) gpioMap;
	register unsigned int r;
	
	r = gpio[GPFSEL0];
	r &= ~(0x7 << 27);
	r |= (0x1 << 27);	
	gpio[GPFSEL0] = r;
	
	r = gpio[GPFSEL1];
	r &= ~(0x7);
	gpio[GPFSEL1] = r;
	
	
	
	gpio[GPPUD] = 0x0;
	r = 150;
	while(r--){
		asm volatile("nop");
	}
	gpio[GPPUDCLK0] = (0x1 << TX);
	r = 150;
	while(r--) {
		asm volatile("nop");
	}
	gpio[GPPUDCLK0] = 0;
	
	gpio[GPCLR0] = 1 << TX;

}
void freeSensor() {
	munmap( gpioMap, BLOCK_SIZE);
	close(fdGPIO);
}

void setTX() {
	
	register unsigned int r;
	
	r = (0x1 << 9);
	gpio[GPSET0] = r;
}

void clearTX() {
	register unsigned int r;
	r = (0x1 << 9);
	gpio[GPCLR0] = r;
}

int readRX() {
	register unsigned int r = gpio[GPLEV0];
	r = ((r >> RX) & 0x1);
	return r;
}

