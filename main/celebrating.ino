
MeBuzzer buzzer;

#define B0  31
#define C1  33
#define CS1 35
#define D1  37
#define DS1 39
#define E1  41
#define F1  44
#define FS1 46
#define G1  49
#define GS1 52
#define A1  55
#define AS1 58
#define B1  62
#define C2  65
#define CS2 69
#define D2  73
#define DS2 78
#define E2  82
#define F2  87
#define FS2 93
#define G2  98
#define GS2 104
#define A2  110
#define AS2 117
#define B2  123
#define C3  131
#define CS3 139
#define D3  147
#define DS3 156
#define E3  165
#define F3  175
#define FS3 185
#define G3  196
#define GS3 208
#define A3  220
#define AS3 233
#define B3  247
#define C4  262
#define CS4 277
#define D4  294
#define DS4 311
#define E4  330
#define F4  349
#define FS4 370
#define G4  392
#define GS4 415
#define A4  440
#define AS4 466
#define B4  494
#define C5  523
#define CS5 554
#define D5  587
#define DS5 622
#define E5  659
#define F5  698
#define FS5 740
#define G5  784
#define GS5 831
#define A5  880
#define AS5 932
#define B5  988
#define C6  1047
#define CS6 1109
#define D6  1175
#define DS6 1245
#define E6  1319
#define F6  1397
#define FS6 1480
#define G6  1568
#define GS6 1661
#define A6  1760
#define AS6 1865
#define B6  1976
#define C7  2093
#define CS7 2217
#define D7  2349
#define DS7 2489
#define E7  2637
#define F7  2794
#define FS7 2960
#define G7  3136
#define GS7 3322
#define A7  3520
#define AS7 3729
#define B7  3951
#define C8  4186
#define CS8 4435
#define D8  4699
#define DS8 4978
//note value definitions, S is sharp
#define NOTE_VALUE 2
//1 for crochet
//2 for quaver
//4 for semiquaver
#define TOTAL_NOTES 96
//take from excel
#define TEMPO 200 //in crochet = BPM
//currently cannot handle tuplets and tempo changes

int score[TOTAL_NOTES] = {D5, A5, D6, 0, 0, 0, D5, A5, D6, 0, 0, 0, E6, 0, 0, F6, E6, F6, E6, C5, A5, 0, 0, 0, A6, 0, D5, 0, F5, G5, A6, 0, 0, 0, 0, 0, A6, 0, D5, 0, F5, G5, E5, 0, 0, 0, 0, 0, D5, A5, D6, 0, 0, 0, D5, A5, D6, 0, 0, 0, E6, 0, 0, F6, E6, F6, E6, C5, A5, 0, 0, 0, A6, 0, D5, 0, F5, G5, A6, 0, 0, 0, A6, 0, D5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//song of storms zelda

void do_rest(int *i, int score[], int beat_duration, int total_notes) {
    int rest_beats = 0;
    while (*i < total_notes && score[(*i)] == 0) {
        rest_beats++;
        (*i)++;
    }
    delay((int)(rest_beats * beat_duration));
}

void do_play_note(int *i, int score[], int beat_duration, int total_notes) {
    int play_beats = 1;
    int note_freq = score[(*i)];
   
    if ((*i) + 1 == total_notes) {
        //edge case
        buzzer.tone(note_freq, beat_duration);
        (*i)++;
        return;
    }//else
    //shift to the next note
    (*i)++;
   
    while (*i < total_notes && score[(*i)] == 0) {
        (*i)++;
        play_beats++;
    }// either ends on another note or a -1 terminate
   
    if (score[(*i)] == -1) {
        (*i)++;
        play_beats++;
    }
   
    buzzer.tone(note_freq, play_beats * beat_duration);
}

void read_score(int score[]) {
    int i = 0;
    int total_notes = TOTAL_NOTES;
    float beat_duration = (60.0/TEMPO)/NOTE_VALUE * 1000;
    while (i < total_notes) {
        if (score[i] == 0) {
            do_rest(&i, score, (int)beat_duration, total_notes);
        } else {
            do_play_note(&i, score, (int)beat_duration, total_notes);
        }
    }
}

void celebrate() {
  read_score(score);
}
