/*
 * Copyright (c) 2013 2014 2015 ... 2023 2024
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

#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#ifdef OpenBSD
#include <err.h>
#endif

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "jfile.h"

#define MAX_FMT_CHAR 5

/*** prototypes ***/
void process_all(struct s_work *, char *, char **, size_t *);
void show_file_heading(struct s_work *, char *);
void process_a_file(struct s_work *, char *, char **, size_t *);
void process_line(struct s_work *, struct s_counts *, char *, size_t, int *, long int *);
int  process_char(struct s_work *, struct s_counts *, unsigned char, unsigned char, size_t, long int);
void utf_8_count(struct s_counts *, struct s_utf_split *);
void fmt_7bit(char *, unsigned char );
int has_bom(char *);

/*
 * main()
 */
int main(int argc, char **argv)

{
  struct s_work w;
  int i;
  char *buf     = (char *) NULL;
  size_t bsize  = (size_t) 0;

#ifdef OpenBSD
  if(pledge("stdio rpath wpath cpath",NULL) == -1)
    err(1,"pledge\n");
#endif

  init(argc, argv, &w);

  /* allocate initial read buffer memory (optional) */
  bsize = 256;
  buf = (char *) calloc(bsize, sizeof(char));
  if (buf == (char *) NULL)
    {
      fprintf(w.err.fp, MSG_ERR_E080, strerror(errno));
      exit(EXIT_FAILURE);
    }

  /*** process all files ***/
  for (i = optind; i < argc; i++)
    process_all(&w, argv[i], &buf, &bsize);
  if (i == optind)
    process_all(&w, FILE_NAME_STDIN, &buf, &bsize);

  fprintf(w.out.fp, "\n%s\n", LIT_INFO_END);
  close_out(&(w.out));
  close_out(&(w.err));
  free(buf);
  if (w.prog_name != (char *) NULL)
    free(w.prog_name);
  exit(EXIT_SUCCESS);

}  /* main() */

/*
* process_all() -- Process all files
*/
void process_all(struct s_work *w, char *fname, char **buf, size_t *bsize)

{

  show_file_heading(w, fname);

  process_a_file(w, fname, buf, bsize);

}  /* process_all() */

/*
 * process_a_file()
 */
void process_a_file(struct s_work *w, char *fname, char **buf, size_t *bsize)

{

  struct s_counts fcount;
  char *use_buf       = (char *) NULL;
  ssize_t cbytes      = (ssize_t) 0;
  size_t use_buf_size = (size_t) 0;
  long int byte_num   = 0L;
  int show_heading    = (int) TRUE;
  FILE *fp;

  init_count(&fcount);

  if ( ! open_in(&fp, fname, w->err.fp) )
    return;

  while ((cbytes = j2_getline(buf, bsize, fp)) > (ssize_t) -1)
    {
      fcount.lines_read++;
      fcount.bytes_read += cbytes;
      use_buf = (*buf);
      use_buf_size = cbytes;
      if ((fcount.lines_read == 1L) && w->flag_bom )
	{
	  if (has_bom((*buf)))
	    {
	      fcount.bom_found = (int) TRUE;
	      use_buf = &(*buf[3]);
	      use_buf_size -= 3;
	    }
	}
      process_line(w, &fcount, use_buf, use_buf_size, &show_heading, &byte_num);
    }

  close_in(&fp, fname);

  fprintf(w->out.fp, "\n");
  fprintf(w->out.fp, MSG_INFO_I023, fcount.lines_read);
  fprintf(w->out.fp, MSG_INFO_I024, fcount.bytes_read);
  fprintf(w->out.fp, MSG_INFO_I025, fcount.count_7bit, fcount.count_7bit);
  fprintf(w->out.fp, MSG_INFO_I026, fcount.count_8bit, fcount.count_8bit);
  fprintf(w->out.fp, MSG_INFO_I027, fcount.count_ctrl, fcount.count_ctrl);
  fprintf(w->out.fp, MSG_INFO_I028, fcount.count_utf8, (fcount.count_utf8_extra + fcount.count_utf8));
  fprintf(w->out.fp, MSG_INFO_I029, fcount.count_utf8_overlong, (fcount.count_utf8_overlong_extra + fcount.count_utf8_overlong));
  fprintf(w->out.fp, MSG_INFO_I030, fcount.count_utf8_unalloc,  (fcount.count_utf8_unalloc_extra + fcount.count_utf8_unalloc));
  fprintf(w->out.fp, MSG_INFO_I031, fcount.count_utf8_invalid,  (fcount.count_utf8_invalid_extra + fcount.count_utf8_invalid));
  if (fcount.bytes_undefined > 0)
    fprintf(w->out.fp, MSG_INFO_I032, fcount.bytes_undefined, fcount.bytes_undefined);
  if (fcount.bom_found)
    fprintf(w->out.fp, MSG_INFO_I033, 1L, 3L);
  fprintf(w->out.fp, "\n");

} /* process_a_file() */

/*
 * process_line() -- show line stats
 */
void process_line(struct s_work *w, struct s_counts *fcount, char *buf, size_t buf_size, int *show_heading, long int *byte_num)
{

  size_t i;
  unsigned char c = JLIB2_UCHAR_NULL;
  unsigned char next_c = JLIB2_UCHAR_NULL;
  struct s_utf_split u;

  if ((*show_heading) == (int) TRUE)
    {
      (*show_heading) = (int) FALSE;
      if (w->show_7bit || w->show_8bit || w->show_invalid || w->show_utf_8)
	{
	  fprintf(w->out.fp, "%8s %8s %5s %-4s %-19s %s %s\n", 
	          LIT_BYTE,     LIT_RECORD,   LIT_POS,      LIT_CHAR,     LIT_HEX, LIT_TYPE, LIT_ASCII_UTF);
	  fprintf(w->out.fp, "%8s %8s %5s %4s %19s %s\n",
                  LIT_ULINE_08, LIT_ULINE_08, LIT_ULINE_05, LIT_ULINE_04, LIT_ULINE_19, LIT_ULINE_23);
	}
    }

  for (i = 0; i < buf_size; i++)
    {
      (*byte_num)++;
      if ( (i+1) < buf_size)
	next_c = (unsigned char) buf[(i+1)];
      else
	next_c = JLIB2_UCHAR_NULL;
      utf_split(buf, i, buf_size, &u);
      if (u.utf_bytes < 2)
	{
	  c = buf[i];
	  process_char(w, fcount, c, next_c, i, (*byte_num));
	}
      else
	{
	  if (w->show_utf_8)
	    show_utf(w, fcount, buf, i, buf_size, (*byte_num), &u);
	  (*byte_num) += (u.utf_bytes - 1);
	  i += (u.utf_bytes - 1);
	  utf_8_count(fcount, &u);
	}
    }

} /* process_line() */

/*
 * utf_8_count() -- increment control counds
 */
void utf_8_count(struct s_counts *fcount, struct s_utf_split *u)
{
  if (u->is_overlong == (int) TRUE)
    {
      (fcount->count_utf8_overlong)++;
      fcount->count_utf8_overlong_extra += (u->utf_bytes - 1);
      return;
    }
  if (u->is_invalid == (int) TRUE)
    {
      (fcount->count_utf8_invalid)++;
      fcount->count_utf8_invalid_extra += (u->utf_bytes - 1);
      return;
    }
  if (u->is_unalloc == (int) TRUE)
    {
      (fcount->count_utf8_unalloc)++;
      fcount->count_utf8_unalloc_extra += (u->utf_bytes - 1);
      return;
    }

  (fcount->count_utf8)++;
  fcount->count_utf8_extra += (u->utf_bytes - 1);

} /* utf_8_count() */

/*
 * process_char() -- process & show a 'normal' character
 */
int process_char(struct s_work *w, struct s_counts *fcount, 
                 unsigned char c, unsigned char next_c, 
                 size_t pos, long int byte_num)
{
  int char_type;
  char *ctype = (char *) NULL;
  char fmt_char[MAX_FMT_CHAR];
  char show = (int) FALSE;

  ctype = LIT_CMSG_INV;
  fmt_7bit(fmt_char, c);

  if ( w->is_delim && (w->delim == c))
    {
      char_type = CHAR_TYPE_7BIT;
      ctype = LIT_CMSG_A7;
    }
  else
    {
      switch (c)
	{
	  case 0:
	    ctype = LIT_CMSG_NULL;
	    char_type = CHAR_TYPE_INVALID;
	    break;
	  case 10:
	    if (w->nl_not_ascii)
	      {
		ctype = LIT_CMSG_NL;
		char_type = CHAR_TYPE_7BIT;
	      }
	    else
	      char_type = CHAR_TYPE_INVALID;
	    break;
	  case 13:
	    if ((next_c == (unsigned char) 10) && w->nl_not_ascii )
	      {
		char_type = CHAR_TYPE_7BIT;
		ctype = LIT_CMSG_RET;
	      }
	    else
	      char_type = CHAR_TYPE_INVALID;
	    break;
	  default:
	    if (c < 32)
	      char_type = CHAR_TYPE_INVALID;
	    else
	      {
		if (c > 126)
		  {
		    char_type = CHAR_TYPE_8BIT;
		    ctype = LIT_CMSG_A8;
		  }
		else
		  {
		    char_type = CHAR_TYPE_7BIT;
		    ctype = LIT_CMSG_A7;
		  }
	      }
	    break;
	}
    }

  switch (char_type)
    {
      case CHAR_TYPE_INVALID:
	(fcount->count_ctrl)++;
	if (w->show_invalid)
	  show = (int) TRUE;
	break;
      case CHAR_TYPE_7BIT:
	(fcount->count_7bit)++;
	if (w->show_7bit)
	  show = (int) TRUE;
	break;
      case CHAR_TYPE_8BIT:
	(fcount->count_8bit)++;
	if (w->show_8bit)
	  show = (int) TRUE;
	break;
      case CHAR_TYPE_UTF8:
	(fcount->count_utf8)++;
	if (w->show_utf_8)
	  show = (int) TRUE;
	break;
      default:
	(fcount->bytes_undefined)++;
	show = (int) TRUE;
	break;
    }

  if (show)
    {
      fprintf(w->out.fp, "%8ld %8ld %5lu ", byte_num, fcount->lines_read, ((unsigned long) (pos + 1)) );
      fprintf(w->out.fp, "%-4s", fmt_char);
      fprintf(w->out.fp, " 0x%02X                %s\n", c, ctype);
    }

  return(char_type);

} /* process_char() */

/*
 * fmt_7bit() -- format a 7 bit char for printing
 */
void fmt_7bit(char *fmt_char, unsigned char c)
{
  memset(fmt_char, JLIB2_CHAR_NULL, MAX_FMT_CHAR);

  if ((c > 32) && (c < 127))
    {
      fmt_char[0] = c;
      return;
    }
  if (c > 126)
    {
      fmt_char[0] = ' ';
      return;
    }

  fmt_char[0] = '^';

  switch (c)
    {
      case 0:
	fmt_char[1] = '@';
	break;
      case 27:
	fmt_char[1] = '[';
	break;
      case 28:
	fmt_char[1] = '\\';
	break;
      case 29:
	fmt_char[1] = ']';
	break;
      case 30:
	fmt_char[1] = '^';
	break;
      case 31:
	fmt_char[1] = '_';
	break;
      case 127:
	fmt_char[1] = '?';
	break;
      default:
	fmt_char[1] = c + (unsigned) 64;
	break;
    }

} /* fmt_7bit() */

/*
 * show_file_heading() -- Show run stats
 */
void show_file_heading(struct s_work *w, char *fname)

{

  fprintf(w->out.fp, "%s\n", LIT_C80);

  if (fname == (char *) NULL)
    fprintf(w->out.fp, "%s\n", LIT_STDIN);
  else
    {
      if (strcmp(fname, FILE_NAME_STDIN) == 0)
	fprintf(w->out.fp, "%s\n", LIT_STDIN);
      else
	fprintf(w->out.fp, "%s\n", fname);
    }

  fprintf(w->out.fp, "%s\n", LIT_C80);

} /* show_file_heading() */

/*
 * has_bom() -- do we have a BOM (Byte Order Mark)
 */
int has_bom(char *buf)
{
  unsigned char a, b, c;

  if (buf == (char *) NULL)
    return((int) FALSE);
  if (strlen(buf) < 3)
    return((int) FALSE);

  a = (unsigned char) buf[0];
  b = (unsigned char) buf[1];
  c = (unsigned char) buf[2];

  if ( (a == 239 ) &&      /* 0xEF */
       (b == 187 ) &&      /* 0xBB */
       (c == 191 ) )       /* 0xBF */
    return((int) TRUE);

  return((int) FALSE);

} /* has_bom() */
