################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../fileupload.cpp \
../httpcli.cpp \
../httpparam.cpp \
../httpposttask.cpp \
../main.cpp \
../range.cpp \
../range_queue.cpp \
../upload.cpp \
../uploadmgr.cpp \
../uriparser.cpp \
../utility.cpp 

OBJS += \
./fileupload.o \
./httpcli.o \
./httpparam.o \
./httpposttask.o \
./main.o \
./range.o \
./range_queue.o \
./upload.o \
./uploadmgr.o \
./uriparser.o \
./utility.o 

CPP_DEPS += \
./fileupload.d \
./httpcli.d \
./httpparam.d \
./httpposttask.d \
./main.d \
./range.d \
./range_queue.d \
./upload.d \
./uploadmgr.d \
./uriparser.d \
./utility.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


