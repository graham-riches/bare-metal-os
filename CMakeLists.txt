cmake_minimum_required(VERSION 3.15.3)

project(stm32_discovery)

enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


set(APP_SOURCES 
		# Standard Peripheral Library Files
		Drivers/STM32F4xx_StdPeriph_Driver/src/misc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_adc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_can.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cec.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_crc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_aes.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_des.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_cryp_tdes.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dac.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dbgmcu.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dcmi.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dfsdm.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dma2d.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_dsi.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_exti.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_flash_ramfunc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fmpi2c.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_fsmc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_gpio.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_md5.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_hash_sha1.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_i2c.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_iwdg.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_lptim.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_ltdc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_pwr.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_qspi.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rcc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rng.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_rtc.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sai.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_sdio.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spdifrx.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_spi.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_syscfg.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_tim.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_usart.c
		Drivers/STM32F4xx_StdPeriph_Driver/src/stm32f4xx_wwdg.c
		
		# Application Source Files
		Src/APP/main.cpp
		Src/APP/Accelerometer/accelerometer.c
		Src/APP/Audio/audioin.c
		Src/APP/Debug/command_line.c
		Src/APP/Debug/debug.c
		Src/APP/Events/event.c
		Src/APP/Utilities/utilities.c
		Src/HWM/ADC/adc.c
		Src/HWM/DMA/dma.c
		Src/HWM/GPIO/gpio.c
		Src/HWM/HAL/hal.c
		Src/HWM/HAL/stm32f4xx_it.c
		Src/HWM/HAL/system_stm32f4xx.c
		Src/HWM/I2C/i2c.c
		Src/HWM/I2S/i2s.c
		Src/HWM/SPI/spi.c
		Src/HWM/System/system.c
		Src/HWM/USART/usart.c
		
		# Start Up File
        startup/startup_stm32f407xx.s)

set(BINARY ${PROJECT_NAME}.out)

add_executable(${BINARY} ${APP_SOURCES})

target_compile_definitions(${BINARY} PRIVATE
		-DARM_MATH_CM4
		-D__FPU_PRESENT
		-DSTM32F40XX
		-DSTM32F407xx
		-DUSE_STDPERIPH_DRIVER        
        )

target_include_directories(${BINARY} PRIVATE
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
		Drivers/CMSIS/Include
		Drivers/CMSIS/DSP_Lib/Include
		Drivers/STM32F4xx_StdPeriph_Driver/inc
		
		Src
		Src/APP
		Src/APP/Accelerometer
		Src/APP/Audio
		Src/APP/Debug
		Src/APP/Events
		Src/APP/Utilities
		
		Src/HWM
		Src/HWM/ADC
		Src/HWM/DMA
		Src/HWM/GPIO
		Src/HWM/HAL
		Src/HWM/I2C
		Src/HWM/I2S
		Src/HWM/SPI
		Src/HWM/System
		Src/HWM/USART
        )

target_compile_options(${BINARY} PRIVATE
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard

        -fdata-sections
        -ffunction-sections
		-Og
		-g3
        -Wall
		-Wextra
		-Wconversion
		-fmessage-length=0        
        )

target_link_options(${BINARY} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F407VgTx_FLASH.ld
        -mcpu=cortex-m4
        -mthumb
        -mfpu=fpv4-sp-d16
        -mfloat-abi=hard
        -specs=nano.specs
        -lc
        -lm
        -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref
        -Wl,--gc-sections
        )

# Print executable size
add_custom_command(TARGET ${BINARY}
        POST_BUILD
        COMMAND arm-none-eabi-size ${BINARY})

# Create hex file
add_custom_command(TARGET ${BINARY}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${BINARY} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${BINARY} ${PROJECT_NAME}.bin)