################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.c \
../Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.c 

OBJS += \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.o \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.o 

C_DEPS += \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.d \
./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32WBAxx_HAL_Driver/Src/%.o Drivers/STM32WBAxx_HAL_Driver/Src/%.su Drivers/STM32WBAxx_HAL_Driver/Src/%.cyclo: ../Drivers/STM32WBAxx_HAL_Driver/Src/%.c Drivers/STM32WBAxx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM33 -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32WBA65xx -c -I../Core/Inc -I"C:/Users/melvy/Documents/EPFL/2025-2026 MA1-MA2/MA2/Association/n-pulse/BMI-EMG-Firmware/test_folder/eeg_decoder_nucleo_wba65/Drivers/CMSIS/DSP/PrivateInclude" -I"C:/Users/melvy/Documents/EPFL/2025-2026 MA1-MA2/MA2/Association/n-pulse/BMI-EMG-Firmware/test_folder/eeg_decoder_nucleo_wba65/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32WBAxx_HAL_Driver/Inc -I../Drivers/STM32WBAxx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32WBAxx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32WBAxx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32WBAxx_HAL_Driver-2f-Src

clean-Drivers-2f-STM32WBAxx_HAL_Driver-2f-Src:
	-$(RM) ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_cortex.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_dma_ex.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_exti.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_flash_ex.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_gpio.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_pwr_ex.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_rcc_ex.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_uart_ex.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart.su ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.cyclo ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.d ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.o ./Drivers/STM32WBAxx_HAL_Driver/Src/stm32wbaxx_hal_usart_ex.su

.PHONY: clean-Drivers-2f-STM32WBAxx_HAL_Driver-2f-Src

