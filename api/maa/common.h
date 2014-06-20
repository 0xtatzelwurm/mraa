/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright © 2014 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#pragma once

#include "types.h"

/** @file
 *
 * This file defines the basic shared values for libmaa
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * MAA boolean type
 * 1 For TRUE
 */
typedef unsigned int maa_boolean_t;

/**
 * Enum representing different possible modes for a pin.
 */
typedef enum {
    MAA_PIN_VALID       = 0, /**< Pin Valid */
    MAA_PIN_GPIO        = 1, /**< General Purpose IO */
    MAA_PIN_PWM         = 2, /**< Pulse Width Modulation */
    MAA_PIN_FAST_GPIO   = 3, /**< Faster GPIO */
    MAA_PIN_SPI         = 4, /**< SPI */
    MAA_PIN_I2C         = 5, /**< I2C */
    MAA_PIN_AIO         = 6  /**< Analog in */
} maa_pinmodes_t;

/**
 * A bitfield representing the capabilities of a pin.
 */
typedef struct {
    /*@{*/
    maa_boolean_t valid:1;     /**< Is the pin valid at all */
    maa_boolean_t gpio:1;      /**< Is the pin gpio capable */
    maa_boolean_t pwm:1;       /**< Is the pin pwm capable */
    maa_boolean_t fast_gpio:1; /**< Is the pin fast gpio capable */
    maa_boolean_t spi:1;       /**< Is the pin spi capable */
    maa_boolean_t i2c:1;       /**< Is the pin i2c capable */
    maa_boolean_t aio:1;       /**< Is the pin analog input capable */
    /*@}*/
} maa_pincapabilities_t;

/**
 * A Structure representing a multiplexer and the required value
 */
typedef struct {
    /*@{*/
    unsigned int pin;   /**< Raw GPIO pin id */
    unsigned int value; /**< Raw GPIO value */
    /*@}*/
} maa_mux_t;

typedef struct {
    maa_boolean_t complex_pin:1;
    maa_boolean_t output_en:1;
    maa_boolean_t output_en_high:1;
    maa_boolean_t pullup_en:1;
    maa_boolean_t pullup_en_hiz:1;
} maa_pin_cap_complex_t;

typedef struct {
    /*@{*/
    unsigned int pinmap; /**< sysfs pin */
    unsigned int parent_id; /** parent chip id */
    unsigned int mux_total; /** Numfer of muxes needed for operation of pin */
    maa_mux_t mux[6]; /** Array holding information about mux */
    unsigned int output_enable; /** Output Enable GPIO, for level shifting */
    unsigned int pullup_enable; /** Pull-Up enable GPIO, inputs */
    maa_pin_cap_complex_t complex_cap;
    /*@}*/
} maa_pin_t;

typedef struct {
    /*@{*/
    char mem_dev[32]; /**< Memory device to use /dev/uio0 etc */
    unsigned int mem_sz; /** Size of memory to map */
    unsigned int bit_pos; /** Position of value bit */
    maa_pin_t gpio; /** GPio context containing none mmap info */
    /*@}*/
} maa_mmap_pin_t;

/**
 * A Structure representing a physical Pin.
 */
typedef struct {
    /*@{*/
    char name[8];                      /**< Pin's real world name */
    maa_pincapabilities_t capabilites; /**< Pin Capabiliites */
    maa_pin_t gpio; /**< GPIO structure */
    maa_pin_t pwm;  /**< PWM structure */
    maa_pin_t aio;  /**< Anaglog Pin */
    maa_mmap_pin_t mmap; /**< GPIO through memory */
    maa_pin_t i2c;  /**< i2c bus/pin */
    maa_pin_t spi;  /**< spi bus/pin */
    /*@}*/
} maa_pininfo_t;

/**
 * A Structure representing the physical properties of a i2c bus.
 */
typedef struct {
    /*@{*/
    unsigned int bus_id; /**< ID as exposed in the system */
    unsigned int scl; /**< i2c SCL */
    unsigned int sda; /**< i2c SDA */
    /*@}*/
} maa_i2c_bus_t;

/**
 * A Structure representing the physical properties of a spi bus.
 */
typedef struct {
    /*@{*/
    unsigned int bus_id; /**< The Bus ID as exposed to the system. */
    unsigned int slave_s; /**< Slave select */
    maa_boolean_t three_wire; /**< Is the bus only a three wire system */
    unsigned int sclk; /**< Serial Clock */
    unsigned int mosi; /**< Master Out, Slave In. */
    unsigned int miso; /**< Master In, Slave Out. */
    unsigned int cs; /**< Chip Select, used when the board is a spi slave */
    /*@}*/
} maa_spi_bus_t;

/**
 * A Structure representing a platform/board.
 */
typedef struct {
    /*@{*/
    unsigned int phy_pin_count; /**< The Total IO pins on board */
    unsigned int gpio_count; /**< GPIO Count */
    unsigned int aio_count;  /**< Analog side Count */
    unsigned int i2c_bus_count; /**< Usable i2c Count */
    maa_i2c_bus_t  i2c_bus[6]; /**< Array of i2c */
    unsigned int def_i2c_bus; /**< Position in array of default i2c bus */
    unsigned int spi_bus_count; /**< Usable spi Count */
    maa_spi_bus_t spi_bus[6];       /**< Array of spi */
    unsigned int def_spi_bus; /**< Position in array of defult spi bus */
    maa_pininfo_t* pins;     /**< Pointer to pin array */
    /*@}*/
} maa_board_t;

/**
 * Initialise MAA
 *
 * Detects running platform and attempts to use included pinmap
 *
 * @return Result of operation
 */
#ifndef SWIG
// this sets a compiler attribute (supported by GCC & clang) to have maa_init()
// be called as a constructor make sure your libc supports this!  uclibc needs
// to be compiled with UCLIBC_CTOR_DTOR
maa_result_t maa_init() __attribute__((constructor));
#else
maa_result_t maa_init();
#endif

/**
 * Checks if a pin is able to use the passed in mode.
 *
 * @param pin Physical Pin to be checked.
 * @param mode the mode to be tested.
 * @return boolean if the mode is supported, 0=false.
 */
maa_boolean_t maa_pin_mode_test(int pin, maa_pinmodes_t mode);

#ifdef __cplusplus
}
#endif
