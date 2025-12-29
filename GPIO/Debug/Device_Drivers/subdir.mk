################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Device_Drivers/gpio.c \
../Device_Drivers/irq_mgmt.c \

OBJS += \
./Device_Drivers/gpio.o \
./Device_Drivers/irq_mgmt.o \

# Each subdirectory must supply rules for building sources it contributes
Device_Drivers/%.o: ../Device_Drivers/%.c
	@echo 'Building file: $^'
	@echo 'Invoking: ARM-GCC C Compiler'
	$(CC) $(CFLAGS) -c -o $@ $^ 
	@echo 'Finished building: $^'

