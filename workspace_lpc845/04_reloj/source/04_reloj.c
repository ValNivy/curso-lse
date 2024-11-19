#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"  // Incluir la cabecera de GPIO

// Definición de pines del display
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

// Mapeo de segmentos para números del 0 al 9 en ánodo común
const uint8_t segment_map[10] = {
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111   // 9
};

// Función de retardo simple
void delay(){
    for (volatile uint32_t val = 0; val < 500000; val++) {
    }
}

// Función para mostrar un número en el display
void display_number(uint8_t number) {
    uint8_t segments = segment_map[number];

    // Configuración de cada segmento: los segmentos en 0 se encienden en ánodo común
    GPIO_PinWrite(GPIO, 0, A, !(segments & 0x01));
    GPIO_PinWrite(GPIO, 0, P, !(segments & 0x02) >> 1);
    GPIO_PinWrite(GPIO, 0, C, !(segments & 0x04) >> 2);
    GPIO_PinWrite(GPIO, 0, D, !(segments & 0x08) >> 3);
    GPIO_PinWrite(GPIO, 0, E, !(segments & 0x10) >> 4);
    GPIO_PinWrite(GPIO, 0, F, !(segments & 0x20) >> 5);
    GPIO_PinWrite(GPIO, 0, A1, !(segments & 0x40) >> 6);
}

int main(void) {
    // Inicialización del hardware
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Estructura de configuración para entrada y salidas
    gpio_pin_config_t input_config = { kGPIO_DigitalInput, 0 };
    gpio_pin_config_t output_config = { kGPIO_DigitalOutput, 1 };

    // Configuración de los pines
    GPIO_PortInit(GPIO, 0);

    // Configuración de pines de entrada y salida
    GPIO_PinInit(GPIO, 0, USER, &input_config);  // Pin de entrada USER
    GPIO_PinInit(GPIO, 0, DP, &output_config);   // Pin de salida DP
    GPIO_PinInit(GPIO, 0, C, &output_config);    // Pin de salida C
    GPIO_PinInit(GPIO, 0, E, &output_config);    // Pin de salida E
    GPIO_PinInit(GPIO, 0, D, &output_config);    // Pin de salida D
    GPIO_PinInit(GPIO, 0, P, &output_config);    // Pin de salida P
    GPIO_PinInit(GPIO, 0, A, &output_config);    // Pin de salida A
    GPIO_PinInit(GPIO, 0, F, &output_config);    // Pin de salida F
    GPIO_PinInit(GPIO, 0, A1, &output_config);   // Pin de salida A1
    GPIO_PinInit(GPIO, 0, A2, &output_config);   // Pin de salida A2

    uint8_t counter = 0;

    while (1){
        // Si el botón USER está presionado, inicia el conteo
        if (GPIO_PinRead(GPIO, 0, USER) == 0) {
            display_number(counter);  // Mostrar el número en el display
            counter = (counter + 1) % 10;  // Incrementar contador (0-9)
            delay();  // Retardo para cambiar cada segundo
        }
    }

    return 0;
}
