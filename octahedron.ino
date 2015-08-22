#include <octahedron.h>
#include <FastLED.h>

#define LED_PIN 9
#define NUM_LEDS 5
#define BRIGHTNESS          200
#define FRAMES_PER_SECOND   120

CRGB leds[NUM_LEDS];

//CRGBPalette16 curPalette;
//TBlendType    curBlending;
uint8_t colorIndex = 0;

// Noise function parameters
uint32_t x,time,hue_x;  // x, & time values
uint8_t octaves=4;  // how many octaves to use for the brightness function
uint8_t hue_octaves=2;  // how many octaves to use for the hue function
int scale=30;  // the 'distance' between points on the x axis
int hue_scale=2;  // the 'distance' between x points for the hue noise
int time_speed=10;  // how fast we move through time & hue noise
int x_speed=400; // adjust these values to move along the x axis between frames

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int8_t vertexSides[][4] = {{1,5,-4,-12},
                     {-2,7,3,-6},
                     {-1,-8,2,9},
                     {-3,11,4,-10},
                     {-5,-9,6,10},
                     {-11,12,8,-7}};
typedef polygon vertex_set[];
vertex_set vertices = {polygon(vertexSides[0],ARRAY_SIZE(vertexSides[0])),
                       polygon(vertexSides[1],ARRAY_SIZE(vertexSides[1])),
                       polygon(vertexSides[2],ARRAY_SIZE(vertexSides[2])),
                       polygon(vertexSides[3],ARRAY_SIZE(vertexSides[3])),
                       polygon(vertexSides[4],ARRAY_SIZE(vertexSides[4])),
                       polygon(vertexSides[5],ARRAY_SIZE(vertexSides[5]))};
                       
uint8_t vIndex = 0;

int8_t symmetric[] = {-1,5,-4,12,-2,7,3,-6,-11,8,-9,10};
polygon octahedron(symmetric,12);

void setup() {
  delay(3000);
//  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
//  FastLED.setDither(0);

//  curBlending = BLEND;

  // initialize the x, hue_x and v_time values
  random16_set_seed(8934);
  random16_add_entropy(analogRead(2));
  hue_x = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
  x = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
  time = (uint32_t)((uint32_t)random16() << 16) + (uint32_t)random16();
}

void loop() {
  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 
  
//  juggle(vertices[vIndex]);
//  pallete_fill(vertices[vIndex]);

  edgeNoise(octahedron, octaves, x, scale, hue_octaves, hue_x, hue_scale, time);
//  edgeNoise(vertices[vIndex], octaves, x, scale, hue_octaves, hue_x, hue_scale, time);

//  confetti();

//  EVERY_N_SECONDS( 10 )
//  {
//    vIndex++;
//    if (vIndex == 3) {
//      vIndex = 0;
//    }
//  }
  
  // adjust the intra-frame time values
  x += x_speed;
  time += time_speed;

  colorIndex++;
}

void pallete_fill(polygon z) {
  CRGBPalette16 palette = RainbowColors_p;
  for(int i = 0; i < z.length(); i++) {
    leds[z.parIndex(i)] = ColorFromPalette(palette, colorIndex+(z.parIndex(i)*2), 200);
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( colorIndex + random8(64), 200, 255);
}

void sinelon(polygon z)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16(60,0,z.length());
  leds[z.parIndex(pos)] += CHSV( colorIndex, 255, 192);
}

void bpm(polygon z) {
  uint8_t BeatsPerMinute = 63;
  CRGBPalette16 palette = ForestColors_p;
//  CRGBPalette16 palette = PartyColors_p;
//  CRGBPalette16 palette = RainbowStripeColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < z.length(); i++) {
    leds[z.parIndex(i)] = ColorFromPalette(palette, colorIndex+(z.parIndex(i)*2), beat-colorIndex+(z.parIndex(i)*10));
  }
}

void juggle(polygon z) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 10);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[z.parIndex(beatsin16(i+7,0,z.length()))] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}

void edgeNoise(polygon z, uint8_t octaves, uint16_t x, int scale, uint8_t hue_octaves, 
               uint16_t hue_x, int hue_scale, uint16_t time) {
  uint8_t num_leds = z.length();

  uint8_t V[num_leds];
  uint8_t H[num_leds];
  
  memset(V,0,z.length());
  memset(H,0,z.length());
  
  fill_raw_noise16into8(V,num_leds,octaves,x,scale,time);
  fill_raw_noise8(H,num_leds,hue_octaves,hue_x,hue_scale,time);
  
  for (uint8_t i = 0; i < num_leds; i++) {
    leds[z.parIndex(i)] = CHSV(H[i]+colorIndex,255,qadd8(V[i],64));
  }
}
