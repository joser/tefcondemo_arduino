// Low power NeoPixel goggles example.  Makes a nice blinky display
// with just a few LEDs on at any time.
 
#include <FileIO.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
 #include <avr/power.h>
#endif
 


// Pixels config 
#define PIN 0
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(12, PIN);
uint32_t RED    =  0xFF0000;
uint32_t GREEN  =  0x00FF00;
uint32_t BLUE   =  0x0000FF;
uint32_t OFF    =  0x000000;
uint8_t THRESHOLD = 0;

// Logs config
String LOG_FILE = "/root/tefcon/luz.logs";

// Context broker config
String CB_SCRIPT = "python /root/tefcon/queryContextBroker.py";
 

// Setup neopixels and bridge 
void setup() {
#ifdef __AVR_ATtiny85__ // Trinket, Gemma, etc.
  if(F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  pixels.begin();
  pixels.setBrightness(85); // 1/3 brightness
  Bridge.begin();
}
 
void loop() {
  uint8_t  data;
  uint32_t color;
  
  // Show blue sparks
  show_sparks(BLUE, 1000);
  
  // get ContextBroker data
  log("requesting data from Context Broker");
  data = runScript(); 
  log("received data from Context Broker");
  
  // evaluate data, >0 enough water
  if(data > 0){
      // if h above threshold round green
      color = GREEN;
      log("turning LED green");
  } 
  else {
      // if h below threshold round red
      color = RED;
      log("turning LED red");
  }
  
  // turn led to evaluated color
  show_spinny(color, 2000, 8000);


}



void show_sparks(uint32_t color, uint32_t sparking_time){
  
  // reset leds
  show_fullcolor(OFF);
  
  uint32_t prevTime = millis();
  while((millis() - prevTime) < sparking_time){
      uint8_t i = random(32);
      //pixels.setPixelColor(i, color);
      pixels.setPixelColor(i,pixels.Color(random(255),random(255),random(255)));
      pixels.show();
      delay(10);
      pixels.setPixelColor(i, 0);
  }
}  
 
void show_spinny(uint32_t color, uint32_t spinning_time, uint32_t fix_time){
  
  // reset leds
  show_fullcolor(OFF);

  uint32_t prevTime = millis();
  uint8_t i;
  uint8_t offset = 0;
  while((millis() - prevTime) < spinning_time){
     for(i=0; i<16; i++) {
      uint32_t c = 0;
      if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
      pixels.setPixelColor(i, c); 
      
    }
    pixels.show();
    offset++;
    delay(60);    
  }
 
  show_fullcolor(color);
  delay(fix_time);

}

void show_fullcolor(uint32_t color){
    for(int i=0;i<12;i++){
        pixels.setPixelColor(i, color); 
        pixels.show(); 
    }
}


int runScript() {
  // Run the script and show results on the Serial
  Process readContextBroker;
  readContextBroker.runShellCommand(CB_SCRIPT);      

  // do nothing until the process finishes, so you get the whole output
  while(readContextBroker.running());  

  // read the output of the script
  return readContextBroker.parseInt();
   
}

void log(String text){
    Process writeLog;
    writeLog.runShellCommand("echo \"$(date) :: " + text + "  \">>"+LOG_FILE);
}


