%{
    #include "maa.h"
    #include "gpio.h"
    #include "pwm.h"
    #include "i2c.h"
    #include "spi.h"
    #include "aio.h"
%}

%rename(get_version) maa_get_version();
const char * maa_get_version();

#### GPIO ####

%rename(Gpio) maa_gpio_context;

typedef struct {
    /*@{*/
    int pin; /**< the pin number, as known to the os. */
    FILE *value_fp; /**< the file pointer to the value of the gpio */
    /*@}*/
} maa_gpio_context;

%nodefault maa_gpio_context;
%extend maa_gpio_context {
  maa_gpio_context(int pin, int raw=0)
  {
    if (raw)
      return maa_gpio_init_raw(pin);
    return maa_gpio_init(pin);
  }
  ~maa_gpio_context()
  {
    maa_gpio_close($self);
  }
  int write(int value)
  {
    return maa_gpio_write($self, value);
  }
  int dir(gpio_dir_t dir)
  {
    return maa_gpio_dir($self, dir);
  }
  int read()
  {
    return maa_gpio_read($self);
  }
  int mode(gpio_mode_t mode)
  {
    return maa_gpio_mode($self, mode);
  }
}

#### i2c ####

%rename(I2c) maa_i2c_context;

typedef struct {
    /*@{*/
    int hz; /**< frequency of communication */
    int fh; /**< the file handle to the /dev/i2c-* device */
    int addr; /**< the address of the i2c slave */
    maa_gpio_context gpio;
    /*@}*/
} maa_i2c_context;

%nodefault maa_i2c_context;
%extend maa_i2c_context {
  maa_i2c_context()
  {
    return maa_i2c_init();
  }
  ~maa_i2c_context()
  {
    maa_i2c_stop($self);
  }
  int frequency(int hz)
  {
    return maa_i2c_frequency($self, hz);
  }
  int read(char *data, int length)
  {
    return maa_i2c_read($self, data, length);
  }
  int read()
  {
    return maa_i2c_read_byte($self);
  }
  int write(char *data, int length)
  {
    return maa_i2c_write($self, data, length);
  }
  int write(int data)
  {
    return maa_i2c_write_byte($self, data);
  }
}

#### PWM ####

%rename(Pwm) maa_pwm_context;

typedef struct {
    /*@{*/
    int pin; /**< the pin number, as known to the os. */
    int chipid; /**< the chip id, which the pwm resides */
    FILE *duty_fp; /**< File pointer to duty file */
    /*@}*/
} maa_pwm_context;

%nodefault maa_pwm_context;
%extend maa_pwm_context {
  maa_pwm_context(int pin)
  {
    return maa_pwm_init(pin);
  }
  maa_pwm_context(int chipid, int pin)
  {
    return maa_pwm_init_raw(chipid, pin);
  }
  ~maa_pwm_context()
  {
    maa_pwm_close($self);
  }
  int write(float percentage)
  {
    return maa_pwm_write($self, percentage);
  }
  int read()
  {
    return maa_pwm_read($self);
  }
  int period(float seconds)
  {
    return maa_pwm_period($self, seconds);
  }
  int period_ms(int ms)
  {
    return maa_pwm_period_ms($self, ms);
  }
  int period_us(int us)
  {
    return maa_pwm_period_us($self, us);
  }
  int pulsewidth(float seconds)
  {
    return maa_pwm_pulsewidth($self, seconds);
  }
  int pulsewidth_ms(int ms)
  {
    return maa_pwm_pulsewidth($self, ms);
  }
  int pulsewidth_us(int us)
  {
    return maa_pwm_pulsewidth($self, us);
  }
  int enable(int enable)
  {
    return maa_pwm_enable($self, enable);
  }
}

#### SPI ####

%rename(Spi) maa_spi_context;

typedef struct {
    /*@{*/
    int spifd; /**< File descriptor to SPI Device */
    /*@}*/
} maa_spi_context;

%nodefault maa_spi_context;
%extend maa_spi_context {
  maa_spi_context()
  {
    return maa_spi_init();
  }
  ~maa_spi_context()
  {
    maa_spi_stop($self);
  }
  int mode(unsigned short mode)
  {
    return maa_spi_mode($self, mode);
  }
  int frequency(int hz)
  {
    return maa_spi_frequency($self, hz);
  }
  unsigned int write(unsigned int data)
  {
    return maa_spi_write($self, data);
  }
}

#### AIO ####

%rename(Aio) maa_aio_context;

typedef struct {
    unsigned int channel;
    FILE *adc_in_fp;
} maa_aio_context;

%nodefault maa_aio_context;
%extend maa_aio_context {
  maa_aio_context(unsigned int aio_channel)
  {
    return maa_aio_init(aio_channel);
  }
  ~maa_aio_context()
  {
    maa_aio_close($self);
  }
  unsigned int read()
  {
    return maa_aio_read_u16($self);
  }
}
