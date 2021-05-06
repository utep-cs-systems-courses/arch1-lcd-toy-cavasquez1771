#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"


u_char width = screenWidth, height = screenHeight;
short redrawScreen = 0;

void wdt_c_handler()
{  
}
/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  enableWDTInterrupts();
  or_sr(0x8);/* GIE (enable interrupts) */
 
  int startCol = 100;
  clearScreen(COLOR_BLUE);

  drawString11x16(20,20, "hey", COLOR_GREEN, COLOR_BLUE);

  drawFish(startCol, 60, 15, COLOR_RED);
  drawDiamond(60, 60, 15, COLOR_YELLOW);

  while(1){
    startCol -= 5;
    clearScreen(COLOR_BLUE);
    drawFish(startCol, 60, 15, COLOR_RED);
    
    



    
  }
}
