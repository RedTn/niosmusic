/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'nios2_processor' in SOPC Builder design 'Scorched_Earth_Hardware_System'
 * SOPC Builder design path: C:/Users/red/Desktop/ubc/381/module2/Scorched_Earth_Hardware_System.sopcinfo
 *
 * Generated: Sun Mar 24 22:14:57 EDT 2013
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_qsys"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x1820
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x19
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x1000020
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 1
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x19
#define ALT_CPU_NAME "nios2_processor"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_RESET_ADDR 0x1000000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x1820
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x19
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x1000020
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 1
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x19
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_RESET_ADDR 0x1000000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_QSYS
#define __ALTERA_UP_AVALON_AUDIO
#define __ALTERA_UP_AVALON_AUDIO_AND_VIDEO_CONFIG
#define __ALTERA_UP_AVALON_RS232
#define __ALTERA_UP_AVALON_SRAM
#define __ALTERA_UP_AVALON_VIDEO_CHARACTER_BUFFER_WITH_DMA
#define __ALTERA_UP_AVALON_VIDEO_PIXEL_BUFFER_DMA
#define __ALTERA_UP_SD_CARD_AVALON_INTERFACE


/*
 * Interface configuration
 *
 */

#define ALT_MODULE_CLASS_Interface Altera_UP_SD_Card_Avalon_Interface
#define INTERFACE_BASE 0x2000
#define INTERFACE_IRQ -1
#define INTERFACE_IRQ_INTERRUPT_CONTROLLER_ID -1
#define INTERFACE_NAME "/dev/Interface"
#define INTERFACE_SPAN 1024
#define INTERFACE_TYPE "Altera_UP_SD_Card_Avalon_Interface"


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone II"
#define ALT_IRQ_BASE NULL
#define ALT_LEGACY_INTERRUPT_API_PRESENT
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart"
#define ALT_STDERR_BASE 0x24b0
#define ALT_STDERR_DEV jtag_uart
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart"
#define ALT_STDIN_BASE 0x24b0
#define ALT_STDIN_DEV jtag_uart
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart"
#define ALT_STDOUT_BASE 0x24b0
#define ALT_STDOUT_DEV jtag_uart
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "Scorched_Earth_Hardware_System"


/*
 * audio configuration
 *
 */

#define ALT_MODULE_CLASS_audio altera_up_avalon_audio
#define AUDIO_BASE 0x24a0
#define AUDIO_IRQ 2
#define AUDIO_IRQ_INTERRUPT_CONTROLLER_ID 0
#define AUDIO_NAME "/dev/audio"
#define AUDIO_SPAN 16
#define AUDIO_TYPE "altera_up_avalon_audio"


/*
 * audio_and_video_config configuration
 *
 */

#define ALT_MODULE_CLASS_audio_and_video_config altera_up_avalon_audio_and_video_config
#define AUDIO_AND_VIDEO_CONFIG_BASE 0x2490
#define AUDIO_AND_VIDEO_CONFIG_IRQ -1
#define AUDIO_AND_VIDEO_CONFIG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define AUDIO_AND_VIDEO_CONFIG_NAME "/dev/audio_and_video_config"
#define AUDIO_AND_VIDEO_CONFIG_SPAN 16
#define AUDIO_AND_VIDEO_CONFIG_TYPE "altera_up_avalon_audio_and_video_config"


/*
 * char_drawer_avalon_char_buffer_slave configuration
 *
 */

#define ALT_MODULE_CLASS_char_drawer_avalon_char_buffer_slave altera_up_avalon_video_character_buffer_with_dma
#define CHAR_DRAWER_AVALON_CHAR_BUFFER_SLAVE_BASE 0x4000
#define CHAR_DRAWER_AVALON_CHAR_BUFFER_SLAVE_IRQ -1
#define CHAR_DRAWER_AVALON_CHAR_BUFFER_SLAVE_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CHAR_DRAWER_AVALON_CHAR_BUFFER_SLAVE_NAME "/dev/char_drawer_avalon_char_buffer_slave"
#define CHAR_DRAWER_AVALON_CHAR_BUFFER_SLAVE_SPAN 8192
#define CHAR_DRAWER_AVALON_CHAR_BUFFER_SLAVE_TYPE "altera_up_avalon_video_character_buffer_with_dma"


/*
 * char_drawer_avalon_char_control_slave configuration
 *
 */

#define ALT_MODULE_CLASS_char_drawer_avalon_char_control_slave altera_up_avalon_video_character_buffer_with_dma
#define CHAR_DRAWER_AVALON_CHAR_CONTROL_SLAVE_BASE 0x24b8
#define CHAR_DRAWER_AVALON_CHAR_CONTROL_SLAVE_IRQ -1
#define CHAR_DRAWER_AVALON_CHAR_CONTROL_SLAVE_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CHAR_DRAWER_AVALON_CHAR_CONTROL_SLAVE_NAME "/dev/char_drawer_avalon_char_control_slave"
#define CHAR_DRAWER_AVALON_CHAR_CONTROL_SLAVE_SPAN 8
#define CHAR_DRAWER_AVALON_CHAR_CONTROL_SLAVE_TYPE "altera_up_avalon_video_character_buffer_with_dma"


/*
 * hal configuration
 *
 */

#define ALT_MAX_FD 32
#define ALT_SYS_CLK SYSTEM_CLOCK_TIMER
#define ALT_TIMESTAMP_CLK TIMESTAMP_TIMER


/*
 * jtag_uart configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart altera_avalon_jtag_uart
#define JTAG_UART_BASE 0x24b0
#define JTAG_UART_IRQ 5
#define JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_NAME "/dev/jtag_uart"
#define JTAG_UART_READ_DEPTH 64
#define JTAG_UART_READ_THRESHOLD 8
#define JTAG_UART_SPAN 8
#define JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_WRITE_DEPTH 64
#define JTAG_UART_WRITE_THRESHOLD 8


/*
 * keys configuration
 *
 */

#define ALT_MODULE_CLASS_keys altera_avalon_pio
#define KEYS_BASE 0x2470
#define KEYS_BIT_CLEARING_EDGE_REGISTER 0
#define KEYS_BIT_MODIFYING_OUTPUT_REGISTER 0
#define KEYS_CAPTURE 0
#define KEYS_DATA_WIDTH 4
#define KEYS_DO_TEST_BENCH_WIRING 0
#define KEYS_DRIVEN_SIM_VALUE 0x0
#define KEYS_EDGE_TYPE "NONE"
#define KEYS_FREQ 50000000u
#define KEYS_HAS_IN 1
#define KEYS_HAS_OUT 0
#define KEYS_HAS_TRI 0
#define KEYS_IRQ -1
#define KEYS_IRQ_INTERRUPT_CONTROLLER_ID -1
#define KEYS_IRQ_TYPE "NONE"
#define KEYS_NAME "/dev/keys"
#define KEYS_RESET_VALUE 0x0
#define KEYS_SPAN 16
#define KEYS_TYPE "altera_avalon_pio"


/*
 * onchip_memory configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory altera_avalon_onchip_memory2
#define ONCHIP_MEMORY_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY_BASE 0x0
#define ONCHIP_MEMORY_CONTENTS_INFO ""
#define ONCHIP_MEMORY_DUAL_PORT 0
#define ONCHIP_MEMORY_GUI_RAM_BLOCK_TYPE "Automatic"
#define ONCHIP_MEMORY_INIT_CONTENTS_FILE "onchip_memory"
#define ONCHIP_MEMORY_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY_IRQ -1
#define ONCHIP_MEMORY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY_NAME "/dev/onchip_memory"
#define ONCHIP_MEMORY_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY_RAM_BLOCK_TYPE "Auto"
#define ONCHIP_MEMORY_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY_SIZE_VALUE 4096u
#define ONCHIP_MEMORY_SPAN 4096
#define ONCHIP_MEMORY_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY_WRITABLE 1


/*
 * pixel_buffer configuration
 *
 */

#define ALT_MODULE_CLASS_pixel_buffer altera_up_avalon_sram
#define PIXEL_BUFFER_BASE 0x80000
#define PIXEL_BUFFER_IRQ -1
#define PIXEL_BUFFER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_BUFFER_NAME "/dev/pixel_buffer"
#define PIXEL_BUFFER_SPAN 524288
#define PIXEL_BUFFER_TYPE "altera_up_avalon_sram"


/*
 * pixel_buffer_dma configuration
 *
 */

#define ALT_MODULE_CLASS_pixel_buffer_dma altera_up_avalon_video_pixel_buffer_dma
#define PIXEL_BUFFER_DMA_BASE 0x2460
#define PIXEL_BUFFER_DMA_IRQ -1
#define PIXEL_BUFFER_DMA_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIXEL_BUFFER_DMA_NAME "/dev/pixel_buffer_dma"
#define PIXEL_BUFFER_DMA_SPAN 16
#define PIXEL_BUFFER_DMA_TYPE "altera_up_avalon_video_pixel_buffer_dma"


/*
 * psController configuration
 *
 */

#define ALT_MODULE_CLASS_psController altera_avalon_pio
#define PSCONTROLLER_BASE 0x2500
#define PSCONTROLLER_BIT_CLEARING_EDGE_REGISTER 0
#define PSCONTROLLER_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PSCONTROLLER_CAPTURE 0
#define PSCONTROLLER_DATA_WIDTH 16
#define PSCONTROLLER_DO_TEST_BENCH_WIRING 0
#define PSCONTROLLER_DRIVEN_SIM_VALUE 0x0
#define PSCONTROLLER_EDGE_TYPE "NONE"
#define PSCONTROLLER_FREQ 50000000u
#define PSCONTROLLER_HAS_IN 1
#define PSCONTROLLER_HAS_OUT 0
#define PSCONTROLLER_HAS_TRI 0
#define PSCONTROLLER_IRQ -1
#define PSCONTROLLER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PSCONTROLLER_IRQ_TYPE "NONE"
#define PSCONTROLLER_NAME "/dev/psController"
#define PSCONTROLLER_RESET_VALUE 0x0
#define PSCONTROLLER_SPAN 16
#define PSCONTROLLER_TYPE "altera_avalon_pio"


/*
 * rs232 configuration
 *
 */

#define ALT_MODULE_CLASS_rs232 altera_up_avalon_rs232
#define RS232_BASE 0x24c0
#define RS232_IRQ 3
#define RS232_IRQ_INTERRUPT_CONTROLLER_ID 0
#define RS232_NAME "/dev/rs232"
#define RS232_SPAN 8
#define RS232_TYPE "altera_up_avalon_rs232"


/*
 * sdram configuration
 *
 */

#define ALT_MODULE_CLASS_sdram altera_avalon_new_sdram_controller
#define SDRAM_BASE 0x1000000
#define SDRAM_CAS_LATENCY 3
#define SDRAM_CONTENTS_INFO ""
#define SDRAM_INIT_NOP_DELAY 0.0
#define SDRAM_INIT_REFRESH_COMMANDS 2
#define SDRAM_IRQ -1
#define SDRAM_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_IS_INITIALIZED 1
#define SDRAM_NAME "/dev/sdram"
#define SDRAM_POWERUP_DELAY 100.0
#define SDRAM_REFRESH_PERIOD 15.625
#define SDRAM_REGISTER_DATA_IN 1
#define SDRAM_SDRAM_ADDR_WIDTH 0x16
#define SDRAM_SDRAM_BANK_WIDTH 2
#define SDRAM_SDRAM_COL_WIDTH 8
#define SDRAM_SDRAM_DATA_WIDTH 16
#define SDRAM_SDRAM_NUM_BANKS 4
#define SDRAM_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_SDRAM_ROW_WIDTH 12
#define SDRAM_SHARED_DATA 0
#define SDRAM_SIM_MODEL_BASE 0
#define SDRAM_SPAN 8388608
#define SDRAM_STARVATION_INDICATOR 0
#define SDRAM_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_T_AC 5.5
#define SDRAM_T_MRD 3
#define SDRAM_T_RCD 20.0
#define SDRAM_T_RFC 70.0
#define SDRAM_T_RP 20.0
#define SDRAM_T_WR 14.0


/*
 * switches configuration
 *
 */

#define ALT_MODULE_CLASS_switches altera_avalon_pio
#define SWITCHES_BASE 0x2480
#define SWITCHES_BIT_CLEARING_EDGE_REGISTER 0
#define SWITCHES_BIT_MODIFYING_OUTPUT_REGISTER 0
#define SWITCHES_CAPTURE 0
#define SWITCHES_DATA_WIDTH 8
#define SWITCHES_DO_TEST_BENCH_WIRING 0
#define SWITCHES_DRIVEN_SIM_VALUE 0x0
#define SWITCHES_EDGE_TYPE "NONE"
#define SWITCHES_FREQ 50000000u
#define SWITCHES_HAS_IN 1
#define SWITCHES_HAS_OUT 0
#define SWITCHES_HAS_TRI 0
#define SWITCHES_IRQ -1
#define SWITCHES_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SWITCHES_IRQ_TYPE "NONE"
#define SWITCHES_NAME "/dev/switches"
#define SWITCHES_RESET_VALUE 0x0
#define SWITCHES_SPAN 16
#define SWITCHES_TYPE "altera_avalon_pio"


/*
 * system_clock_timer configuration
 *
 */

#define ALT_MODULE_CLASS_system_clock_timer altera_avalon_timer
#define SYSTEM_CLOCK_TIMER_ALWAYS_RUN 0
#define SYSTEM_CLOCK_TIMER_BASE 0x2400
#define SYSTEM_CLOCK_TIMER_COUNTER_SIZE 32
#define SYSTEM_CLOCK_TIMER_FIXED_PERIOD 0
#define SYSTEM_CLOCK_TIMER_FREQ 50000000u
#define SYSTEM_CLOCK_TIMER_IRQ 0
#define SYSTEM_CLOCK_TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SYSTEM_CLOCK_TIMER_LOAD_VALUE 49999ull
#define SYSTEM_CLOCK_TIMER_MULT 0.0010
#define SYSTEM_CLOCK_TIMER_NAME "/dev/system_clock_timer"
#define SYSTEM_CLOCK_TIMER_PERIOD 1
#define SYSTEM_CLOCK_TIMER_PERIOD_UNITS "ms"
#define SYSTEM_CLOCK_TIMER_RESET_OUTPUT 0
#define SYSTEM_CLOCK_TIMER_SNAPSHOT 1
#define SYSTEM_CLOCK_TIMER_SPAN 32
#define SYSTEM_CLOCK_TIMER_TICKS_PER_SEC 1000u
#define SYSTEM_CLOCK_TIMER_TIMEOUT_PULSE_OUTPUT 0
#define SYSTEM_CLOCK_TIMER_TYPE "altera_avalon_timer"


/*
 * timer_0 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_0 altera_avalon_timer
#define TIMER_0_ALWAYS_RUN 0
#define TIMER_0_BASE 0x2440
#define TIMER_0_COUNTER_SIZE 32
#define TIMER_0_FIXED_PERIOD 0
#define TIMER_0_FREQ 50000000u
#define TIMER_0_IRQ 4
#define TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_0_LOAD_VALUE 49999ull
#define TIMER_0_MULT 0.0010
#define TIMER_0_NAME "/dev/timer_0"
#define TIMER_0_PERIOD 1
#define TIMER_0_PERIOD_UNITS "ms"
#define TIMER_0_RESET_OUTPUT 0
#define TIMER_0_SNAPSHOT 1
#define TIMER_0_SPAN 32
#define TIMER_0_TICKS_PER_SEC 1000u
#define TIMER_0_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_0_TYPE "altera_avalon_timer"


/*
 * timestamp_timer configuration
 *
 */

#define ALT_MODULE_CLASS_timestamp_timer altera_avalon_timer
#define TIMESTAMP_TIMER_ALWAYS_RUN 0
#define TIMESTAMP_TIMER_BASE 0x2420
#define TIMESTAMP_TIMER_COUNTER_SIZE 32
#define TIMESTAMP_TIMER_FIXED_PERIOD 0
#define TIMESTAMP_TIMER_FREQ 50000000u
#define TIMESTAMP_TIMER_IRQ 1
#define TIMESTAMP_TIMER_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMESTAMP_TIMER_LOAD_VALUE 49999ull
#define TIMESTAMP_TIMER_MULT 0.0010
#define TIMESTAMP_TIMER_NAME "/dev/timestamp_timer"
#define TIMESTAMP_TIMER_PERIOD 1
#define TIMESTAMP_TIMER_PERIOD_UNITS "ms"
#define TIMESTAMP_TIMER_RESET_OUTPUT 0
#define TIMESTAMP_TIMER_SNAPSHOT 1
#define TIMESTAMP_TIMER_SPAN 32
#define TIMESTAMP_TIMER_TICKS_PER_SEC 1000u
#define TIMESTAMP_TIMER_TIMEOUT_PULSE_OUTPUT 0
#define TIMESTAMP_TIMER_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */
