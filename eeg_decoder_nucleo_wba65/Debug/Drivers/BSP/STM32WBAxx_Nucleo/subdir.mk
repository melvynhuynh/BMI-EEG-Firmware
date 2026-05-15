################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.c 

OBJS += \
./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.o 

C_DEPS += \
./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32WBAxx_Nucleo/%.o Drivers/BSP/STM32WBAxx_Nucleo/%.su Drivers/BSP/STM32WBAxx_Nucleo/%.cyclo: ../Drivers/BSP/STM32WBAxx_Nucleo/%.c Drivers/BSP/STM32WBAxx_Nucleo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM33 -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32WBA65xx -c -I../Core/Inc -I"C:/Users/melvy/Documents/EPFL/2025-2026 MA1-MA2/MA2/Association/n-pulse/BMI-EMG-Firmware/test_folder/eeg_decoder_nucleo_wba65/Drivers/CMSIS/DSP/PrivateInclude" -I"C:/Users/melvy/Documents/EPFL/2025-2026 MA1-MA2/MA2/Association/n-pulse/BMI-EMG-Firmware/test_folder/eeg_decoder_nucleo_wba65/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32WBAxx_HAL_Driver/Inc -I../Drivers/STM32WBAxx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32WBAxx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32WBAxx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32WBAxx_Nucleo

clean-Drivers-2f-BSP-2f-STM32WBAxx_Nucleo:
	-$(RM) ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.cyclo ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.d ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.o ./Drivers/BSP/STM32WBAxx_Nucleo/stm32wbaxx_nucleo.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32WBAxx_Nucleo

