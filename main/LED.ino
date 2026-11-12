MeRGBLed leds(PORT_7, 2);


void setColor(int c[3]) {
  leds.setColor(0, c[0], c[1], c[2]);
  leds.show();
}