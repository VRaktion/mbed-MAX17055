#ifndef _MAX17055_H_
#define _MAX17055_H_

#include "mbed.h"

#define MAX17055_I2C_ADDRESS   0x36

#define MAX17055_NO_ERROR 0
#define MAX17055_ERROR    -1

#define MAX17055_V_LSB_MV         7.8125E-5f
#define MAX17055_V_MAX_MIN_LSB_MV 2.0E-2f
#define MAX17055_I_LSB_UV         1.5625E-3f
#define MAX17055_I_MAX_MIN_LSB_MV 0.0004f

class MAX17055
{
public:

    typedef enum {
        STATUS = 0x00,
        V_ALRT_TH,
        T_ALRT_TH,
        S_ALRT_TH,
        AT_RATE,
        REP_CAP,
        REP_SOC,
        AGE,
        TEMP,
        V_CELL,
        CURRENT,
        AVG_CURRENT,
        Q_RESIDUAL,
        MIX_SOC,
        AV_SOC,
        MIX_CAP,
        FULL_CAP_REP,
        TTE,
        QR_TABLE_00,
        FULL_SOC_THR,
        R_CELL,
        AVG_TA = 0x16,
        CYCLES,
        DESIGN_CAP,
        AVG_V_CELL,
        MAX_MIN_TEMP,
        MAX_MIN_VOLT,
        MAX_MIN_CURR,
        CONFIG,
        I_CHG_TERM,
        AV_CAP,
        TTF,
        DEV_NAME,
        QR_TABLE_10,
        FULL_CAP_NOM,
        AIN = 0x27,
        LEARN_CFG,
        FILTER_CFG,
        RELAX_CFG,
        MISC_CFG,
        T_GAIN,
        T_OFF,
        C_GAIN,
        C_OFF,
        QR_TABLE_20 = 0x32,
        DIE_TEMP = 0x34,
        FULL_CAP,
        R_COMP0 = 0x38,
        TEMP_CO,
        V_EMPTY,
        F_STAT = 0x3D,
        TIMER,
        SHDN_TIMER,
        USER_MEM1,
        QR_TABLE_30 = 0x42,
        R_GAIN,
        DQ_ACC = 0x45,
        DP_ACC,
        CONVG_CFG = 0x49,
        VF_REM_CAP,
        QH = 0x4D,
        STATUS_2 = 0xB0,
        POWER,
        ID_USER_MEM2,
        AVG_POWER,
        I_ALRT_TH,
        TTF_CFG,
        CV_MIX_CAP,
        CV_HALF_TIME,
        CG_TEMP_CO,
        CURVE,
        HIB_CFG,
        CONFIG2,
        V_RIPPLE,
        RIPPLE_CFG,
        TIMER_H,
        R_SENSE_USER_MEM3 = 0xD0,
        SC_OCV_LIM,
        SOC_HOLD,
        MAX_PEAK_POWER,
        SUS_PEAK_POWER,
        PACK_RESISTANCE,
        SYS_RESISTANCE,
        MIN_SYS_VOLTAGE,
        MPP_CURRENT,
        SPP_CURRENT,
        MODEL_CFG,
        AT_Q_RESIDUAL,
        AT_TTE,
        AT_AV_SOC,
        AT_AV_CA
    } reg_t;

    MAX17055(I2C *i2c, int address = MAX17055_I2C_ADDRESS);
    int status();
    int get_charge();
    int get_tte();
    int get_ttf();
    int get_cycles();
    int get_current();
    int get_average_current();
    int get_capacity();
    int get_full_capacity();
    int get_v_cell();

    void set_nominal_capacity(uint16_t cap);
    void set_full_capacity(uint16_t cap);
    void set_empty_voltage(float u);
    void set_charge_termination_current(float i);

private:

    int max17055TomAh(int val);
    int max17055ToPerc(int val);
    int max17055ToMinutes(int val);
    int max17055ToCelsius(int val);
    int max17055ToOhm(int val);
    signed int max17055TouA(int val);
    int max17055TomV(int val);

    int readMax17055(reg_t reg);
    int writeReg(reg_t reg, char value, bool verify = false);
    int writeReg(reg_t reg, uint16_t value, bool verify = false);
    int readReg(reg_t reg, char *value);
    int readReg(reg_t reg, char *buf, int len);


    I2C *i2c;
    int addr;
};

#endif
