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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jfile.h"

char *jfile_h_c="$Id: jfile_h.c,v 2.5 2021/02/21 19:53:16 jmccue Exp $";

extern char *jfile_c;
extern char *jfile_c_c;
extern char *jfile_a_c;
extern char *jfile_h_c;
extern char *jfile_i_c;
extern char *jfile_u_c;

#define MSG_HELP_11  "Show Text File Character Statistics"

/*
 * show_brief_help()
 */
int show_brief_help(FILE *fp, char *pname)

{

  fprintf(fp, USG_MSG_USAGE, pname);
  fprintf(fp, "\t%s\n", MSG_HELP_11);
  fprintf(fp, USG_MSG_OPTIONS);
  fprintf(fp, USG_MSG_ARG_7BIT_FILE_1, SWITCH_CHAR, ARG_7BIT_FILE);
  fprintf(fp, USG_MSG_ARG_8BIT_FILE_1, SWITCH_CHAR, ARG_8BIT_FILE);
  fprintf(fp, USG_MSG_ARG_BOM_NO_1,    SWITCH_CHAR, ARG_BOM_NO);
  fprintf(fp, USG_MSG_ARG_DELM,        SWITCH_CHAR, ARG_DELM);
  fprintf(fp, USG_MSG_ARG_DELM_A);
  fprintf(fp, USG_MSG_ARG_DELM_B);
  fprintf(fp, USG_MSG_ARG_DELM_X);
  fprintf(fp, USG_MSG_ARG_ERR,         SWITCH_CHAR, ARG_ERR);
  fprintf(fp, USG_MSG_ARG_FORCE,       SWITCH_CHAR, ARG_FORCE);

  fprintf(fp, USG_MSG_ARG_HELP,        SWITCH_CHAR, ARG_HELP);
  fprintf(fp, USG_MSG_ARG_INVALID_1,   SWITCH_CHAR, ARG_INVALID);
  fprintf(fp, USG_MSG_ARG_NL_ASCII,    SWITCH_CHAR, ARG_NL_ASCII);
  fprintf(fp, USG_MSG_ARG_OUT,         SWITCH_CHAR, ARG_OUT);
  fprintf(fp, USG_MSG_ARG_PRINT_UTF8,  SWITCH_CHAR, ARG_PRINT_UTF8);
  fprintf(fp, USG_MSG_ARG_UTF8_FILE_1, SWITCH_CHAR, ARG_UTF8_FILE);
  fprintf(fp, USG_MSG_ARG_VERSION,     SWITCH_CHAR, ARG_VERSION);
  fprintf(fp, USG_MSG_ARG_VERBOSE_7,   SWITCH_CHAR, ARG_VERBOSE);

  return(EXIT_FAILURE);

}  /* show_brief_help() */

/******************************************************************************
* show_rev()
******************************************************************************/
int show_rev(FILE *fp, char *pname)

{

  fprintf(fp,"%s %s:\n", pname, LIT_REV);
  fprintf(fp,"\t%s\n", JFILE_H);
  fprintf(fp,"\t%s\n", jfile_c);
  fprintf(fp,"\t%s\n", jfile_a_c);
  fprintf(fp,"\t%s\n", jfile_c_c);
  fprintf(fp,"\t%s\n", jfile_h_c);
  fprintf(fp,"\t%s\n", jfile_i_c);
  fprintf(fp,"\t%s\n", jfile_u_c);
  fprintf(fp,"\t%s\n", J_LIB2_H);
  fprintf(fp,"\t     %s %s\n", LIT_INFO_02, j2_get_build());

#ifdef J_LIB2M_H
  fprintf(fp, "\t%s\n", J_LIB2M_H);
#endif

#ifdef OSTYPE
  fprintf(fp,"\t%s\n",OSTYPE);
#endif  /* OSTYPE  */
#ifdef PRETTY_NAME
  fprintf(fp,"\t%s\n",PRETTY_NAME);
#endif  /* PRETTY_NAME  */
  fprintf(fp, LIT_INFO_01, __DATE__, __TIME__);

  return(EXIT_FAILURE);

}  /* show_rev() */

/* END: jfile_h.c */
