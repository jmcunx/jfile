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
#include <ctype.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jfile.h"

/*
 * init_w() -- initialize work area
 */
void init_w(struct s_work *w, char *a)

{
  init_finfo(&(w->out));
  init_finfo(&(w->err));
  w->err.fp    = stderr;
  w->out.fp    = stdout;

  w->prog_name        = j2_get_prgname(a, PROG_NAME);
  w->num_files        = 0;
  w->show_raw_utf8    = (int) FALSE;
  w->verbose          = (int) FALSE;
  w->show_7bit        = (int) FALSE;
  w->show_8bit        = (int) FALSE;
  w->show_invalid     = (int) FALSE;
  w->show_utf_8       = (int) FALSE;
  w->nl_not_ascii     = (int) TRUE;
  w->flag_bom         = (int) FALSE;
  w->force            = (int) FALSE;

  w->delim            = JLIB2_CHAR_NULL;
  w->is_delim         = (int) FALSE;

}  /* init_w() */

/*
 * init() -- initialize
 */
void init(int argc, char **argv, struct s_work *w)

{

  init_w(w, argv[0]);

  process_arg(argc, argv, w);

}  /* init() */

/*
 * init_count() -- initialize count area
 */
void init_count(struct s_counts *c)

{
  c->bom_found                 = (int) FALSE;
  c->lines_read                = 0L;
  c->bytes_read                = 0L;
  c->count_utf8                = 0L;
  c->count_utf8_extra          = 0L;
  c->count_utf8_overlong       = 0L;
  c->count_utf8_overlong_extra = 0L;
  c->count_utf8_invalid        = 0L;
  c->count_utf8_invalid_extra  = 0L;
  c->count_utf8_unalloc        = 0L;
  c->count_utf8_unalloc_extra  = 0L;
  c->count_7bit                = 0L;
  c->count_8bit                = 0L;
  c->count_ctrl                = 0L;
  c->bytes_undefined           = 0L;

} /* init_count() */
