/* ************************************************************************************
* This is a little dittie that uses the smart pixel leds to drive a marquee display on an arduino Uno driving 2 rows of leds
* to replace the original incandescent mini buldbs that were int he original movie theater poster display.
*/

/* 
 * --- MARQUEE SIGN QUICK INTERFACE MANUAL ---
 * TURN DIAL     = Adjusts active parameter (Speed / Dimmer / Menu Scroll)
 * SINGLE CLICK  = Cycles mode sequentially: Speed (0) -> Brightness (1) -> Choice Menu (2)
 * DOUBLE CLICK  = Instantly inverts animation spin direction (Forward / Backward)
 * LONG PRESS    = Master Power Toggle (Turns the entire 96-LED display On / Off)
 */

/*  These are the 35 options for displays: 

  marqueeTheaterGold,
  marqueeChristmas,
  marqueeRainbow,
  marqueeMazeBlue,
  marqueeGreenWhite,
  marqueePolice,
  marqueeWayneState,
  marqueeCMU,
  marqueeOakland,
  chase,
  chaseit,
  chaseone,
  acceleratingCylon,
  rainbow,
  rainbowWithGlitter, 
  confetti, 
  sinelon, 
  juggle, 
  bpm ,
  pacifica_loop,
  pride,
  cylon,
  multiCylon,
  marqueeHollywood,
  MaizeAndBlueChase,
  GreenAndWhiteChase,
  WayneChase,
  CMUChase,
  OaklandChase,
  ticketChase,
  carnivalChase,
  curtainsOpen, 
  twinkleStar,
  shootingStar,
  solid
  
  */

#include <Versatile_RotaryEncoder.h>
#include "FastLED.h"


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// SET READING PINS ACCORDINGLY TO YOUR ENCODER TO BOARD CONNECTIONS
#define clk 17  // (A3)
#define dt 16   // (A2)
#define sw 18   // (A4)

bool selection = false;
uint8_t choice = 0;

// Functions prototyping
void handleRotate(int8_t rotation);
void handlePress();
void handleDoublePress();
void handleLongPress();
void blinkOne(int led);
void blankem(int speed);
void showChoice();

// Create a global pointer for the encoder object
Versatile_RotaryEncoder *versatile_encoder;
#define LED_TYPE WS2812B
#define DATA_PIN            3
#define MAX_POWER_MILLIAMPS 3000 // We are plannig to use a 3 amp power. 

#define COLOR_ORDER         RGB  
#define NUM_LEDS 96
#define SLOWEST_SPEED 1000
#define FASTEST_SPEED 10

#define DEFAULT_BRIGHTNESS 180
#define MAX_BRIGHTNESS_LIMIT 220  // 🌟 Your safe high-end saturation ceiling
/*
OLD tests 
#define MICHIGAN_BLUE  CRGB(0, 39, 76)
#define MICHIGAN_MAIZE CRGB(255, 203, 5)
#define SPARTAN_GREEN  CRGB(0, 135, 50)
#define SPARTAN_WHITE  CRGB(255, 255, 255)
*/

// 〽️ UNIVERSITY OF MICHIGAN
#define MICHIGAN_MAIZE CRGB(255, 185, 0)   // 💡 Optimized: Shifts pure yellow slightly toward orange to prevent an ugly lime-green tint.
#define MICHIGAN_BLUE  CRGB(0, 15, 180)    // 💡 Optimized: Prevents the deep navy from looking like a weak, dim purple or black.

// ⚔️ MICHIGAN STATE UNIVERSITY
#define SPARTAN_GREEN  CRGB(0, 145, 45)    // 💡 Optimized: Rich, punchy forest kelly green that holds its color weight at high brightness.
#define SPARTAN_WHITE  CRGB(255, 245, 230) // 💡 Optimized: Warm white that balances the heavy green without looking like a harsh blue-white strobe.

// 🔴 CENTRAL MICHIGAN UNIVERSITY (CMU) - Digitally Separated
#define CMU_MAROON     CRGB(150, 0, 40)   // ⚡ OPTIMIZED: Boosts pure crimson and snaps the blue down to prevent a muddy wash, maintaining a deep jewel-tone burgundy.
#define CMU_GOLD       CRGB(255, 130, 0)  // ⚡ OPTIMIZED: Drops the green to 130, turning it into a sharp, rich amber-gold that forces the maroon to pop.


// 🐻 OAKLAND UNIVERSITY (OU) - Golden Grizzlies Black & Gold
//#define OAKLAND_GOLD   CRGB(210, 140, 5)   // 💡 Optimized: A deep, metallic-bronze gold to distinguish it from CMU/UM.
//#define OAKLAND_BLACK  CRGB::Black         // 💡 Standard unlit gaps for high-contrast negative space chasing.

// 🐻 OAKLAND UNIVERSITY (OU) - Golden Grizzlies
#define OAKLAND_GOLD   CRGB(210, 140, 5)

// OPTION 1: Deep Midnight Obsidian (Highly Recommended)
// A tiny splash of blue and green creates a cold, ink-black contrast that forces the gold to pop.
//#define OAKLAND_BLACK  CRGB(2, 5, 10) 

// OPTION 2: Low-Embers Chocolate Brown
// A very dark, rich warm undertone that perfectly compliments the Golden Grizzlies team theme.
#define OAKLAND_BLACK  CRGB(12, 4, 0)

// OPTION 3: Dark Velvet Charcoal
// An ultra-dim, completely balanced gray that softens the negative space without turning off the pixels.
//#define OAKLAND_BLACK  CRGB(6, 6, 6)

// 🛡️ WAYNE STATE UNIVERSITY (WSU) - Tartars Green & Gold
#define WAYNE_GREEN    CRGB(0, 84, 73)     // 💡 Official #0C5449: This specific green actually mixes beautifully and holds its deep forest tone.
#define WAYNE_GOLD     CRGB(255, 204, 51)  // 💡 Official #FFCC33: A bright, distinct "canary" theater gold.


uint8_t BRIGHTNESS = DEFAULT_BRIGHTNESS;

// Forward declarations for pattern functions
void pacifica_one_layer(CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
void pacifica_loop();
void pacifica_add_whitecaps();
void pacifica_deepen_colors();
void rainbow();
void rainbowWithGlitter();
void confetti();
void sinelon();
void juggle();
void bpm();
void nextPattern();
void lastPattern();
void pride();
void cylon();
void multiCylon();
void addGlitter(fract8 chanceOfGlitter);
void chase();
void chaseit();
void acceleratingCylon();
void chaseone();
void solid();
void marqueeTheaterGold();
void marqueeChristmas();
void marqueeRainbow();
void marqueeMazeBlue();
void marqueeGreenWhite();
void marqueeWayneState();
void marqueeCMU();
void marqueeOakland();
void marqueePolice();
void twinkleStar();
void shootingStar();
void marqueeHollywood();
void GreenAndWhiteChase();
void MaizeAndBlueChase();
void WayneChase();
void CMUChase();
void OaklandChase();
void curtainsOpen();
void ticketChase();
void carnivalChase();
void runMarqueeFrame(uint8_t mode, CRGB color1, CRGB color2, int spacing, int blockSize);

// Global control variables
bool marqueeForward = true;       
uint8_t marqueeStep = 0;          
uint16_t globalDelayMs = 250;      
bool changeBrightnessMode = false; // Unified tracking flag
bool displayPowerOn = true;        
// 🆕 UNIFIED UI STATE MACHINE
// 0 = Adjust Speed, 1 = Adjust Brightness, 2 = Browse/Select Layout Menu (White Dots)
uint8_t uiControlState = 0; 

CRGB leds[NUM_LEDS];

// List of patterns to cycle through.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  marqueeTheaterGold,
  marqueeChristmas,
  marqueeRainbow,
  marqueeMazeBlue,
  marqueeGreenWhite,
  marqueePolice,
  marqueeWayneState,
  marqueeCMU,
  marqueeOakland,
  chase,
  chaseit,
  chaseone,
  acceleratingCylon,
  rainbow,
  rainbowWithGlitter, 
  confetti, 
  sinelon, 
  juggle, 
  bpm ,
  pacifica_loop,
  pride,
  cylon,
  multiCylon,
  marqueeHollywood,
  MaizeAndBlueChase,
  GreenAndWhiteChase,
  WayneChase,
  CMUChase,
  OaklandChase,
  ticketChase,
  carnivalChase,
  curtainsOpen, 
  twinkleStar,
  shootingStar,
  solid
};

uint8_t gCurrentPatternNumber = 0; 
uint8_t gHue = 0; 
uint8_t phase = 0; 

void setup() {
  Serial.begin(115200);
  FastLED.setBrightness(BRIGHTNESS);
  delay(3000); // 3 second delay for boot recovery
  
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, MAX_POWER_MILLIAMPS);	
  
  // Initialize the pointer
  versatile_encoder = new Versatile_RotaryEncoder(clk, dt, sw);

  // Bind your "Human Interface" actions
  versatile_encoder->setHandleRotate(handleRotate);
  versatile_encoder->setHandlePress(handlePress);
  versatile_encoder->setHandleDoublePress(handleDoublePress);
  versatile_encoder->setHandleLongPress(handleLongPress);
}

void loop() {
    // 1. Keep encoder interactions active at maximum clock speed
    versatile_encoder->ReadEncoder();    

    // HARD SECURITY GATE: Drop calculations instantly if system is sleeping
    if (!displayPowerOn) {
        return; 
    }

    // --- STATE 2: DISPLAY SELECTION DOT MENU ---
    if (uiControlState == 2) { // 💡 FIXED: Now matches your state machine variable directly!
        // The main loop stays silent to prevent data line interrupt clashing.
        // Screen updates are painted instantly inside your handleRotate() hook.
    }
    // --- STATES 0 & 1: ACTIVE PATTERN RUNTIME ---
    else {
        static unsigned long lastFrameTime = 0;
        
        if (millis() - lastFrameTime >= globalDelayMs) {
            lastFrameTime = millis();
            
            gPatterns[gCurrentPatternNumber](); // Execute active routine
            FastLED.show();                     
        }
    }
}

// --- Fixed Array Macro Calculation (Line 160) ---
#define CORRECT_ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))


// 1. STANDARD ROTATE: Handles normal speed scaling, item scrolling, or fine-tuned dimming
void handleRotate(int8_t rotation) {
  if (!displayPowerOn) return; 

  // Calculate the true length of your pattern pointer matrix (21 patterns)
  uint16_t trueNumPatterns = sizeof(gPatterns) / sizeof(gPatterns[0]);

  switch(uiControlState) {
    case 0: // ⏱️ STATE 0: Speed Control Mode
      Serial.print("rotation="); Serial.println(rotation);
      if (rotation == 1) {
        if (globalDelayMs > FASTEST_SPEED) globalDelayMs -= 3; // Clockwise = Faster
      } else {
        if (globalDelayMs < SLOWEST_SPEED) globalDelayMs += 3; // Counter-Clockwise = Slower
      }
      Serial.print("Speed Delay Changed To: ");
      Serial.println(globalDelayMs);
      break;

    case 1: // 🔆 STATE 1: Brightness Control Mode (Your exact custom math tiers)
      if (rotation == 1) {
                // --- HIGH-END CEILING CHECK ---
        // Lock brightness to the define if it attempts to overshoot the limit
        Serial.print("rotation="); Serial.println(rotation);
        if (FastLED.getBrightness() >= MAX_BRIGHTNESS_LIMIT) {
          FastLED.setBrightness(MAX_BRIGHTNESS_LIMIT); 
        } 
        else if (FastLED.getBrightness() > 20) {
          FastLED.setBrightness(qadd8(FastLED.getBrightness(), 15));
        } else {
          FastLED.setBrightness(qadd8(FastLED.getBrightness(), 2));
        }  
      } else {
        if (FastLED.getBrightness() > 20) {
          FastLED.setBrightness(qsub8(FastLED.getBrightness(), 15));
        } else {
          FastLED.setBrightness(qsub8(FastLED.getBrightness(), 2));
        } 
        if (FastLED.getBrightness() < 1) FastLED.setBrightness(1); // Hard safety boundary floor patch
      }
      Serial.print("Brightness Changed To: ");
      Serial.println(FastLED.getBrightness());
      break;

    case 2: // 🗂️ STATE 2: Display Selection Menu Mode (Fixed Index Calculation!)
      if (rotation == 1) {
        gCurrentPatternNumber = (gCurrentPatternNumber + 1) % trueNumPatterns; // ✅ Advances cleanly up to 21
      } else {
        Serial.print("rotation="); Serial.println(rotation);
;        gCurrentPatternNumber = (gCurrentPatternNumber == 0) ? (trueNumPatterns - 1) : (gCurrentPatternNumber - 1);
      }
      
      // Update canvas updates immediately on click turn
      FastLED.clear();
      showChoice();
      FastLED.show();
      
      Serial.print("Menu Index Changed To: ");
      Serial.println(gCurrentPatternNumber);
      break;
  }
}

// 2. SINGLE PRESS: Safe sequential progression loops through your 3 layout environments
void handlePress() {
  if (!displayPowerOn) return;

  // Cycle the operational context: 0 -> 1 -> 2 -> back to 0
  uiControlState = (uiControlState + 1) % 3; 

  if (uiControlState == 0) {
    selection = false; 
    Serial.println(">>> MODE ACTIVE: SPEED TEMPO SCALER <<<");
  } 
  else if (uiControlState == 1) {
    selection = false;
    Serial.println(">>> MODE ACTIVE: BRIGHTNESS HARDWARE DIMMER <<<");
  } 
  else if (uiControlState == 2) {
    selection = true; // Set flag to maintain display safety checks
    
    // Draw initial tracking status dots layout immediately when the menu pops open
    FastLED.clear();
    showChoice();
    FastLED.show(); 
    Serial.println(">>> MODE ACTIVE: DISPLAY CHOICE DOT MENU <<<");
  }
}

// 3. SHOW CHOICE HELPER: Updated to use the true array size boundaries
void showChoice() {
  uint16_t trueNumPatterns = sizeof(gPatterns) / sizeof(gPatterns[0]);

  for (uint16_t i = 0; i < NUM_LEDS; i++) {
    if (i < trueNumPatterns) {
      if (i <= gCurrentPatternNumber) {
        leds[i] = CRGB::White; // Clean assignment prevents accumulation saturation
      } else {
        leds[i] = CRGB::Black; 
      }
    } else {
      leds[i] = CRGB::Black; // Ensure remaining 96-LED pads remain dead dark
    }
  }
}

// 4. DOUBLE PRESS: Dedicated to reversing marquee direction safely during execution
void handleDoublePress() {
  if (!displayPowerOn) return; 
  
  if (uiControlState != 2) { // Only switch direction while a style is actively running
    marqueeForward = !marqueeForward;
    Serial.print("Marquee Inverted! Spin Direction Forward: ");
    Serial.println(marqueeForward ? "TRUE" : "FALSE");
  }
}

// 5. LONG PRESS: Isolated system power sleep trigger
void handleLongPress() {
  static unsigned long lastPowerToggleTime = 0;
  
  if (millis() - lastPowerToggleTime < 1500) {
    return; 
  }
  lastPowerToggleTime = millis(); 

  displayPowerOn = !displayPowerOn;
  
  if (!displayPowerOn) {
    blankem(50);
    FastLED.clear();
    FastLED.show();    // Black out all 96 LEDs immediately
    selection = false; 
    uiControlState = 0; // Default back to speed mode on reboot
    Serial.println("System Powered OFF");
  } else {
    Serial.println("System Powered ON");
  }
}

// --- Unified Non-Blocking Marquee Core ---
void runMarqueeFrame(uint8_t mode, CRGB color1, CRGB color2, int spacing, int blockSize) {
  static unsigned long lastUpdate = 0;
  static uint8_t rainbowHue = 0;
  static uint8_t lastPatternNumber = 255;

  if (gCurrentPatternNumber != lastPatternNumber) {
    lastPatternNumber = gCurrentPatternNumber;
    FastLED.clear();
  }
  
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();
    if (marqueeForward) {
      marqueeStep = (marqueeStep + 1) % spacing;
    } else {
      marqueeStep = (marqueeStep == 0) ? (spacing - 1) : (marqueeStep - 1);
    }
    rainbowHue++;
  }

  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    int posInPattern = (i + marqueeStep) % spacing;
    if (posInPattern < blockSize) {
      switch(mode) {
        case 0: leds[i] = color1; break;
        case 1: 
          if (posInPattern >= (blockSize / 2) && blockSize > 1) {
            leds[i] = color2;
          } else {
            int blockIndex = (i + marqueeStep) / spacing;
            leds[i] = (blockIndex % 2 == 0) ? color1 : color2;
          }
          break;
        case 2: leds[i] = CHSV(rainbowHue + (i * 2), 255, 255); break;
      }
    }
  }
}

void carnivalChase(){
  patternCarnivalChase();
}


void ticketChase(){
  patternTicketChase();
}
void CMUChase() { 
  runCollegeChase(CRGB(150, 0, 40), CRGB(255, 130, 0)); 
}

void WayneChase() { 
  runCollegeChase(CRGB(255, 185, 0), CRGB(0, 15, 180)); 
}
void OaklandChase() { 
  runCollegeChase(CRGB(255, 185, 0), CRGB(0, 15, 180)); 
}
void MaizeAndBlueChase() { 
  runCollegeChase(CRGB(255, 185, 0), CRGB(0, 15, 180)); 
}

void GreenAndWhiteChase() { 
  runCollegeChase(CRGB(0, 145, 45), CRGB(255, 245, 230)); 
}

// 3. Central Michigan Chasing Block Sequence (Maroon & Gold)
void marqueeCMU() {
  runCollegeChase(CMU_MAROON, CMU_GOLD);
}

// 4. Oakland University Chasing Block Sequence (Gold & Black Negative Space)
void marqueeOakland() {
  runCollegeChase(OAKLAND_GOLD, OAKLAND_BLACK);
}

// 5. Wayne State Chasing Block Sequence (Forest Green & Canary Gold)
void marqueeWayneState() {
  runCollegeChase(WAYNE_GREEN, WAYNE_GOLD);
}




void marqueeHollywood(){
  patternHollywoodFlash();
}

void marqueeTheaterGold() {
  // Mode 0, Gold Color, Black background, Spacing 6, Block 2
  runMarqueeFrame(0, CRGB(255, 140, 0), CRGB::Black, 6, 2);
}

void marqueeChristmas() {
  // Mode 1, Red & Green Alternating, Spacing 4, Block 2
  runMarqueeFrame(1, CRGB::Red, CRGB::Green, 4, 2);
}

void marqueeRainbow() {
  // Mode 2, Rainbow Cycling, Spacing 6, Block 2
  runMarqueeFrame(2, CRGB::Black, CRGB::Black, 6, 2);
}

void marqueeMazeBlue() {
  // Mode 1, Red & Green Alternating, Spacing 4, Block 2
  runMarqueeFrame(1, CRGB::MICHIGAN_BLUE, CRGB::MICHIGAN_MAIZE, 4, 2);
}

void marqueeGreenWhite() {
  // Mode 1, Red & Green Alternating, Spacing 4, Block 2
  runMarqueeFrame(1, CRGB::SPARTAN_WHITE, CRGB::SPARTAN_GREEN, 4, 2);
}

void marqueePolice() {
  runMarqueeFrame(1, CRGB::Red, CRGB::Blue, 6, 2);
}

void twinkleStar() {
  runTwinkleFrame();
}

void shootingStar(){
  runShootingStarFrame();
}

void curtainsOpen(){
  patternHorizonSync();
}
//////////////////////////////////////////////////////////////////////////
//
// In this animation, there are four "layers" of waves of light.  
//
// Each layer moves independently, and each is scaled separately.
//
// All four wave layers are added together on top of each other, and then 
// another filter is applied that adds "whitecaps" of brightness where the 
// waves line up with each other more.  Finally, another pass is taken
// over the led array to 'deepen' (dim) the blues and greens.
//
// The speed and scale and motion each layer varies slowly within independent 
// hand-chosen ranges, which is why the code has a lot of low-speed 'beatsin8' functions
// with a lot of oddly specific numeric ranges.
//
// These three custom blue-green color palettes were inspired by the colors found in
// the waters off the southern coast of California, https://goo.gl/maps/QQgd97jjHesHZVxQ7
//
CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };
CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

// Added "bool forward" parameter to control direction
void oldMarquee(uint8_t mode, CRGB color1, CRGB color2, int spacing, int blockSize, int speedMs, bool forward) {
  static uint8_t hue = 0;

  // The direction controls whether our animation offset steps forward or backward
  for (int step = 0; step < spacing; step++) {
    int offset = forward ? step : (spacing - 1 - step); // Reverses the step order
    
    FastLED.clear();
    
    for (int i = 0; i < NUM_LEDS; i++) {
      int posInPattern = (i + offset) % spacing;
      
      if (posInPattern < blockSize) {
        switch(mode) {
          case 0: leds[i] = color1; break;
          case 1: 
            if (posInPattern >= (blockSize / 2) && blockSize > 1) {
              leds[i] = color2;
            } else {
              int blockIndex = (i + offset) / spacing;
              leds[i] = (blockIndex % 2 == 0) ? color1 : color2;
            }
            break;
          case 2: leds[i] = CHSV(hue + (i * 2), 255, 255); break;
        }
      }
    }
    
    FastLED.show();
    delay(speedMs);
    hue++;
  }
}

void patternTicketChase() {
  static uint8_t step = 0;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();
    // Advance forward or backward based on your double-click direction tracker
    if (marqueeForward) {
      step = (step + 1) % 12; // Length of the cycle window
    } else {
      step = (step == 0) ? 11 : step - 1;
    }
  }
  
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    // Lights up a solid block of 4 LEDs, followed by 8 blank dark pixels
    if (((i + step) % 12) < 4) {
      leds[i] = CRGB(255, 160, 0); // Warm Vintage Gold
    }
  }
}

void patternHollywoodFlash() {
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();
    
    // Maintain a baseline theater gold background
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB(120, 60, 0); 
    }
    
    // Randomly select a group of 3 adjacent bulbs to flash brightly
    if (random8(100) < 12) { // Adjust for frequency
      int flashPoint = random16(NUM_LEDS - 2);
      CRGB flashColor = CRGB::White; 
      
      leds[flashPoint]     = flashColor;
      leds[flashPoint + 1] = flashColor;
      leds[flashPoint + 2] = flashColor;
    }
  }
}

void patternHorizonSync() {
  static uint8_t wavePos = 0;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();
    
    if (marqueeForward) {
      wavePos = (wavePos + 1) % 12; // 12-pixel cycle length
    } else {
      wavePos = (wavePos == 0) ? 11 : wavePos - 1;
    }
    
    FastLED.clear();
    for (int i = 0; i < NUM_LEDS; i++) {
      // Mirror the pattern math every 24 pixels (perfect quadrant splits)
      int mirroredIndex = i % 24;
      if (mirroredIndex > 12) mirroredIndex = 24 - mirroredIndex;
      
      if ((mirroredIndex + wavePos) % 6 < 2) {
        leds[i] = CRGB(255, 140, 0); // Theater Gold
      }
    }
  }
}

void patternCarnivalChase() {
  static uint8_t step = 0;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();
    if (marqueeForward) {
      step = (step + 1) % 9; // 3 colors * 3 pixels per block
    } else {
      step = (step == 0) ? 8 : step - 1;
    }
  }
  
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    int rem = (i + step) % 9;
    
    if (rem < 3) {
      leds[i] = CRGB(255, 0, 0);   // Red Block (3 pixels wide)
    } else if (rem < 6) {
      leds[i] = CRGB(230, 230, 230); // Dimmer White Block (prevents blinding glare)
    } else {
      leds[i] = CRGB(255, 160, 0); // Gold Block
    }
  }
}



void runCollegeChase(CRGB primaryColor, CRGB secondaryColor) {
  static uint8_t step = 0;
  static unsigned long lastUpdate = 0;
  
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();
    step = (step + 1) % 8; // Block pattern size
  }
  
  FastLED.clear();
  for (int i = 0; i < NUM_LEDS; i++) {
    // Blocks of 4 pixels chasing each other down the tracks
    if (((i + step) % 8) < 4) {
      leds[i] = primaryColor;
    } else {
      leds[i] = secondaryColor;
    }
  }
}


void pacifica_loop()
{
  // Increment the four "color index start" counters, one for each wave layer.
  // Each is incremented at a different speed, and the speeds vary over time.
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  // Clear out the LED array to a dim background blue-green
  fill_solid( leds, NUM_LEDS, CRGB( 2, 6, 10));

  // Render each of four layers, with different scales and speeds, that vary over time
  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  // Add brighter 'whitecaps' where the waves lines up more
  pacifica_add_whitecaps();

  // Deepen the blues and greens a bit
  pacifica_deepen_colors();
}

// Add one layer of waves into the led array
void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff)
{
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
  }
}

// Add extra 'white' to areas where the four layers of light have lined up brightly
void pacifica_add_whitecaps()
{
  uint8_t basethreshold = beatsin8( 9, 55, 65);
  uint8_t wave = beat8( 7 );
  
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    uint8_t threshold = scale8( sin8( wave), 20) + basethreshold;
    wave += 7;
    uint8_t l = leds[i].getAverageLight();
    if( l > threshold) {
      uint8_t overage = l - threshold;
      uint8_t overage2 = qadd8( overage, overage);
      leds[i] += CRGB( overage, overage2, qadd8( overage2, overage2));
    }
  }
}

// Deepen the blues and greens
void pacifica_deepen_colors()
{
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
  }
}
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
  Serial.print("Next pattern="); Serial.println(gCurrentPatternNumber);
}
void lastPattern()
{
  // add one to the current pattern number, and wrap around at the end
  if (gCurrentPatternNumber > 0) {
    gCurrentPatternNumber = (gCurrentPatternNumber - 1) % ARRAY_SIZE( gPatterns);
    Serial.print("Last pattern="); Serial.println(gCurrentPatternNumber);
    }
}
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62; // was 62
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
 
  uint32_t ms = ::millis(); 

  uint32_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}
void multiCylon() {
    static uint8_t pos1 = 0;
    static uint8_t pos2 = NUM_LEDS / 2;
    static int8_t direction1 = 1;
    static int8_t direction2 = -1;

    fadeToBlackBy(leds, NUM_LEDS, 20);
    FastLED.setBrightness(BRIGHTNESS);  // softten the display  

    // First scanner
    leds[pos1] = CRGB::Salmon;
    pos1 += direction1;
    if (pos1 == 0 || pos1 == NUM_LEDS - 1) {
        direction1 = -direction1;
    }
    // Second scanner
    leds[pos2] = CRGB::Blue;
    pos2 += direction2;
    if (pos2 == 0 || pos2 == NUM_LEDS - 1) {
        direction2 = -direction2;
    }
}
void cylon() {
    static uint8_t pos = 0;
    static int8_t direction = 1;

    // Fade all LEDs
    fadeToBlackBy(leds, NUM_LEDS, 5);
    FastLED.setBrightness(BRIGHTNESS);  // softten the display  

    // Set current position
    leds[pos] = CRGB::Red;

    // Move position
    pos += direction;
    if (pos == 0 || pos == NUM_LEDS - 1) {
        direction = -direction;
    }
}
void chaseone() {
    static uint8_t pos = 0;

//    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.setBrightness(BRIGHTNESS);  // softten the display  

    leds[pos] = CRGB::Yellow;
    pos++;
    delay(60);
    leds[pos-1] = CRGB::Black;
    leds[pos] = CRGB::Yellow;
    delay(60);
    if (pos >= NUM_LEDS) {
        pos = 0;
    }
}
void chaseit() {
  FastLED.clear();
  FastLED.setBrightness(BRIGHTNESS);  // softten the display  

  for (int i = 0; i < NUM_LEDS; i++) {
    // Light LED if (i + phase) is even
    if (((i + phase) & 1) == 0) {
      leds[i] = CRGB::Yellow; // Or any color you prefer
    } else {
      leds[i] = CRGB::Black; // Off
    }
  }
  // Increment phase for next call to flip the pattern
  phase = (phase + 1) % 2;

  // Optional: Small delay for visibility (remove if you want to control timing elsewhere)
  delay(150);
}
void solid(){
  FastLED.clear();
  FastLED.setBrightness(BRIGHTNESS);  // softten the display  
  for (int i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Yellow; // Or any color you prefer
  }
  versatile_encoder->ReadEncoder();    // Do the encoder reading and processing  
}
void chase() {
    static uint8_t pos = 0;

    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.setBrightness(BRIGHTNESS);  // softten the display 
    leds[pos] = CRGB::Yellow;

    pos++;
    if (pos >= NUM_LEDS) {
        pos = 0;
    }
}
void acceleratingCylon() {
    static uint8_t pos = 0;
    static int8_t direction = 1;
    static uint8_t speed = 1;

    fadeToBlackBy(leds, NUM_LEDS, 20);
    FastLED.setBrightness(BRIGHTNESS);  // softten the display  
    leds[pos] = CRGB::Yellow;

    // Calculate speed based on distance from center
    uint8_t distFromCenter = abs(pos - NUM_LEDS / 2);
    speed = map(distFromCenter, 0, NUM_LEDS / 2, 1, 3);

    pos += direction * speed;
    if (pos <= 0 || pos >= NUM_LEDS - 1) {
        direction = -direction;
        pos = constrain(pos, 0, NUM_LEDS - 1);
    }
}

// --- Star Twinkle Core Animation ---
void runTwinkleFrame() {
  // Static arrays to remember the state of each of your 96 LEDs between loop passes
  static uint8_t currentBri[NUM_LEDS] = {0}; // Current brightness of each LED
  static int8_t  fadeDirection[NUM_LEDS] = {0}; // 0 = off, 1 = fading up, -1 = fading down
  static uint8_t targetBri[NUM_LEDS] = {0};    // The peak brightness for this specific star
  
  static unsigned long lastUpdate = 0;

  // Pace the animation frames using your global encoder speed setting
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();

    // 1. Spontaneously ignite new stars
    // Lower numbers mean fewer stars; higher numbers mean a denser, busier sky
    // We adjust the chance based on how fast the timer is running so it looks good at any speed
    uint16_t spawnChance = map(globalDelayMs, 5, 300, 15, 5); 
    
    if (random8(100) < spawnChance) {
      int randomLED = random16(NUM_LEDS);
      
      // Only ignite it if it is currently dark / completely off
      if (fadeDirection[randomLED] == 0) {
        fadeDirection[randomLED] = 1; // Start fading UP
        currentBri[randomLED] = 1;     // Initial spark
        targetBri[randomLED] = random8(100, 255); // Random peak brightness for variance
      }
    }

    // 2. Loop through all 96 LEDs to calculate their fade trajectories
    for (int i = 0; i < NUM_LEDS; i++) {
      if (fadeDirection[i] == 1) { // FADING UP
        // Make individual stars fade up at different rates (adding 4 to 12 per frame)
        uint8_t step = random8(4, 12);
        if ((int)currentBri[i] + step >= targetBri[i]) {
          currentBri[i] = targetBri[i];
          fadeDirection[i] = -1; // Hit peak! Switch to fading DOWN
        } else {
          currentBri[i] += step;
        }
      } 
      else if (fadeDirection[i] == -1) { // FADING DOWN
        uint8_t step = random8(3, 8); // Stars usually fade out slightly slower than they fade in
        if (currentBri[i] <= step) {
          currentBri[i] = 0;
          fadeDirection[i] = 0; // Turned completely off. Ready to spawn again later.
        } else {
          currentBri[i] -= step;
        }
      }

      // 3. Render the calculated brightness natively to the strip as White
      // Because we write directly to currentBri, FastLED's global setBrightness() 
      // will scale these down safely relative to your dial settings!
      leds[i] = CRGB(currentBri[i], currentBri[i], currentBri[i]);
    }
  }
}



// --- High-Speed Fluid Shooting Star with Natural Trail Decay ---
void runShootingStarFrame() {
  // Background twinkle memory arrays
  static uint8_t currentBri[NUM_LEDS] = {0};
  static int8_t  fadeDirection[NUM_LEDS] = {0};
  static uint8_t targetBri[NUM_LEDS] = {0};
  
  // High-Speed Meteor Tracking Variables
  static bool    starActive = false;
  static float   starPos = 0.0;
  static float   starSpeed = 6.0; 
  
  static unsigned long lastUpdate = 0;

  // 1. BACKGROUND SKY PROCESSOR: Kept paced via your encoder's global speed timer
  if (millis() - lastUpdate >= globalDelayMs) {
    lastUpdate = millis();

    // Spontaneously trigger a shooting star if one isn't alive
    if (!starActive && random16(1000) < 6) { 
      starActive = true;
      starPos = 0.0; 
      starSpeed = random8(5, 10); // Adjusts the speed velocity of the streak
    }

    // Spawn standard background twinkle stars
    if (random8(100) < 5) {
      int randomLED = random16(NUM_LEDS);
      if (fadeDirection[randomLED] == 0) {
        fadeDirection[randomLED] = 1;
        currentBri[randomLED] = 1;
        targetBri[randomLED] = random8(20, 80); // Keep background dim
      }
    }

    // Process background twinkle step trajectories
    for (int i = 0; i < NUM_LEDS; i++) {
      if (fadeDirection[i] == 1) {
        uint8_t step = random8(1, 4);
        if ((int)currentBri[i] + step >= targetBri[i]) {
          currentBri[i] = targetBri[i];
          fadeDirection[i] = -1;
        } else {
          currentBri[i] += step;
        }
      } 
      else if (fadeDirection[i] == -1) {
        uint8_t step = random8(1, 3);
        if (currentBri[i] <= step) {
          currentBri[i] = 0;
          fadeDirection[i] = 0;
        } else {
          currentBri[i] -= step;
        }
      }
    }
  }

  // 2. 🌊 LIQUID TRAIL DECAY ENGINE: Continuous uniform canvas fade-out pass
  // This smoothly bleeds away a percentage of light on every processor loop pass,
  // creating a completely fluid, tail melt instead of jagged, blinking steps.
  // 32 out of 256 is roughly a 12.5% reduction per frame pass. 
  fadeToBlackBy(leds, NUM_LEDS, 32); 

  // Re-inject the subtle background stars over the faded buffer canvas
  for (int i = 0; i < NUM_LEDS; i++) {
    if (currentBri[i] > 0 && leds[i].r < currentBri[i]) {
      leds[i] = CRGB(currentBri[i], currentBri[i], currentBri[i]);
    }
  }

  // 3. ⚡ METEOR PHYSICS: Moves at raw CPU clock velocity
  if (starActive) {
    starPos += starSpeed; 
    
    if (starPos >= NUM_LEDS) {
      starActive = false; // Exited bounds safely
    } else {
      int headIndex = (int)starPos;
      
      // Ignite the meteor head point at maximum capacity
      leds[headIndex] = CRGB(255, 255, 255);
      
      // Smooth interpolation: light up the pixel immediately ahead 
      // based on the fraction of the float steps to bridge integer gaps smoothly
      if (headIndex + 1 < NUM_LEDS) {
        float fraction = starPos - headIndex;
        uint8_t branchBrightness = fraction * 255;
        leds[headIndex + 1] |= CRGB(branchBrightness, branchBrightness, branchBrightness);
      }
    }
  }
}












  void blankem(int speed){
    for(int i = 0; i < NUM_LEDS; i++) {
      leds[i]= CRGB::Black;
      FastLED.setBrightness(BRIGHTNESS);  // softten the display  
      FastLED.show();
      delay (speed);
    }
  }

  void blinkOne(int led){
    FastLED.setBrightness(BRIGHTNESS);  // softten the display  
    leds[led].nscale8(250); delay (10);
  }
