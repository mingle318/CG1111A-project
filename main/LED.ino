MeRGBLed leds(PORT_7, 2);


void setColor(const int c[3]) {
  leds.setColor(0, c[0], c[1], c[2]); //Set both mBot LED to color {R, G, B} = c;
  leds.show();
}