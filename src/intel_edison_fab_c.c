/*
 * Author: Thomas Ingleby <thomas.c.ingleby@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "intel_edison_fab_c.h"

#define SYSFS_PINMODE_PATH "/sys/kernel/debug/gpio_debug/gpio"
#define MAX_SIZE 64
#define MAX_MODE_SIZE 8

typedef struct {
    int sysfs;
    int mode;
} mraa_intel_edision_pindef_t;

typedef struct {
    mraa_intel_edision_pindef_t gpio;
    mraa_intel_edision_pindef_t pwm;
    mraa_intel_edision_pindef_t i2c;
    mraa_intel_edision_pindef_t spi;
    mraa_intel_edision_pindef_t uart;
} mraa_intel_edison_pinmodes_t;

static mraa_intel_edison_pinmodes_t pinmodes[MRAA_INTEL_EDISON_PINCOUNT];
static unsigned int outputen[] = {248,249,250,251,252,253,254,255,256,257,258,259,260,261,232,233,234,235,236,237};

static mraa_result_t
mraa_intel_edison_pinmode_change(int sysfs, int mode)
{
    if (mode < 0 )

    printf("//EDISON// PINMODE CHANGE - gpio%i changing to mode -%u", sysfs, mode);

    char buffer[MAX_SIZE];
    snprintf(buffer, MAX_SIZE, SYSFS_PINMODE_PATH "%i/current_pinmux",sysfs);
    int modef = open(buffer, O_WRONLY);
    if (modef == -1) {
        fprintf(stderr, "Failed to open SoC pinmode for opening\n");
        return MRAA_ERROR_INVALID_RESOURCE;
    }

    char mode_buf[MAX_MODE_SIZE];
    int length = sprintf(mode_buf, "mode%u",mode);
    printf("//EDISON// PIMODE = %s", mode_buf);
    if (write(modef, mode_buf, length*sizeof(char)) == -1) {
        return MRAA_ERROR_INVALID_RESOURCE;
    }
    close(modef);

    return MRAA_SUCCESS;
}

mraa_result_t
mraa_intel_edison_gpio_dir_pre(mraa_gpio_context dev, gpio_dir_t dir)
{
    if (dev->phy_pin >= 0) {
        int pin = dev->phy_pin;

        mraa_gpio_context output_e;
        output_e = mraa_gpio_init_raw(outputen[pin]);
        if (mraa_gpio_dir(output_e, MRAA_GPIO_OUT) != MRAA_SUCCESS)
            return MRAA_ERROR_INVALID_RESOURCE;
        int output_val = 0;
        if (dir == MRAA_GPIO_OUT)
            output_val = 1;
        if (mraa_gpio_write(output_e, output_val) != MRAA_SUCCESS)
            return MRAA_ERROR_INVALID_RESOURCE;
    }
    return MRAA_SUCCESS;
}

mraa_result_t
mraa_intel_edison_gpio_init_post(mraa_gpio_context dev)
{
    if (dev == NULL)
        return MRAA_ERROR_INVALID_RESOURCE;

    int sysfs = pinmodes[dev->phy_pin].gpio.sysfs;
    int mode = pinmodes[dev->phy_pin].gpio.mode;
    mraa_result_t ret = mraa_intel_edison_pinmode_change(sysfs, mode);
    if (ret != MRAA_SUCCESS);
        return ret;
    return MRAA_SUCCESS;
}

mraa_board_t*
mraa_intel_edison_fab_c()
{
    mraa_board_t* b = (mraa_board_t*) malloc(sizeof(mraa_board_t));
    if (b == NULL)
        return NULL;

    b->phy_pin_count = 20;
    b->gpio_count = 14;
    b->aio_count = 6;

    advance_func->gpio_dir_pre = &mraa_intel_edison_gpio_dir_pre;
    advance_func->gpio_init_post = &mraa_intel_edison_gpio_init_post;

    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t)*MRAA_INTEL_EDISON_PINCOUNT);

    strncpy(b->pins[4].name, "IO4", 8);
    b->pins[4].capabilites = (mraa_pincapabilities_t) {1,1,0,0,0,0,0};
    b->pins[4].gpio.pinmap = 129;
    b->pins[4].gpio.parent_id = 0;
    b->pins[4].gpio.mux_total = 0;

    strncpy(b->pins[5].name, "IO5", 8);
    b->pins[5].capabilites = (mraa_pincapabilities_t) {1,1,0,0,0,0,0};
    b->pins[5].gpio.pinmap = 13;
    b->pins[5].gpio.parent_id = 0;
    b->pins[5].gpio.mux_total = 0;

    //BUS DEFINITIONS
    b->i2c_bus_count = 1;
    b->def_i2c_bus = 0;
    b->i2c_bus[0].bus_id = 0;
    b->i2c_bus[0].sda = 18;
    b->i2c_bus[0].scl = 19;

    b->spi_bus_count = 1;
    b->def_spi_bus = 0;
    b->spi_bus[0].bus_id = 1;
    b->spi_bus[0].slave_s = 0;
    b->spi_bus[0].cs = 10;
    b->spi_bus[0].mosi = 11;
    b->spi_bus[0].miso = 12;
    b->spi_bus[0].sclk = 13;

    int il;

    for(il =0; il < MRAA_INTEL_EDISON_PINCOUNT; il++) {
        pinmodes[il].gpio.sysfs = -1;
        pinmodes[il].gpio.mode = -1;
        pinmodes[il].pwm.sysfs = -1;
        pinmodes[il].pwm.mode = -1;
        pinmodes[il].i2c.sysfs = -1;
        pinmodes[il].i2c.mode = -1;
        pinmodes[il].spi.sysfs = -1;
        pinmodes[il].spi.mode = -1;
        pinmodes[il].uart.sysfs = -1;
        pinmodes[il].uart.mode = -1;
    }
    pinmodes[0].gpio.sysfs = 130;
    pinmodes[0].gpio.mode = 0;
    pinmodes[0].uart.sysfs = 130;
    pinmodes[0].uart.mode = 1;
    pinmodes[1].gpio.sysfs = 131;
    pinmodes[1].gpio.mode = 0;
    pinmodes[1].uart.sysfs = 131;
    pinmodes[1].uart.mode = 1;
    pinmodes[2].gpio.sysfs = 128;
    pinmodes[2].gpio.mode = 0;
    pinmodes[2].uart.sysfs = 128;
    pinmodes[2].uart.mode = 1;
    pinmodes[3].gpio.sysfs = 12;
    pinmodes[3].gpio.mode = 0;
    pinmodes[3].pwm.sysfs = 12;
    pinmodes[3].pwm.mode = 1;

    pinmodes[4].gpio.sysfs = 129;
    pinmodes[4].gpio.mode = 0;
    pinmodes[4].uart.sysfs = 129;
    pinmodes[4].uart.mode = 1;
    pinmodes[5].gpio.sysfs = 13;
    pinmodes[5].gpio.mode = 0;
    pinmodes[5].pwm.sysfs = 13;
    pinmodes[5].pwm.mode = 1;
    pinmodes[6].gpio.sysfs = 182;
    pinmodes[6].gpio.mode = 0;
    pinmodes[6].pwm.sysfs = 182;
    pinmodes[6].pwm.mode = 1;

    //7 and 8 are provided by something on i2c, very simplepinmodes[3].gpio.sysfs = 12;
    pinmodes[9].gpio.sysfs = 183;
    pinmodes[9].gpio.mode = 0;
    pinmodes[9].pwm.sysfs = 183;
    pinmodes[9].pwm.mode = 1;

    pinmodes[10].gpio.sysfs = 41;
    pinmodes[10].gpio.mode = 0;
    pinmodes[10].spi.sysfs = 111; // Different pin provides, switched at mux level.
    pinmodes[10].spi.mode = 1;

    pinmodes[11].gpio.sysfs = 43;
    pinmodes[11].gpio.mode = 0;
    pinmodes[11].spi.sysfs = 115; // Different pin provides, switched at mux level.
    pinmodes[11].spi.mode = 1;

    pinmodes[12].gpio.sysfs = 42;
    pinmodes[12].gpio.mode = 0;
    pinmodes[12].spi.sysfs = 114; // Different pin provides, switched at mux level.
    pinmodes[12].spi.mode = 1;

    pinmodes[13].gpio.sysfs = 40;
    pinmodes[13].gpio.mode = 0;
    pinmodes[13].spi.sysfs = 109; // Different pin provides, switched at mux level.
    pinmodes[13].spi.mode = 1;
    //Everything else but A4 A5 LEAVE
    pinmodes[18].gpio.sysfs = 14;
    pinmodes[18].gpio.mode = 0;
    pinmodes[18].i2c.sysfs = 28;
    pinmodes[18].i2c.mode = 1;

    pinmodes[19].gpio.sysfs = 165;
    pinmodes[19].gpio.mode = 0;
    pinmodes[19].i2c.sysfs = 27;
    pinmodes[19].i2c.mode = 1;

    return b;
}
