#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/timer.h>
#include "pwm.h"

void init_pwm_TIM4(void) {
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_TIM4);

	//TIM4 CH1-CH4
	gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO6);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO7);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO8);
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO9);

	timer_reset(TIM4);

    // timer mode: no divider (72MHz), edge aligned, upcounting
    timer_set_mode(TIM4, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

    timer_set_prescaler(TIM4, 16);
    timer_set_period(TIM4, 1023-1);

    // PWM mode 1 (output high if CNT > CCR1)
	timer_set_oc_mode(TIM4, TIM_OC1, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM4, TIM_OC2, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM4, TIM_OC3, TIM_OCM_PWM1);
    timer_set_oc_mode(TIM4, TIM_OC4, TIM_OCM_PWM1);

	timer_enable_oc_preload(TIM4, TIM_OC1);
    timer_enable_oc_preload(TIM4, TIM_OC2);
    timer_enable_oc_preload(TIM4, TIM_OC3);
    timer_enable_oc_preload(TIM4, TIM_OC4);
    timer_enable_preload(TIM4);  

    timer_enable_oc_output(TIM4, TIM_OC1);
    timer_enable_oc_output(TIM4, TIM_OC2);
    timer_enable_oc_output(TIM4, TIM_OC3);
    timer_enable_oc_output(TIM4, TIM_OC4);

	timer_enable_counter(TIM4);
    return;	    
}
