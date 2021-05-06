#include <msp430.h>
#include "p2switches.h"

static unsigned char switch_mask;
static unsigned char switches_last_reported;
static unsigned char switches_current;

static void
switch_update_interrupt_sense()
{
  switches_current = P2IN & switch_mask;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (switches_current);  /* if switch up, sense down */
  P2IES &= (switches_current | ~switch_mask); /* if switch down, sense up */
}

void 
p2sw_init(unsigned char mask)
{
  switch_mask = mask;
  P2REN |= mask;    /* enables resistors for switches */
  P2IE = mask;      /* enable interrupts from switches */
  P2OUT |= mask;    /* pull-ups for switches */
  P2DIR &= ~mask;   /* set switches' bits for input */

  switch_update_interrupt_sense();
}

void

switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();

  switch1_state_down = (p2val & SW1) ? 0 : 1; /* 0 when SW1 is up */
  switch2_state_down = (p2val & SW2) ? 0 : 1; /* 0 when SW2 is up */
  switch3_state_down = (p2val & SW3) ? 0 : 1; /* 0 when SW3 is up */
  switch4_state_down = (p2val & SW4) ? 0 : 1; /* 0 when SW4 is up */

  switch_state_changed = 1;

  switch_state_down = switch1_state_down || switch2_state_down || switch3_state_down;

  if(switch_state_down) redrawScreen = 1;
}

/* Returns a word where:
 * the high-order byte is the buttons that have changed,
 * the low-order byte is the current state of the buttons
 */
unsigned int 
p2sw_read() {
  unsigned int sw_changed = switches_current ^ switches_last_reported;
  switches_last_reported = switches_current;
  return switches_current | (sw_changed << 8);
}

/* Switch on P2 (S1) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & switch_mask) {  /* did a button cause this interrupt? */
    P2IFG &= ~switch_mask;	/* clear pending sw interrupts */
    switch_update_interrupt_sense();
  }
}
