/*
 * Copyright (c) 2013 2014 2015 ... 2020 2021
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jfile.h"

char *jfile_c_c="$Id: jfile_c.c,v 2.4 2021/02/21 19:53:16 jmccue Exp $";

/*** prototypes ***/
void load_utf(char *, size_t, size_t, struct s_utf_split *);
void init_s_utf(struct s_utf_split *);
int  utf_8_cont_byte(unsigned char);
int  utf_8_overlong(struct s_utf_split *);
int  utf_8_is3(struct s_utf_split *);
int  utf_8_is4(struct s_utf_split *);

/*
 * utf_split() -- Split a UTF-8 character into separate bytes
 *                Returns # of bytes for the UTF-8
 *                   0 = not UTF-8
 *                otherwise probably 1 or 2 or 3 or 4
 */
int utf_split(char *buf, size_t pos, size_t max_size, struct s_utf_split *u)
{

  init_s_utf(u);
  load_utf(buf, pos, max_size, u);

  if (u->a < (unsigned char) 128)
    u->utf_bytes = 1;

  /* 2 byte sequence */
  if ( (u->a >= (unsigned char) 192) && (u->a <= (unsigned char) 223) ) 
    {
      if (utf_8_cont_byte(u->b))
	{
	  u->utf_bytes = 2;
	  utf_8_overlong(u);
	}
    }

  /* 3 byte sequence */
  utf_8_is3(u);

  /* 4 byte sequence */
  utf_8_is4(u);

  /* correct bytes */
  switch (u->utf_bytes)
    {
      case 4:
	break;
      case 3:
	u->d = JLIB2_UCHAR_NULL;
	break;
      case 2:
	u->c = JLIB2_UCHAR_NULL;
	u->d = JLIB2_UCHAR_NULL;
	break;
      case 1:
	u->b = JLIB2_UCHAR_NULL;
	u->c = JLIB2_UCHAR_NULL;
	u->d = JLIB2_UCHAR_NULL;
	break;
      default:
	u->a = JLIB2_UCHAR_NULL;
	u->b = JLIB2_UCHAR_NULL;
	u->c = JLIB2_UCHAR_NULL;
	u->d = JLIB2_UCHAR_NULL;
	break;
    }

  /*** see if private or other odd type ***/
  if (u->utf_bytes > 1)
    {
      switch (u->a)
	{
	  case 238:  /* EE -- same as F0 */
	  case 244:  /* F4               */
	    u->is_private = (int) TRUE;
	    break;
	  case 239:  /* EF -- considered 'forms', no idea what that is */
	    u->is_forms = (int) TRUE;
	    break;
	  default:
	    break;
	}
    }

  /* done */
  return(u->utf_bytes);

} /* utf_split()  */

/*
 * utf_8_is3() -- Valid 3 byte sequence ?
 *                   Byte 1 : >= 224  and <= 239
 *                   Byte 2 : must be a continue byte
 *                   Byte 3 : must be a continue byte
 *                   and finally cannot conflict with UTF-16 
 */
int utf_8_is3(struct s_utf_split *u)
{

  /*** basic checks, if any failed NOT 3 byte UTF-8 ***/
  if (u->a < (unsigned char) 224)
    return((int) FALSE);
  if (u->a > (unsigned char) 239)
    return((int) FALSE);
  if (! utf_8_cont_byte(u->b))
    return((int) FALSE);
  if (! utf_8_cont_byte(u->c))
    return((int) FALSE);

  /*** look like somekind of UTF-8 3 byte character ***/
  u->utf_bytes = 3;

  /*** Invalid range (U+D800 -- U+DFFF) ? -- used by UTF-16 ***/
  if ((u->a == (unsigned char) 237) && (u->b > (unsigned char) 159))
    {
      u->is_invalid = (int) TRUE;
      return((int) FALSE);
    }

  /*** Check for overlong ***/
  utf_8_overlong(u);

  return((int) TRUE);

} /* utf_8_is3() */

/*
 * utf_8_is4() -- Valid 4 byte sequence ?
 *                   Byte 1 : >= 240  and <= 247
 *                   Byte 2 : must be a continue byte
 *                   Byte 3 : must be a continue byte
 *                   and finally cannot conflict with UTF-16 
 */
int utf_8_is4(struct s_utf_split *u)
{

  /*** basic checks, if any failed NOT 3 byte UTF-8 ***/
  if (u->a < (unsigned char) 240)
    return((int) FALSE);
  if (u->a > (unsigned char) 247)
    return((int) FALSE);
  if (! utf_8_cont_byte(u->b))
    return((int) FALSE);
  if (! utf_8_cont_byte(u->c))
    return((int) FALSE);
  if (! utf_8_cont_byte(u->d))
    return((int) FALSE);

  /*** look like somekind of UTF-8 4 byte character ***/
  u->utf_bytes = 4;

  /*** 245/246/247 F5/F6/F7 not used for UTF-8 ***/
  if ( u->a > (unsigned char) 244 )
    {
      u->is_invalid = (int) TRUE;
      return((int) FALSE);
    }

  /*** decodes > U+10FFFF considered Invalid ***/
  if (( u->a == (unsigned char) 244 ) && ( u->b > (unsigned char) 143 ))
    {
      u->is_invalid = (int) TRUE;
      return((int) FALSE);
    }

  /*** Flag unallocated ranges ***/
  if ( u->a > (unsigned char) 241 )
    {
      u->is_unalloc = (int) TRUE;
      return((int) FALSE);
    }

  /*** Check for overlong ***/
  utf_8_overlong(u);

  return((int) TRUE);

} /* utf_8_is4() */

/*
 * utf_8_overlong() -- Overlong Sequence ?
 *
 *       from http://www.cl.cam.ac.uk/~mgk25/unicode.html#refs
 *       .--------.-------------.-------------.-------------.-------------.
 *       | 2-byte | 1100 000x   | (10xx xxxx) |             |             |
 *       |        | 192 -- 193  |             |             |             |
 *       |        | C0  -- C1   |             |             |             |
 *       |--------+-------------+-------------+-------------+-------------|
 *       | 3-byte | 1110 0000   | 100x xxxx   | (10xx xxxx) |             |
 *       |        | 224         | 128 -- 159  |             |             |
 *       |        | E0          | 80  -- 9F   |             |             |
 *       |--------+-------------+-------------+-------------+-------------|
 *       | 4-byte | 1111 0000   | 1000 xxxx   | (10xx xxxx) | (10xx xxxx) |
 *       |        | 240         | 128 -- 143  |             |             |
 *       |        | F0          | 80  -- 8F   |             |             |
 *       '--------'-------------'-------------'-------------'-------------'
 */
int utf_8_overlong(struct s_utf_split *u)
{
  u->is_overlong = (int) FALSE;
  
  if (u->utf_bytes == 2)  /* 2-byte UTF-8 Sequence */
    {
      if (u->a == (unsigned char) 192) /* C0 */
	{
	  u->is_overlong = (int) TRUE;
	  return((int)TRUE);
	}
      if (u->a == (unsigned char) 193) /* C1 */
	{
	  u->is_overlong = (int) TRUE;
	  return((int)TRUE);
	}
      return((int) FALSE);
    }

  if (u->utf_bytes == 3)  /* 3-byte UTF-8 Sequence */
    {
      if ( (u->a == (unsigned char) 224) && ((u->b > (unsigned char) 127) && (u->b < (unsigned char) 160)) )
	{
	  u->is_overlong = (int) TRUE;
	  return((int) TRUE);
	}
      return((int) FALSE);
    }

  if (u->utf_bytes == 4)  /* 4-byte UTF-8 Sequence */
    {
      if ( (u->a == (unsigned char) 240) && ((u->b > (unsigned char) 127) && (u->b < (unsigned char) 144)) )
	{
	  u->is_overlong = (int) TRUE;
	  return((int) TRUE);
	}
      return((int) FALSE);
    }

  /* default, assume an overlong */
  return((int) TRUE);

} /* utf_8_overlong() */

/*
 * utf_8_cont_byte() -- is this a valid 'contunue byte' for UTF-8
 */
int utf_8_cont_byte(unsigned char x)
{

  if (x < (unsigned char) 128)
    return((int) FALSE);
  if (x > (unsigned char) 191)
    return((int) FALSE);

  return((int) TRUE);

} /* utf_8_cont_byte() */

/*
 * show_utf() -- displays UTF-8
 */
void show_utf(struct s_work *w, struct s_counts *fcount, 
              char *buf, size_t pos, size_t max_size, 
              long int byte_num, struct s_utf_split *u)
{
  int i;
  char *ctype     = (char *) NULL;

  fprintf(w->out.fp, "%8ld %8ld %5lu ", byte_num, fcount->lines_read, ((unsigned long) (pos + 1)));

  /*** determine messages ***/
  if (u->is_overlong == (int) TRUE)
    ctype = LIT_CMSG_U8L;
  if ((ctype == (char *) NULL) && (u->is_invalid == (int) TRUE) )
    ctype = LIT_CMSG_U8I;
  if ((ctype == (char *) NULL) && (u->is_unalloc == (int) TRUE) )
    ctype = LIT_CMSG_U8U;

  /*   these are cosidered 'valid', but unsure if they really are   */
  if ((ctype == (char *) NULL) && (u->is_forms == (int) TRUE) )
    ctype = LIT_CMSG_U8F;
  if ((ctype == (char *) NULL) && (u->is_private == (int) TRUE) )
    ctype = LIT_CMSG_U8P;

  if (ctype == (char *) NULL)
    ctype = LIT_CMSG_U8;

  /*** show raw characters ***/
  if (w->show_raw_utf8)
    {
      for (i = 0; i < 4; i++)
	{
	  switch (i)
	    {
	      case 0:
		fprintf(w->out.fp, "%c", (u->a == JLIB2_CHAR_NULL ? ' ' : u->a));
		break;
	      case 1:
		fprintf(w->out.fp, "%c", (u->b == JLIB2_CHAR_NULL ? ' ' : u->b));
		break;
	      case 2:
		fprintf(w->out.fp, "%c", (u->c == JLIB2_CHAR_NULL ? ' ' : u->c));
		break;
	      case 3:
		fprintf(w->out.fp, "%c", (u->d == JLIB2_CHAR_NULL ? ' ' : u->d));
		break;
	    }
	}
      fprintf(w->out.fp, " ");
    }
  else
    {
      if (u->a == JLIB2_UCHAR_NULL)
	fprintf(w->out.fp, " ");
      else
	fprintf(w->out.fp, "*");
      if (u->b == JLIB2_UCHAR_NULL)
	fprintf(w->out.fp, " ");
      else
	fprintf(w->out.fp, "*");
      if (u->c == JLIB2_UCHAR_NULL)
	fprintf(w->out.fp, " ");
      else
	fprintf(w->out.fp, "*");
      if (u->d == JLIB2_UCHAR_NULL)
	fprintf(w->out.fp, " ");
      else
	fprintf(w->out.fp, "*");
  }

  /*** show HEX ***/
  fprintf(w->out.fp, " ");
  fprintf(w->out.fp, "0x%02X", u->a);
  if (u->b == JLIB2_UCHAR_NULL)
    fprintf(w->out.fp, "     ");
  else
    fprintf(w->out.fp, " 0x%02X", u->b);
  if (u->c == JLIB2_UCHAR_NULL)
    fprintf(w->out.fp, "     ");
  else
    fprintf(w->out.fp, " 0x%02X", u->c);
  if (u->d == JLIB2_UCHAR_NULL)
    fprintf(w->out.fp, "     ");
  else
    fprintf(w->out.fp, " 0x%02X", u->d);
    
  /*** show raw characters ***/
  fprintf(w->out.fp, " ");
  fprintf(w->out.fp, "%s\n", ctype);

} /* show_utf() */

/*
 * init_s_utf() -- initislize utf-8 structure
 */
void  init_s_utf(struct s_utf_split *u)
{

  u->utf_bytes   = 0;
  u->is_overlong = (int) FALSE;
  u->is_invalid  = (int) FALSE;
  u->is_unalloc  = (int) FALSE;
  u->is_private  = (int) FALSE;
  u->is_forms    = (int) FALSE;
  u->a           = JLIB2_UCHAR_NULL;
  u->b           = JLIB2_UCHAR_NULL;
  u->c           = JLIB2_UCHAR_NULL;
  u->d           = JLIB2_UCHAR_NULL;

} /* init_s_utf() */

/*
 * load_utf() -- load characters for UTF-8 check
 */
void load_utf(char *buf, size_t pos, size_t max_size, struct s_utf_split *u)
{
  size_t next = pos;

  /* set characters sequence */ 
  u->a = (unsigned char) buf[pos];
  if (u->a == JLIB2_UCHAR_NULL)
    return;
  next = pos + 1;
  if (next < max_size)
    {
      u->b = (unsigned char) buf[next];
    }

  if (u->b == JLIB2_UCHAR_NULL)
    return;
  next = pos + 2;
  if (next < max_size)
    {
      u->c = (unsigned char) buf[next];
    }

  if (u->c == JLIB2_UCHAR_NULL)
    return;
  next = pos + 3;
  if (next < max_size)
    {
      u->d = (unsigned char) buf[next];
    }

} /* load_utf() */

/* END: jfile_c.c */
