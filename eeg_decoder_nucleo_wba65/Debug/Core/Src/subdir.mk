################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/eeg_features.c \
../Core/Src/eeg_filter.c \
../Core/Src/eeg_model.c \
../Core/Src/eeg_window.c \
../Core/Src/main.c \
../Core/Src/stm32wbaxx_hal_msp.c \
../Core/Src/stm32wbaxx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32wbaxx.c 

OBJS += \
./Core/Src/eeg_features.o \
./Core/Src/eeg_filter.o \
./Core/Src/eeg_model.o \
./Core/Src/eeg_window.o \
./Core/Src/main.o \
./Core/Src/stm32wbaxx_hal_msp.o \
./Core/Src/stm32wbaxx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32wbaxx.o 

C_DEPS += \
./Core/Src/eeg_features.d \
./Core/Src/eeg_filter.d \
./Core/Src/eeg_model.d \
./Core/Src/eeg_window.d \
./Core/Src/main.d \
./Core/Src/stm32wbaxx_hal_msp.d \
./Core/Src/stm32wbaxx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32wbaxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM33 -DUSE_NUCLEO_64 -DUSE_HAL_DRIVER -DSTM32WBA65xx -c -I../Core/Inc -I"C:/Users/melvy/Documents/EPFL/2025-2026 MA1-MA2/MA2/Association/n-pulse/BMI-EMG-Firmware/test_folder/eeg_decoder_nucleo_wba65/Drivers/CMSIS/DSP/PrivateInclude" -I"C:/Users/melvy/Documents/EPFL/2025-2026 MA1-MA2/MA2/Association/n-pulse/BMI-EMG-Firmware/test_folder/eeg_decoder_nucleo_wba65/Drivers/CMSIS/DSP/Include" -I../Drivers/STM32WBAxx_HAL_Driver/Inc -I../Drivers/STM32WBAxx_HAL_Driver/Inc/Legacy -I../Drivers/BSP/STM32WBAxx_Nucleo -I../Drivers/CMSIS/Device/ST/STM32WBAxx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/eeg_features.cyclo ./Core/Src/eeg_features.d ./Core/Src/eeg_features.o ./Core/Src/eeg_features.su ./Core/Src/eeg_filter.cyclo ./Core/Src/eeg_filter.d ./Core/Src/eeg_filter.o ./Core/Src/eeg_filter.su ./Core/Src/eeg_model.cyclo ./Core/Src/eeg_model.d ./Core/Src/eeg_model.o ./Core/Src/eeg_model.su ./Core/Src/eeg_window.cyclo ./Core/Src/eeg_window.d ./Core/Src/eeg_window.o ./Core/Src/eeg_window.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32wbaxx_hal_msp.cyclo ./Core/Src/stm32wbaxx_hal_msp.d ./Core/Src/stm32wbaxx_hal_msp.o ./Core/Src/stm32wbaxx_hal_msp.su ./Core/Src/stm32wbaxx_it.cyclo ./Core/Src/stm32wbaxx_it.d ./Core/Src/stm32wbaxx_it.o ./Core/Src/stm32wbaxx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32wbaxx.cyclo ./Core/Src/system_stm32wbaxx.d ./Core/Src/system_stm32wbaxx.o ./Core/Src/system_stm32wbaxx.su

.PHONY: clean-Core-2f-Src

