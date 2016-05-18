################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Avion.cpp \
../src/BulletList.cpp \
../src/CargadorXML.cpp \
../src/Client.cpp \
../src/ClientList.cpp \
../src/DrawableObject.cpp \
../src/Escenario.cpp \
../src/GameManager.cpp \
../src/LogWriter.cpp \
../src/MenuPresenter.cpp \
../src/MessageBuilder.cpp \
../src/Object.cpp \
../src/Procesador.cpp \
../src/SocketManager.cpp \
../src/XmlParser.cpp \
../src/taller2016Server.cpp \
../src/tinystr.cpp \
../src/tinyxml.cpp \
../src/tinyxmlerror.cpp \
../src/tinyxmlparser.cpp 

OBJS += \
./src/Avion.o \
./src/BulletList.o \
./src/CargadorXML.o \
./src/Client.o \
./src/ClientList.o \
./src/DrawableObject.o \
./src/Escenario.o \
./src/GameManager.o \
./src/LogWriter.o \
./src/MenuPresenter.o \
./src/MessageBuilder.o \
./src/Object.o \
./src/Procesador.o \
./src/SocketManager.o \
./src/XmlParser.o \
./src/taller2016Server.o \
./src/tinystr.o \
./src/tinyxml.o \
./src/tinyxmlerror.o \
./src/tinyxmlparser.o 

CPP_DEPS += \
./src/Avion.d \
./src/BulletList.d \
./src/CargadorXML.d \
./src/Client.d \
./src/ClientList.d \
./src/DrawableObject.d \
./src/Escenario.d \
./src/GameManager.d \
./src/LogWriter.d \
./src/MenuPresenter.d \
./src/MessageBuilder.d \
./src/Object.d \
./src/Procesador.d \
./src/SocketManager.d \
./src/XmlParser.d \
./src/taller2016Server.d \
./src/tinystr.d \
./src/tinyxml.d \
./src/tinyxmlerror.d \
./src/tinyxmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/taller2016Server.o: ../src/taller2016Server.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/taller2016Server.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


