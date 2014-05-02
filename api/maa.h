/*
 * Author: Brendan Le Foll
 *
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

/** @file
 *
 * This file defines the basic shared values for libmaa
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * MAA return codes
 */
typedef enum {
    MAA_SUCCESS                              =  0, /**< Expected response */
    MAA_ERROR_FEATURE_NOT_IMPLEMENTED        =  1, /**< Feature TODO */
    MAA_ERROR_FEATURE_NOT_SUPPORTED          =  2, /**< Feature not supported by HW */
    MAA_ERROR_INVALID_VERBOSITY_LEVEL        =  3, /**< Verbosity level wrong */
    MAA_ERROR_INVALID_PARAMETER              =  4, /**< Parameter invalid */
    MAA_ERROR_INVALID_HANDLE                 =  5, /**< Handle invalid */
    MAA_ERROR_NO_RESOURCES                   =  6, /**< No resource of that type avail */
    MAA_ERROR_INVALID_RESOURCE               =  7, /**< Resource invalid */
    MAA_ERROR_INVALID_QUEUE_TYPE             =  8, /**< Queue type incorrect */
    MAA_ERROR_NO_DATA_AVAILABLE              =  9, /**< No data available */
    MAA_ERROR_INVALID_PLATFORM               = 10, /**< Platform not recognised */
    MAA_ERROR_PLATFORM_NOT_INITIALISED       = 11, /**< Board information not initialised */

    MAA_ERROR_UNSPECIFIED                    = 99 /**< Unknown Error */
} maa_result_t;

/**
 * MAA boolean type
 * 1 For TRUE
 */
typedef unsigned int maa_boolean_t;

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

/**
 * A Structure representing a physical Pin.
 */
typedef struct {
    /*@{*/
    char name[8];                      /**< Pin's real world name */
    unsigned int pin;                  /**< Pin ID */
    int parent_id;                     /**< IO Parent ID*/
    maa_pincapabilities_t capabilites; /**< Pin Capabiliites */
    maa_mux_t mux[4];                  /**< Multiplexer array */
    unsigned int mux_total;            /**< Total Multiplexors required */
    /*@}*/
} maa_pininfo_t;

/**
 * A Structure representing a platform/board.
 */
typedef struct {
    /*@{*/
    unsigned int gpio_count; /**< GPIO Count */
    unsigned int aio_count;  /**< Analog In Count */
    unsigned int pwm_count;  /**< PWM Count */
    unsigned int i2c_bus_count; /**< Usable i2c Count */
    unsigned int i2c_bus[8]; /**< Array of i2c */
    unsigned int spi_bus_count; /**< Usable spi Count */
    double spi_bus[8];       /**< Array of spi */
    maa_pininfo_t* pins;     /**< Pointer to pin array */
    /*@}*/
} maa_board_t;

/** Initialise MAA
 *
 * Detects running platform and attempts to use included pinmap
 * @return maa_result_t maa result
 */
maa_result_t maa_init();

/** Check GPIO
 *
 * Will check input is valid for gpio and will also setup required multiplexers.
 * @param pin the pin as read from the board surface. i.e IO3 would be 3/
 * @return the pin as found in the pinmap
 */
unsigned int maa_check_gpio(int pin);

/** Check AIO
 *
 * Will check input is valid for aio and will also setup required multiplexers.
 * @param pin the pin as read from the board surface. i.e A3 would be 3/
 * @return the pin as found in the pinmap
 */
unsigned int maa_check_aio(int pin);

/** Check i2c interface, sets up multiplexer on device.
 *
 * @return unsigned int if using /dev/i2c-2 returned would be 2
 */
unsigned int maa_check_i2c();

/** Get the version string of maa autogenerated from git tag
 *
 * The version returned may not be what is expected however it is a reliable
 * number associated with the git tag closest to that version at build time
 * @return version string from version.h
 */
const char* maa_get_version();

#ifdef __cplusplus
}
#endif
