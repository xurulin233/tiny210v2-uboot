#define	COPY_BL2_SIZE		0x80000

typedef unsigned int (*copy_sd_mmc_to_mem) (\
			unsigned int  channel, \
			unsigned int  start_block, \
			unsigned short block_size, \
			unsigned int  *trg, \
			unsigned int  init);

static void copy_code_to_dram(void)
{
	unsigned long ch;
	void (*BL2)(void);
	ch = *(volatile unsigned int *)(0xD0037488);
	unsigned char channel = 0;
#if 0
	putc('\n');putc('\r');
	putc('B');putc('L');putc('1');putc(' ');putc('V');putc('e');putc('r');putc(':');
	putc('1');putc('3');putc('0');putc('6');putc('2');putc('6');
	putc('\n');putc('\r');
	putc('S');putc('t');putc('a');putc('r');putc('t');putc(' ');putc('c');putc('p');putc(' ');
	putc('\n');putc('\r');
#endif
	// 函数指针
	copy_sd_mmc_to_mem copy_bl2 = (copy_sd_mmc_to_mem) (*(unsigned int *) (0xD0037F98));

	unsigned int ret;
	
	// 通道0
	if (ch == 0xEB000000)
		channel = 0;

	// 通道2
	else if (ch == 0xEB200000)
		channel = 2;
		
	else
		return;
		
	// 0:channel 0
	// 33:源,代码位于扇区33,1 sector = 512 bytes
	// COPY_BL2_SIZE/512:长度，拷贝COPY_BL2_SIZE/512 sector，即COPY_BL2_SIZE K
	// CONFIG_SYS_TEXT_BASE:目的,链接地址CONFIG_SYS_TEXT_BASE	
	ret = copy_bl2(channel, 33, COPY_BL2_SIZE/512,(unsigned int *)CONFIG_SYS_TEXT_BASE, 0);
#if 0
	putc('R');putc('u');putc('n');putc('n');putc('i');putc('n');putc('g');putc('!');putc(' ');
	putc('\n');putc('\r');
#endif
	// 跳转到DRAM
	BL2 = (void *)CONFIG_SYS_TEXT_BASE;
	(*BL2)();
}

void board_init_f_mmc()
{
	copy_code_to_dram();	
}
