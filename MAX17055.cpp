#include "MAX17055.h"

MAX17055::MAX17055(I2C *i2c, int address) :
    i2c(i2c),
    addr(address << 1)
{
}

int MAX17055::status()
{
    char buf[2];

    if (readReg(MAX17055::STATUS, buf, 2)) {
        return MAX17055_ERROR;
    }

    return(unsigned int)((buf[1] << 8) | buf[0]);
}

int MAX17055::get_charge()
{
  return max17055ToPerc(readMax17055(MAX17055::REP_SOC));
}

int MAX17055::get_tte()
{
  return max17055ToMinutes(readMax17055(MAX17055::TTE));
}

int MAX17055::get_ttf()
{
  return max17055ToMinutes(readMax17055(MAX17055::TTF));
}

int MAX17055::get_cycles()
{
  return readMax17055(MAX17055::CYCLES);
}

int MAX17055::get_current()
{
  return max17055TouA(readMax17055(MAX17055::CURRENT));
}

int MAX17055::get_average_current()
{
  return max17055TouA(readMax17055(MAX17055::AVG_CURRENT));
}

int MAX17055::get_capacity()
{
  return max17055TomAh(readMax17055(MAX17055::REP_CAP));
}

int MAX17055::get_full_capacity()
{
  return max17055TomAh(readMax17055(MAX17055::FULL_CAP_REP));
}

int MAX17055::get_v_cell()
{
  return max17055TomV(readMax17055(MAX17055::V_CELL));
}

void MAX17055::set_nominal_capacity(uint16_t cap){
    this->writeReg(DESIGN_CAP, (uint16_t ) (cap * 2));
}

void MAX17055::set_full_capacity(uint16_t cap){
    this->writeReg(FULL_CAP_REP, (uint16_t ) (cap * 2));
}

void MAX17055::set_empty_voltage(float u){
    this->writeReg(FULL_CAP_REP, (uint16_t) (u * 16.0));
}

void MAX17055::set_charge_termination_current(float i){
    this->writeReg(FULL_CAP_REP, (uint16_t) (i * 6.4));
}

int MAX17055::max17055TomAh(int val)
{
  return val / 2; // for 10mOhm Rsense
}

int MAX17055::max17055ToPerc(int val) { return val / 256; }

int MAX17055::max17055ToMinutes(int val)
{
  int sec = val * 5625;
  return sec / 60000;
}

int MAX17055::max17055ToCelsius(int val) { return val / 256; }

int MAX17055::max17055ToOhm(int val) { return val / 4096; }

signed int MAX17055::max17055TouA(int val)
{
  signed int uA = (signed int)((int16_t)(val)) * 15625; // for 10mOhm Rsense
  return uA / 100;
}

int MAX17055::max17055TomV(int val)
{
  int uA = val * 125;
  return uA / 1600;
}

int MAX17055::readMax17055(MAX17055::reg_t reg)
{
    char buf[2];
    if (this->readReg(reg, buf, 2))
    {
      return 0;
    }

    return (int)((buf[1] << 8) | buf[0]);
}

int MAX17055::writeReg(reg_t reg, char value, bool verify)
{
    char buf[] = { (char)reg, value };

    if (this->i2c->write(addr, buf, sizeof(buf))) {
        return MAX17055_ERROR;
    }

    return MAX17055_NO_ERROR;
}

int MAX17055::writeReg(reg_t reg, uint16_t value, bool verify)
{
    char wbuf[3];
    char rbuf[2];
    int attempt;

    wbuf[0] = reg;
    wbuf[1] = value;
    wbuf[2] = value >> 8;

    attempt = 0;

    do {
        if (this->i2c->write(addr, wbuf, 3)) return MAX17055_ERROR;

        if (!verify) break;

        // Thread::wait(1);
        wait_us(1000);

        if (this->i2c->read(addr, rbuf, 2)) return MAX17055_ERROR;

    } while ((((rbuf[1] << 8) | rbuf[0]) != value) && (attempt++ < 3));

    if (attempt == 3) return MAX17055_ERROR;

    return MAX17055_NO_ERROR;
}

int MAX17055::readReg(reg_t reg, char *value)
{
    char buf[] = { (char)reg };

    if (this->i2c->write(addr, buf, sizeof(buf))) {
        return MAX17055_ERROR;
    }

    if (this->i2c->read(addr, value, 1)) {
        return MAX17055_ERROR;
    }

    return MAX17055_NO_ERROR;
}

int MAX17055::readReg(reg_t reg, char *buf, int len)
{
    *buf = (char)reg;

    if (this->i2c->write(addr, buf, 1)) {
        return MAX17055_ERROR;
    }

    if (this->i2c->read(addr, buf, len)) {
        return MAX17055_ERROR;
    }

    return MAX17055_NO_ERROR;
}
