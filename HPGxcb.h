/* HPGxcb.h
 */

#ifndef _XCB_UTIL_H_
#define _XCB_UTIL_H_

#include <xcb/xcb.h>

static xcb_connection_t *connection;
static xcb_drawable_t    window;
static xcb_gcontext_t    foreground;

void draw_text ( xcb_connection_t *c,
                 xcb_screen_t     *screen,
                 xcb_window_t      window,
                 int16_t           x1,
                 int16_t           y1,
                 uint32_t          fg_color,
                 uint32_t          bg_color,
                 const char       *label );


xcb_gc_t get_font_gc ( xcb_connection_t *c,
                       xcb_screen_t     *screen,
                       xcb_window_t      window,
                       uint32_t          fg_color,
                       uint32_t          bg_color,
                       const char       *font_name );


void print_modifiers ( uint32_t mask );


void test_cookie ( xcb_void_cookie_t  cookie,
                   xcb_connection_t  *connection,
                   char              *errMessage );

#endif
