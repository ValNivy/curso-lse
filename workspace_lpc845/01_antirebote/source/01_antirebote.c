#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "fsl_debug_console.h"

// Etiqueta para el LED azul
#define rojo 2
#define boton 4

/*
 * @brief   Application entry point.
*/
int main(void) {
	// Inicializacion
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // Estructura de configuracion para salida
    gpio_pin_config_t config = { kGPIO_DigitalOutput, 0};
    gpio_pin_config_t config2 = { kGPIO_DigitalInput, 1};
    // Configuro el pin 0 del GPIO 1 como salida
    GPIO_PinInit(GPIO, 1, rojo, &config);
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit (GPIO, 0, boton, &config2);
    GPIO_PortInit (GPIO, 0);

    if (GPIO_PinRead (GPIO, 0, boton)==0){
    	GPIO_PinWrite(GPIO, 1, rojo, 1);
    }

    return 0;
}
