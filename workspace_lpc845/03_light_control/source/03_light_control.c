#include "board.h"
#include "fsl_sctimer.h"
#include "fsl_swm.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "fsl_common.h"
#include "fsl_i2c.h"
#include "fsl_debug_console.h"

// Dirección del BH1750
#define BH1750_ADDR 0x5c
// Frecuencia del PWM
#define PWM_FREQ 1000

/**
 * @brief Inicializa el I2C y el sensor BH1750
 */
void InitBH1750(void) {
    // Inicializo el clock del I2C1
    CLOCK_Select(kI2C1_Clk_From_MainClk);

    // Asigno las funciones de I2C1 a los pines PIO0_27 (SDA) y PIO0_26 (SCL)
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, kSWM_PortPin_P0_27);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, kSWM_PortPin_P0_26);
    CLOCK_DisableClock(kCLOCK_Swm);

    // Configuración de master para el I2C con 400 KHz de clock
    i2c_master_config_t config;
    I2C_MasterGetDefaultConfig(&config);
    config.baudRate_Bps = 400000;
    I2C_MasterInit(I2C1, &config, SystemCoreClock);

    // Inicializo el sensor BH1750
    if (I2C_MasterStart(I2C1, BH1750_ADDR, kI2C_Write) == kStatus_Success) {
        uint8_t cmd = 0x01;  // Comando de encendido
        I2C_MasterWriteBlocking(I2C1, &cmd, 1, kI2C_TransferDefaultFlag);
        I2C_MasterStop(I2C1);
    }
    if (I2C_MasterStart(I2C1, BH1750_ADDR, kI2C_Write) == kStatus_Success) {
        uint8_t cmd = 0x10;  // Modo de medición continua a 1 lux de resolución
        I2C_MasterWriteBlocking(I2C1, &cmd, 1, kI2C_TransferDefaultFlag);
        I2C_MasterStop(I2C1);
    }
}

/**
 * @brief Lee el valor de luz del sensor BH1750
 * @return El valor de lux medido
 */
float ReadLux(void) {
    uint8_t res[2] = {0};
    if (I2C_MasterStart(I2C1, BH1750_ADDR, kI2C_Read) == kStatus_Success) {
        I2C_MasterReadBlocking(I2C1, res, 2, kI2C_TransferDefaultFlag);
        I2C_MasterStop(I2C1);
    }
    return ((res[0] << 8) + res[1]) / 1.2;
}

/**
 * @brief Programa principal
 */
int main(void) {
    // Inicialización de la consola
    BOARD_InitDebugConsole();
    InitBH1750();

    // Conecto la salida 4 del SCT al LED azul
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_SCT_OUT4, kSWM_PortPin_P0_29);
    CLOCK_DisableClock(kCLOCK_Swm);

    // Eligo el clock para el Timer
    uint32_t sctimer_clock = CLOCK_GetFreq(kCLOCK_Fro);
    sctimer_config_t sctimer_config;
    SCTIMER_GetDefaultConfig(&sctimer_config);
    SCTIMER_Init(SCT0, &sctimer_config);

    // Configuro el PWM
    sctimer_pwm_signal_param_t pwm_config = {
        .output = kSCTIMER_Out_4,       // Salida del Timer
        .level = kSCTIMER_LowTrue,      // Lógica negativa
        .dutyCyclePercent = 0            // Comienza apagado
    };

    uint32_t event;
    SCTIMER_SetupPwm(SCT0, &pwm_config, kSCTIMER_CenterAlignedPwm, PWM_FREQ, sctimer_clock, &event);
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);

    while (1) {
        // Leer lux del sensor BH1750
        float lux = ReadLux();
        PRINTF("LUX : %d \r\n", (uint16_t)lux);

        // Ajustar el ciclo de trabajo basado en el valor de lux (máximo 65535)
        uint8_t dutyCycle = (lux * 100) / 15000;  // Convertir lux a porcentaje
        if (dutyCycle > 100) dutyCycle = 100;

        // Actualizar el ciclo de trabajo
        SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_4, dutyCycle, event);

        // Delay para evitar lecturas excesivas
        for (volatile int i = 0; i < 100000; i++);  // Simple delay
    }
}
