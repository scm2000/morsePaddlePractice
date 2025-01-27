#include <Arduino.h>
#include <Keyboard.h>
#include <map>
#include <string>
#include "main.h"


//// Wiring for Stero Jack
///
// Micro         Jack
// -----|     _____________
//      |     |           |
// GND  |-----|sleeve      \
// GPIO |-----|tip          |
// GPIO |-----|ring        /
// -----|     |___________|

// Wiring for active buzzer
//
// Micro          Module
// -----|      _______________
// 3.3v |-----| Vcc          |
// GPIO |-----| IO           |
// GND  |-----| GND          |
// -----|     |______________|

unsigned long WPM = 15;
unsigned long dotLen = 1200 / WPM;
unsigned long dashLen = 3 * dotLen;
unsigned long intraCharLen = dotLen;
unsigned long interLetterLen = 3 * dotLen;
unsigned long interWordLen = 7 * dotLen;
uint8_t tonePin = 2;
unsigned int toneFrequency = 700;

uint8_t dotPin = 3;
uint8_t dashPin = 4;

std::map<std::string, char> morseToChar = {
    {".-", 'A'}, {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'}, {".", 'E'}, {"..-.", 'F'}, {"--.", 'G'}, {"....", 'H'}, {"..", 'I'}, {".---", 'J'}, {"-.-", 'K'}, {".-..", 'L'}, {"--", 'M'}, {"-.", 'N'}, {"---", 'O'}, {".--.", 'P'}, {"--.-", 'Q'}, {".-.", 'R'}, {"...", 'S'}, {"-", 'T'}, {"..-", 'U'}, {"...-", 'V'}, {".--", 'W'}, {"-..-", 'X'}, {"-.--", 'Y'}, {"--..", 'Z'}};

std::map<char, const char *> charToMorse{
    {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."}, {'E', "."}, {'F', "..-."}, {'G', "--."}, {'H', "...."}, {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."}, {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."}, {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"}, {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"}, {'Y', "-.--"}, {'Z', "--.."}

};

const char *words[100] = {
    "THE", "OF", "AND", "TO", "A", "IN", "IS", "IT", "YOU", "THAT",
    "HE", "WAS", "FOR", "ON", "ARE", "AS", "WITH", "HIS", "THEY", "I",
    "AT", "BE", "THIS", "HAVE", "FROM", "OR", "ONE", "HAD", "BY", "WORD",
    "BUT", "NOT", "WHAT", "ALL", "WERE", "WE", "WHEN", "YOUR", "CAN", "SAID",
    "THERE", "USE", "AN", "EACH", "WHICH", "SHE", "DO", "HOW", "THEIR", "IF",
    "WILL", "UP", "OTHER", "ABOUT", "OUT", "MANY", "THEN", "THEM", "THESE", "SO",
    "SOME", "HER", "WOULD", "MAKE", "LIKE", "HIM", "INTO", "TIME", "HAS", "LOOK",
    "TWO", "MORE", "WRITE", "GO", "SEE", "NUMBER", "NO", "WAY", "COULD", "PEOPLE",
    "MY", "THAN", "FIRST", "WATER", "BEEN", "CALL", "WHO", "OIL", "ITS", "NOW",
    "FIND", "LONG", "DOWN", "DAY", "DID", "GET", "COME", "MADE", "MAY", "PART"};

void playDot()
{
  tone(tonePin, 500);
  // digitalWrite(tonePin, 0);
  delay(dotLen);
  noTone(tonePin);
  digitalWrite(tonePin, 1);
}

void playDash()
{
  tone(tonePin, 500);
  delay(dashLen);
  noTone(tonePin);
  digitalWrite(tonePin, 1);
}

void playIntraCharDelay()
{
  delay(intraCharLen);
}
void playInterCharDelay()
{
  delay(interLetterLen);
}
void playInterWordDelay()
{
  delay(interWordLen);
}

void playMorse(const char *morse)
{
  Serial.print("playMorse ");
  Serial.println(morse);

  while (*morse)
  {
    playIntraCharDelay();
    if (*morse++ == '.')
    {
      playDot();
    }
    else
    {
      playDash();
    }
  }
  playInterCharDelay();
}
const char *testWord;
void playOutAWord()
{
  Serial.print("playOutAWord ");
  const char *wrd = words[random(100)];
  Serial.println(wrd);

  testWord = wrd;
  playMorseForWord(wrd);
}

void playMorseForWord(const char *wrd)
{
  while (*wrd)
  {
    char c = *wrd++;
    const char *morseForChar = charToMorse[c];
    playMorse(morseForChar);
  }
}
void setup()
{
  randomSeed(analogRead(A0));

  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Hello");
  pinMode(tonePin, OUTPUT);
  digitalWrite(tonePin, 1);

  pinMode(dotPin, INPUT_PULLUP);
  pinMode(dashPin, INPUT_PULLUP);

  playMorseForWord("VVV");
}

unsigned long lastSymTime = 0;

std::string morseChar("");
std::string morseWord("");

bool wasPlaying = false;
int lastWas = 0;
bool wordPracticeMode = false;
bool wordInProgress = true;
void loop()
{
  if (wordInProgress && millis() - lastSymTime >= interWordLen)
  {
    wordInProgress = false;
    if (wordPracticeMode)
    {
      if (morseWord == testWord)
      {
        playMorseForWord("R");
        delay(2000);
        playOutAWord();
      }
      else
      {
        Serial.println();
        delay(1000);
        Serial.println(testWord);
        playMorseForWord(testWord);
      }
    }
    morseWord = "";
    Serial.println();
  }
  if (morseChar.length() && millis() - lastSymTime >= interLetterLen)
  {
    if (morseChar == ".......")
    {
      wordPracticeMode = true;
      morseChar = "";
      morseWord = "";
      delay(1000);
      playOutAWord();
    }
    else if (morseChar == "-------")
    {
      wordPracticeMode = false;
      morseChar = "";
    }
    else
    {
      wordInProgress = true;
      char c = morseToChar[morseChar];
      Serial.print(c);
      morseChar = "";
      morseWord += c;
    }
  }

  bool dotOn = digitalRead(dotPin) == 0;
  bool dashOn = digitalRead(dashPin) == 0;
  if (dotOn && dashOn)
  {
    // Serial.println("both");
    if (wasPlaying)
    {
      playIntraCharDelay();
    }
    wasPlaying = true;

    if (lastWas == 0)
    {
      lastWas = 1;
      playDash();
      morseChar += "-";
      lastSymTime = millis();
    }
    else
    {
      lastWas = 0;
      playDot();
      morseChar += ".";
      lastSymTime = millis();
    }
  }
  else if (dotOn)
  {
    // Serial.println("Dot");
    if (wasPlaying)
    {
      playIntraCharDelay();
    }
    wasPlaying = true;
    playDot();
    lastWas = 0;
    morseChar += ".";
    lastSymTime = millis();
  }
  else if (dashOn)
  {
    // Serial.println("Dash");
    if (wasPlaying)
    {
      playIntraCharDelay();
    }
    wasPlaying = true;
    playDash();
    lastWas = 1;
    morseChar += "-";
    lastSymTime = millis();
  }
  else
  {
    wasPlaying = false;
    lastWas = 1;
  }
}
