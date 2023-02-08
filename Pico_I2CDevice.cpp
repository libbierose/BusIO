#include "Pico_I2CDevice.h"

/*!
 *    @brief  Create an I2C device at a given address
 *    @param  addr The 7-bit I2C address for the device
 *    @param  whichI2c The I2C bus to use
 */
Pico_I2CDevice::Pico_I2CDevice(uint8_t addr, i2c_inst_t *whichI2c) {
  _addr = addr;
  _i2c = whichI2c;
  _begun = false;
  _maxBufferSize = 32;
}


/*!
 *    @brief  Initializes and does basic address detection
 *    @param  addr_detect Whether we should attempt to detect the I2C address
 *    @return True if I2C initialized and a device with the addr found
 */
bool Pico_I2CDevice::begin(bool addr_detect) {
  i2c_init(_i2c, 400 * 1000);
  _begun = true;

  if (addr_detect) {
    return detected();
  }
  return true;
}

/*!
 *    TODO  De-initialize device, turn off the Wire interface
 */
void Pico_I2CDevice::end(void) {

}

/*!
 *    @brief  Scans I2C for the address - note will give a false-positive
 *    if there's no pullups on I2C
 *    @return True if I2C initialized and a device with the addr found
 */
bool Pico_I2CDevice::detected(void) {
  // Init I2C if not done yet
  if (!_begun && !begin()) {
    return false;
  }

  int ret;
  uint8_t rxdata;
  ret = i2c_read_blocking(i2c1, _addr, &rxdata, 1, false);

  if(ret < 0){
    printf("Not detected");
    return false;
  }else{
    printf("Detected");
    return true;
  }

  printf("Not detected");
  return false;
}

/*!
 *    @brief  Write a buffer or two to the I2C device. Cannot be more than
 * maxBufferSize() bytes.
 *    @param  buf Pointer to buffer of data to write. This is const to
 *            ensure the content of this buffer doesn't change.
 *    @return True if write was successful, otherwise false.
 */

bool Pico_I2CDevice::write(const uint8_t reg,
                            uint8_t *buf,
                            const uint8_t nbytes){  
  int num_bytes = 0;
  uint8_t msg[nbytes + 1];

  // Check to make sure caller is sending 1 or more bytes
  if (nbytes < 1) {
    return false;
  }

  // Append register address to front of data packet
  msg[0] = reg;

  for (int i = 0; i < nbytes; i++) {
    msg[i + 1] = buf[i];
  }

  i2c_write_blocking(_i2c, _addr, msg, (nbytes + 1), false);

  return true;
}

int Pico_I2CDevice::read(const uint8_t reg,
                          uint8_t *buf,
                          const uint8_t nbytes){
  int bytes = 0;

  // Check to make sure caller is asking for 1 or more bytes
  if (nbytes < 1) {
    return 0;
  }

  // Read data from register(s) over I2C
  i2c_write_blocking(_i2c, _addr, &reg, 1, true);
  bytes = i2c_read_blocking(_i2c, _addr, buf, nbytes, false);

  return bytes;
}

int Pico_I2CDevice::write_then_read(const uint8_t reg, 
                                    uint8_t *w_buf, 
                                    const uint8_t w_nbytes,
                                    uint8_t *r_buf, 
                                    const uint8_t r_nbytes,
                                    bool stop = false){
  
  if(!write(reg, w_buf, w_nbytes))
    return -1;

  return read(reg, r_buf, r_nbytes);
}

uint8_t Pico_I2CDevice::address(void){ return _addr; }

bool Pico_I2CDevice::setSpeed(uint32_t desiredclk){
  // TODO - Add changing i2c speed
  return false;
}
