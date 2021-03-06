cmake_minimum_required(VERSION 3.15.3)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME Generic)

# Set the project name
project(bare_metal_os)

# Set the language standards used
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set which source files to compile for the project
set(APP_SOURCES 		      
    # HAL++ source files
    source/HAL/hal_gpio.cpp
    source/HAL/hal_rcc.cpp
    source/HAL/hal_flash.cpp
    source/HAL/hal_power.cpp
    source/HAL/hal_usart.cpp
    source/HAL/hal_interrupt.cpp
    source/HAL/hal_spi.cpp
    source/HAL/hal_exti.cpp

    # Application Source Files
    source/Application/main.cpp
    source/Application/Peripherals/USART/debug_port.cpp
    source/Application/Peripherals/peripherals.cpp
    source/Application/Peripherals/SPI/lis3dsh.cpp

    # OS files
    source/OS/scheduler/scheduler.cpp
    source/OS/thread/thread_impl.cpp
    source/OS/system_clock/system_clock.cpp
    source/OS/interrupts.cpp
    source/OS/os.cpp
    source/OS/cm4_port.cpp


    # Start Up File
    startup/startup_stm32f407xx.s
    )

# Set the target to build
set(BINARY ${PROJECT_NAME}.elf)
add_executable(${BINARY} ${APP_SOURCES})

# Add compile time definitions
target_compile_definitions(${BINARY} PRIVATE
    -DMAX_THREAD_COUNT=8
    -DARM_MATH_CM4
    -DSTM32F40XX
    -DSTM32F407xx
    -D__FPU_PRESENT
    -D__FPU_USED
    )

# Set source include directories
target_include_directories(${BINARY} PRIVATE
    source
    source/HAL      
    source/Application
    source/Application/Accelerometer      
    source/Application/Audio
    source/Application/Debug
    source/Application/Utilities

    source/OS
    source/OS/scheduler
    source/OS/sempahore
    source/OS/thread
    source/OS/mutex
    source/OS/system_clock
    source/Utilities
    source/HW_Port

    source/Application/Peripherals			
    source/Application/Peripherals/USART
    source/Application/Peripherals/SPI
    )

# Set compiler flags - switch to -03 here for optimization*
target_compile_options(${BINARY} PRIVATE
    -std=c++17
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -fno-data-sections
    -fno-function-sections
    -fno-exceptions
    -O0
    -g3
    -Wall
    -Wextra
    -fmessage-length=0        
    )

# Set the linker options
target_link_options(${BINARY} PRIVATE
    -T${CMAKE_SOURCE_DIR}/STM32F407VgTx_FLASH.ld
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nosys.specs
    -lc
    -lm
    -lnosys
    -Wl,-Map=${PROJECT_NAME}.map,--cref
    -Wl,--gc-sections
    --no-exceptions
    )

# Print executable size
add_custom_command(TARGET ${BINARY}
    POST_BUILD
    COMMAND arm-none-eabi-size ${BINARY})

# Create hex file
add_custom_command(TARGET ${BINARY}
    POST_BUILD
    COMMAND arm-none-eabi-objcopy -O ihex ${BINARY} ${PROJECT_NAME}.hex
    COMMAND arm-none-eabi-objcopy -O binary ${BINARY} ${PROJECT_NAME}.bin
    )

# Create disassembly
add_custom_command( TARGET ${BINARY}
    POST_BUILD
    COMMAND arm-none-eabi-objdump -d ${BINARY} > ${PROJECT_NAME}_dasm.txt)
