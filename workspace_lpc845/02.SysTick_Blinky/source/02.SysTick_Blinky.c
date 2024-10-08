/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/**
 * @file    LPC845_Project.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/* TODO: insert other include files here. */
#define LED_BLUE 29
#define LED_ROJO 2
/* TODO: insert other definitions and declarations here. */

int main (void){
    BOARD_InitBootClocks();
	gpio_pin_config_t config = { kGPIO_DigitalOutput, 1 };
    GPIO_PortInit(GPIO, 0);
    GPIO_PinInit(GPIO, 0, LED_BLUE, &config);

    gpio_pin_config_t config2 = { kGPIO_DigitalOutput, 1 };
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, 1, LED_ROJO, &config2);



    SysTick_Config (SystemCoreClock/2);
	while(1){
	}

	return 0;
}


void SysTick_Handler (void ) {

	static int valor = 0;
	// Cambio el estado anterior del LED azul
	uint8_t val = GPIO_PinRead(GPIO, 0, LED_BLUE);
	GPIO_PinWrite(GPIO, 0, LED_BLUE, !val);
	// Demora
	valor++;

	if (valor==3){
		uint8_t val2 = GPIO_PinRead(GPIO, 1, LED_ROJO);
		GPIO_PinWrite (GPIO, 1, LED_ROJO, !val2);
		valor=0;
	}
}
