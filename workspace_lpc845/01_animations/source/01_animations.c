#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"  // Incluir la cabecera de GPIO

// Etiquetas para los pines
#define C 6
#define DP 1
#define E 0
#define D 14
#define P 10
#define A 11
#define F 13
#define A1 8
#define A2 9
#define USER 4

/*
 * @brief   Application entry point.
*/

void delay(){
    for (volatile uint32_t val = 0; val < 500000; val++) {
    }
}


int main(void) {
    // Inicialización

	BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Estructura de configuración para entrada y salidas
    gpio_pin_config_t config = { kGPIO_DigitalInput, 0 };
    gpio_pin_config_t config2 = { kGPIO_DigitalOutput, 1 };

    // Configuración de los pines
    GPIO_PortInit(GPIO, 0);

    GPIO_PinInit(GPIO, 0, USER, &config);  // Pin de entrada USER
    GPIO_PinInit(GPIO, 0, DP,&config2);   // Pin de salida DP
    GPIO_PinInit(GPIO, 0, C, &config2);    // Pin de salida C
    GPIO_PinInit(GPIO, 0, E, &config2);    // Pin de salida E
    GPIO_PinInit(GPIO, 0, D, &config2);    // Pin de salida D
    GPIO_PinInit(GPIO, 0, P, &config2);    // Pin de salida P
    GPIO_PinInit(GPIO, 0, A, &config2);    // Pin de salida P
    GPIO_PinInit(GPIO, 0, F, &config2);
    config2.outputLogic = 0;
    GPIO_PinInit(GPIO, 0, A1, &config2);   // Pin de salida A1
    GPIO_PinInit(GPIO, 0, A2, &config2);   // Pin de salida A2

    while (1){
        // Si el botón USER está presionado (1)
        if (GPIO_PinRead(GPIO, 0, USER) == 0) {
            GPIO_PinWrite(GPIO, 0, DP, 0);  // Encender C
            delay();
            GPIO_PinWrite(GPIO, 0, C, 0);  // Encender C
            delay();                       // Esperar
            GPIO_PinWrite(GPIO, 0, D, 0); // Encender DP
            delay();                      // Esperar
            GPIO_PinWrite(GPIO, 0, E, 0); // Encender DP
            delay();
            GPIO_PinWrite(GPIO, 0, F, 0);  // Encender C
            delay();                       // Esperar
            GPIO_PinWrite(GPIO, 0, P, 0); // Encender DP
            delay();                       // Esperar
            GPIO_PinWrite(GPIO, 0, A, 0); // Encender DP
        }
        delay();  // Pequeño retardo en cada ciclo para evitar sobrecarga de CPU
}
    return 0;
}
