
const int softpotPin = A0;
//const int buttonPlayPin = 12;
//const int buttonRecordPin = 11;
const int speakerPin = 10;
const int ledPins[] = {9, 8, 7, 6, 5, 4, 3, 2};

int tempo = 300;

volatile int playingState = LOW;

// Soundtrack 1
int st1_tempo = 300;
int st1_length = 15;
char st1_notes[] = "ccggaagffeeddc";
int st1_beats[] = {1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 2, 4};

// Soundtrack 2
int st2_tempo = 150;
int st2_length = 32;
char st2_notes[] = "cdeccdecefgefggagfecgagfecd5cd5c";
int st2_beats[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 4, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 4, 2, 2, 4};

// Play the tone(Hz) for a duration(ms)
void playTone(int tone, int duration)
{
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(speakerPin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speakerPin, LOW);
        delayMicroseconds(tone);
    }
}

// Play the note for a duration(ms)
void playNote(char note, int duration)
{
    char names[] = {'5', '6', '7', 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
    int tones[] = {2551, 2273, 2024, 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};
    
    for (int i = 0; i < 11; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}

// Play preset soundtracks
void playTrack() {
    playingState = !playingState;
}

void setup()
{
    pinMode(speakerPin, OUTPUT);
    
    pinMode(ledPins[3], OUTPUT);
    pinMode(ledPins[6], OUTPUT);
    
    digitalWrite(ledPins[3], HIGH);
    
    attachInterrupt(0, playTrack, FALLING);
    
    Serial.begin(9600);
}

void loop()
{
    int sensorValue = analogRead(softpotPin);
    int toneValue;
    
    if (sensorValue > 9) {
        delay(50);
        sensorValue = analogRead(softpotPin);
        
        if (sensorValue > 9) {
            int temp = log(sensorValue) / log(10) * 1000;
            toneValue = constrain(map(temp, 1000, 3000, 2000, 900), 900, 2000);
            
            playTone(toneValue, tempo);
            Serial.println(toneValue);
        }
    }
    
    if (playingState) {
        digitalWrite(ledPins[3], LOW);
        digitalWrite(ledPins[6], HIGH);
        
        for (int i = 0; i < st1_length; i++) {
            if (st1_notes[i] == ' ') {
                delay(st1_beats[i] * st1_tempo);
            } else {
                playNote(st1_notes[i], st1_beats[i] * st1_tempo);
            }
            
            delay(st1_tempo / 2); // pause between notes
        }
        
        for (int i = 0; i < st1_length; i++) {
            if (st1_notes[i] == ' ') {
                delay(st1_beats[i] * st1_tempo);
            } else {
                playNote(st1_notes[i], st1_beats[i] * st1_tempo);
            }
            
            delay(st1_tempo / 2); // pause between notes
        }
        
        delay(500);
        
        for (int i = 0; i < st2_length; i++) {
            if (st2_notes[i] == ' ') {
                delay(st2_beats[i] * st2_tempo);
            } else {
                playNote(st2_notes[i], st2_beats[i] * st2_tempo);
            }
            
            delay(st2_tempo / 2); // pause between notes
        }
        
        digitalWrite(ledPins[3], HIGH);
        digitalWrite(ledPins[6], LOW);
        
        playingState = LOW;
    }
    
}
