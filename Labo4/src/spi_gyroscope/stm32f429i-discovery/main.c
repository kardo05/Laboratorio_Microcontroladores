#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
#include "clock.h"
#include "console.h"
#include "spi-mems.c"




/*
 * This then is the actual bit of example. It initializes the
 * SPI port, and then shows a continuous display of values on
 * the console once you start it. Typing ^C will reset it.
 */
int main(void)
{
	int16_t vecs[3];
	int16_t baseline[3];
	int tmp, i;
	int count;
	uint32_t cr_tmp;

	clock_setup();
	console_setup(115200);

	/* Enable the GPIO ports whose pins we are using */
	rcc_periph_clock_enable(RCC_GPIOF | RCC_GPIOC);

	gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN,
			GPIO7 | GPIO8 | GPIO9);
	gpio_set_af(GPIOF, GPIO_AF5, GPIO7 | GPIO8 | GPIO9);
	gpio_set_output_options(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ,
				GPIO7 | GPIO9);

	/* Chip select line */
	gpio_set(GPIOC, GPIO1);
	gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO1);

	rcc_periph_clock_enable(RCC_SPI5);

	cr_tmp = SPI_CR1_BAUDRATE_FPCLK_DIV_8 |
		 SPI_CR1_MSTR |
		 SPI_CR1_SPE |
		 SPI_CR1_CPHA |
		 SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE;

	put_status("\nBefore init: ");
	SPI_CR2(SPI5) |= SPI_CR2_SSOE;
	SPI_CR1(SPI5) = cr_tmp;
	put_status("After init: ");


	baseline[0] = 0;
	baseline[1] = 0;
	baseline[2] = 0;
	console_puts("MEMS demo (new version):\n");
	console_puts("Press a key to read the registers\n");
	console_getc(1);
	tmp = read_reg(0xf);
	if (tmp != 0xD4) {
		console_puts("Maybe this isn't a Gyroscope.\n");
	}
	/*
	 * These parameters are sort of random, clearly I need
	 * set something. Based on the app note I reset the 'baseline'
	 * values after 100 samples. But don't see a lot of change
	 * when I move the board around. Z doesn't move at all but the
	 * temperature reading is correct and the ID code returned is
	 * as expected so the SPI code at least is working.
	 */
	write_reg(0x20, 0xcf);  /* Normal mode */
	write_reg(0x21, 0x07);  /* standard filters */
	write_reg(0x23, 0xb0);  /* 250 dps */
	tmp = (int) read_reg(0x26);
	console_puts("Temperature: ");
	print_decimal(tmp);
	console_puts(" C\n");

	count = 0;
	while (1) {
		tmp = read_xyz(vecs);
		for (i = 0; i < 3; i++) {
			int pad;
			console_puts(axes[i]);
			tmp = vecs[i] - baseline[i];
			pad = print_decimal(tmp);
			pad = 15 - pad;
			while (pad--) {
				console_puts(" ");
			}
		}
		console_putc('\r');
		if (count == 100) {
			baseline[0] = vecs[0];
			baseline[1] = vecs[1];
			baseline[2] = vecs[2];
		} else {
			count++;
		}
		msleep(100);
	}
}
