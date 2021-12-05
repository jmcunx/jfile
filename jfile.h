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

#ifndef JFILE_H

#define JFILE_H "$Id: jfile.h,v 2.3 2020/06/14 18:31:09 jmccue Exp $"

#define PROG_NAME  "jfile"

#define CHAR_TYPE_INVALID 0
#define CHAR_TYPE_7BIT    1
#define CHAR_TYPE_8BIT    2
#define CHAR_TYPE_UTF8    3

/*** structures ***/
struct s_utf_split
  {
  int utf_bytes;   /* # of bytes for utf-8 character      */
  int is_overlong; /* TRUE or FALSE, FALSE = overlong     */
  int is_invalid;  /* TRUE or FALSE, FALSE = invalid      */
  int is_unalloc;  /* TRUE or FALSE, FALSE = Unallocated  */
  int is_private;  /* TRUE or FALSE, FALSE = Private      */
  int is_forms;    /* TRUE or FALSE, FALSE = Forms        */
  unsigned char a;
  unsigned char b;
  unsigned char c;
  unsigned char d;
  } ;

struct s_file_info
  {
    FILE *fp;
    char *fname;
  } ;

struct s_counts
  {
    int bom_found;
    long int count_utf8;
    long int count_utf8_extra;
    long int count_utf8_overlong;
    long int count_utf8_overlong_extra;
    long int count_utf8_invalid;
    long int count_utf8_invalid_extra;
    long int count_utf8_unalloc;
    long int count_utf8_unalloc_extra;
    long int count_7bit;
    long int count_8bit;
    long int count_ctrl;
    long int lines_read;
    long int bytes_read;
    long int bytes_undefined;
  } ;

struct s_work
  {
    struct s_file_info out;         /* default stdout              */
    struct s_file_info err;         /* default stderr              */
    char *prog_name;                /* real program name           */
    unsigned char delim;            /* Field Delimiter             */
    int is_delim;                   /* delimitd ? TRUE or FALSE    */
    int num_files;                  /* # of files to process       */
    int show_raw_utf8;              /* TRUE or FALSE               */
    int verbose;                    /* TRUE or FALSE               */
    int force;                      /* TRUE or FALSE               */
    int show_7bit;                  /* TRUE or FALSE               */
    int show_8bit;                  /* TRUE or FALSE               */
    int show_invalid;               /* TRUE or FALSE               */
    int show_utf_8;                 /* TRUE or FALSE               */
    int nl_not_ascii;               /* TRUE or FALSE               */
    int flag_bom;                   /* TRUE or FALSE               */
  } ;

/*** messages ***/

/*** prototypes ***/
void init(int, char **, struct s_work *);
void init_finfo(struct s_file_info *);
void init_count(struct s_counts *c);
int  show_brief_help(FILE *, char *);
int  show_rev(FILE *, char *);
void process_arg(int, char **, struct s_work *);
int  open_out(FILE *, struct s_file_info *, char *, int);
void close_out(struct s_file_info *);
int  open_in(FILE **, char *, FILE *);
void close_in(FILE **, char *);
char *get_progname(char *, char *);
void show_utf(struct s_work *, struct s_counts *, char *, size_t, size_t, long int, struct s_utf_split *);
int  utf_split(char *, size_t, size_t, struct s_utf_split *);

#endif /*  JFILE_H  */

/******************************* END: jfile.h ********************************/
