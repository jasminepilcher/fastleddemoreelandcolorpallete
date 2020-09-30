#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>
FASTLED_USING_NAMESPACE
 
// ColorWavesWithPalettes
// Animated shifting color waves, with several cross-fading color palettes.
// by Mark Kriegsman, August 2015
//
// Color palettes courtesy of cpt-city and its contributors:
//   http://soliton.vm.bytemark.co.uk/pub/cpt-city/
//
// Color palettes converted for FastLED using "PaletteKnife" v1:
//   http://fastled.io/tools/paletteknife/
//
 
#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif
 
#define DATA_PIN    23
//#define CLK_PIN   4
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    120
#define BRIGHTNESS  50
#define FRAMES_PER_SECOND  120
 
CRGB leds[NUM_LEDS];
 
// ten seconds per color palette makes a good demo
// 20-120 is better for deployment
#define SECONDS_PER_PALETTE 10
 
 
void setup() {
  //  delay(3000); // 3 second delay for recovery
 
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS)
  //.setCorrection(TypicalLEDStrip) // cpt-city palettes have different color balance
  .setDither(BRIGHTNESS < 255);
 
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
 
// Forward declarations of an array of cpt-city gradient palettes, and
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;
 
// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;
 
CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );
 
 
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { confetti, confetti2, sinelon, sinelon2, bpm, juggle, colorwaves};
 
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
 
void loop()
{
  // Call the current pattern function once, updating the 'leds' array
  gPatterns[gCurrentPatternNumber]();
 
  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000 / FRAMES_PER_SECOND);
 
  // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) {
    gHue++;  // slowly cycle the "base color" through the rainbow
  }
  EVERY_N_SECONDS( 10 ) {
    nextPattern();  // change patterns periodically
  }
 
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }
 
  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }
 
//  colorwaves( leds, NUM_LEDS, gCurrentPalette);
}
 
 
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
 
void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}
 

 void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  delay (5);
}

void confetti2()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
//  leds[pos] += CHSV( gHue + random8(64), 200, 255);
  leds[pos] += ColorFromPalette(gCurrentPalette, gHue + random8(64), 255);
  delay(30);
}


 
void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 8, 0, NUM_LEDS - 1 );  //beatsin16(BEAT, MIN, MAX)
//  leds[pos] += CHSV( gHue, 255, 192);
  leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192);
}

void sinelon2()
{
  // a colored dot sweeping back and forth, with fading trails 
//
  
  fadeToBlackBy( leds, 12, 20); // fading leds 
  int pos = beatsin16( 12, 0, 12-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).
  
 {
  fadeToBlackBy( leds, 24, 20);
  int pos = beatsin16( 12, 12, 24-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}

{
  fadeToBlackBy( leds, 36, 20);
  int pos = beatsin16( 12, 24, 36-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}

{
  fadeToBlackBy( leds, 48, 20); //The first line of the function is fading all LEDs on the strip by about 8% (20/255).
  int pos = beatsin16( 12, 36, 48-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
{
  fadeToBlackBy( leds, 60, 20);
  int pos = beatsin16( 12, 48, 60-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
{
  fadeToBlackBy( leds, 72, 20);
  int pos = beatsin16( 12, 60, 72-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
{
  fadeToBlackBy( leds, 84, 20);
  int pos = beatsin16( 12, 72, 84-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
{
  fadeToBlackBy( leds, 96, 20);
  int pos = beatsin16( 12, 84, 96-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
{
  fadeToBlackBy( leds, 108, 20);
  int pos = beatsin16( 12, 96, 108-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
{
  fadeToBlackBy( leds, 120, 20);
  int pos = beatsin16( 12, 108, 120-1); //  beatsin16(BEAT, MIN, MAX)
   leds[pos] += ColorFromPalette(gCurrentPalette, gHue, 192); // Led Color (gHue), full saturation (255) and brightness (192).

}
}


 
void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 20;
//  CRGBPalette16 palette = OceanColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
//    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
    leds[i] = ColorFromPalette(gCurrentPalette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

 
void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  byte dothue = 0;
  for ( int i = 0; i < 8; i++) {
//    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= ColorFromPalette(gCurrentPalette, dothue, 255);
    dothue += 32;
    delay(20);
  }
}


void colorwaves()
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);
 
  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);
 
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5, 9);
  uint16_t brightnesstheta16 = sPseudotime;
 
  for ( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if ( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }
 
    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;
 
    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
 
    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);
 
    CRGB newcolor = ColorFromPalette( gCurrentPalette, index, bri8);
 
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS - 1) - pixelnumber;
 
    nblend( leds[pixelnumber], newcolor, 128);
  }
}
 
// Alternate rendering function just scrolls the current palette
// across the defined LED strip.
void palettetest( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& gCurrentPalette)
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( ledarray, numleds, startindex, (256 / NUM_LEDS) + 1, gCurrentPalette, 255, LINEARBLEND);
}
 
 
 
 
 
// Gradient Color Palette definitions for 33 different cpt-city color palettes.
//    956 bytes of PROGMEM for all of the palettes together,
//   +618 bytes of PROGMEM for gradient palette code (AVR).
//  1,494 bytes total for all 34 color palettes and associated code.
 



 

 
 
// Gradient palette "retro2_16_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/retro2/tn/retro2_16.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( retro2_16_gp ) {
  0, 188, 135,  1,
  255,  46,  7,  1
};
 
// Gradient palette "Analogous_1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/red/tn/Analogous_1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Analogous_1_gp ) {
  0,   3,  0, 255,
  63,  23,  0, 255,
  127,  67,  0, 255,
  191, 142,  0, 45,
  255, 255,  0,  0
};
 
// Gradient palette "es_pinksplash_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/pink_splash/tn/es_pinksplash_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( es_pinksplash_08_gp ) {
  0, 126, 11, 255,
  127, 197,  1, 22,
  175, 210, 157, 172,
  221, 157,  3, 112,
  255, 157,  3, 112
};
 

// 
// Gradient palette "Coral_reef_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/other/tn/Coral_reef.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Coral_reef_gp ) {
  0,  40, 199, 197,
  50,  10, 152, 155,
  96,   1, 111, 120,
  96,  43, 127, 162,
  139,  10, 73, 111,
  255,   1, 34, 71
};
 
// Gradient palette "es_ocean_breeze_068_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_068.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( es_ocean_breeze_068_gp ) {
  0, 100, 156, 153,
  51,   1, 99, 137,
  101,   1, 68, 84,
  104,  35, 142, 168,
  178,   0, 63, 117,
  255,   1, 10, 10
};
 
// Gradient palette "es_ocean_breeze_036_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_036.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( es_ocean_breeze_036_gp ) {
  0,   1,  6,  7,
  89,   1, 99, 111,
  153, 144, 209, 255,
  255,   0, 73, 82
};
 

 
 
// Gradient palette "es_landscape_33_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/landscape/tn/es_landscape_33.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( es_landscape_33_gp ) {
  0,   1,  5,  0,
  19,  32, 23,  1,
  38, 161, 55,  1,
  63, 229, 144,  1,
  66,  39, 142, 74,
  255,   1,  4,  1
};
 
// Gradient palette "rainbowsherbet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ma/icecream/tn/rainbowsherbet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( rainbowsherbet_gp ) {
  0, 255, 33,  4,
  43, 255, 68, 25,
  86, 255,  7, 25,
  127, 255, 82, 103,
  170, 255, 255, 242,
  209,  42, 255, 22,
  255,  87, 255, 65
};
 
// Gradient palette "gr65_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr65_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( gr65_hult_gp ) {
  0, 247, 176, 247,
  48, 255, 136, 255,
  89, 220, 29, 226,
  160,   7, 82, 178,
  216,   1, 124, 109,
  255,   1, 124, 109
};
 
// Gradient palette "gr64_hult_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/hult/tn/gr64_hult.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( gr64_hult_gp ) {
  0,   1, 124, 109,
  66,   1, 93, 79,
  104,  52, 65,  1,
  130, 115, 127,  1,
  150,  52, 65,  1,
  201,   1, 86, 72,
  239,   0, 55, 45,
  255,   0, 55, 45
};
 
// Gradient palette "GMT_drywet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/gmt/tn/GMT_drywet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( GMT_drywet_gp ) {
  0,  47, 30,  2,
  42, 213, 147, 24,
  84, 103, 219, 52,
  127,   3, 219, 207,
  170,   1, 48, 214,
  212,   1,  1, 111,
  255,   1,  7, 33
};
 
// Gradient palette "ib15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( ib15_gp ) {
  0, 113, 91, 147,
  72, 157, 88, 78,
  89, 208, 85, 33,
  107, 255, 29, 11,
  141, 137, 31, 39,
  255,  59, 33, 89
};
 
// Gradient palette "Fuschia_7_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ds/fuschia/tn/Fuschia-7.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Fuschia_7_gp ) {
  0,  43,  3, 153,
  63, 100,  4, 103,
  127, 188,  5, 66,
  191, 161, 11, 115,
  255, 135, 20, 182
};
 
 
// Gradient palette "lava_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/neota/elem/tn/lava.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( lava_gp ) {
  0,   0,  0,  0,
  46,  18,  0,  0,
  96, 113,  0,  0,
  108, 142,  3,  1,
  119, 175, 17,  1,
  146, 213, 44,  2,
  174, 255, 82,  4,
  188, 255, 115,  4,
  202, 255, 156,  4,
  218, 255, 203,  4,
  234, 255, 255,  4,
  244, 255, 255, 71,
  255, 255, 255, 255
};
 
 
// Gradient palette "Colorfull_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Colorfull.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Colorfull_gp ) {
  0,  10, 85,  5,
  25,  29, 109, 18,
  60,  59, 138, 42,
  93,  83, 99, 52,
  106, 110, 66, 64,
  109, 123, 49, 65,
  113, 139, 35, 66,
  116, 192, 117, 98,
  124, 255, 255, 137,
  168, 100, 180, 155,
  255,  22, 121, 174
};
 

 
// Gradient palette "Pink_Purple_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Pink_Purple.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Pink_Purple_gp ) {
  0,  19,  2, 39,
  25,  26,  4, 45,
  51,  33,  6, 52,
  76,  68, 62, 125,
  102, 118, 187, 240,
  109, 163, 215, 247,
  114, 217, 244, 255,
  122, 159, 149, 221,
  149, 113, 78, 188,
  183, 128, 57, 155,
  255, 146, 40, 123
};
 
// Gradient palette "Sunset_Real_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/atmospheric/tn/Sunset_Real.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Sunset_Real_gp ) {
  0, 120,  0,  0,
  22, 179, 22,  0,
  51, 255, 104,  0,
  85, 167, 22, 18,
  135, 100,  0, 103,
  198,  16,  0, 130,
  255,   0,  0, 160
};
 

 
// Gradient palette "BlacK_Blue_Magenta_White_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Blue_Magenta_White.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( BlacK_Blue_Magenta_White_gp ) {
  0,   0,  0,  0,
  42,   0,  0, 45,
  84,   0,  0, 255,
  127,  42,  0, 255,
  170, 255,  0, 255,
  212, 255, 55, 255,
  255, 255, 255, 255
};
 
// Gradient palette "BlacK_Magenta_Red_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Magenta_Red.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( BlacK_Magenta_Red_gp ) {
  0,   0,  0,  0,
  63,  42,  0, 45,
  127, 255,  0, 255,
  191, 255,  0, 45,
  255, 255,  0,  0
};
 
// Gradient palette "BlacK_Red_Magenta_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/BlacK_Red_Magenta_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( BlacK_Red_Magenta_Yellow_gp ) {
  0,   0,  0,  0,
  42,  42,  0,  0,
  84, 255,  0,  0,
  127, 255,  0, 45,
  170, 255,  0, 255,
  212, 255, 55, 45,
  255, 255, 255,  0
};
 
// Gradient palette "Blue_Cyan_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Blue_Cyan_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.
 
DEFINE_GRADIENT_PALETTE( Blue_Cyan_Yellow_gp ) {
  0,   0,  0, 255,
  63,   0, 55, 255,
  127,   0, 255, 255,
  191,  42, 255, 45,
  255, 255, 255,  0
};

// Gradient palette "ib11_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general/tn/ib11.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( ib11_gp ) {
    0, 252, 22,  0,
   42, 255,100,  1,
   53, 255,244,  1,
   99,  42,191, 45,
  119,   0,144,247,
  186,  91,  1,199,
  219, 247,  1,  0,
  255,  34,255,  1};


// Gradient palette "pastel_rb3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/classic/tn/pastel-rb3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( pastel_rb3_gp ) {
    0,  54,138,170,
   33,  58,186,109,
   63, 173,239, 83,
   89, 229,201, 61,
  119, 249,127, 29,
  140, 252,103, 56,
  165, 232, 72,155,
  193, 206,108,219,
  219, 169,128,233,
  242,  95,139,205,
  255,  95,139,205};

// Gradient palette "huetint_cyan_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/classic/tn/huetint-cyan.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( huetint_cyan_gp ) {
    0,   0,  0,  0,
  127,   3,178,151,
  255, 255,255,255};

// Gradient palette "moonbath_2_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/1/tn/moonbath-2.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 48 bytes of program space.

DEFINE_GRADIENT_PALETTE( moonbath_2_gp ) {
    0, 118,161,226,
   25, 182,221,207,
   51, 171,146,219,
   76, 140,178,210,
   96, 177,211,201,
  127, 112,146,226,
  150, 182,213,197,
  170, 146,164,205,
  193, 142,156,214,
  214, 169,176,219,
  234, 179,152,223,
  255, 128,166,201};


// Gradient palette "meadow_T_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/1/tn/meadow-T.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE( meadow_T_gp ) {
    0,   1,239, 49,
   20,   1, 50,  1,
   56,   1,239, 49,
   76,  40,164, 69,
  101,  50,166, 19,
  127, 125,203,160,
  153,  34,141, 52,
  183,   1, 50,  1,
  201,   1,105, 21,
  216, 109,195, 10,
  234,  61,166, 16,
  252,   1,239, 49,
  255,   1,239, 49};

// Gradient palette "tropicisle_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/tropicisle.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( tropicisle_gp ) {
    0, 255,250,216,
   35,   6,229,240,
   96,   6,119,240,
  124,  10, 73,  9,
  153, 239,255, 20,
  178, 239,161,  7,
  206,   6,119,240,
  252, 255,250,216,
  255, 255,250,216};

// Gradient palette "orb1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/orb1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( orb1_gp ) {
    0, 249,250,245,
   15, 232,244,240,
   38, 175,171,226,
   66,  79,146,188,
   94, 194,252,240,
  122,  57,173,142,
  153, 137,128,221,
  181,  72, 91,233,
  209, 249,199,255,
  234, 155,114,242,
  255, 249,250,245};

// Gradient palette "peacerose2_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/peacerose2.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( peacerose2_gp ) {
    0, 255,255,255,
   48, 255,255,125,
  229, 217,  4, 51,
  252, 125,  5,  5,
  255, 125,  5,  5};

// Gradient palette "tranquil_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/tranquil.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE( tranquil_gp ) {
    0, 128,166,201,
   28, 194,146,197,
  112, 242,149,207,
  135,  58,135,186,
  170,  39,156,221,
  198,  50,189,207,
  229,  26,128,105,
  252, 128,166,201,
  255, 128,166,201};

// Gradient palette "xanidu_2_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/3/tn/xanidu-2.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( xanidu_2_gp ) {
    0, 118,161,226,
   53,  79,  1,162,
   94,  67,  1,  7,
  132,   1, 55,156,
  173,   1,127, 61,
  211,  39, 45, 72,
  255, 118,161,226};
// Gradient palette "picnicweather_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/3/tn/picnicweather.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( picnicweather_gp ) {
    0, 217, 32,  1,
   61, 255,171,242,
  101,   1, 65,174,
  145, 146,195,155,
  175,   1, 65,174,
  209, 255,171,242,
  247, 217, 32,  1,
  255, 217, 32,  1};
// Gradient palette "keet_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/4/tn/keet.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE( keet_gp ) {
    0, 132,159,186,
   28, 123,227,255,
   58,  86,246,255,
   89,  39, 73, 84,
  119,  77,147,158,
  150,  33, 52, 56,
  183,  34,157,164,
  211,   4,161,160,
  242, 182,207,245,
  255, 182,207,245};
// Gradient palette "water1_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/4/tn/water1.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( water1_gp ) {
    0,   1,  1, 46,
   28,  79,136,190,
   53,  16, 81,184,
   73,  79,136,190,
   99,  75,201,233,
  124,   6, 12,103,
  158,  75,201,233,
  186,  84,109,108,
  198,  16, 81,184,
  232,  79,136,190,
  255,   1,  1, 46};
  
// Gradient palette "tropical_beach_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/5/tn/tropical-beach.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE( tropical_beach_gp ) {
    0,   1, 61, 85,
   38,   9,176,255,
   63, 255, 97,  5,
   96, 255, 26, 19,
  130, 206,201,  6,
  165, 255, 29, 16,
  191, 255, 97,  5,
  221,   9,176,255,
  247,   1, 61, 85,
  255,   1, 61, 85};

// Gradient palette "tropical_sun_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/5/tn/tropical-sun.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 84 bytes of program space.

DEFINE_GRADIENT_PALETTE( tropical_sun_gp ) {
    0, 213, 65, 42,
   20, 247, 36, 11,
   33, 247, 38, 11,
   45, 213, 65, 42,
   58, 247, 34, 10,
   71, 242, 66, 37,
   81, 247, 36, 11,
   96, 247, 58, 14,
  112, 247, 49, 14,
  119, 239,112, 16,
  130, 247, 79, 19,
  147, 237,117, 17,
  160, 239,111, 16,
  168, 239,161, 21,
  175, 237,122, 16,
  193, 239,117, 16,
  206, 239,161, 21,
  221, 234,161, 19,
  234, 220,141, 43,
  244, 220,151, 68,
  255, 220,151, 68};
// Gradient palette "faewing3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/6/tn/faewing3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( faewing3_gp ) {
    0, 206,149,133,
   40, 190,  8, 61,
   76,  45,  7, 85,
  101,   9, 18,130,
  140,   7,162,138,
  188, 153,219,194,
  229,   7,162,138,
  255,   7,162,138};
// Gradient palette "slyvana_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/6/tn/slyvana.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE( slyvana_gp ) {
    0,   7,162,138,
   63, 232,241,203,
  117,  82,205, 92,
  170,   7,162,138,
  196, 232,241,203,
  244,   2, 48, 95,
  255,   2, 48, 95};
// Gradient palette "springlife2_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/6/tn/springlife2.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE( springlife2_gp ) {
    0, 182,207,166,
   28, 182,209,109,
   51, 210,227, 18,
   76, 117,171, 31,
  117,  16, 73, 52,
  145,   7, 46, 18,
  163,  22, 74,  4,
  188,  24, 55,  8,
  237, 117,171, 31,
  255, 182,207,166};
// Gradient palette "omageerte3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/6/tn/omageerte3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 32 bytes of program space.

DEFINE_GRADIENT_PALETTE( omageerte3_gp ) {
    0,  87,175, 60,
   45, 132,107,205,
   96, 210,133,149,
  142, 229, 22, 51,
  175, 137,199,245,
  226, 144,225, 56,
  249,  87,175, 60,
  255,  87,175, 60};
// Gradient palette "calmsea_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/7/tn/calmsea.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( calmsea_gp ) {
    0, 103,130,247,
   63, 110,189,221,
  142, 115,178, 60,
  186,  27,164,255,
  239, 103,130,247,
  255, 103,130,247};
// Gradient palette "firstlove_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/7/tn/firstlove.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 40 bytes of program space.

DEFINE_GRADIENT_PALETTE( firstlove_gp ) {
    0, 249, 63, 72,
   35, 155, 81,192,
   79, 249,108, 93,
   99, 247, 33,100,
  119, 247, 59, 80,
  153, 188, 70,106,
  186, 137, 26, 99,
  211, 249,195,212,
  242, 247, 33,100,
  255, 247, 33,100};
// Gradient palette "bhw1_05_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_05.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.


 
// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly
// by name every time.
// Since it is const, this array could also be moved
// into PROGMEM to save SRAM, but for simplicity of illustration
// we'll keep it in a regular SRAM array.
//
// This list of color palettes acts as a "playlist"; you can
// add or delete, or re-arrange as you wish.
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {

  pastel_rb3_gp,
  huetint_cyan_gp,
  moonbath_2_gp,
  meadow_T_gp,
  tropicisle_gp,
  orb1_gp,
  peacerose2_gp,
  tranquil_gp,
  xanidu_2_gp,
  picnicweather_gp,
  keet_gp,
  water1_gp,
  tropical_beach_gp,
  tropical_sun_gp,
  faewing3_gp,
  slyvana_gp,
  springlife2_gp,
  omageerte3_gp,
  calmsea_gp,
  firstlove_gp,
  
};
 
 
// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount =
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
