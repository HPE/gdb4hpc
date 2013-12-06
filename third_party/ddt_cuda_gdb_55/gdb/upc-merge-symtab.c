/*
** Copyright 2007 by Hewlett-Packard.  Permission granted to copy and modify
** provided attribution is given.
**
** Merge UPC Symbol Table (MUST) addition to gdb
** Brian Wibecan and Tanya Klinchina, HP
*/

#include "defs.h"
#include "arch-utils.h"
#include "symtab.h"
#include "gdbtypes.h"
#include "block.h"
#include "dictionary.h"
#include "value.h"
#include "symfile.h"
#include "psympriv.h"
#include "psymtab.h"
#include "objfiles.h"
#include "gdbcmd.h"
#include "language.h"
#include "gdb_assert.h"
#include "gdbthread.h"
#include "upc-thread.h"

extern int upc_thread_count ();

extern char *tilde_expand(char*);

static int MUST_contains_shared_parts(struct type *);
static int MUST_is_shared_type(struct type *);
static struct type *MUST_embed_shared_type(struct block *, 
					   struct type *, struct type *);
static struct type *MUST_copy_type(struct block *, struct type *);
static struct type *MUST_copy_fields(struct block *, struct type *,
				     struct type *);
static int MUST_find_function(struct blockvector *, int, struct block **);
static int MUST_local_to_func(struct block *, struct block *);
static struct block *MUST_find_parent_function(struct block *);
static void MUST_modify_csymbol(struct block *, struct symbol *, 
				struct symbol *);
static void MUST_add_csymbol(struct block *, struct symbol *);
static int MUST_match_symbol(struct block *, int, struct symbol *,
			     struct blockvector *);
static void MUST_walk_block_symbols(struct blockvector *,
				    struct blockvector *, int);
static void MUST_upc_merge_command(char *, int);
static void MUST_merge_symtabs (void);

static void MUST_show_debug(struct ui_file *, int,
			    struct cmd_list_element *, const char *);

static int MUST_num_threads = 0;

static struct objfile *MUST_current_c_objfile = 0;
static struct symtab *MUST_ctab = NULL;
static struct symtab *MUST_upctab = NULL;

static int MUST_debug = 0;

#define MUST_trace_ptype(t) \
  if (MUST_debug) (type_print((t), "", gdb_stdout, 0),	\
		 printf_filtered("\n") )
#define MUST_trace_psym(s) \
  if (MUST_debug) \
    (type_print(SYMBOL_TYPE((s)), SYMBOL_PRINT_NAME((s)), gdb_stdout, 0), \
     printf_filtered("\n") )

#define MUST_trace(args...) \
  if (MUST_debug) printf_filtered(args)

/* from psymtab.c */
#define PSYMTAB_TO_SYMTAB(pst)  \
     ((pst) -> symtab != NULL ? (pst) -> symtab : psymtab_to_symtab (pst))
extern struct symtab *psymtab_to_symtab (struct partial_symtab *pst);

/* this type or any subcomponents of this type
 * are shared.  This thus includes shared types or types
 * with embedded pointers to shared data.
 */
static int MUST_contains_shared_parts(struct type *type) {
  int i, len;
  if (!type) {
    return 0;
  }
  if (TYPE_UPC_SHARED(type)) {
    return 1;
  }
  switch (TYPE_CODE(type)) {

  case TYPE_CODE_ARRAY:
  case TYPE_CODE_PTR:
    return MUST_contains_shared_parts(TYPE_TARGET_TYPE(type));
    break;

    /* Check all fields in structs and unions */
  case TYPE_CODE_STRUCT:
  case TYPE_CODE_UNION:
    len = TYPE_NFIELDS(type);
    for (i = 0; i < len; i++) {
      if (MUST_contains_shared_parts(TYPE_FIELD_TYPE(type, i))) {
	return 1;
      }
    }
    break;

    /* Check the return type and all parameter types */
  case TYPE_CODE_FUNC:
    if (MUST_contains_shared_parts(TYPE_TARGET_TYPE(type))) {
      return 1;
    }
    len = TYPE_NFIELDS(type);
    for (i = 0; i < len; i++) {
      if (MUST_contains_shared_parts(TYPE_FIELD_TYPE(type, i))) {
	return 1;
      }
    }
    break;
  }

  /* No shared components found */
  return 0;
}


/* Checks if a type is fundamentally a shared type.
 * It may be a shared aggregate or scalar, or it may
 * be an array of shared aggregates or scalars.  In
 * either case, it requires a pointer-to-shared to
 * create a pointer reference to such a type.
 */
static int MUST_is_shared_type(struct type *cand) {
  while (TYPE_CODE(cand) == TYPE_CODE_ARRAY) {
    cand = TYPE_TARGET_TYPE(cand);
  }
  if (TYPE_UPC_SHARED(cand)) {
    return 1;
  }
  return 0;
}

/* Replace as necessary any type components that are shared
 * in the original UPC code.
 */
/* FIXME: Error checking?  Error reporting? */
static struct type *MUST_embed_shared_type(struct block *cblk,
					   struct type *utype,
					   struct type *ctype) {
  struct type *newtype;
  int len, i;
  struct gdbarch *gdbarch = get_current_arch();
    MUST_trace("Begin Must_embed_shared_type\n");
  if (!MUST_contains_shared_parts(utype)) {
    MUST_trace("No components need to be modified\n");

    /* No components that need to be modified */
  } else if (TYPE_UPC_SHARED(utype)) {

    /* This type is shared.  Find or create a non-shared version
     * of the type in the UPC table, process the non-shared version
     * in the C table, then create a shared version in the C table.
     */
    newtype = make_qual_variant_type(null_type_quals, utype, 0);
    ctype = MUST_embed_shared_type(cblk, newtype, ctype);
    ctype = make_qual_variant_type(TYPE_QUALS(utype), ctype, 0);
  } else {
    switch (TYPE_CODE(utype)) {

    case TYPE_CODE_ARRAY:
      if (TYPE_CODE(utype) != TYPE_CODE(ctype)) {
	printf_filtered("Embed: Non-matching type codes\n");
      } else if (MUST_is_shared_type(utype)) {
	struct type *c_elem_type, *upc_elem_type,
	  *subscr_done, *next_subscr, *arr_type;

	/* This is an array of shared data.  The number of dimensions
	 * in the UPC and the C versions, and the dimensions of those
	 * versions, may not match.
	 */

	/* Find the underlying element type in both the UPC and C
	 * versions.
	 */
	c_elem_type = ctype;
	while (TYPE_CODE(c_elem_type) == TYPE_CODE_ARRAY) {
	  c_elem_type = TYPE_TARGET_TYPE(c_elem_type);
	}
	upc_elem_type = utype;
	while (TYPE_CODE(upc_elem_type) == TYPE_CODE_ARRAY) {
	  upc_elem_type = TYPE_TARGET_TYPE(upc_elem_type);
	}
	/* Walk backwards through the subscripts, creating array
	 * types and ranges as needed.
	 */

	/* Start the backwards walk at the element type */
	subscr_done = upc_elem_type;
	c_elem_type = 
	  MUST_embed_shared_type(cblk, upc_elem_type, c_elem_type);
	arr_type = c_elem_type;
	do {
	  struct type *range, *new_range;
	  int new_high_bound;

	  /* Start at the main type.  Walk forward through the dimensions
	   * until we get to the one that points to the last one processed.
	   */
	  next_subscr = utype;
	  while (TYPE_TARGET_TYPE(next_subscr) != subscr_done) {
	    next_subscr = TYPE_TARGET_TYPE(next_subscr);
	  }

	  /* Create a range type that matches the one for the current
	   * subscript.
	   */
	  range = TYPE_INDEX_TYPE(next_subscr);
	  new_high_bound = TYPE_HIGH_BOUND(range);
          if (MUST_num_threads == 0) MUST_num_threads = upc_thread_count();
	  if (TYPE_UPC_HAS_THREADS_FACTOR(range)) {
	    new_high_bound = (new_high_bound + 1) * MUST_num_threads - 1;
	  }
	  new_range = create_range_type(0,
					builtin_type (gdbarch)->builtin_int,
					TYPE_LOW_BOUND(range),
					new_high_bound);

	  /* Create an array type for this sub-array. */
	  arr_type = create_array_type(0, arr_type, new_range);

	  /* Move the "last subscript done" pointer to the one
	   * just finished.
	   */
	  subscr_done = next_subscr;

	  /* Keep going until we hit the top-level type. */
	} while (next_subscr != utype);
	ctype = arr_type;
      } else {
	/* This is an array of non-shared data, and the non-shared
	 * data contains pointers to shared data.  The array dimensions
	 * are thus accurate and we only need to handle the array data
	 * type.
	 */
	newtype = 
	  MUST_embed_shared_type(cblk, TYPE_TARGET_TYPE(utype),
				 TYPE_TARGET_TYPE(ctype));
	TYPE_TARGET_TYPE(ctype) = newtype;
	return ctype;
      }
      break;

    case TYPE_CODE_PTR:
      MUST_trace("\nFound ptr type\n");
      if (MUST_is_shared_type(TYPE_TARGET_TYPE(utype))) {
	struct type *targ;

	/* This is a pointer to shared.  It is represented in
	 * some other fashion, so we have to copy the type rather
	 * than massage it.
	 */
	MUST_trace("\nCopy target type\n");
	MUST_trace_ptype(TYPE_TARGET_TYPE(utype));	
	/* Copy the target type. */
	targ = MUST_copy_type(cblk, TYPE_TARGET_TYPE(utype));
	MUST_trace("\nNew targ type is\n");
	MUST_trace_ptype(targ);	
	if (targ) {
	  ctype = lookup_pointer_type(targ);
	} else {
	  printf_filtered("Copy: Could not copy target type\n");
	}
      } else {
	MUST_trace("\nEmbed shared type\n");
	MUST_trace_ptype(TYPE_TARGET_TYPE(utype));	
	if (TYPE_CODE(utype) != TYPE_CODE(ctype)) {
	  printf_filtered("Embed: Non-matching type codes\n");
	} else {
	  newtype = 
	    MUST_embed_shared_type(cblk, TYPE_TARGET_TYPE(utype),
				   TYPE_TARGET_TYPE(ctype));
	  TYPE_TARGET_TYPE(ctype) = newtype;
	}
      }
      MUST_trace("\nNew type is\n");
      MUST_trace_ptype(TYPE_TARGET_TYPE(ctype));	
      break;

    case TYPE_CODE_STRUCT:
    case TYPE_CODE_UNION:
      /* Convert all the fields for a struct or union. */
      if (TYPE_CODE(utype) != TYPE_CODE(ctype)) {
	printf_filtered("Embed: Non-matching type codes\n");
      } else if (TYPE_NFIELDS(utype) != TYPE_NFIELDS(ctype)) {
	printf_filtered("Embed: Different number of structure members\n");
      } else {
	len = TYPE_NFIELDS(utype);
	for (i = 0; i < len; i++) {
	  TYPE_FIELD_TYPE(ctype, i) =
	    MUST_embed_shared_type(cblk, TYPE_FIELD_TYPE(utype, i),
				   TYPE_FIELD_TYPE(ctype, i));
	}
      }
      break;

    case TYPE_CODE_FUNC:
      if (TYPE_CODE(utype) != TYPE_CODE(ctype)) {
	printf_filtered("Embed: Non-matching type codes\n");
      } else {
	len = TYPE_NFIELDS(utype);
	/* Convert all the parameters. */
	for (i = 0; i < len; i++) {
	  TYPE_FIELD_TYPE(ctype, i) =
	    MUST_embed_shared_type(cblk, TYPE_FIELD_TYPE(utype, i),
				   TYPE_FIELD_TYPE(ctype, i));
	}
	/* Convert the return type. */
	TYPE_TARGET_TYPE(ctype) =
	  MUST_embed_shared_type(cblk, TYPE_TARGET_TYPE(utype),
				 TYPE_TARGET_TYPE(ctype));
      }
      break;

    case TYPE_CODE_TYPEDEF:
      /* Convert the definition of the typedef. */
      if (TYPE_CODE(utype) != TYPE_CODE(ctype)) {
	printf_filtered("Embed: Non-matching type codes\n");
	return ctype;
      } else {
	newtype = 
	  MUST_embed_shared_type(cblk, TYPE_TARGET_TYPE(utype),
				 TYPE_TARGET_TYPE(ctype));
	/* Make the typedef point to the new definition, in case
	 * it was changed.
	 */
	TYPE_TARGET_TYPE(ctype) = newtype;
      }
      break;

    default:
      printf_filtered("Embed: non-handled type code %d\n", TYPE_CODE(utype));
    }
  }
  /* Return whatever was converted or the original type if it was left
   * unconverted.
   */
    MUST_trace("\nReturn\n");
    MUST_trace_ptype(ctype);
    MUST_trace("End Must_embed_shared_type\n");
  return ctype;
}


/* Copy important fields from one type in one symbol table to a type in a
 * different symbol table.
 */ 
struct type *MUST_copy_common_data(struct type *new_type,
				   struct type *type) {
  if (TYPE_NAME(type)) {
    TYPE_NAME(new_type) = xstrdup(TYPE_NAME(type));
  } else {
    TYPE_NAME(new_type) = 0;
  }
  if (TYPE_TAG_NAME(type)) {
    TYPE_TAG_NAME(new_type) = xstrdup(TYPE_TAG_NAME(type));
  } else {
    TYPE_TAG_NAME(new_type) = 0;
  }
  *TYPE_MAIN_TYPE (new_type) = *TYPE_MAIN_TYPE (type);
  TYPE_VPTR_FIELDNO(new_type) = TYPE_VPTR_FIELDNO(type);
  TYPE_INSTANCE_FLAGS(new_type) = TYPE_INSTANCE_FLAGS(type);
  TYPE_LENGTH(new_type) = TYPE_LENGTH(type);
  new_type = make_qual_variant_type(TYPE_QUALS(type), new_type, 0);
  return new_type;
}

static struct type *MUST_copy_fields(struct block *cblk, 
				     struct type *to, struct type *from) {
  struct type *fld_type;
  size_t fsize;
  int fcount, i;
  char *fname;

  /* Allocate a new type, fill it in, and copy the types
   * of the structure members. */
  fcount = TYPE_NFIELDS(from);
  fsize = sizeof(struct field) * fcount;
  TYPE_FIELDS(to) = xmalloc(fsize);
  TYPE_NFIELDS(to) = fcount;
  if (fcount) {
    memset(TYPE_FIELDS(to), 0, fsize);
  }
  for (i = 0; i < fcount; i++) {
    fld_type = MUST_copy_type(cblk, TYPE_FIELD_TYPE(from, i));
    if (fld_type) {
      TYPE_FIELD_TYPE(to, i) = fld_type;
    }
    fname = TYPE_FIELD_NAME(from, i);
    TYPE_FIELD_NAME(to, i) = fname ? xstrdup(fname) : 0;
    TYPE_FIELD_BITPOS(to, i) = TYPE_FIELD_BITPOS(from, i);
  }
  return to;
}

/* Copy a type to the C symbol table.  The type is presumed to be
 * in the UPC symbol table, and so must have data copied elsewhere.
 */
static struct type *MUST_copy_type(struct block *cblk, 
				   struct type *type) {
  struct gdbarch *gdbarch = get_current_arch();
  struct type *copy, *subtype;
  int i;

  if (!type) return 0;

  /* If there exists a type with the same name or, failing that, the
   * same tag, assume they correspond.  The found type may need to be
   * massaged for shared components.
   */
  if (TYPE_NAME(type)) {
    copy = lookup_typename(current_language, gdbarch, TYPE_NAME(type), cblk, 1);
    if (copy && TYPE_CODE(copy) == TYPE_CODE(type)) {
      copy = MUST_embed_shared_type(cblk, type, copy);
      return copy;
    }
  } else if (TYPE_TAG_NAME(type)) {
    struct symbol *s;
    s = lookup_symbol(TYPE_TAG_NAME(type), cblk, STRUCT_DOMAIN, 0);
    if (s && TYPE_CODE(SYMBOL_TYPE(s)) == TYPE_CODE(type)) {
      copy = MUST_embed_shared_type(cblk, type, SYMBOL_TYPE(s));
      return copy;
    }
  }
  switch (TYPE_CODE(type)) {

  case TYPE_CODE_PTR:
    /* Copy the target and create a pointer to it.  The lookup
     * routine will find a pointer type if it exists, otherwise
     * create one.
     */
    subtype = MUST_copy_type(cblk, TYPE_TARGET_TYPE(type));
    if (subtype) {
      copy = lookup_pointer_type(subtype);
      copy = MUST_copy_common_data(copy, type);
    } else {
      printf_filtered("Copy: could not copy target type\n");
      return 0;
    }
    break;

  case TYPE_CODE_ARRAY: {
    struct type *urange, *crange;

    /* Create an array type with the desired range.  Copy
     * the element type.
     */
    copy = alloc_type(MUST_current_c_objfile);
    TYPE_CODE(copy) = TYPE_CODE(type);
    urange = TYPE_INDEX_TYPE(type);
    subtype = MUST_copy_type(cblk, TYPE_TARGET_TYPE(type));
    crange = create_range_type(0, builtin_type (gdbarch)->builtin_int,
			       TYPE_LOW_BOUND(urange),
			       TYPE_HIGH_BOUND(urange));
    crange = MUST_copy_common_data(crange, urange);
    copy = create_array_type(0, subtype, crange);
    copy = MUST_copy_common_data(copy, type);
    break;
  }

  case TYPE_CODE_STRUCT:
  case TYPE_CODE_UNION:
  case TYPE_CODE_ENUM:

    /* Allocate a new type, fill it in, and copy the types
     * of the composite type  members.
     */
    copy = alloc_type(MUST_current_c_objfile);
    TYPE_CODE(copy) = TYPE_CODE(type);
    INIT_CPLUS_SPECIFIC(copy);
    copy = MUST_copy_fields(cblk, copy, type);
    if (!copy) {
      printf_filtered("Copy: could not copy member type\n");
      return 0;
    }
    copy = MUST_copy_common_data(copy, type);
    break;

  case TYPE_CODE_FLAGS:
    printf_filtered("Flags copy NYI\n");
    return 0;

  case TYPE_CODE_FUNC:
    /* Copy the return type.  Make a new function type with that
     * type as the return type.  Copy the parameters to the new
     * function type.
     */
    subtype = MUST_copy_type(cblk, TYPE_TARGET_TYPE(type));
    copy = make_function_type(subtype, 0);
    copy = MUST_copy_fields(cblk, copy, type);
    if (!copy) {
      printf_filtered("Copy: could not copy parameter type\n");
      return 0;
    }
    copy = MUST_copy_common_data(copy, type);
    break;

  case TYPE_CODE_INT:
  case TYPE_CODE_FLT:
  case TYPE_CODE_VOID:
    printf_filtered("Copy: scalar base type copy must match name\n");
    return 0;

  case TYPE_CODE_RANGE:
    printf_filtered("Range copy NYI\n");
    return 0;

  case TYPE_CODE_TYPEDEF:
    /* Create a new typedef referring to a copy of the target. */
    copy = alloc_type(MUST_current_c_objfile);
    TYPE_CODE(copy) = TYPE_CODE(type);
    INIT_CPLUS_SPECIFIC(copy);
    subtype = MUST_copy_type(cblk, TYPE_TARGET_TYPE(type));
    if (subtype) {
      TYPE_TARGET_TYPE(copy) = subtype;
      copy = MUST_copy_common_data(copy, type);
    } else {
      printf_filtered("Copy: could not copy target type\n");
      return 0;
    }
    break;

  default:
    printf_filtered("Unknown type code %d in copy\n", TYPE_CODE(type));
    return 0;
  }
  return copy;
}

static int MUST_find_function(struct blockvector *bv,
			      int cur_bnum,
			      struct block **retblock) {
  int bcnt = BLOCKVECTOR_NBLOCKS(bv);
  int bnum;
  struct block *b;

  /* Find the next function beginning with block number cur_bnum */
  if (retblock) *retblock = NULL;

  for (bnum = cur_bnum; bnum < bcnt; bnum++) {
    b = BLOCKVECTOR_BLOCK(bv, bnum);
    if (BLOCK_FUNCTION(b) != 0) {
      if (retblock) *retblock = b;
      break;
    } /* if */
  } /* for */
  return bnum;
} /* MUST_find_function */

static int MUST_local_to_func(struct block *block, struct block *func) {
  struct block *super;

  /* Return true if function block is found in the superblock chain. */
  for (super = BLOCK_SUPERBLOCK(block); 
       super != 0; // GLOBAL_BLOCK
       super = BLOCK_SUPERBLOCK(super)) {
    if (super == func)
      return 1;
  } /* for */
  return 0;
} /* MUST_local_to_func */

static struct block *MUST_find_parent_function(struct block *block) {
  struct block *super;
  struct block *func = NULL;

  /* Find parent function containing the block.
     Return true if function block is found in the superblock chain. */
  func = NULL;
  for (super = block; 
       super != 0; // GLOBAL_BLOCK
       super = BLOCK_SUPERBLOCK(super)) {
    if (BLOCK_FUNCTION(super) != 0) {
      func = super;
      break;
    } /* if */
  } /* for */
  return func;
} /* MUST_find_parent_function */

static void MUST_modify_csymbol(struct block *cblk, struct symbol *csym, 
				struct symbol *upcsym) {
  struct type *ctype;

  /* Modify existing C symbol */
  MUST_trace("  Modify C symbol from: ");
  MUST_trace_psym(csym);
  ctype = MUST_embed_shared_type(cblk, SYMBOL_TYPE(upcsym), 
				 SYMBOL_TYPE(csym));
  MUST_trace_ptype(ctype);
  SYMBOL_TYPE(csym) = ctype;
  MUST_trace("                    to: ");
  MUST_trace_psym(csym);
} /* MUST_modify_csymbol */

static void MUST_add_csymbol(struct block *cblk, struct symbol *upcsym) {
  struct symbol *csym;
  struct type *ctype;
  char *cname;

  /* Add new c symbol */
  MUST_trace("  Add new C Symbol: ");
  BLOCK_DICT(cblk) = dict_convert_dictionary(BLOCK_DICT(cblk));
  csym = (struct symbol *)
    obstack_alloc(&MUST_current_c_objfile->objfile_obstack,
		  sizeof(struct symbol));
  if (csym) {
    *csym = *upcsym;
    ctype = MUST_copy_type(cblk, SYMBOL_TYPE(upcsym));
    cname = SYMBOL_PRINT_NAME(upcsym);
    if (cname) {
      SYMBOL_SET_NAMES(csym, cname,strlen(cname), 0, MUST_current_c_objfile);
      SYMBOL_TYPE(csym) = ctype;
      dict_add_symbol(BLOCK_DICT(cblk), csym);
      MUST_trace_psym(csym);
    } else {
      printf_filtered("Add: Could not add symbol %s\n",
		      SYMBOL_PRINT_NAME(upcsym));
    } /* if */
  } else {
    printf_filtered("Add: Could not allocate symbol\n");
  } /* if */
} /* MUST_add_csymbol */

static int MUST_match_symbol(struct block *upcblk, int upcbnum,
			     struct symbol *upcsym, struct blockvector *cbv) {
  struct block *cblk;
  struct symbol *csym = NULL;

  MUST_trace("\nMatching symbol %s\n", SYMBOL_PRINT_NAME(upcsym));

  if (upcbnum < FIRST_LOCAL_BLOCK) {
    /* UPC symbol is declared at global or static scope */
    cblk = BLOCKVECTOR_BLOCK(cbv, upcbnum);
    MUST_trace("  Assuming block %p for upc symbol %s\n)\n", 
	       cblk, SYMBOL_PRINT_NAME(upcsym));
    csym = 
      lookup_block_symbol(cblk, SYMBOL_PRINT_NAME(upcsym), VAR_DOMAIN);
    if (csym) {
      /* Modify c symbol */
      gdb_assert(SYMBOL_LINE(upcsym) == SYMBOL_LINE(csym));
      MUST_modify_csymbol(cblk, csym, upcsym);
    } else {
      /* Check that the symbol exists in some c block, 
	 different from upc block */
      CORE_ADDR pc;
      find_line_pc(MUST_ctab, SYMBOL_LINE(upcsym), &pc);
      if (block_for_pc(pc)) {
	cblk = block_for_pc(pc);
	MUST_trace("  Found upc symbol %s in block %p\n)", 
		   SYMBOL_PRINT_NAME(upcsym), cblk);
      } /* if */
      /* Add new c symbol */
      MUST_add_csymbol(cblk, upcsym);
    } /* if */
  } else {
    /* UPC symbol is declared inside a function scope */
    struct block *upcfunc_block, *cfunc_block;
    int cfunc_bnum, next_cfunc_bnum;
    int cfunc_found = 0;
    int cbnum;

    /* Find UPC symbol's parent function */
    upcfunc_block = MUST_find_parent_function(upcblk);
    gdb_assert(upcfunc_block != NULL);

    /* Walk the list of C blocks looking for a function matching the
       UPC symbol parent function. Start with the first local block. */
    for (cbnum = upcbnum; cbnum < BLOCKVECTOR_NBLOCKS(cbv); cbnum++) {
      cfunc_bnum = MUST_find_function(cbv, cbnum, &cfunc_block);
      if (strcmp(SYMBOL_PRINT_NAME(BLOCK_FUNCTION(upcfunc_block)), 
		 SYMBOL_PRINT_NAME(BLOCK_FUNCTION(cfunc_block))) == 0) {
	/* Found matching function in C symbol table. */
	cfunc_found = 1;
	break;
      } /* if */
    } /* for */

    if (cfunc_found) {
      /* We now have a matching function in UPC and C symbol tables.
	 Walk local scopes of the C function looking for a symbol match. */
      next_cfunc_bnum = MUST_find_function(cbv, cfunc_bnum+1, NULL);
      for (cbnum = cfunc_bnum; cbnum < next_cfunc_bnum; cbnum++) {
	cblk = BLOCKVECTOR_BLOCK(cbv, cbnum);
	gdb_assert (cblk == cfunc_block || 
		    MUST_local_to_func(cblk, cfunc_block));
	/* This is an inner function block */
	csym = 
	  lookup_block_symbol(cblk, SYMBOL_PRINT_NAME(upcsym), VAR_DOMAIN);
	if (csym) {
	  if (SYMBOL_LINE(csym) == SYMBOL_LINE(upcsym)) {
	    MUST_trace("  Found matching C symbol %s in func %s, block %d\n", 
		       SYMBOL_PRINT_NAME(upcsym), 
		       SYMBOL_PRINT_NAME(BLOCK_FUNCTION(upcfunc_block)), 
		       cbnum);
	    MUST_modify_csymbol(cblk, csym, upcsym);
	    break;
	  } /* if */
	} /* if */
      } /* for */

      if (!csym) {
	/* No symbol match inside function blocks is found,
	   look for the block to insert a new entry. */
	CORE_ADDR pc;
	if (find_line_pc(MUST_ctab, SYMBOL_LINE(upcsym), &pc)) {
	  MUST_trace("  No matching C symbol %s in %s at line %d pc %lx)\n",
		     SYMBOL_PRINT_NAME(upcsym), 
		     SYMBOL_PRINT_NAME(BLOCK_FUNCTION(upcfunc_block)), 
		     SYMBOL_LINE(upcsym), pc);
	  cblk = block_for_pc(pc);
	  MUST_trace("  Found block %p\n", cblk);
	  MUST_add_csymbol(cblk, upcsym);
	} else {
	  printf_filtered("Match: Could not match line %d to any block\n", 
			  SYMBOL_LINE(upcsym));
	} /* if */
      } /* if */
    } else {
      printf_filtered("Match: Corresponding C function %s is not found\n", 
		      SYMBOL_PRINT_NAME(BLOCK_FUNCTION(upcfunc_block)));
    } /* if */
  } /* if */
  return 0;
} /* MUST_match_symbol */

static void MUST_walk_block_symbols(struct blockvector *upcbv, 
				    struct blockvector *cbv,
				    int bnum) {
  struct block *upcblk, *func;
  struct symbol *upcsym;
  struct dict_iterator iter;

  upcblk = BLOCKVECTOR_BLOCK(upcbv, bnum);
  ALL_BLOCK_SYMBOLS (upcblk, iter, upcsym) {
    if (MUST_contains_shared_parts(SYMBOL_TYPE(upcsym))) {
#if DEBUG
      func = MUST_find_parent_function(upcblk);
      if (func) {
	MUST_trace("  Found symbol %s at line %d in function %s:\n", 
		   SYMBOL_PRINT_NAME(upcsym), SYMBOL_LINE(upcsym),
		   SYMBOL_PRINT_NAME(BLOCK_FUNCTION(func)));
      } else {
	MUST_trace("  Found symbol %s at line %d in block %d:\n", 
		   SYMBOL_PRINT_NAME(upcsym), SYMBOL_LINE(upcsym), 
		   bnum);
      } /* if */
#endif /* DEBUG */
      MUST_match_symbol(upcblk, min(FIRST_LOCAL_BLOCK, bnum), upcsym, cbv);
    } /* if */
  } /* ALL_BLOCK_SYMBOLS */
} /* MUST_walk_block_symbols */

static void MUST_merge_symtabs (void) {
  struct blockvector *upcbv, *cbv;
  int upcbcnt, cbcnt;
  int bnum;
  
  /* 
   *  Walk through the corresponding symbol table,
   *  matching function scopes.
   */

  /* Get a pointer to upc block vector and it's block count */
  upcbv = BLOCKVECTOR(MUST_upctab);
  upcbcnt = BLOCKVECTOR_NBLOCKS(upcbv);

  /* Get a pointer to C block vector and it's block count */
  cbv = BLOCKVECTOR(MUST_ctab);
  cbcnt = BLOCKVECTOR_NBLOCKS(cbv);

  /* Find all the symbols in the global block */
  MUST_trace("\nUPC symbols in global block %d:\n", GLOBAL_BLOCK);
  MUST_walk_block_symbols(upcbv, cbv, GLOBAL_BLOCK);

  /* Find all the symbols in the static block */
  MUST_trace("\nUPC symbols in static block %d:\n", STATIC_BLOCK);
  MUST_walk_block_symbols(upcbv, cbv, STATIC_BLOCK);

  /* Find all the symbols in the local blocks */
  MUST_trace("\nUPC symbols in local blocks %d through %d:\n",
	     FIRST_LOCAL_BLOCK, upcbcnt);
  bnum = FIRST_LOCAL_BLOCK;
  for (bnum = FIRST_LOCAL_BLOCK; bnum < upcbcnt; bnum++) {
    MUST_walk_block_symbols(upcbv, cbv, bnum);
  } /* for */
} /* MUST_merge_symtabs */

/* TBD: Handle multiple files
 * TBD: Multiple source modules in a single file
 */
static void MUST_upc_merge_command(char *a1, int a2) {
  struct symtab *s;
  struct partial_symtab *pst;
  struct objfile *objfile;
  struct objfile_instance *objfile_instance;
  struct blockvector *bv;
  struct block *b;
  struct symbol *upcsym;
  struct dict_iterator iter;
  struct section_addr_info *sec_addrs;
  struct cleanup *my_cleanups;
  char *filename;
  char *lookfor = 0;
  CORE_ADDR addr;
  int i;

  MUST_ctab = MUST_upctab = 0;
  my_cleanups = make_cleanup(null_cleanup, NULL);

  /* Initialization and command parsing */
#if DEBUG
  MUST_trace("upc-merge command executed, %p and %d.\n", a1, a2);
#endif /* DEBUG */
  addr = parse_and_eval_address("0");
  filename = tilde_expand(a1);
  make_cleanup(xfree, filename);

  /* Read the UPC symbol file object dst.o */
  sec_addrs = alloc_section_addr_info(1);
  make_cleanup(xfree, sec_addrs);
  sec_addrs->other[0].name = ".text";
  sec_addrs->other[0].addr = addr;
  printf_filtered("Reading file %s\n", filename);

  /* Process dst.o file as a dynamically loaded file */
  objfile_instance = 
    symbol_file_add(filename, /*from_tty*/0, sec_addrs, /*main*/0);   

  if (objfile_instance == 0) {
    printf_filtered("Could not read it\n");
  } else {
    objfile = objfile_instance->objfile;

    MUST_trace("Read: %s\n", objfile->name);

    /* Convert all partial symbol tables for this file, we need them. */
    ALL_OBJFILE_PSYMTABS(objfile, pst) {
      PSYMTAB_TO_SYMTAB(pst);
    }

    /* Find the symbol table we just added. */
    ALL_OBJFILE_SYMTABS(objfile, s) {
      if (lookfor)
	printf_filtered("Multiple source files specified\n");
      else {
	lookfor = s->filename;
	MUST_upctab = s;
      } /* if */
    }

    printf_filtered("Corresponding source file is %s\n", lookfor);
    /* Convert partial symbol tables for the same source to full
     * symbol tables, we'll need them, too.
     */
    ALL_PSYMTABS (objfile_instance, pst) {
      if (strcmp(lookfor, pst->filename) == 0) {
	PSYMTAB_TO_SYMTAB (pst);
      }
    }
    /* Find the C version of the symbol table we just read */
    ALL_SYMTABS (objfile_instance, s) {
      if (strcmp(lookfor, s->filename) == 0 &&
	  s->language == language_c) {
	printf_filtered("Found C symbol file for %s, object file is %s\n",
			s->filename, s->objfile->name);
	if (MUST_ctab) {
	  printf_filtered("Multiple C symbol tables with that name\n");
	} else {
	  MUST_current_c_objfile = s->objfile;
	  MUST_ctab = s;
	}
      }
    }
  }

  if (MUST_ctab && MUST_upctab) {
    printf_filtered("Merging symbol tables\n");
    MUST_merge_symtabs ();
  } else {
    printf_filtered("Unable to merge symbol tables\n");
  } /* if */

  do_cleanups(my_cleanups);
} /* MUST_upc_merge_command */

static void MUST_show_debug(struct ui_file *file, int from_tty,
			    struct cmd_list_element *c, const char *value)
{
  fprintf_filtered (file,
		    _("Merge UPC Symbol Table (MUST) debugging is %s.\n"),
		    value);
}

void
_initialize_upcsymtab (void)
{
  add_com ("upc-merge", no_class, MUST_upc_merge_command, _("\
Merge UPC symbol table from .dust file into main C symbol table\
for the same module."));
  add_setshow_zinteger_cmd ("must", class_maintenance, &MUST_debug, _("\
Set Merge UPC Symbol Table (MUST) debugging."), _("\
Show Merge UPC Symbol Table (MUST) debugging."), _("\
When non-zero, Merge UPC Symbol Table debugging is enabled."),
			    NULL,
			    MUST_show_debug,
			    &setdebuglist, &showdebuglist);
}
