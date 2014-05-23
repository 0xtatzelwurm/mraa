MAA - Low Level Skeleton Library for Communication on Intel platforms
==============

Library in C/C++ to interface with Galileo & other Intel platforms, in a
structured and sane API with port nanmes/numbering that match boards & with
bindings to javascript & python.

The intent is to make it easier for developers and sensor manufacturers to map
their sensors & actuators on top of supported hardware and to allow control of
low level communication protocol by high level languages & constructs.

## API

These interfaces allow you to interact with all libmaa functionality. The C++
classes directly wrap the C API and provide a near 1:1 mapping of
functionality.

<center>
C API Modules        | C++ API Classes
:-------------------:|:-------------------:
 @ref gpio.h "gpio"  | @ref maa::Gpio "Gpio class"
 @ref i2c.h "i2c"    | @ref maa::I2c "I2c class"
 @ref aio.h "aio"    | @ref maa::Aio "Aio class"
 @ref pwm.h "pwm"    | @ref maa::Pwm "Pwm class"
 @ref spi.h "spi"    | @ref maa::Spi "Spi class"
 @ref maa.h "maa"    | @ref maa.h "maa"
</center>

@snippet gpio_read6.c Interesting

## Supported platforms

- Galileo (Fab D)

### ENV RECOMENDATIONS

All of these are 'optional', however they are recommended. Only a C compiler,
cmake, libm and pthreads are technically required to compile.

- Swig 3.0.1+ built with node.js & python support (0.10.x)
- doxygen
- sphinx (requires doxygen)
- pygments

## COMPILING

mkdir build/
cmake -i ..
make

## DEVELOPMENT

Please fork the code on github and then send pull requests. Please avoid merges
in your forks. I will also accept patches sent in git style with signoffs to
brendan.le.foll@intel.com

## API Changelog

When the API breaks in a small way there will be a changelog listed here

**0.2.6**
  * C++ examples added, using c++ headers/api.
  * moved to open instead of fopen in all modules
  * rename maa_check functions and made them internal to maa only.
  * removed "export" functions from api
  * Intel Galileo Gen 1 (rev d) fixes, incorrect definition of some items
  * SPI, implementation completed.
  * I2C internal function, completed.
  * PWM fix bug in period set method.
  * Swig upstream can be used for building.
  * No longer builds docs on default cmake, needs flag
  * GPIO/PWM ownership guard prevents closing on existing pins, still can be forced.

**0.2.5**
  * C++/Python/Node Enums/const names now do not contain MAA_GPIO
  * Enum type declaration for C++ changed
  * Python/Node get_version() -> GetVersion()
  * i2c read calls don't use const char* and i2c write calls do

**0.2.4**
  * All maa_ contexts now are pointers to the actual struct which is not
    delcared in the header. The only end user change is that instead of
    returning a type maa_gpio_context pointer maa_gpio_init() now returns a
    maa_gpio_context (which is actually a pointer to struct _gpio internally).
  * C++ API is available, use the .hpp headers and add stdc++  to the linker
    flags.
  * Initial SPI implementation is provided

**0.2.3**
  * maa_aio_read_u16() -> maa_aio_read()
  * maa_aio_read() now returns a uint16_t instead of an unsigned int

**0.2.2**
  * First version with API considered 'stable'
