#include "board.h"
#include "pin_mux.h"
#include "fsl_dac.h"
#include "fsl_swm.h"
#include "fsl_power.h"
#include "fsl_iocon.h"
#include <math.h>   // Para usar sin()

// Definir M_PI si no está disponible en math.h
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Programa principal
 */
int main(void) {
    // Clock del sistema a 30 MHz
    BOARD_BootClockFRO30M();

    // Configuro la salida del DAC al PIO0_29 (pin 15)
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetFixedPinSelect(SWM0, kSWM_DAC_OUT1, true);  // Usamos el DAC1
    CLOCK_DisableClock(kCLOCK_Swm);

    // Habilito la función de DAC en el PIO0_29 (pin 15)
    CLOCK_EnableClock(kCLOCK_Iocon);
    IOCON_PinMuxSet(IOCON, 29, IOCON_PIO_DACMODE_MASK);  // Configurar PIO0_29 como salida DAC
    CLOCK_DisableClock(kCLOCK_Iocon);

    // Enciendo el DAC
    POWER_DisablePD(kPDRUNCFG_PD_DAC1);

    // Configuración por defecto del DAC
    dac_config_t dac_config;
    DAC_GetDefaultConfig(&dac_config);
    DAC_Init(DAC1, &dac_config);
    DAC_SetBufferValue(DAC1, 0);

    // Configuro el Systick para 10 us
    SysTick_Config(SystemCoreClock / 100000);  // 10 us

    while(1);
    return 0;
}

// Frecuencia de la señal senoidal (10 kHz)
const float freq = 10000.0;
// Variable de tiempo (en pasos de 10 us)
static uint32_t time_step = 0;
// Valor máximo del DAC (1023 para un DAC de 10 bits)
const uint16_t dac_max_value = 1023;
// Offset en el valor del DAC para 1.65 V (mitad de 3.3V)
const uint16_t dac_offset = dac_max_value / 2;  // 511 para 1.65V
// Amplitud de la señal senoidal (ajustada para no sobrepasar el rango del DAC)
const uint16_t amplitude = dac_offset - 1;  // Amplitud para que la señal no exceda 0V o 3.3V

/**
 * @brief Handler para la interrupción del Systick
 */
void SysTick_Handler(void) {
    // Calculo el valor senoidal con offset, ajustando para pasos de 10 us
    float sin_value = sinf(2.0f * M_PI * freq * time_step * 0.00001f); // 0.00001 = 10 us

    // Escalo el valor al rango del DAC (0 a 1023) con offset de 1.65V
    uint16_t dac_value = (uint16_t)((sin_value * amplitude) + dac_offset);

    // Cargo el valor al DAC
    DAC_SetBufferValue(DAC1, dac_value);

    // Incremento el tiempo en pasos de 10 us
    time_step++;
}
