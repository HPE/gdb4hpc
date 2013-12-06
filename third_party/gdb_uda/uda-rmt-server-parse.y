/* UDA server remote protocol command parser.

Copyright 2007 by Intrepid Technology, Inc.
Copyright 2007 by Hewlett-Packard.

Written by; Gary Funck <gary@intrepid.com>
Reviewed by: Brian Wibecan and Tanya Klinchina, HP

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

(1) Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
(2) Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.
(3) Neither the name of Intrepid Technology, or Hewlett-Packard
nor the names of its contributors may be used to endorse or promote
products derived from this software without specific prior written
permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  */

%{

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include "uda-types.h"
#include "uda-defs.h"
#include "uda-rmt-utils.h"
#include "uda-server.h"
#define uda_server_cmd_yyline uda_server_cmd_uda_server_cmd_yyline
#define uda_server_cmd_yytext uda_server_cmd_uda_server_cmd_yytext
#define uda_server_cmd_yyleng uda_server_cmd_uda_server_cmd_yyleng
#define uda_server_cmd_yysstate uda_server_cmd_uda_server_cmd_yysstate
#define uda_server_cmd_yycp uda_server_cmd_uda_server_cmd_yycp
#define uda_server_cmd_yykw uda_server_cmd_uda_server_cmd_yykw
#define uda_server_cmd_exec uda_server_cmd_exec


#define YYERRORVERBOSE 1
#define YYINITDEPTH 20
#define YYMAXDEPTH  20

extern int uda_server_cmd_yylex(void);
extern void uda_server_cmd_yyerror(const char *msg);

/* Lexer state communication variables  */

typedef enum {scan_cmd, scan_args, scan_data, scan_id, scan_pts} uda_server_cmd_yysstate_t;

static uda_server_cmd_yysstate_t uda_server_cmd_yysstate;

%}

%union {
  uda_tword_t val;
  uda_tword_t val2[2];
  uda_tint_t signed_val;
  uda_taddr_t addr_val;
  uda_debugger_pts_t pts;
  char *str;
  uda_binary_data_t data;
}

%token KW_ADDRESS
%token KW_LEN
%token KW_READ
%token KW_TYPE
%token KW_UPC
%token KW_WRITE
%token KW_DIFF
%token KW_INDEX
%token KW_PACK
%token KW_PTS
%token KW_SHARED
%token KW_SIZES
%token KW_SYM
%token KW_THREADS
%token KW_UNPACK

%token <str> ID_STRING
%token <data> BINARY_DATA
%token <val> HEX_NUM

%type <data> data
%type <val> length
%type <str> symbol
%type <val> thread_num
%type <addr_val> addrfield
%type <val> block_size
%type <val> byte_order
%type <val> elem_size
%type <signed_val> index
%type <val> int_size
%type <val> long_long_size
%type <val> long_size
%type <val> num_threads
%type <addr_val> opt_addrfield
%type <val2> opt_block_and_elem_size
%type <val> opt_block_size
%type <val> opt_block_size_arg
%type <val> opt_block_size_arg1
%type <val> opt_elem_size
%type <val2> opt_phase_and_block_size_arg
%type <signed_val> opt_sign
%type <data> packed_pts
%type <val> phase
%type <val> pointer_size
%type <val> short_size
%type <signed_val> signed_hex_num
%type <pts> unpacked_pts
%type <void> unpack_upc_pts_cmd

%%
/* Begin Grammar */

command:
	server_cmd
	;

/* Server commands are commands that the client (GDB)
   can send to the UDA server. */
server_cmd:
	calc_upc_pts_diff_cmd
	| calc_upc_pts_index_add_cmd
	| length_of_upc_pts_cmd
	| pack_upc_pts_cmd
	| read_upc_shared_mem_cmd
	| set_num_threads_cmd
	| set_type_sizes_and_byte_order_cmd
	| shared_sym_to_pts_cmd
	| unpack_upc_pts_cmd
        | upc_pts_to_addr_cmd
	| write_upc_shared_mem_cmd
	;


set_num_threads_cmd:
	'Q' KW_UPC '.' KW_THREADS begin_args num_threads
	    {
	      int num_threads = (int)$6;
	      int status;
	      status = uda_server_set_num_threads_cmd (num_threads);
	      uda_rmt_send_status (status);
	    }
	;

set_type_sizes_and_byte_order_cmd:
	'Q' KW_UPC '.' KW_TYPE '.' KW_SIZES begin_args
	short_size ',' int_size ',' long_size ','
	long_long_size ',' pointer_size ',' byte_order
	    {
	      int byte_order = (int)$18;
	      int status;
	      uda_target_type_sizes_t sizes;
	      sizes.short_size = (int)$8;
	      sizes.int_size = (int)$10;
	      sizes.long_size = (int)$12;
	      sizes.long_long_size = (int)$14;
	      sizes.pointer_size = (int)$16;
	      status = uda_server_set_target_info_cmd (sizes, byte_order);
	      uda_rmt_send_status (status);
	    }
	;

shared_sym_to_pts_cmd:
	'q' KW_UPC '.' KW_SYM begin_args
	  opt_elem_size ',' opt_block_size ',' opt_addrfield
	  begin_id_arg symbol
	    {
	      const uda_tword_t elem_size  = $6;
	      const uda_tword_t block_size = $8;
	      const uda_taddr_t sym_addr = $10;
	      const char *symbol = $12;
	      uda_debugger_pts_t pts;
	      int status;
	      status = uda_server_symbol_to_pts_cmd (symbol,
				 sym_addr, block_size, elem_size, &pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lux,%lux",
		                    pts.addrfield, pts.thread);
	      uda_free ((void *) symbol);
	    }
	;

length_of_upc_pts_cmd:
	'q' KW_UPC '.' KW_PTS '.' KW_LEN opt_block_size_arg1
	    {
	      const uda_tword_t block_size = $7;
	      uda_tword_t pts_len;
	      int status;
	      status = uda_server_length_of_pts_cmd (block_size, &pts_len);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lux", pts_len);
	    }
	;

unpack_upc_pts_cmd:
	'q' KW_UPC '.' KW_PTS '.' KW_UNPACK begin_args
	begin_packed_pts packed_pts opt_block_size_arg
	    {
	      const uda_target_pts_t *packed_pts =
	                 (uda_target_pts_t *)$9.bytes;
	      const uda_tword_t block_size = $10;
	      uda_debugger_pts_t pts;
	      int status;
              status = uda_server_unpack_pts_cmd (packed_pts, block_size, &pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        {
	          const int has_opaque = uda_job->image->target_pts_has_opaque
	                                 && (pts.opaque != 0);
		  if (has_opaque)
		    {
		      if (pts.phase != 0)
	                uda_rmt_send_reply ("%lux,%lux,%lux,%lux",
		             pts.addrfield, pts.thread,
			     pts.phase, pts.opaque);
		      else
	                uda_rmt_send_reply ("%lux,%lux,,%lux",
		             pts.addrfield, pts.thread, pts.opaque);
		    }
		  else
		    {
		      if (pts.phase != 0)
	                uda_rmt_send_reply ("%lux,%lux,%lux",
		             pts.addrfield, pts.thread, pts.phase);
		      else
	                uda_rmt_send_reply ("%lux,%lux",
		             pts.addrfield, pts.thread);
		    }
	        }
	       uda_free ((void *) packed_pts);
	    }
	;

pack_upc_pts_cmd:
	'q' KW_UPC '.' KW_PTS '.' KW_PACK begin_args
	addrfield ',' thread_num opt_phase_and_block_size_arg
	    {
	      uda_debugger_pts_t pts;
	      uda_tword_t block_size;
	      uda_target_pts_t packed_pts;
	      size_t packed_pts_len;
	      int status;
	      pts.addrfield = $8;
	      pts.thread = $10;
	      pts.phase = $11[0];
	      pts.opaque = 0;
	      block_size = $11[1];
              status = uda_server_pack_pts_cmd (&pts, block_size,
						&packed_pts_len,
	                                        &packed_pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%*X", packed_pts_len, &packed_pts);
	    }
	;

upc_pts_to_addr_cmd:
	'q' KW_UPC '.' KW_PTS '.' KW_ADDRESS begin_args
        addrfield ',' thread_num opt_block_and_elem_size
	    {
	      uda_debugger_pts_t pts;
	      uda_tword_t block_size;
	      uda_tword_t elem_size;
	      uda_taddr_t local_addr;
	      int status;
	      pts.addrfield = $8;
	      pts.thread = $10;
	      pts.phase = 0;
	      pts.opaque = 0;
	      block_size = $11[0];
	      elem_size = $11[1];
              status = uda_server_pts_to_addr_cmd (&pts, block_size,
	                                           elem_size, &local_addr);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lx", local_addr);
	    }
	;

calc_upc_pts_index_add_cmd:
	'q' KW_UPC '.' KW_PTS '.' KW_INDEX begin_args
	unpacked_pts ',' index ',' elem_size
	opt_block_size_arg
	    {
	      const uda_debugger_pts_t pts_operand = $8;
	      const uda_tint_t index = $10;
	      const uda_tword_t elem_size = $12;
	      const uda_tword_t block_size = $13;
	      uda_debugger_pts_t pts;
	      int status;
              status = uda_server_pts_index_add_cmd (&pts_operand,
	                                             index,
	                                             elem_size, block_size,
						     &pts);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        {
		  if (pts.phase != 0)
	            uda_rmt_send_reply ("%lux,%lux,%lux",
		                        pts.addrfield, pts.thread,
					pts.phase);
                  else
	            uda_rmt_send_reply ("%lux,%lux",
		                        pts.addrfield, pts.thread);
		}
	    }
	;

calc_upc_pts_diff_cmd:
	'q' KW_UPC '.' KW_PTS '.' KW_DIFF begin_args
	unpacked_pts ',' unpacked_pts ',' elem_size
	opt_block_size_arg
	    {
	      const uda_debugger_pts_t pts_oprnd_1 = $8;
	      const uda_debugger_pts_t pts_oprnd_2 = $10;
	      const uda_tword_t elem_size = $12;
	      const uda_tword_t block_size = $13;
	      uda_tint_t diff;
	      int status;
              status = uda_server_pts_diff_cmd (&pts_oprnd_1, &pts_oprnd_2,
	                                        elem_size, block_size,
				                &diff);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lx", diff);
	    }
	;

read_upc_shared_mem_cmd:
	'q' KW_UPC '.' KW_READ '.' KW_SHARED begin_args
	addrfield ',' thread_num ',' length
	    {
	      const uda_taddr_t addr = $8;
	      const uda_tword_t thread_num = $10;
	      const uda_tword_t length = $12;
	      uda_binary_data_t data;
	      int status;
	      data.len = length;
              data.bytes = (uda_byte_t *) uda_malloc ((size_t) length);
              status = uda_server_read_upc_shared_mem_cmd (
				  addr, thread_num, length, &data);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%*b", data.len, data.bytes);
	      uda_free (data.bytes);
	    }
	;

write_upc_shared_mem_cmd:
	'Q' KW_UPC '.' KW_WRITE '.' KW_SHARED begin_args
	addrfield ',' thread_num begin_binary_data_arg data
	    {
	      const uda_taddr_t addrfield = $8;
	      const uda_tword_t thread_num = $10;
	      const uda_binary_data_t data = $12;
	      uda_tword_t bytes_written;
	      int status;
              status = uda_server_write_upc_shared_mem_cmd (
				  addrfield, thread_num,
				  &bytes_written, &data);
	      if (status != uda_ok)
	        uda_rmt_send_status (status);
	      else
	        uda_rmt_send_reply ("%lux", bytes_written);
	      uda_free ((void *) data.bytes);
	    }
	;


begin_args:
	':' { uda_server_cmd_yysstate = scan_args; }
	;

begin_binary_data_arg:
	',' { uda_server_cmd_yysstate = scan_data; }
	;

begin_id_arg:
	',' { uda_server_cmd_yysstate = scan_id; }

data:
	BINARY_DATA
	;

length:
	HEX_NUM
	;

symbol:
	ID_STRING
	;

thread_num:
	HEX_NUM
	;

addrfield:
	HEX_NUM
	    { $$ = (uda_taddr_t)$1; }
	;

begin_packed_pts:
	    { uda_server_cmd_yysstate = scan_pts; }
	;

block_size:
	HEX_NUM
	;

byte_order:
	HEX_NUM
	;

elem_size:
	HEX_NUM
	;

index:
	signed_hex_num
	;

int_size:
	HEX_NUM
	;

long_long_size:
	HEX_NUM
	;

long_size:
	HEX_NUM
	;

num_threads:
	HEX_NUM
	;

opt_addrfield:
	/* empty */
	    { $$ = 0; }
	| addrfield
	    { $$ = $1; }
	;

opt_block_size:
	/* empty */
	    { $$ = 1; }
	| block_size
	    { $$ = $1; }
	;

opt_block_size_arg:
	/* empty */
	    { $$ = 1; }
	| ',' opt_block_size
	    { $$ = $2; }
	;

opt_block_size_arg1:
	/* empty */
	    { $$ = 1; }
	| begin_args block_size
	    { $$ = $2; }
	;

opt_block_and_elem_size:
	/* empty */
	    { $$[0] = 0; $$[1] = 1; }
	| ',' opt_elem_size
	    { $$[0] = 0; $$[1] = $2; }
	| ',' block_size ',' opt_elem_size
	    { $$[0] = $2; $$[1] = $4; }
	;

opt_elem_size:
	/* empty */
	    { $$ = 1; }
	| elem_size
	    { $$ = $1; }
	;

opt_phase_and_block_size_arg:
	/* empty */
	    { $$[0] = 0; $$[1] = 1; }
	| ',' opt_block_size
	    { $$[0] = 0; $$[1] = $2; }
	| ',' phase ',' opt_block_size
	    { $$[0] = $2; $$[1] = $4; }
	;

opt_sign:
	/* empty */ { $$ = 1; }
	| '+' { $$ = 1; }
	| '-' { $$ = -1; }
	;

packed_pts:
	BINARY_DATA
	;

phase:
	HEX_NUM
	;

pointer_size:
	HEX_NUM
	;

short_size:
	HEX_NUM
	;

signed_hex_num:
	opt_sign HEX_NUM
	    { $$ = $1 * (uda_tint_t)$2; }
	;

unpacked_pts:
	addrfield ',' thread_num ',' phase
	    {
	      $$.addrfield = $1;
	      $$.thread = $3;
	      $$.phase = $5;
	      $$.opaque = 0;
	    }
	;

/* End Grammar */
%%

static const char *uda_server_cmd_yyline;
static const char *uda_server_cmd_yycp;
static const char *uda_server_cmd_yytext;
static size_t uda_server_cmd_yyleng;

typedef struct kw_entry_struct
  {
    const char *kw_id;
    const int kw_token;
  } kw_entry_t;

const kw_entry_t uda_server_cmd_yykw[] =
{
  {"address", KW_ADDRESS},
  {"len", KW_LEN},
  {"read", KW_READ},
  {"type", KW_TYPE},
  {"upc", KW_UPC},
  {"write", KW_WRITE},
  {"diff", KW_DIFF},
  {"index", KW_INDEX},
  {"pack", KW_PACK},
  {"pts", KW_PTS},
  {"shared", KW_SHARED},
  {"sizes", KW_SIZES},
  {"sym", KW_SYM},
  {"threads", KW_THREADS},
  {"unpack", KW_UNPACK},
};

#define NUM_KW (sizeof (uda_server_cmd_yykw) / sizeof (kw_entry_t))

#define init_scan(BUF) { uda_server_cmd_yyline = (BUF); uda_server_cmd_yycp = uda_server_cmd_yyline; uda_server_cmd_yysstate = scan_cmd; }
#define nextch() { if (*uda_server_cmd_yycp) { ++uda_server_cmd_yycp; ++uda_server_cmd_yyleng; } }
#define unch() { if (uda_server_cmd_yycp != uda_server_cmd_yyline) { --uda_server_cmd_yycp; --uda_server_cmd_yyleng; } }
#define reset_scan() { uda_server_cmd_yycp = uda_server_cmd_yytext; uda_server_cmd_yyleng = 0; }

int
uda_server_cmd_yylex()
{
  int ch_token;
  uda_server_cmd_yytext = uda_server_cmd_yycp;
  reset_scan ();
  switch (uda_server_cmd_yysstate)
  {
  case scan_cmd:
    {
      /* The first character is a special case, because it
         is used in the protocol to qualify the type of command. */
      if (uda_server_cmd_yycp != uda_server_cmd_yyline && isalpha (*uda_server_cmd_yycp))
        {
	  size_t i;
	  while (isalpha (*uda_server_cmd_yycp)) { nextch(); }
	  for (i = 0; i < NUM_KW; ++i)
	    {
	      if ((strlen (uda_server_cmd_yykw[i].kw_id) == uda_server_cmd_yyleng)
	          && (strncmp (uda_server_cmd_yytext, uda_server_cmd_yykw[i].kw_id, uda_server_cmd_yyleng) == 0))
		{
#ifdef DEBUG_LEX
		  printf ("scan: keyword: %s\n", uda_server_cmd_yykw[i].kw_id);
#endif
		  return uda_server_cmd_yykw[i].kw_token;
	        }
	    }
	  reset_scan ();
        }
      break;
    }
  case scan_args:
    {
      if (isxdigit (*uda_server_cmd_yycp))
        {
	  while (isxdigit (*uda_server_cmd_yycp)) { nextch (); }
          yylval.val = uda_decode_hex_word (uda_server_cmd_yytext, uda_server_cmd_yyleng);
#ifdef DEBUG_LEX
	  printf ("scan: hex: %lx\n", yylval.val);
#endif
	  return HEX_NUM;
	}
      break;
    }
  case scan_pts:
    {
      if (isxdigit (*uda_server_cmd_yycp))
        {
          const size_t n_bytes = uda_scan_hex_bytes (&uda_server_cmd_yycp);
	  uda_server_cmd_yyleng = uda_server_cmd_yycp - uda_server_cmd_yytext;
          yylval.data.bytes = (unsigned char *)uda_malloc(n_bytes);
          yylval.data.len = uda_decode_hex_bytes ((char *) yylval.data.bytes,
	                                           uda_server_cmd_yytext, uda_server_cmd_yyleng);
          uda_server_cmd_yysstate = scan_args;
          return BINARY_DATA;
	}
      break;
    }
  case scan_data:
    {
      const size_t n_bytes = uda_scan_binary_data (&uda_server_cmd_yycp);
      uda_server_cmd_yyleng = uda_server_cmd_yycp - uda_server_cmd_yytext;
      yylval.data.bytes = (unsigned char *)uda_malloc(n_bytes);
      yylval.data.len = uda_decode_binary_data ((char *) yylval.data.bytes,
                                                 uda_server_cmd_yytext, uda_server_cmd_yyleng);
      uda_server_cmd_yysstate = scan_args;
#ifdef DEBUG_LEX
      printf ("scan: biinary len: %ld\n", yylval.data.len);
#endif
      return BINARY_DATA;
    }
  case scan_id:
    {
      if (isalpha (*uda_server_cmd_yycp) || *uda_server_cmd_yycp == '$' || *uda_server_cmd_yycp == '_')
        {
	  nextch();
          while (isalnum (*uda_server_cmd_yycp) || *uda_server_cmd_yycp == '$' || *uda_server_cmd_yycp == '_'
	         || *uda_server_cmd_yycp == '-' || *uda_server_cmd_yycp == '.' || *uda_server_cmd_yycp == '@')
	    { nextch(); }
          yylval.str = (char *)uda_malloc(uda_server_cmd_yyleng + 1);
	  memcpy (yylval.str, uda_server_cmd_yytext, uda_server_cmd_yyleng);
	  yylval.str[uda_server_cmd_yyleng] = '\0';
          uda_server_cmd_yysstate = scan_args;
#ifdef DEBUG_LEX
	  printf ("scan: id: %s\n", yylval.str);
#endif
	  return ID_STRING;
	}
      break;
    }
  default:
    abort ();
  }
  ch_token = (*uda_server_cmd_yycp & 0xff);
  nextch ();
#ifdef DEBUG_LEX
  printf ("scan: char: '%c'\n", ch_token);
#endif
  return ch_token;
}

void
uda_server_cmd_yyerror (const char *msg)
{
  fprintf (stderr, "%s at '%s'\n", msg, uda_server_cmd_yycp);
}

int
uda_server_cmd_exec (const char *cmd)
{
  init_scan (cmd);
  return yyparse();
}
