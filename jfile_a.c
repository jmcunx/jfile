/*
 * Copyright (c) 2013 2014 2015 ... 2021 2022
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
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jfile.h"

#define SIZ_ARG_FMT 80

/*
 * init_get_delm() -- translate a string into a delimiter
 */
int init_get_delm(FILE *fp, char *s, char *prog_name)
{
  int d;
  int c = JLIB2_CHAR_NULL;

  if (s == (char *) NULL)
    return(c);

  if (strlen(s) == 1)
    {
      if ( ((*s) > 47)  && ((*s) < 58) ) /* 0 -- 9 */
	c = (*s) - 48;
      else
	c = (*s);
    }
  else
    {
      if (j2_is_numr(s) == (int) TRUE)
	{
	  d = atoi(s);
	  if ((d > -1) && (d < 128))
	    c = (char) d;
	  else
	    {
	      fprintf(fp, MSG_ERR_E049, s);
	      fprintf(fp, MSG_ERR_E000, prog_name, SWITCH_CHAR, ARG_HELP);
	      exit(EXIT_FAILURE);
	    }
	}
      else
	{
	  fprintf(fp, MSG_ERR_E049, s);
	  fprintf(fp, MSG_ERR_E000, prog_name, SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	}
    }

  return(c);

} /* init_get_delm() */

/*
 * process_arg() -- process arguments
 */
void process_arg(int argc, char **argv, struct s_work *w)

{
  char ckarg[SIZ_ARG_FMT];
  int opt;
  int ok;
  int i;
  char *fout = (char *) NULL; 
  char *ferr = (char *) NULL; 
  int display_help = (int) FALSE;
  int display_rev  = (int) FALSE;

  snprintf(ckarg, SIZ_ARG_FMT, "%c%c%c%c%c%c%c%c%c%c%c%c:%c:%c:", 
	         ARG_NL_ASCII, ARG_BOM_NO, ARG_PRINT_UTF8,
		 ARG_7BIT_FILE, ARG_8BIT_FILE, ARG_INVALID, ARG_UTF8_FILE,
		 ARG_FORCE, ARG_HELP, ARG_VERBOSE, ARG_VERSION,
		 ARG_ERR, ARG_OUT, ARG_DELM);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_DELM:
	    w->delim    = (unsigned char) init_get_delm(w->err.fp, optarg, w->prog_name);
	    w->is_delim = (int) TRUE;
	    break;
	  case ARG_PRINT_UTF8:
	    w->show_raw_utf8 = (int) TRUE;
	    break;
	  case ARG_BOM_NO:
	    w->flag_bom     = (int) TRUE;
	    break;
	  case ARG_7BIT_FILE:
	    w->show_7bit    = (int) TRUE;
	    break;
	  case ARG_8BIT_FILE:
	    w->show_8bit    = (int) TRUE;
	    break;
	  case ARG_INVALID:
	    w->show_invalid = (int) TRUE;
	    break;
	  case ARG_UTF8_FILE:
	    w->show_utf_8 = (int) TRUE;
	    break;
	  case ARG_NL_ASCII:
	    w->nl_not_ascii = (int) FALSE;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
	  case ARG_VERBOSE:
	    w->verbose      = (int) TRUE;
	    w->show_8bit    = (int) TRUE;
	    w->show_invalid = (int) TRUE;
	    w->show_utf_8   = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    display_rev = (int) TRUE;
	    break;
	  case ARG_ERR:
	    ferr = optarg;
	    break;
	  case ARG_OUT:
	    fout = optarg;
	    break;
	  default:
	    fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
	    exit(EXIT_FAILURE);
	    break;
	}
    }

  /*** if necessary - save stdout/err files and open ***/
  ok = open_out(stderr, &(w->err), ferr, w->force);
  if (ok == EXIT_SUCCESS)
    ok = open_out(w->err.fp, &(w->out), fout, w->force);
  if (ok != EXIT_SUCCESS)
    {
      fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  /*** show help/rev and exit ? ***/
  ok = EXIT_SUCCESS;
  if (display_help)
    ok = show_brief_help(w->out.fp, w->prog_name);
  if (display_rev)
    ok = show_rev(w->out.fp, w->prog_name);
  if (ok != EXIT_SUCCESS)
    {
      close_out(&(w->out));
      close_out(&(w->err));
      exit(ok);
    }

  /*** Count number of files to process */
  for (i = optind; i < argc; i++)
    (w->num_files)++;
  if (w->num_files == 0)
    (w->num_files)++;  /* stdin when no files */

} /* process_arg() */
