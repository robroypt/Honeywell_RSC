#ifndef HONEYWELL_RSC_H
#define HONEYWELL_RSC_H

#include <Arduino.h>
#include <SPI.h>

#include "rsc_regs.h"

class Honeywell_RSC {
public:
  Honeywell_RSC(int drdy_pin, int cs_ee_pin, int cs_adc_pin);

  void init(RSC_DATA_RATE dr = N_DR_20_SPS, RSC_MODE mode = NORMAL_MODE);

  // chip selection
  void select_eeprom();
  void deselect_eeprom();
  void select_adc();
  void deselect_adc();

  // read number of bytes stored in EEPROM, starting from an address
  void eeprom_read(uint16_t address, uint8_t num_bytes, uint8_t *data);
  void get_catalog_listing();
  void get_serial_number();
  void get_pressure_range();
  void get_pressure_minimum();
  void get_pressure_unit();
  void get_pressure_type();
  void get_coefficients();
  void get_initial_adc_values(uint8_t *adc_init_values);

  // read from ADC
  void adc_read(READING_T type, uint8_t *data);
  float get_temperature();
  float get_pressure();

  // write to ADC
  void adc_write(uint8_t reg, uint8_t num_bytes, uint8_t *data);

  // other ADC related functions
  void reset_adc();
  void setup_adc(uint8_t* adc_init_values);
  void add_dr_delay();
  
  // setter functions
  void set_data_rate(RSC_DATA_RATE dr);
  void set_mode(RSC_MODE mode);
  
  // getter functions
  const unsigned char* catalog_listing() const {return &_catalog_listing[0];}
  const unsigned char* serial_number() const {return &_serial_number[0];}
  float pressure_range() const {return _pressure_range;}
  float pressure_minimum() const {return _pressure_minimum;}
  const char* pressure_unit_name() const {return &_pressure_unit_name[0];}
  const char* pressure_type_name() const {return &_pressure_type_name[0];}
  void print_catalog_listing(void);
  void print_serial_number(void);
  void print_pressure_unit_name(void);
  void print_pressure_type_name(void);

private:
  static constexpr size_t name_buff_sizes = 32;

  // physical pin connections
  uint8_t _drdy_pin;
  uint8_t _cs_ee_pin;
  uint8_t _cs_adc_pin;

  // from EEPROM
  unsigned char _catalog_listing[RSC_SENSOR_NAME_LEN];
  unsigned char _serial_number[RSC_SENSOR_NUMBER_LEN];
  float _pressure_range;
  float _pressure_minimum;
  PRESSURE_U _pressure_unit;
  char _pressure_unit_name[name_buff_sizes];
  PRESSURE_T _pressure_type;
  char _pressure_type_name[name_buff_sizes];

  // ADC
  RSC_DATA_RATE _data_rate;
  RSC_MODE _mode;

  // for calculating compensated pressure
  float _coeff_matrix[RSC_COEFF_T_ROW_NO][RSC_COEFF_T_COL_NO];
  int32_t _t_raw;
};

template<typename T>
void print_array_from_memory(T const * const start, size_t length){
  Serial.print(F("DBG "));
  for (size_t i=0; i<length; i++){
    Serial.print(F("0x"));
    Serial.print(*(start+i), HEX);
    Serial.print(F("="));
    Serial.print(*(start+i));
    Serial.print(F(" | "));
  }
  Serial.println(F(""));
}

#endif // HONEYWELL_RSC_H
