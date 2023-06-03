################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/dma.c \
../Core/Src/ecph.c \
../Core/Src/gpio.c \
../Core/Src/lights.c \
../Core/Src/main.c \
../Core/Src/stepper.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/utils.c \
../Core/Src/virtual_main.c \
../Core/Src/water_level_sensor.c 

OBJS += \
./Core/Src/adc.o \
./Core/Src/dma.o \
./Core/Src/ecph.o \
./Core/Src/gpio.o \
./Core/Src/lights.o \
./Core/Src/main.o \
./Core/Src/stepper.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/utils.o \
./Core/Src/virtual_main.o \
./Core/Src/water_level_sensor.o 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/dma.d \
./Core/Src/ecph.d \
./Core/Src/gpio.d \
./Core/Src/lights.d \
./Core/Src/main.d \
./Core/Src/stepper.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/utils.d \
./Core/Src/virtual_main.d \
./Core/Src/water_level_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I/home/envy0/STM32Cube/Repository/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Inc -I/home/envy0/STM32Cube/Repository/STM32Cube_FW_L4_V1.17.2/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I/home/envy0/STM32Cube/Repository/STM32Cube_FW_L4_V1.17.2/Drivers/CMSIS/Device/ST/STM32L4xx/Include -I/home/envy0/STM32Cube/Repository/STM32Cube_FW_L4_V1.17.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/ecph.cyclo ./Core/Src/ecph.d ./Core/Src/ecph.o ./Core/Src/ecph.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/lights.cyclo ./Core/Src/lights.d ./Core/Src/lights.o ./Core/Src/lights.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stepper.cyclo ./Core/Src/stepper.d ./Core/Src/stepper.o ./Core/Src/stepper.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/utils.cyclo ./Core/Src/utils.d ./Core/Src/utils.o ./Core/Src/utils.su ./Core/Src/virtual_main.cyclo ./Core/Src/virtual_main.d ./Core/Src/virtual_main.o ./Core/Src/virtual_main.su ./Core/Src/water_level_sensor.cyclo ./Core/Src/water_level_sensor.d ./Core/Src/water_level_sensor.o ./Core/Src/water_level_sensor.su

.PHONY: clean-Core-2f-Src

