#define LED_PIN     2    // Controlled via GPIO2 for this kit
#define BRIGHTNESS  50   // Set safe brightness limit (0-255)


void setup() {
400d1820:       006136                  entry   a1, 48
  Serial.begin(115200);
400d1823:       78a082                  movi    a8, 120
400d1826:       1189                    s32i.n  a8, a1, 4
400d1828:       082c                    movi.n  a8, 32
400d182a:       4ed882                  addmi   a8, a8, 0x4e00
400d182d:       e1a0b2                  movi    a11, 225
400d1830:       0f0c                    movi.n  a15, 0
400d1832:       f9fbc1                  l32r    a12, 400d0020 <_stext> (800001c <UserFrameTotalSize+0x7ffff1c>)
400d1835:       fe7c                    movi.n  a14, -1
400d1837:       fd7c                    movi.n  a13, -1
400d1839:       11bb70                  slli    a11, a11, 9
400d183c:       f9faa1                  l32r    a10, 400d0024 <_stext+0x4> (3ffc2078 <Serial0>)
400d183f:       0189                    s32i.n  a8, a1, 0
400d1841:       002825                  call8   400d1ac4 <_ZN14HardwareSerial5beginEmmaabmh>
  // Initialize the WS2812 LED configuration

}
400d1844:       0020c0                  memw
400d1847:       f01d                    retw.n
400d1849:       000000                  ill

400d184c <_Z4loopv>:

void loop() {
400d184c:	004136               	entry	a1, 32
  // Blink Red: neopixelWrite(pin, Red, Green, Blue)
  neopixelWrite(LED_PIN, BRIGHTNESS, 0, 0);
400d184f:	00a0d2               	movi	a13, 0
400d1852:	00a0c2               	movi	a12, 0
400d1855:	2b3c                	movi.n	a11, 50
400d1857:	2a0c                	movi.n	a10, 2
400d1859:	00bca5               	call8	400d2424 <neopixelWrite>
  Serial.println("red on");
400d185c:	f9f3b1               	l32r	a11, 400d0028 <_stext+0x8> (3f400120 <_flash_rodata_start>)
400d185f:	f9f1a1               	l32r	a10, 400d0024 <_stext+0x4> (3ffc2078 <Serial0>)
400d1862:	005b65               	call8	400d1e18 <_ZN5Print7printlnEPKc>
  delay(1000);
400d1865:	e8a3a2               	movi	a10, 0x3e8
400d1868:	00a025               	call8	400d226c <delay>

  // Turn LED Off (All values zero)
  neopixelWrite(LED_PIN, 0, 0, 0);
400d186b:	0d0c                	movi.n	a13, 0
400d186d:	0c0c                	movi.n	a12, 0
400d186f:	0b0c                	movi.n	a11, 0
400d1871:	2a0c                	movi.n	a10, 2
400d1873:	00bb25               	call8	400d2424 <neopixelWrite>
  Serial.println("red off");
400d1876:	f9edb1               	l32r	a11, 400d002c <_stext+0xc> (3f400127 <_flash_rodata_start+0x7>)
400d1879:	f9eaa1               	l32r	a10, 400d0024 <_stext+0x4> (3ffc2078 <Serial0>)
400d187c:	0059a5               	call8	400d1e18 <_ZN5Print7printlnEPKc>
  delay(1000);
400d187f:	e8a3a2               	movi	a10, 0x3e8
400d1882:	009ea5               	call8	400d226c <delay>
}
400d1885:	f01d                	retw.n
	...
