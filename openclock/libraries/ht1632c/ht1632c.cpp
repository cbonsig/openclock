#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ht1632c.h"
#include "Print.h"
#include "font.h"
#include "font_koi8.h"

/* fast integer (1 uint8_t) modulus - returns n % d */

uint8_t _mod(uint8_t n, uint8_t d)
{
  while(n >= d)
    n -= d;

  return n;
}

/* fast integer (1 uint8_t) division - returns n / d */

uint8_t _div(uint8_t n, uint8_t d)
{
  uint8_t q = 0;
  while(n >= d)
  {
    n -= d;
    q++;
  }
  return q;
}

/* fast integer (1 uint8_t) PRNG */

uint8_t _rnd(uint8_t min, uint8_t max)
{
  static uint8_t seed;
  seed = (21 * seed + 21);
  return min + _mod(seed, --max);
}

/* ht1632c class properties */

uint8_t *ht1632c::g_fb;
uint8_t *ht1632c::r_fb;
volatile uint8_t *ht1632c::_port;
#if defined (__AVR__)
uint8_t ht1632c::_data;
uint8_t ht1632c::_wr;
uint8_t ht1632c::_clk;
uint8_t ht1632c::_cs;
#elif defined (__ARMEL__) || defined (__PIC32MX__)
_port_t ht1632c::_data;
_port_t ht1632c::_wr;
_port_t ht1632c::_clk;
_port_t ht1632c::_cs;
#endif

/* ht1632c class constructor */

ht1632c::ht1632c(const uint8_t data, const uint8_t wr, const uint8_t clk, const uint8_t cs, const uint8_t geometry, const uint8_t number)
{
#if defined (__ARMEL__)
  _data.dev = PIN_MAP[data].gpio_device;
  _data.mask = PIN_MAP[data].gpio_bit;
  _wr.dev = PIN_MAP[wr].gpio_device;
  _wr.mask = PIN_MAP[wr].gpio_bit;
  _clk.dev = PIN_MAP[clk].gpio_device;
  _clk.mask = PIN_MAP[clk].gpio_bit;
  _cs.dev = PIN_MAP[cs].gpio_device;
  _cs.mask = PIN_MAP[cs].gpio_bit;

  gpio_set_mode(_data.dev, _data.mask, GPIO_OUTPUT_PP);
  gpio_set_mode(_wr.dev, _wr.mask, GPIO_OUTPUT_PP);
  gpio_set_mode(_clk.dev, _clk.mask, GPIO_OUTPUT_PP);
  gpio_set_mode(_cs.dev, _cs.mask, GPIO_OUTPUT_PP);
#elif defined (__PIC32MX__)
  _data.regs = (volatile p32_ioport *)portRegisters(digitalPinToPort(data));
  _wr.regs = (volatile p32_ioport *)portRegisters(digitalPinToPort(wr));
  _clk.regs = (volatile p32_ioport *)portRegisters(digitalPinToPort(clk));
  _cs.regs = (volatile p32_ioport *)portRegisters(digitalPinToPort(cs));

  _data.mask = digitalPinToBitMask(data);
  _wr.mask = digitalPinToBitMask(wr);
  _clk.mask = digitalPinToBitMask(clk);
  _cs.mask = digitalPinToBitMask(cs);

  _data.regs->tris.clr = _data.mask;
  _data.regs->odc.clr  = _data.mask;
  _wr.regs->tris.clr = _wr.mask;
  _wr.regs->odc.clr  = _wr.mask;
  _clk.regs->tris.clr = _clk.mask;
  _clk.regs->odc.clr  = _clk.mask;
  _cs.regs->tris.clr = _cs.mask;
  _cs.regs->odc.clr  = _cs.mask;
#endif
  _setup(number);
  clear();
}

ht1632c::ht1632c(volatile uint8_t *port, const uint8_t data, const uint8_t wr, const uint8_t clk, const uint8_t cs, const uint8_t geometry, const uint8_t number)
{
#if defined (__AVR__)
  _port = port;

  _data = 1 << (data & 7);
  _wr = 1 << (wr & 7);
  _clk = 1 << (clk & 7);
  _cs = 1 << (cs & 7);

  _port--;
  _set(_data);
  _set(_wr);
  _set(_clk);
  _set(_cs);
  _port++;
#endif
  _setup(number);
  clear();
}

/* ht1632c class low level functions */

inline void ht1632c::_set(_port_t port)
{
#if defined (__ARMEL__)
  port.dev->regs->BSRR = BIT(port.mask);
#elif defined (__PIC32MX__)
  port.regs->lat.set = port.mask;
#endif
}

inline void ht1632c::_toggle(_port_t port)
{
#if defined (__ARMEL__)
  port.dev->regs->ODR = port.dev->regs->ODR ^ BIT(port.mask);
#elif defined (__PIC32MX__)
  port.regs->lat.inv = port.mask;
#endif
}

inline void ht1632c::_reset(_port_t port)
{
#if defined (__ARMEL__)
  port.dev->regs->BRR = BIT(port.mask);
#elif defined (__PIC32MX__)
  port.regs->lat.clr = port.mask;
#endif
}

void ht1632c::_set(uint8_t val)
{
#ifdef USE_ASM
  asm (
    "ld      __tmp_reg__, %a0"  "\n"
    "or      __tmp_reg__,  %1"  "\n"
    "st      %a0, __tmp_reg__"  "\n"
    :
    : "e" (_port), "r" (val)
    :
  );
#else
  *_port |= val;
#endif
}

void ht1632c::_toggle(uint8_t val)
{
#ifdef USE_ASM
  asm (
    "ld      __tmp_reg__, %a0"  "\n"
    "eor     __tmp_reg__,  %1"  "\n"
    "st      %a0, __tmp_reg__"  "\n"
    :
    : "e" (_port), "r" (val)
    :
  );
#else
  *_port ^= val;
#endif
}

void ht1632c::_reset(uint8_t val)
{
#ifdef USE_ASM
  asm (
    "ld      __tmp_reg__, %a0"  "\n"
    "or      __tmp_reg__,  %1"  "\n"
    "eor     __tmp_reg__,  %1"  "\n"
    "st      %a0, __tmp_reg__"  "\n"
    :
    : "e" (_port), "r" (val)
    :
  );
#else
  *_port &= ~val;
#endif
}

inline void ht1632c::_pulse(uint8_t num, _port_t port)
{
  while(num--)
  {
    _set(port);
    _toggle(port);
  }
}

void ht1632c::_pulse(uint8_t num, uint8_t val)
{
  while(num--)
  {
    _set(val);
    _toggle(val);
  }
}

void ht1632c::_writebits (uint8_t bits, uint8_t msb)
{
  do {
    (bits & msb) ? _set(_data) : _reset(_data);
    _reset(_wr);
    _set(_wr);
  } while (msb >>= 1);
}

void ht1632c::_chipselect(uint8_t cs)
{
  _reset(_cs);
  if (cs == HT1632_CS_ALL) {
    _pulse(HT1632_CS_ALL, _clk);
  } else if (cs == HT1632_CS_NONE) {
    _set(_cs);
    _pulse(HT1632_CS_ALL, _clk);
  } else {
    _pulse(1, _clk);
    _set(_cs);
    _pulse(cs - 1, _clk);
  }
}

/* send a command to selected HT1632Cs */

void ht1632c::_sendcmd(uint8_t cs, uint8_t command)
{
  _chipselect(cs);
  _writebits(HT1632_ID_CMD, HT1632_ID_LEN);
  _writebits(command, HT1632_CMD_LEN);
  _writebits(0, 1);
  _chipselect(HT1632_CS_NONE);
}

/* HT1632Cs based display initialization  */

void ht1632c::_setup(uint8_t number)
{
  bicolor = true;
  x_max = (32 * number) - 1;
  y_max = 15;
  cs_max = 4 * number;
  fb_size = 16 * cs_max;
  g_fb = (uint8_t*) malloc(fb_size);
  r_fb = (uint8_t*) malloc(fb_size);
  setfont(FONT_5x7W);
  x_cur = 0;
  y_cur = 0;

  noInterrupts();
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_SYSDIS);
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_COMS00);
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_MSTMD);
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_RCCLK);
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_SYSON);
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_LEDON);
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_PWM);
  interrupts();
}

/* set the display brightness */ 

void ht1632c::pwm(uint8_t value)
{
  noInterrupts();
  _sendcmd(HT1632_CS_ALL, HT1632_CMD_PWM | value);
  interrupts();
}

/* write the framebuffer to the display - to be used after one or more textual or graphic functions */ 

void ht1632c::sendframe()
{
  uint8_t data, offs, cs;
  uint8_t addr, csm;
  csm = cs_max;

  noInterrupts();
  for (cs = 0; cs < csm; cs++)
  {
#ifdef USE_NLFB
    addr = cs << 4;
#else
    addr = cs + (cs & ~1);
    if (_div(cs << 1, csm))
      addr -= (csm - 2);
    addr <<= 4;
#endif
    _chipselect(cs + 1);
    _writebits(HT1632_ID_WR, HT1632_ID_LEN);
    _writebits(0, HT1632_ADDR_LEN);
    for (offs = 0; offs < 16; offs++)
      _writebits(g_fb[addr+offs], HT1632_DATA_LEN);
    for (offs = 0; offs < 16; offs++)
      _writebits(r_fb[addr+offs], HT1632_DATA_LEN);
    _chipselect(HT1632_CS_NONE);
  }
  interrupts();
}

/* clear the display */

void ht1632c::clear()
{
  x_cur = 0;
  y_cur = 0;
  memset(g_fb, 0, fb_size);
  memset(r_fb, 0, fb_size);
  sendframe();
}

inline void ht1632c::_update_fb(uint8_t *ptr, uint8_t target, uint8_t pixel)
{
  uint8_t &val = *ptr;
#if defined (__ARMEL__)
  (target) ? val |= pixel : val &= ~pixel;
#else
  val |= pixel;
  if (!target)
    val ^= pixel;
#endif
}

/* put a single pixel in the coordinates x, y */

void ht1632c::plot (uint8_t x, uint8_t y, uint8_t color)
{
  uint8_t val, csm;
  uint8_t addr;

  if (x > x_max)
    return;

#ifdef USE_NLFB
  addr = (x & 31) + ((x & ~31) << 1) + ((y & ~7) << 2);
#else
  csm = cs_max;
  addr = x + csm * (y & ~7);
#endif
  val = 128 >> (y & 7);

  _update_fb(g_fb+addr, (color & GREEN), val);
  _update_fb(r_fb+addr, (color & RED), val);
}

/* print a single character */

uint8_t ht1632c::putchar(int x, int y, char c, uint8_t color, uint8_t attr, uint8_t bgcolor)
{
  uint16_t dots, msb;
  char col, row;

  uint8_t width = font_width;
  uint8_t height = font_height;

  if (x < -width || x > x_max + width || y < -height || y > y_max + height)
    return 0;

  if ((unsigned char) c >= 0xc0) c -= 0x41;
  c -= 0x20;
  msb = 1 << (height - 1);

  // some math with pointers... don't try this at home ;-)
  prog_uint8_t *addr = font + c * width;
  prog_uint16_t *waddr = wfont + c * width;

  for (col = 0; col < width; col++) {
    dots = (height > 8) ? pgm_read_word_near(waddr + col) : pgm_read_byte_near(addr + col);
    for (row = 0; row < height; row++) {
      if (dots & (msb >> row)) {
        plot(x + col, y + row, (color & MULTICOLOR) ? _rnd(1, 4) : color);
      } else {
        plot(x + col, y + row, bgcolor);
      }
    }
  }
  return ++width;
}

/* put a bitmap in the coordinates x, y */

void ht1632c::putbitmap(int x, int y, prog_uint16_t *bitmap, uint8_t w, uint8_t h, uint8_t color)
{
  uint16_t dots, msb;
  char col, row;

  if (x < -w || x > x_max + w || y < -h || y > y_max + h)
    return;

  msb = 1 << (w - 1);
  for (row = 0; row < h; row++)
  {
    dots = pgm_read_word_near(&bitmap[row]);
    if (dots && color)
      for (uint8_t col = 0; col < w; col++)
      {
	if (dots & (msb >> col))
	  plot(x + col, y + row, color);
	else
	  plot(x + col, y + row, BLACK);
      }
  }
}

/* text only scrolling functions */

void ht1632c::hscrolltext(int y, char *text, uint8_t color, int delaytime, int times, uint8_t dir, uint8_t attr, uint8_t bgcolor)
{
  uint8_t showcolor;
  int i, x, offs, len = strlen(text);
  uint8_t width = font_width;
  uint8_t height = font_height;
  width++;

  while (times) {
    for ((dir) ? x = - (len * width) : x = x_max; (dir) ? x <= x_max : x > - (len * width); (dir) ? x++ : x--)
    {
      for (i = 0; i < len; i++)
      {
        offs = width * i;
        (dir) ? offs-- : offs++;
        putchar(x + offs,  y, text[i], bgcolor, attr, bgcolor);
      }
      for (i = 0; i < len; i++)
      {
        showcolor = (color & RANDOMCOLOR) ? _rnd(1, 4) : color;
        showcolor = ((color & BLINK) && (x & 2)) ? BLACK : (showcolor & ~BLINK);
        offs = width * i;
        putchar(x + offs,  y, text[i], showcolor, attr, bgcolor);
      }
      sendframe();
      delay(delaytime);
    }
    times--;
  }
}

void ht1632c::vscrolltext(int x, char *text, uint8_t color, int delaytime, int times, uint8_t dir, uint8_t attr, uint8_t bgcolor)
{
  uint8_t showcolor;
  int i, y, voffs, len = strlen(text);
  uint8_t offs;
  uint8_t width = font_width;
  uint8_t height = font_height;
  width++;

  while (times) {
    for ((dir) ? y = - font_height : y = y_max + 1; (dir) ? y <= y_max + 1 : y > - font_height; (dir) ? y++ : y--)
    {
      for (i = 0; i < len; i++)
      {
        offs = width * i;
        voffs = (dir) ? -1 : 1;
        putchar(x + offs,  y + voffs, text[i], bgcolor, attr, bgcolor);
      }
      for (i = 0; i < len; i++)
      {
        showcolor = (color & RANDOMCOLOR) ? _rnd(1, 4) : color;
        showcolor = ((color & BLINK) && (y & 2)) ? BLACK : (showcolor & ~BLINK);
        putchar(x + width * i,  y, text[i], showcolor, attr, bgcolor);
      }
      sendframe();
      delay(delaytime);
    }
    times--;
  }
}

/* choose/change font to use (for next putchar) */

void ht1632c::setfont(uint8_t userfont)
{
  switch(userfont) {

#ifdef FONT_4x6
    case FONT_4x6:
	font = (prog_uint8_t *) &font_4x6[0];
	font_width = 4;
	font_height = 6;
	break;
#endif
#ifdef FONT_5x7
    case FONT_5x7:
	font = (prog_uint8_t *) &font_5x7[0];
	font_width = 5;
	font_height = 7;
	break;
#endif
#ifdef FONT_5x8
    case FONT_5x8:
	font = (prog_uint8_t *) &font_5x8[0];
	font_width = 5;
	font_height = 8;
	break;
#endif
#ifdef FONT_5x7W
    case FONT_5x7W:
	font = (prog_uint8_t *) &font_5x7w[0];
	font_width = 5;
	font_height = 8;
	break;
#endif
#ifdef FONT_6x10
    case FONT_6x10:
	wfont = (prog_uint16_t *) &font_6x10[0];
	font_width = 6;
	font_height = 10;
	break;
#endif
#ifdef FONT_6x12
    case FONT_6x12:
	wfont = (prog_uint16_t *) &font_6x12[0];
	font_width = 6;
	font_height = 12;
	break;
#endif
#ifdef FONT_6x13
    case FONT_6x13:
	wfont = (prog_uint16_t *) &font_6x13[0];
	font_width = 6;
	font_height = 13;
	break;
#endif
#ifdef FONT_6x13B
    case FONT_6x13B:
	wfont = (prog_uint16_t *) &font_6x13B[0];
	font_width = 6;
	font_height = 13;
	break;
#endif
#ifdef FONT_6x13O
    case FONT_6x13O:
	wfont = (prog_uint16_t *) &font_6x13O[0];
	font_width = 6;
	font_height = 13;
	break;
#endif
#ifdef FONT_6x9
    case FONT_6x9:
	wfont = (prog_uint16_t *) &font_6x9[0];
	font_width = 6;
	font_height = 9;
	break;
#endif
#ifdef FONT_7x13
    case FONT_7x13:
	wfont = (prog_uint16_t *) &font_7x13[0];
	font_width = 7;
	font_height = 13;
	break;
#endif
#ifdef FONT_7x13B
    case FONT_7x13B:
	wfont = (prog_uint16_t *) &font_7x13B[0];
	font_width = 7;
	font_height = 13;
	break;
#endif
#ifdef FONT_7x13O
    case FONT_7x13O:
	wfont = (prog_uint16_t *) &font_7x13O[0];
	font_width = 7;
	font_height = 13;
	break;
#endif
#ifdef FONT_7x14
    case FONT_7x14:
	wfont = (prog_uint16_t *) &font_7x14[0];
	font_width = 7;
	font_height = 14;
	break;
#endif
#ifdef FONT_7x14B
    case FONT_7x14B:
	wfont = (prog_uint16_t *) &font_7x14B[0];
	font_width = 7;
	font_height = 14;
	break;
#endif
#ifdef FONT_8x8
    case FONT_8x8:
	font = (prog_uint8_t *) &font_8x8[0];
	font_width = 8;
	font_height = 8;
	break;
#endif
#ifdef FONT_8x13
    case FONT_8x13:
	wfont = (prog_uint16_t *) &font_8x13[0];
	font_width = 8;
	font_height = 13;
	break;
#endif
#ifdef FONT_8x13B
    case FONT_8x13B:
	wfont = (prog_uint16_t *) &font_8x13B[0];
	font_width = 8;
	font_height = 13;
	break;
#endif
#ifdef FONT_8x13O
    case FONT_8x13O:
	wfont = (prog_uint16_t *) &font_8x13O[0];
	font_width = 8;
	font_height = 13;
	break;
#endif
#ifdef FONT_9x15
    case FONT_9x15:
	wfont = (prog_uint16_t *) &font_9x15[0];
	font_width = 9;
	font_height = 15;
	break;
#endif
#ifdef FONT_9x15B
    case FONT_9x15B:
	wfont = (prog_uint16_t *) &font_9x15b[0];
	font_width = 9;
	font_height = 15;
	break;
#endif
#ifdef FONT_8x16
    case FONT_8x16:
	wfont = (prog_uint16_t *) &font_8x16[0];
	font_width = 8;
	font_height = 16;
	break;
#endif
#ifdef FONT_8x16B
    case FONT_8x16B:
	wfont = (prog_uint16_t *) &font_8x16b[0];
	font_width = 8;
	font_height = 16;
	break;
#endif
#ifdef FONT_8x13BK
    case FONT_8x13BK:
	wfont = (prog_uint16_t *) &font_8x13bk[0];
	font_width = 8;
	font_height = 13;
	break;
#endif
  }
}

/* graphic primitives based on Bresenham's algorithms */

void ht1632c::line(int x0, int y0, int x1, int y1, uint8_t color)
{
  int dx =  abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
  int err = dx + dy, e2; /* error value e_xy */

  for(;;) {
    plot(x0, y0, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

void ht1632c::rect(int x0, int y0, int x1, int y1, uint8_t color)
{
  line(x0, y0, x0, y1, color); /* left line   */
  line(x1, y0, x1, y1, color); /* right line  */
  line(x0, y0, x1, y0, color); /* top line    */
  line(x0, y1, x1, y1, color); /* bottom line */
}

void ht1632c::circle(int xm, int ym, int r, uint8_t color)
{
  int x = -r, y = 0, err = 2 - 2 * r; /* II. Quadrant */ 
  do {
    plot(xm - x, ym + y, color); /*   I. Quadrant */
    plot(xm - y, ym - x, color); /*  II. Quadrant */
    plot(xm + x, ym - y, color); /* III. Quadrant */
    plot(xm + y, ym + x, color); /*  IV. Quadrant */
    r = err;
    if (r >  x) err += ++x * 2 + 1; /* e_xy+e_x > 0 */
    if (r <= y) err += ++y * 2 + 1; /* e_xy+e_y < 0 */
  } while (x < 0);
}

void ht1632c::ellipse(int x0, int y0, int x1, int y1, uint8_t color)
{
  int a = abs(x1 - x0), b = abs(y1 - y0), b1 = b & 1; /* values of diameter */
  long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
  long err = dx + dy + b1 * a * a, e2; /* error of 1.step */

  if (x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
  if (y0 > y1) y0 = y1; /* .. exchange them */
  y0 += (b + 1) / 2; /* starting pixel */
  y1 = y0 - b1;
  a *= 8 * a; 
  b1 = 8 * b * b;

  do {
    plot(x1, y0, color); /*   I. Quadrant */
    plot(x0, y0, color); /*  II. Quadrant */
    plot(x0, y1, color); /* III. Quadrant */
    plot(x1, y1, color); /*  IV. Quadrant */
    e2 = 2 * err;
    if (e2 >= dx) { x0++; x1--; err += dx += b1; } /* x step */
    if (e2 <= dy) { y0++; y1--; err += dy += a; }  /* y step */ 
  } while (x0 <= x1);

  while (y0 - y1 < b) {  /* too early stop of flat ellipses a=1 */
    plot(x0 - 1, ++y0, color); /* -> complete tip of ellipse */
    plot(x0 - 1, --y1, color); 
  }
}

void ht1632c::bezier(int x0, int y0, int x1, int y1, int x2, int y2, uint8_t color)
{
  int sx = x0 < x2 ? 1 : -1, sy = y0 < y2 ? 1 : -1; /* step direction */
  int cur = sx * sy * ((x0 - x1) * (y2 - y1) - (x2 - x1) * (y0 - y1)); /* curvature */
  int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2, xy = 2 * x * y * sx * sy;
                                /* compute error increments of P0 */
  long dx = (1 - 2 * abs(x0 - x1)) * y * y + abs(y0 - y1) * xy - 2 * cur * abs(y0 - y2);
  long dy = (1 - 2 * abs(y0 - y1)) * x * x + abs(x0 - x1) * xy + 2 * cur * abs(x0 - x2);
                                /* compute error increments of P2 */
  long ex = (1 - 2 * abs(x2 - x1)) * y * y + abs(y2 - y1) * xy + 2 * cur * abs(y0 - y2);
  long ey = (1 - 2 * abs(y2 - y1)) * x * x + abs(x2 - x1) * xy - 2 * cur * abs(x0 - x2);

  if (cur == 0) { line(x0, y0, x2, y2, color); return; } /* straight line */

  x *= 2 * x; y *= 2 * y;
  if (cur < 0) {                             /* negated curvature */
    x = -x; dx = -dx; ex = -ex; xy = -xy;
    y = -y; dy = -dy; ey = -ey;
  }
  /* algorithm fails for almost straight line, check error values */
  if (dx >= -y || dy <= -x || ex <= -y || ey >= -x) {        
    line(x0, y0, x1, y1, color);                /* simple approximation */
    line(x1, y1, x2, y2, color);
    return;
  }
  dx -= xy; ex = dx+dy; dy -= xy;              /* error of 1.step */

  for(;;) {                                         /* plot curve */
    plot(x0, y0, color);
    ey = 2 * ex - dy;                /* save value for test of y step */
    if (2 * ex >= dx) {                                   /* x step */
      if (x0 == x2) break;
      x0 += sx; dy -= xy; ex += dx += y; 
    }
    if (ey <= 0) {                                      /* y step */
      if (y0 == y2) break;
      y0 += sy; dx -= xy; ex += dy += x; 
    }
  }
}

/* returns the pixel value (RED, GREEN, ORANGE or 0/BLACK) of x, y coordinates */

uint8_t ht1632c::getpixel(uint8_t x, uint8_t y)
{
  uint8_t g, r, val;
  uint16_t addr, csm;

#ifdef USE_NLFB
  addr = (x & 31) + ((x & ~31) << 1) + ((y & ~7) << 2);
#else
  csm = cs_max;
  addr = x + csm * (y & ~7);
#endif
  val = 1 << (7 - y & 7);
  g = g_fb[addr];
  r = r_fb[addr];
  return ((g & val) ? GREEN : BLACK) | ((r & val) ? RED : BLACK);
}

/* boundary flood fill with the seed in x, y coordinates */

void ht1632c::_fill_r(uint8_t x, uint8_t y, uint8_t color)
{
  if (x > x_max) return;
  if (y > y_max) return;
  if (!getpixel(x, y))
  {
    plot(x, y, color);
    _fill_r(++x, y ,color);
    x--;
    _fill_r(x, y - 1, color);
    _fill_r(x, y + 1, color);
  }
}

void ht1632c::_fill_l(uint8_t x, uint8_t y, uint8_t color)
{
  if (x > x_max) return;
  if (y > y_max) return;
  if (!getpixel(x, y))
  {
    plot(x, y, color);
    _fill_l(--x, y, color);
    x++;
    _fill_l(x, y - 1, color);
    _fill_l(x, y + 1, color);
  }
}

void ht1632c::fill(uint8_t x, uint8_t y, uint8_t color)
{
  _fill_r(x, y, color);
  _fill_l(x - 1, y, color);
}

/* Print class extension: TBD */

#ifdef PRINT_NEW
size_t ht1632c::write(uint8_t chr)
#else
void ht1632c::write(uint8_t chr)
#endif
{
  uint8_t x, y;
  if (chr == '\n') {
    //y_cur += font_height;
  } else {
    //x_cur += putchar(x_cur, y_cur, chr, GREEN);
    //x_cur = 0;
    //y_cur = 0;
  }
  //sendframe();
#ifdef PRINT_NEW
  return 1;
#endif
}

#ifdef PRINT_NEW
size_t ht1632c::write(const char *str)
#else
void ht1632c::write(const char *str)
#endif
{
  uint8_t x, y;
  uint8_t len = strlen(str);
  if (x_cur + font_width <= x_max)
    for (uint8_t i = 0; i < len; i++)
    {
      if (str[i] == '\n') {
        y_cur += font_height;
        x_cur = 0;
        if (y_cur > y_max) break;
      } else {
        x_cur += putchar(x_cur, y_cur, str[i], GREEN);
        if (x_cur + font_width > x_max) break;
      }
    }
  //x_cur = 0;
  //y_cur = 0;
  sendframe();
#ifdef PRINT_NEW
  return len;
#endif
}

/* calculate frames per second speed, for benchmark */

void ht1632c::profile() {
  const uint8_t frame_interval = 50;
  static unsigned long last_micros = 0;
  static uint8_t frames = 0;
  if (++frames == frame_interval) {
    fps = (frame_interval * 1000000L) / (micros() - last_micros);
    frames = 0;
    last_micros = micros();
  }
}
