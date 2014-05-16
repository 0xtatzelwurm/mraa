MAA - Low Level Skeleton Library for Communication on Intel platforms
==============

Library in C/C++ to interface with Galileo & other Intel platforms over:

- I2C
- SPI
- GPIO
- PWM
- AIO

In a structured and sane API with port nanmes/numbering that match boards &
with bindings to javascript & python.

The intent is to make it easier for developers and sensor manufacturers to map
their sensors & actuators on top of supported hardware and to allow control of
low level communication protocol by high level languages & constructs.

### ENV RECOMENDATIONS

1. node.js 0.10.26
2. python 3.3.x or 2.7.x
3. swig-v8 3.0.1
4. doxygen 1.8.7

To install swig-v8 the swig-v8-git package from AUR is helpful

## COMPILING

NOTE: The **only** supported cmake build configuration is to have the build/ dir
inside of the repo/tarball.

*if swig-v8 is not in your default path you can try run cmake with
"-DCMAKE_PREFIX_PATH="/path/to/swig-v8"*

mkdir build/
cmake ..
make

Install is currently unsuported. Javascript and python modules will be in
build/src/{javascript, python}

## DEVELOPMENT

Please fork the code on github and then send pull requests. Please avoid merges
in your forks. I will also accept patches sent in git style with signoffs to
brendan.le.foll@intel.com

## USING

see examples/

*for node.js make sure that maajs.node is in the current dir and set export
NODE_PATH=.*

Python html documentation can be generated with **make pydoc**

## API Changelog

When the API breaks in a small way there will be a changelog listed here

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
