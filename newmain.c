/*
 * Cofee alarm
 * For PIC10F322
 */


#include <xc.h>
#include <stdint.h>

// CONFIG
#pragma config FOSC = INTOSC    // Oscillator Selection bits (INTOSC oscillator: CLKIN function disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
#pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)
#pragma config LPBOR = OFF      // Brown-out Reset Selection bits (BOR disabled)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)

#define _XTAL_FREQ 16000000

typedef struct State {
  struct State *(*fire)(struct State *this);
  uint16_t counter0;
} State;

static State * onInit(State *);
static State * onSilent(State *);

static State initState = {
    onInit, 0
};

static State silentState = {
    onSilent, 0
};

static State * onInit(State *this) {
    if (++this->counter0 > 70) {
      this->counter0 = 0;
      return &silentState;
    }
    RA2 = ~RA2;
    return this;
}

static State * onSilent(State *this) {
    if (++this->counter0 > 15000) {
      this->counter0 = 0;
      return &initState;
    }
    RA2 = 0;
    return this;
}

int main(void) {
    TRISA = 0b000;//Output
    ANSELA = 0b000;//for Digital I/O
    OSCCONbits.IRCF = 0b001;//250kMHz
    State *currentState = &initState;

    for (;;) {
        currentState = currentState->fire(currentState);
    }
}