#include "AD732x.h"

AD732X_ADC::AD732X_ADC(spi_inst_t *spi, uint8_t cs, uint8_t sck, uint8_t mosi, uint8_t miso, uint32_t speed)
{
    _cs = cs;
    _sck = sck;
    _mosi = mosi;
    _miso = miso;
    _speed = speed;
    _spi = spi;
    _spi_class = new SPIClassRP2040(_spi, _sck, _miso, _mosi, _speed);
    _spi_settings = SPISettings(speed, MSBFIRST, SPI_MODE2);
};

void AD732X_ADC::update()
{
    _spi_class->beginTransaction(_spi_settings);
    for (uint8_t i = 0; i < 8; i++)
    {
        digitalWrite(_cs, LOW);
        int16_t b = _spi_class->transfer16(0) & 0x1FFF;
        int16_t sign = (b >> 12) & 0x1 ? 1 : -1;
        if (sign == -1)
            b = 8192 - b;
        _data[i] = sign * (b & 0xFFF);
        digitalWrite(_cs, HIGH);
    }
    _spi_class->endTransaction();
}

int16_t AD732X_ADC::readAnalog(uint8_t channel)
{
    if (channel > 7)
        return 0;
    return _data[channel];
}

void AD732X_ADC::setRange(uint8_t channel, ad732x_chan_range_t range)
{
    uint8_t offset = 0;
    uint8_t reg_sel = 0;
    if (channel > 4)
    {
        offset = 4;
        reg_sel = 1;
    }

    switch (channel)
    {
    case 0:
        _range_reg0.range0 = range;
        break;
    case 1:
        _range_reg0.range1 = range;
        break;
    case 2:
        _range_reg0.range2 = range;
        break;
    case 3:
        _range_reg0.range3 = range;
        break;
    case 4:
        _range_reg1.range0 = range;
        break;
    case 5:
        _range_reg1.range1 = range;
        break;
    case 6:
        _range_reg1.range2 = range;
        break;
    case 7:
        _range_reg1.range3 = range;
        break;
    }

    writeRangeRegister(reg_sel);
}

void AD732X_ADC::setInputMode(ad73xx_input_mode mode)
{
    // ...
}

void AD732X_ADC::setPowerMode(ad73xx_power_mode mode)
{
    _control_reg.power_mode = mode;
    writeControlRegister();
}

void AD732X_ADC::setCoding(ad73xx_coding coding)
{
    // ...
    _control_reg.coding = coding;
    writeControlRegister();
}

void AD732X_ADC::setReference(ad73xx_ref_src ref_src)
{
    _control_reg.ref_src = ref_src;
    writeControlRegister();
}

void AD732X_ADC::writeControlRegister()
{
    _spi_class->beginTransaction(_spi_settings);
    digitalWrite(_cs, LOW);
    _control_reg.write = 1;
    _spi_class->transfer16(_control_reg.raw);
    digitalWrite(_cs, HIGH);
    _spi_class->endTransaction();
}

void AD732X_ADC::writeRangeRegister(uint8_t reg_sel)
{
    _spi_class->beginTransaction(_spi_settings);
    digitalWrite(_cs, LOW);
    if(reg_sel == 0){
        _range_reg0.write = 1;
        _range_reg0.reg_sel = reg_sel;
        _spi_class->transfer16(_range_reg0.raw);
    }else{
        _range_reg1.write = 1;
        _range_reg1.reg_sel = reg_sel;
        _spi_class->transfer16(_range_reg1.raw);
    }
    digitalWrite(_cs, HIGH);
    _spi_class->endTransaction();
}

ad73xx_reg_control AD732X_ADC::readControlRegister()
{
    ad73xx_reg_control reg;
    _spi_class->beginTransaction(_spi_settings);
    digitalWrite(_cs, LOW);
    _control_reg.write = 0;
    reg.raw = SPI.transfer16(_control_reg.raw);
    digitalWrite(_cs, HIGH);
    _spi_class->endTransaction();
    return reg;
}

void AD732X_ADC::printControlRegister()
{
    // Note: the AD732x doesn't seem to have reading capabilities
    ad73xx_reg_control reg = readControlRegister();
    Serial.print("Control Register: ");
    Serial.println(reg.raw, BIN);
    Serial.print("  SEQ: ");
    Serial.println(reg.seq);
    Serial.print("  REF_SRC: ");
    Serial.println(reg.ref_src);
    Serial.print("  CODING: ");
    Serial.println(reg.coding);
    Serial.print("  POWER_MODE: ");
    Serial.println(reg.power_mode);
    Serial.print("  INPUT_MODE: ");
    Serial.println(reg.input_mode);
    Serial.print("  CHAN_ADDR: ");
    Serial.println(reg.chan_addr);
    Serial.print("  REG_SEL: ");
    Serial.println(reg.reg_sel);
    Serial.print("  WRITE: ");
    Serial.println(reg.write);
}