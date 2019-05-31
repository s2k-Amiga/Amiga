#include <stdio.h>
#ifdef NULL
  #undef NULL
#endif
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <intuition/intuition.h>


UBYTE handleIDCMP( struct Window *);
struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;
ULONG signalmask, signals;
UBYTE done = 0;

  char palette[] = {
    0x0, 0x0, 0x0, 
    0xF, 0xE, 0xD,
    0x9, 0x9, 0x9,
    0x0, 0x0, 0xF,
    0x0, 0x8, 0xF,
    0x0, 0xB, 0xA,
    0x0, 0xF, 0x0,
    0xA, 0xF, 0x0,
    0xD, 0xF, 0x0,
    0xF, 0xF, 0x0,
    0xF, 0xC, 0x0,
    0xF, 0xA, 0x0,
    0xF, 0x7, 0x0,
    0xF, 0x5, 0x0,
    0xF, 0x2, 0x0,
    0x8, 0x0, 0xF 
  };

struct Screen *my_screen;
struct NewScreen my_new_screen = {
  0,            /* LeftEdge  Should always be 0. */
  0,            /* TopEdge   Top of the display.*/
  320,          /* Width     We are using a low-resolution screen. */
  256,          /* Height    Non-Interlaced NTSC (American) display. */
  4,            /* Depth     16 colours. */
  0,            /* DetailPen Text should be printed with colour reg. 0 */
  1,            /* BlockPen  Blocks should be printed with colour reg. 1 */
  NULL,         /* ViewModes Low-resolution. (Non-Interlaced) */
  CUSTOMSCREEN, /* Type      Your own customized screen. */
  NULL,         /* Font      Default font. */
  NULL,    /* Title     The screen's title. */
  NULL,         /* Gadget    Must for the moment be NULL. */
  NULL          /* BitMap    No special CustomBitMap. */
};


struct Window *my_window;
struct NewWindow my_new_window = {
  0,            /* LeftEdge    x position of the window. */
  11,            /* TopEdge     y position of the window. */
  320,           /* Width       147 pixels wide. */
  245,            /* Height      63 lines high. */
  0,             /* DetailPen   Text should be drawn with colour reg. 0 */
  1,             /* BlockPen    Blocks should be drawn with col. reg. 1 */
  CLOSEWINDOW,   /* IDCMPFlags: Window close event */
  WINDOWCLOSE|   /* Flags:      Window close gadget */
  SMART_REFRESH| /*             Intuition should refresh the window. */
//  WINDOWDRAG|    /*             Drag gadget. */
//  WINDOWDEPTH|   /*             Depth arrange Gadgets. */
  ACTIVATE,      /*             The window should be Active when opened. */
  NULL,          /* FirstGadget No Custom Gadgets. */
  NULL,          /* CheckMark   Use Intuition's default CheckMark (v). */
  "AMIGA!",      /* Title       Title of the window. */
  NULL,          /* Screen      We will later connect it to the screen. */
  NULL,          /* BitMap      No Custom BitMap. */
  0,             /* MinWidth    We do not need to care about these */
  0,             /* MinHeight   since we have not supplied the window */
  0,             /* MaxWidth    with a Sizing Gadget. */
  0,             /* MaxHeight */
  CUSTOMSCREEN   /* Type        Connected to a Custom screen. */
};
//WBENCHSCREEN

BOOL init()
{
  IntuitionBase = (struct IntuitionBase *) OpenLibrary( "intuition.library", 0 );
  if( IntuitionBase == NULL ) Exit(FALSE);
  GfxBase = (struct GfxBase *) OpenLibrary( "graphics.library", 0 );
  if( GfxBase == NULL ) {
    CloseLibrary( (struct Library *) IntuitionBase );
    Exit(FALSE);
  }
  my_screen= (struct Screen *) OpenScreen( &my_new_screen );
  if(my_screen == NULL) {
    CloseLibrary( (struct Library *) GfxBase );
    CloseLibrary( (struct Library *) IntuitionBase );
    Exit(FALSE);
   }
  my_new_window.Screen = my_screen;
  my_window = (struct Window *) OpenWindow( &my_new_window );
  if (my_window == NULL) {
    CloseScreen( my_screen );
    CloseLibrary( (struct Library *) GfxBase );
    CloseLibrary( (struct Library *) IntuitionBase );
    Exit(FALSE);
  }
  int b = 0;
  for( int a = 0; a < 16; a = a + 1 ) {
      SetRGB4( &my_screen->ViewPort,  a, palette[b], palette[b+1], palette[b+2] );
      b = b + 3;
  }
  signalmask = 1L << my_window->UserPort->mp_SigBit;
  return TRUE;
}

BOOL close()
{
  while( !done ) {
	signals = Wait(signalmask);    
	if (signals & signalmask)
		done = handleIDCMP(my_window);
	};
	CloseWindow( my_window );
	CloseScreen( my_screen );
	CloseLibrary( (struct Library *) GfxBase );
	CloseLibrary( (struct Library *) IntuitionBase );
	return TRUE;
}

UBYTE handleIDCMP( struct Window *win ) {
    UBYTE flag = 0;
    struct IntuiMessage *message = NULL;
    ULONG class;
    while( message = (struct IntuiMessage *)GetMsg(win->UserPort) ) {
        class = message->Class;
        ReplyMsg( (struct Message *)message);
        switch( class ) {
            case CLOSEWINDOW:
                flag = 1;
                break;
            default:
                break;
        }        
    }
    return(flag);
}
