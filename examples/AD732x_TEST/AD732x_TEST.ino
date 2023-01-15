// AD732x Test (untested)

#include <AD732x.h>
AD732X_ADC adc(spi1, 14, 10, 11, 12, 1000000UL);

void setup() {
  Serial.begin(115200);
  adc.setReference(AD72XX_REF_SRC_INT);
  adc.setCoding(AD72XX_CODING_BINARY);

  // first 4 inputs -10v to +10v, then 2x 0-10v and then 2x -5v to +5v.
  adc.setRange(0, RANGE_10VN_TO_10VP);
  adc.setRange(1, RANGE_10VN_TO_10VP);
  adc.setRange(2, RANGE_10VN_TO_10VP);
  adc.setRange(3, RANGE_10VN_TO_10VP);
  adc.setRange(4, RANGE_0_TO_10VP);
  adc.setRange(5, RANGE_0_TO_10VP);
  adc.setRange(6, RANGE_5VN_TO_5VP);
  adc.setRange(7, RANGE_5VN_TO_5VP);
}

void loop() {
  // put your main code here, to run repeatedly:
  adc.update();
  for(int i=0; i<8; i++){
    Serial.printf("%i, ", adc.readAnalog(i));
  }
  Serial.println();
  delay(15);
}
