/*
 * ATTiny Sound Generator Test sketch for Arduino Due
 *
 * Created: 31.07.2017 11:14:31
 * Author : Tomasz Jaworski
 */ 

#define SOUND_CLOCK  12
#define SOUND_DATA 13

void soundInit(void)
{
  pinMode(SOUND_DATA, OUTPUT);
  pinMode(SOUND_CLOCK, OUTPUT);

  digitalWrite(SOUND_CLOCK, 0);
  digitalWrite(SOUND_DATA, 0);

REG_PIOB_OER = 0x90000000;
REG_PIOB_PER = 0x90000000;

  
  delay(1);

}

#define C 261
#define D 294
#define E 330
#define F 349
#define G 392
#define A 440
#define H 494
#define C2 523

int16_t s1[] = 
{
  C, 2, D, 2, E, 2, F, 2,
  G, 1, E, 1,
  G, 1, E, 1, 
  E, 2, E, 2, D, 2, D, 2,
  C, 1,
  0, 0,
};

void sound(int16_t freq)
{
  // przelicz wartosc
  /*
   *  1   1 8MHz 1
   * ---- - ---- -
   * freq 2  8   2
   */

  uint16_t value = 250000L / freq;

  
  for (int i = 0; i < 16; i++)
  {

    digitalWrite(SOUND_DATA, value & 0x8000);
    if (value&0x8000)
      REG_PIOB_SODR = 0x10000000;
    else
      REG_PIOB_CODR = 0x10000000;
    
    value <<= 1;

    // pulse the data
    digitalWrite(SOUND_CLOCK, 1);
    REG_PIOB_SODR = 0x80000000;

    delayMicroseconds(100);
    REG_PIOB_CODR = 0x80000000;
    digitalWrite(SOUND_CLOCK, 0);
  }
}

void noSound(void)
{
  sound(0);
}


void setup() {
  // put your setup code here, to run once:

  soundInit();
}


void loop()
{
  // taken from http://www.elektroda.pl/rtvforum/topic1812254.html
  
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (262); 
  delay (150);
  sound (440); 
  delay (150);
  sound (294); 
  delay (150);
  sound (440); 
  delay (150);
  sound (330); 
  delay (150);
  sound (294); 
  delay (150);
  sound (262); 
  delay (150);
  sound (262); 
  delay (150);
  sound (330); 
  delay (150);
  sound (262); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  sound (330); 
  delay (150);
  sound (262); 
  delay (150);
  sound (392); 
  delay (150);
  sound (392); 
  delay (150);
  sound (494); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  sound (392); 
  delay (150);
  sound (294); 
  delay (150);
  sound (262); 
  delay (150);
  sound (349); 
  delay (150);
  sound (349); 
  delay (150);
  sound (440); 
  delay (150);
  sound (349); 
  delay (150);
  sound (262); 
  delay (150);
  sound (349); 
  delay (150);
  sound (494); 
  delay (150);
  sound (262); 
  delay (150);
  //druga zwrotka
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (262); 
  delay (150);
  sound (440); 
  delay (150);
  sound (294); 
  delay (150);
  sound (440); 
  delay (150);
  sound (330); 
  delay (150);
  sound (294); 
  delay (150);
  sound (262); 
  delay (150);
  sound (262); 
  delay (150);
  sound (330); 
  delay (150);
  sound (262); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  sound (330); 
  delay (150);
  sound (262); 
  delay (150);
  sound (392); 
  delay (150);
  sound (392); 
  delay (150);
  sound (494); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  sound (392); 
  delay (150);
  sound (294); 
  delay (150);
  sound (262); 
  delay (150);
  sound (349); 
  delay (150);
  sound (349); 
  delay (150);
  sound (440); 
  delay (150);
  sound (349); 
  delay (150);
  sound (262); 
  delay (150);
  sound (349); 
  delay (150);
  sound (494); 
  delay (150);
  sound (262); 
  delay (150);
  
  //{refren}
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (392); 
  delay (150);
  sound (330); 
  delay (150);
  
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (392); 
  delay (150);
  sound (262); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  sound (440); 
  delay (150);
  

}

void loop1() {
  // put your main code here, to run repeatedly:

  for (int i = 0; s1[i] != 0; i += 2)
  {
    sound(s1[i]);
    delay(400/s1[i+1]);
    noSound();
    delay(40);
  }

  noSound();

  delay(4000);
  
}
  

