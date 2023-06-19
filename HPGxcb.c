/* HPGxcb.c - driver routines for xcb graphics */


/* reference
   https://www.x.org/releases/X11R7.7/doc/libxcb/tutorial/index.html
*/


#include <string.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "HPGxcb.h"

/* draw_text ------------------------------------------------------------------
 * put a text label with a specified color on the screen at a specified location
 * --------------------------------------------------------------------------*/
void draw_text ( xcb_connection_t   *connection,
                 xcb_screen_t       *screen, 
                 xcb_window_t        window,
                     int16_t         x1,
                     int16_t         y1,
                    uint32_t         fg_color,
                    uint32_t         bg_color,
               const char           *label )
{
  /* get graphics context */
  xcb_gcontext_t gc = get_font_gc (connection, screen, window, fg_color, bg_color, "fixed");

  /* draw the text */
  xcb_void_cookie_t textCookie = 
  xcb_image_text_8_checked (connection, strlen(label), window, gc, x1,y1, label );

  test_cookie(textCookie, connection, "can't paste text");

  /* free the gc */
  xcb_void_cookie_t gcCookie = xcb_free_gc (connection, gc);

  test_cookie(gcCookie, connection, "can't free gc");
  return;
}

               
/* get_font_gc ----------------------------------------------------------------
 * --------------------------------------------------------------------------*/
xcb_gc_t get_font_gc ( xcb_connection_t  *connection,
                       xcb_screen_t      *screen,
                       xcb_window_t       window,
                       uint32_t           fg_color, 
                       uint32_t           bg_color, 
                       const char        *font_name )
{
  // get font 
  xcb_font_t font = xcb_generate_id (connection);
  xcb_void_cookie_t fontCookie = xcb_open_font_checked (connection,
                                                        font,
                                                        strlen(font_name),
                                                        font_name );

  test_cookie(fontCookie, connection, "can't open font");


  // create graphics context for font 
  xcb_gcontext_t  gc      = xcb_generate_id (connection);
  uint32_t    gc_mask     = XCB_GC_FOREGROUND | XCB_GC_BACKGROUND | XCB_GC_FONT;
  uint32_t    gc_value[3] = { fg_color, bg_color, font };
                                                            
  xcb_void_cookie_t gcCookie = xcb_create_gc_checked ( connection,
                                                       gc,
                                                       window,
                                                       gc_mask,
                                                       gc_value );

  test_cookie(gcCookie, connection, "can't create gc");

  // close font 
  fontCookie = xcb_close_font_checked (connection, font);

  test_cookie(fontCookie, connection, "can't close font");
 
  return gc;
}


/* print_modifiers -----------------------------------------------------------
 * print the names of modifiers present in a gc_mask 
 * --------------------------------------------------------------------------*/
void print_modifiers ( uint32_t gc_mask )
{
  const char *MODIFIERS[] = {
          "Shift", "Lock", "Ctrl", "Alt",
          "Mod2", "Mod3", "Mod4", "Mod5",
          "Button1", "Button2", "Button3", "Button4", "Button5"
  };

  printf ("Modifier mask: ");
  for (const char **modifier = MODIFIERS ; gc_mask; gc_mask >>= 1, ++modifier) {
      if (gc_mask & 1) {
          printf (*modifier);
      }
  }
  printf ("\n");
  return;
}


/* test_cookie -----------------------------------------------------------------
 * --------------------------------------------------------------------------*/
void test_cookie ( xcb_void_cookie_t   cookie, 
                   xcb_connection_t   *connection,
                   char               *errMessage )
{
  xcb_generic_error_t *error = xcb_request_check (connection, cookie);
  if (error) {
      fprintf (stderr, "ERROR: %s : %"PRIu8"\n", errMessage, error->error_code);
      xcb_disconnect (connection);
            exit(-1);
  }
  return;
}


void xcb_window_title(xcb_connection_t *conn, xcb_window_t window, char title) {
}

