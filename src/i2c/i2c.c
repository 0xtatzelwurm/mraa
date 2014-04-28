/*
 * Author: Brendan Le Foll
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

#include "i2c.h"
#include "smbus.h"

maa_i2c_context*
maa_i2c_init()
{
    maa_i2c_context* dev = (maa_i2c_context*) malloc(sizeof(maa_i2c_context));
    if (dev == NULL)
        return NULL;

    // Galileo only has one I2C master which should be /dev/i2c-0
    // reliability is a fickle friend!
    if ((dev->fh = open("/dev/i2c-0", O_RDWR)) < 1) {
        fprintf(stderr, "Failed to open requested i2c port");
    }
    return MAA_SUCCESS;
}

void
maa_i2c_frequency(maa_i2c_context* dev, int hz)
{
    dev->hz = hz;
}

int
maa_i2c_receive(maa_i2c_context* dev)
{
    return -1;
}

int
maa_i2c_read(maa_i2c_context* dev, char *data, int length)
{
    // this is the read(3) syscall not maa_i2c_read()
    if (read(dev->fh, data, length) == length) {
        return length;
    }
    return -1;
}

int
maa_i2c_read_byte(maa_i2c_context* dev)
{
    int byte;
    byte = i2c_smbus_read_byte(dev->fh);
    if (byte < 0) {
        return -1;
    }
    return byte;
}

int
maa_i2c_write(maa_i2c_context* dev, const char* data, int length)
{
    if (i2c_smbus_write_i2c_block_data(dev->fh, data[0], length-1, (uint8_t*) data+1) < 0) {
        fprintf(stderr, "Failed to write to I2CSlave slave\n");
	return -1;
    }
    return 0;
}

int
maa_i2c_write_byte(maa_i2c_context* dev, int data)
{
    if (i2c_smbus_write_byte(dev->fh, data) < 0) {
        fprintf(stderr, "Failed to write to I2CSlave slave\n");
	return -1;
    }
    return 0;
}

void
maa_i2c_address(maa_i2c_context* dev, int addr)
{
    dev->addr = addr;
    if (ioctl(dev->fh, I2C_SLAVE_FORCE, addr) < 0) {
        fprintf(stderr, "Failed to set slave address %d\n", addr);
    }
}

void
maa_i2c_stop(maa_i2c_context* dev)
{
    free(dev);
}
