################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ClientAttendant.cpp \
../src/ClientMessage.cpp \
../src/taller2016Server.cpp 

OBJS += \
./src/ClientAttendant.o \
./src/ClientMessage.o \
./src/taller2016Server.o 

CPP_DEPS += \
./src/ClientAttendant.d \
./src/ClientMessage.d \
./src/taller2016Server.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/taller2016Server.o: ../src/taller2016Server.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/taller2016Server.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


