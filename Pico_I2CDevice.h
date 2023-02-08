#ifndef Pico_I2CDevice_h
#define Pico_I2CDevice_h

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"


// The class which defines how we will talk to this device over I2C
class Pico_I2CDevice {
    public:
        Pico_I2CDevice(uint8_t addr, i2c_inst_t *whichI2c = i2c0);
        uint8_t address(void);
        bool begin(bool addr_detect = true);
        void end(void);
        bool detected(void);

        int read(const uint8_t reg, uint8_t *buf, const uint8_t nbytes);
        bool write(const uint8_t reg, uint8_t *buf, const uint8_t nbytes);
        int write_then_read(const uint8_t reg, uint8_t *w_buf, const uint8_t w_nbytes,
                            uint8_t *r_buf, const uint8_t r_nbytes,
                            bool stop = false);
        bool setSpeed(uint32_t desiredclk);

        /*!   @brief  How many bytes we can read in a transaction
        *    @return The size of the Wire receive/transmit buffer */
        size_t maxBufferSize() { return _maxBufferSize; }

    private:
        uint8_t _addr;
        i2c_inst_t *_i2c;
        bool _begun;
        size_t _maxBufferSize;
        bool _read(uint8_t *buffer, size_t len, bool stop);
};

#endif