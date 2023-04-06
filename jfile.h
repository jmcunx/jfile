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

#ifndef JFILE_H

#define JFILE_H "INCLUDED"

#define PROG_NAME  "jfile"

#define CHAR_TYPE_INVALID 0
#define CHAR_TYPE_7BIT    1
#define CHAR_TYPE_8BIT    2
#define CHAR_TYPE_UTF8    3

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_INT_NULL   ((int) '\0')
#define JLIB2_CHAR_NULL  ((char) '\0')
#define JLIB2_UCHAR_NULL ((unsigned char) '\0' )
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif
#ifndef TAB_CHAR
#define TAB_CHAR 0x09
#endif

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
#ifdef NO_JLIB
#define ARG_7BIT_FILE     '7'  /* 7-bit ASCII                        */
#define ARG_8BIT_FILE     '8'  /* 8-bit Data File                    */
#define ARG_BOM_NO        'B'  /* do NOT write BOM (Byte Order Mark) */
#define ARG_DELM          'd'  /* Field Delimiter                    */
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FORCE         'f'  /* force create files                 */
#define ARG_HELP          'h'  /* Show Help                          */
#define ARG_INVALID       'I'  /* Invalid Data File                  */
#define ARG_NL_ASCII      'n'  /* add new line to output             */
#define ARG_OUT           'o'  /* Output File                        */
#define ARG_PRINT_UTF8    'p'  /* print 'real' UTF-8 character       */
#define ARG_UTF8_FILE     'U'  /* UTF-8 Data File                    */
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define ARG_VERSION       'V'  /* Show Version Information           */
#define FILE_NAME_STDIN   "-"
#define LIT_ASCII_UTF     "(ASCII/UTF-8 ...)"
#define LIT_BYTE          "Byte"
#define LIT_C80           "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"
#define LIT_CHAR          "Char"
#define LIT_CMSG_A7       "Valid 7-ASCII"
#define LIT_CMSG_A8       "Invalid, Extended ASCII"
#define LIT_CMSG_INV      "Invalid"
#define LIT_CMSG_NL       "Valid New Line"
#define LIT_CMSG_NULL     "Invalid null"
#define LIT_CMSG_RET      "Return, valid (DOS)"
#define LIT_CMSG_U8       "Valid UTF-8"
#define LIT_CMSG_U8F      "Valid UTF-8, but Forms"
#define LIT_CMSG_U8I      "Invalid UTF-8"
#define LIT_CMSG_U8L      "Invalid UTF-8, overlong"
#define LIT_CMSG_U8P      "Valid UTF-8, but private"
#define LIT_CMSG_U8U      "Invalid UTF-8, Unalloc"
#define LIT_HEX           "Hex"
#define LIT_INFO_01       "\tBuild: %s %s\n"
#define LIT_INFO_02       "j_lib2 built:"
#define LIT_INFO_END      "******* End of Report *******"
#define LIT_POS           "Pos"
#define LIT_RECORD        "Record"
#define LIT_REV           "Revision"
#define LIT_STDIN         "(standard input)"
#define LIT_TYPE          "Type"
#define LIT_ULINE_04      "----"
#define LIT_ULINE_05      "-----"
#define LIT_ULINE_08      "--------"
#define LIT_ULINE_19      "-------------------"
#define LIT_ULINE_23      "-----------------------"
#define MSG_ERR_E000      "Try '%s %c%c' for more information\n"
#define MSG_ERR_E002      "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E025      "ERROR E025: File %s cannot be created, already exists\n"
#define MSG_ERR_E049      "ERROR E049: delimiter '%s' invalid\n"
#define MSG_ERR_E080      "ERROR E080: cannot allocate initial memory : %s\n"
#define MSG_INFO_I023     "I023:            Lines Read:  %9ld\n"
#define MSG_INFO_I024     "I024:            Bytes Read:                      %9ld\n"
#define MSG_INFO_I025     "I025:     7 bit ASCII Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I026     "I026:     8 bit ASCII Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I027     "I027:  Invalid (ctrl) Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I028     "I028:           UTF-8 Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I029     "I029:  Overlong UTF-8 Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I030     "I030:  Unallocd UTF-8 Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I031     "I031:   Invalid UTF-8 Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I032     "I032:       Undefined Found:  %9ld -- Bytes: %9ld\n"
#define MSG_INFO_I033     "I033:             BOM Found:  %9ld -- Bytes: %9ld\n"
#define MSG_WARN_W002     "W002: Open Error Bypass File '%s' : %s\n"
#define SWITCH_CHAR       '-'
#define USG_MSG_ARG_7BIT_FILE_1   "\t%c%c\t\t: Print '7-bit' ASCII characters\n"
#define USG_MSG_ARG_8BIT_FILE_1   "\t%c%c\t\t: Print '8-bit' ASCII (invalid) characters\n"
#define USG_MSG_ARG_BOM_NO_1      "\t%c%c\t\t: Flag the BOM (Byte Order Mark)\n"
#define USG_MSG_ARG_DELM          "\t%c%c c\t\t: Field delimiter, the actual character OR\n"
#define USG_MSG_ARG_DELM_A        "\t\t\t  the decimal # representing the character.\n"
#define USG_MSG_ARG_DELM_B        "\t\t\t  Ex: 7 = '^G', 9 = TAB, 124 = '|' or just '|'.\n"
#define USG_MSG_ARG_DELM_X        "\t\t\t  Must be a value < 128 (ie: < 0x80)\n"
#define USG_MSG_ARG_ERR           "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FORCE         "\t%c%c\t\t: force create of files when found\n"
#define USG_MSG_ARG_HELP          "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_INVALID_1     "\t%c%c\t\t: Print Invalid (ctrl) characters\n"
#define USG_MSG_ARG_NL_ASCII      "\t%c%c\t\t: do NOT treat New Line characters as 7-bit ASCII\n"
#define USG_MSG_ARG_OUT           "\t%c%c file\t\t: Write output to file 'file', default stdout\n"
#define USG_MSG_ARG_PRINT_UTF8    "\t%c%c\t\t: Print 'real' UTF-8 character instead of '*'\n"
#define USG_MSG_ARG_UTF8_FILE_1   "\t%c%c\t\t: Print 'UTF-8' characters\n"
#define USG_MSG_ARG_VERBOSE_7     "\t%c%c\t\t: Print 7bit, 8bit, UTF-8 and ctrl characters\n"
#define USG_MSG_ARG_VERSION       "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_OPTIONS           "Options\n"
#define USG_MSG_USAGE             "usage:\t%s [OPTIONS] [FILES ...]\n"
#endif /* NO_JLIB */

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

#ifdef NO_JLIB
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp);
char *j2_get_prgname(char *argv_0, char *default_name);
int j2_f_exist(char *file_name);
int j2_is_numr(char *s);
long int j2_clr_str(char *s, char c, int size);
#endif /* NO_JLIB */

#endif /*  JFILE_H  */
