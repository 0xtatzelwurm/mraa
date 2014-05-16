/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
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

#pragma once

/** @file
 *
 * This file defines the gpio C++ interface for libmaa
 *
 */

#include "gpio.h"

namespace maa {

class Gpio {
    public:
        Gpio(int pin, bool raw=false) {
            if (raw)
                m_gpio = maa_gpio_init_raw(pin);
            else
                m_gpio = maa_gpio_init(pin);
        }
        ~Gpio() {
            maa_gpio_close(m_gpio);
        }
        maa_result_t edge(gpio_edge_t mode) {
            return maa_gpio_edge_mode(m_gpio, mode);
        }
#if defined(SWIGPYTHON)
        maa_result_t isr(gpio_edge_t mode, PyObject *pyfunc) { 
            return maa_gpio_isr(m_gpio, mode, (void (*) ()) pyfunc);
        }
#else
        maa_result_t isr(gpio_edge_t mode, void (*fptr)(void)) {
            return maa_gpio_isr(m_gpio, mode, fptr);
        }
#endif
        maa_result_t isr_exit() {
            return maa_gpio_isr_exit(m_gpio);
        }
        maa_result_t mode(gpio_mode_t mode) {
            return maa_gpio_mode(m_gpio, mode);
        }
        maa_result_t dir(gpio_dir_t dir) {
            return maa_gpio_dir(m_gpio, dir);
        }
        int read() {
            return maa_gpio_read(m_gpio);
        }
        int write(int value) {
            return maa_gpio_write(m_gpio, value);
        }
    private:
        maa_gpio_context m_gpio;
};

}
