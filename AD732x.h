#ifndef __AD732x_H__
#define __AD732x_H__

#include <Arduino.h>
#include <SPI.h>

typedef enum {
    RANGE_10VN_TO_10VP = 0,
    RANGE_5VN_TO_5VP = 1,
    RANGE_2_5VN_TO_2_5VP = 2,
    RANGE_0_TO_10VP = 3
} ad732x_chan_range_t;


typedef enum {
    AD72XX_REF_SRC_EXT = 0,
    AD72XX_REF_SRC_INT = 1
} ad73xx_ref_src;

typedef enum {
    AD72XX_CODING_TWOS_COMPLEMENT = 0,
    AD72XX_CODING_BINARY = 1
} ad73xx_coding;

typedef enum {
    AD72XX_PM_NORMAL = 0,
    AD72XX_PM_AUTO_STANDBY = 1,
    AD72XX_PM_AUTO_SHUTDOWN = 2,
    AD72XX_PM_FULL_SHUTDOWN = 3
} ad73xx_power_mode;

typedef enum {
    AD72XX_INPUT_MODE_8X_SINGLE_ENDED = 0,
    AD72XX_INPUT_MODE_4X_PSEUDO_DIFF = 1,
    AD72XX_INPUT_MODE_4X_FULL_DIFF = 2,
    AD72XX_INPUT_MODE_7X_PSEUDO_DIFF = 3
} ad73xx_input_mode;

typedef union {
    uint16_t raw;
    struct {
        bool dummy0 : 1;
        bool dummy1 : 1;
        uint8_t seq : 2;
        ad73xx_ref_src ref_src : 1;
        ad73xx_coding coding : 1;
        ad73xx_power_mode power_mode : 2;
        ad73xx_input_mode input_mode : 2;
        uint8_t chan_addr : 3;
        uint8_t reg_sel : 2;
        bool write : 1;
    };
} ad73xx_reg_control;

typedef union {
    uint16_t raw;
    struct {
        bool dummy0 : 5;
        ad732x_chan_range_t range3 : 2; 
        ad732x_chan_range_t range2 : 2; 
        ad732x_chan_range_t range1 : 2;
        ad732x_chan_range_t range0 : 2;
        uint8_t reg_sel : 2;
        bool write : 1;

    };
} ad73xx_reg_range;


class AD732X_ADC
{
    public:
        AD732X_ADC(spi_inst_t *spi, uint8_t cs, uint8_t sck, uint8_t mosi, uint8_t miso, uint32_t speed);
        ~AD732X_ADC();

        void update();
        int16_t readAnalog(uint8_t channel);
        void setRange(uint8_t channel, ad732x_chan_range_t range);
        void setInputMode(ad73xx_input_mode mode);
        void setPowerMode(ad73xx_power_mode mode);
        void setCoding(ad73xx_coding coding);
        void setReference(ad73xx_ref_src ref_src);

        void printControlRegister();
        
    private:
        void writeControlRegister();
        void writeRangeRegister(uint8_t reg_sel);
        ad73xx_reg_control readControlRegister();
        
        uint8_t _cs;
        uint8_t _sck;
        uint8_t _mosi;
        uint8_t _miso;
        uint32_t _speed;
        int16_t _data[8];
        ad732x_chan_range_t _range[8];
        
        SPISettings _spi_settings;
        spi_inst_t *_spi;
        SPIClass *_spi_class;
        ad73xx_reg_control _control_reg;
        ad73xx_reg_range _range_reg0, _range_reg1;

};

#endif // __AD732x_H__