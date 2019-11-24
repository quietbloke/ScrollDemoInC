#include "Scroller.h"
#include "layer2.h"
#include "defines.h"

Scroller scrollers[5];

void Scroller_Init()
{
  scrollers[0].fontBank   = fontBankStart;
  scrollers[0].xTile      = 0;
  scrollers[0].yTile      = 0;
  scrollers[0].charPos    = 0;       // Currently selected character
  scrollers[0].charXPos   = 0;      // Position within the current character
  scrollers[0].scrollSize = 1;
  scrollers[0].scrXPos    = scrollers[0].scrollSize;       // Used by copper to set xOffset ??
  scrollers[0].pauseTime  = 12 * 256 + 96;
  scrollers[0].message    = "\"FLASH\" HERE WELCOMING YOU ALL TO OUR LATEST CODING DEMO 'SCROLLNUTTER'. THIS DEMO USES A HOST OF THE FEATURES OF THE WONDERFUL \"NEXT\", INCLUDING SCROLLING, MEMORY BANKING, COPPER, DMA, AUDIO, LORES,SPRITES, PRIORITIES, AND SOME OF OTHER THINGS. HOPEFULLY THE CODE SHARED WITH THIS DEMO IS EASY TO FOLLOW AND WILL HELP SOME OF YOU TO GET STARTED CODING THE GLORIOUS \"NEXT\". ANY ISSUES? FEEL FREE TO YELL ON THE FORUM OR ON THE FB PAGE....       ";

  scrollers[1].fontBank   = font2BankStart;
  scrollers[1].xTile      = 0;
  scrollers[1].yTile      = 1;
  scrollers[1].charPos    = 0;       // Currently selected character
  scrollers[1].charXPos   = 0;      // Position within the current character
  scrollers[1].scrollSize = 2;
  scrollers[1].scrXPos    = scrollers[1].scrollSize;       // Used by copper to set xOffset ??
  scrollers[1].pauseTime  = 15 * 256 + 114;
  scrollers[1].message    = "HI ALL BAGGERS # HERE :) HOPE YOU ALL ENJOY LOOKING THROUGH THIS DEMO CODE WHICH WILL HELP YOU GET TO GRIPS WITH SCROLLING AND USING THE COPPER, AND CHANGING COLOURS. LOOK FOR OUR UPCOMING GAMES WARHAWK AND A SLEW OF BAGGERS GAMES AND DEMOS FOR YOU TO ENJOY! AS 'FLASH' SAID, IF YOU NEED HELP JUST GIVE A POST ON THE FACEBOOK PAGE......       ";

  scrollers[2].fontBank   = font3BankStart;
  scrollers[2].xTile      = 0;
  scrollers[2].yTile      = 2;
  scrollers[2].charPos    = 0;       // Currently selected character
  scrollers[2].charXPos   = 0;      // Position within the current character
  scrollers[2].scrollSize = 4;
  scrollers[2].scrXPos    = scrollers[2].scrollSize;       // Used by copper to set xOffset ??
  scrollers[2].pauseTime  = 19 * 256 + 40;
  scrollers[2].message    = 	"JUST TO FILL THE SCROLLERS UP, HERE IS THE WHIZZY BIT IN THE MIDDLE. THIS SCROLLER IS PRETTY SHY AND NOT TOO TALKATIVE. DON'T SIT TOO CLOSE TO THE SCREEN AS THIS SCROLLER IS ALSO RATHER NERVOUS AND MAY RUN AND HIDE AT THE SLIGHTEST NOISE OR A RATHER INTENT STARE. SO, IT'S PERHAPS BEST TO JUST WRAP THIS SCROLLER BACK TO THE BEGINNING BEFORE IT GOES PEAR-SHAPED......  ACTUALLY, THE FAST SCROLLER HAS JUST SAID THAT IT WOULD RATHER SCROLL A LITTLE LONGER. SEE, THATS THE THING WITH SCROLLERS, EVEN THE SHY ONES JUST WANT TO KEEP GOING... WELL US AT TEAM RUSTY PIXELS WILL NOT ALLOW SUCH DISSENTION IN THE RANKS AND HAVE DECIDED THAT THIS SCROLLER MUST WRAP, IT IS NOT HAPPY, BUT IT IS HAPPENING... IT REALLY IS..... SEE.....         ";

  scrollers[3].fontBank   = font2BankStart;
  scrollers[3].xTile      = 0;
  scrollers[3].yTile      = 3;
  scrollers[3].charPos    = 0;       // Currently selected character
  scrollers[3].charXPos   = 0;      // Position within the current character
  scrollers[3].scrollSize = 2;
  scrollers[3].scrXPos    = scrollers[3].scrollSize;       // Used by copper to set xOffset ??
  scrollers[3].pauseTime  = 32 * 256 + 160;
  scrollers[3].message    = "\"FLASH\" HERE AGAIN, LUMBERED WITH THE 'CREDITS' SCROLLER..... ALL CODE BY TEAM RUSTY PIXELS AND THE USUAL JAZZ, 6 CHANNEL MUSIC IS 'GODDY' BY KARBOFOS, MAIN LOGO GRAPHICS BY MY DEAR DEVLISH PIXEL MASTER LOBO, WHO HAS BEEN WORKING HARD ON OUR GLORIOUS 'WARHAWK NEXT' SHOOTER. THE FONT IMAGES ARE FROM VARIOUS SOURCES AND I AM AFRAID I DO NOT KNOW WHO THE ORIGINAL CREATORS ARE, SO APPOLOGIES.... IT GOES WITHOUT SAYING THAT I WOULD LIKE TO SAY A MASSIVE THANKS TO MY DEAR FRIEND \"JIM BAGLEY #\" FOR HIS SUPPORT AND FOR PUTTING UP WITH ME ALL THIS TIME, AND WITHOUT WHOME THIS DEMO WOULD NOT BE POSSIBLE. I WOULD EQUALLY LIKE TO THANK A DEAR FRIEND 'HEADKAZE' FOR PULLING ME BACK INTO CODING IN 2009 FROM A LONG HIATUS AND GETTING ME BACK ON TRACK, AND ALSO FOR THE JOY IN CREATING THE NINTENDO DS GAMES WE RELEASED. I MUST ALSO MENTION ANOTHER DEAR FRIEND \"SOKURAH\" WHO HAS RELEASED SEVERAL GAMES ON THE VANILLA SPECTRUM AND (I AM SURE) IS GOING TO GIVE US A TREAT OR TWO ON OUR BELOVED NEXT. I WOULD ALSO LIKE TO THANK MY WIFE FOR SOMETHING, THOUGH I AM STRUGGLING TO THINK WHAT? ER.... WELL, SHE DOES COOK A MEAN SANDWICH!.. SOD IT, LET'S WRAP......        ";

  scrollers[4].fontBank   = fontBankStart;
  scrollers[4].xTile      = 0;
  scrollers[4].yTile      = 4;
  scrollers[4].charPos    = 0;       // Currently selected character
  scrollers[4].charXPos   = 0;      // Position within the current character
  scrollers[4].scrollSize = 1;
  scrollers[4].scrXPos    = scrollers[4].scrollSize;       // Used by copper to set xOffset ??
  scrollers[4].pauseTime  = 28 * 256 + 122;
  scrollers[4].message    = "WELCOME TO THE FINAL SCROLLER, YES! I AM PRETTY SURE YOU ALL ARE RATHER TIRED OF READING THESE SCROLLERS... BUT BY ALL MEANS TAKE A GANDER AT THE SOURCE CODE AND SEE HOW EASY IT SHOULD BE TO REUSE THE CODE TO ADD SCROLLERS TO YOUR OWN PROJECTS. THE CODE ALSO INCLUDES TILE PLOT ROUTINES FOR 8, 16, AND 32 PIXEL TILES, SO THEY CAN BE USED FOR LEVEL TILING, TEXT DISPLAY, AND A WHOLE CACOPHONY OF OTHER USES, WELL, A COUPLE MORE AT LEAST...  THOUGH IT HAS BEEN FUN, IT IS TIME TO WRAP THIS LITTLE SCROLLER UP... WRAP BEING THE OPERATIVE WORD, WELL... SEEING AS IT IS GOING TO WRAP, SO A WRAPPING WE SHALL GO........     ";
}

Scroller* Scroller_Get(unsigned char scrollerNum)
{
  return &scrollers[scrollerNum];
}

void Scroller_Update()
{
  for ( unsigned char scrollerIndex = 0; scrollerIndex < 5; scrollerIndex++)
  {
    Scroller* scroller = &scrollers[scrollerIndex];

    if ( scroller->pauseTime == 0)
    {
      scroller->charXPos = scroller->charXPos + scroller->scrollSize;
      scroller->scrXPos += scroller->scrollSize;
      if ( scroller->charXPos > 31)
      {
        scroller->charXPos -= 32;
        scroller->xTile++;
        if ( scroller->xTile > 7)
        {
          scroller->xTile -= 8;
        }
        scroller->charPos += 1;
        if (scroller->message[scroller->charPos] == 0)
        {
          scroller->charPos = 0;
        }
      }
    }
    else
    {
      scroller->pauseTime--;
    }
  }
}

void Scroller_Render()
{
  for ( unsigned char scrollerIndex = 0; scrollerIndex < 5; scrollerIndex++)
  {
    Scroller* scroller = &scrollers[scrollerIndex];

    if ( scroller->pauseTime == 0)
    {
      layer2WriteBigCharacterSliceFast( scroller);
    }
  }
}