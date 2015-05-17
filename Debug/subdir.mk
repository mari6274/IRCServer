################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Channel.cpp \
../Client.cpp \
../IrcProtocolParser.cpp \
../Server.cpp \
../main.cpp 

OBJS += \
./Channel.o \
./Client.o \
./IrcProtocolParser.o \
./Server.o \
./main.o 

CPP_DEPS += \
./Channel.d \
./Client.d \
./IrcProtocolParser.d \
./Server.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


