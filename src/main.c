/* 
 * 
 * Licenseing: This is public domain code.
 * (2019 Nils Stec)
 * 
 */

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>

#include <libopencm3/cm3/systick.h>
#include "systick.h"
#include "pwm.h"

// pa8 data
// pa9 strobe/oe
// pa10 clk

#define SEG_A 0x02
#define SEG_B 0x04
#define SEG_C 0x08
#define SEG_D 0x10
#define SEG_E 0x80
#define SEG_F 0x40
#define SEG_G 0x20
#define SEG_dp 0x01

#define NUM_0   SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F
#define NUM_1   SEG_B|SEG_C
#define NUM_2   SEG_A|SEG_B|SEG_D|SEG_E|SEG_G
#define NUM_3   SEG_A|SEG_B|SEG_C|SEG_D|SEG_G
#define NUM_4   SEG_B|SEG_C|SEG_F|SEG_G
#define NUM_5   SEG_A|SEG_F|SEG_G|SEG_C|SEG_D
#define NUM_6   SEG_A|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G
#define NUM_7   SEG_A|SEG_B|SEG_C
#define NUM_8   SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G
#define NUM_9   SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G

const uint8_t digits[10] = { NUM_0, NUM_1, NUM_2, NUM_3, NUM_4, NUM_5, NUM_6, NUM_7, NUM_8, NUM_9 };

int sr4094_put7seg(int d0, int d1, int d2, int dp);
void init_sr4094(void);
void sr4094_sendbyte(uint8_t data);

int main(void) {
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    init_systick();

    /* ~4.14kHz PWM for LEDs (0-1023) */
    init_pwm_TIM4();
    TIM_CCR1(TIM4) = 1023;
    TIM_CCR2(TIM4) = 1023;
    TIM_CCR3(TIM4) = 1023;
    TIM_CCR4(TIM4) = 512;

    init_sr4094();

    sr4094_put7seg(1, 2, 8, 1); /* show "12.8" on 7 segment display */

    for(;;) {
        _systick_delay(75);
	}

    return 0;
}



void init_sr4094(void) {
    rcc_periph_clock_enable(RCC_GPIOA);

    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);     // DATA
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);     // STROBE/OE
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO10);    // CLK

    gpio_clear(GPIOA, GPIO8);
    gpio_set(GPIOA, GPIO9);
    gpio_clear(GPIOA, GPIO10);

    sr4094_sendbyte(0);
    sr4094_sendbyte(0);
    sr4094_sendbyte(0);
}

void sr4094_sendbyte(uint8_t data) {
	uint8_t i;

    gpio_clear(GPIOA, GPIO9);   // STROBE/OE

	for(i=0x80; i>=1; i/=2) {
		if(data & i)
			gpio_set(GPIOA, GPIO8);
		else
			gpio_clear(GPIOA, GPIO8);

        /* CLK */
        gpio_set(GPIOA, GPIO10);
        gpio_clear(GPIOA, GPIO10);
    }

    gpio_set(GPIOA, GPIO9);     // STROBE/OE
}

int sr4094_put7seg(int d0, int d1, int d2, int dp) {
    if(d0 > 9) d0 = 9;
    if(d1 > 9) d1 = 9;
    if(d2 > 9) d2 = 9;

    int digit0, digit1, digit2;
    
    digit0 = digits[d0];
    digit1 = digits[d1];
    digit2 = digits[d2];
    
    switch(dp) {
        case 0:
            digit0 |= SEG_dp;
            break;
        case 1:
            digit1 |= SEG_dp;
            break;
        case 2:
            digit2 |= SEG_dp;
            break;
    }
    
    sr4094_sendbyte(digit0);
    sr4094_sendbyte(digit1);
    sr4094_sendbyte(digit2);
    
    return 0;
}
