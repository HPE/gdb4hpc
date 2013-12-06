/* Source to source DWARF-like debugging format support for GDB.

   Copyright (C) Allinea Software Ltd. 2012-2013

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "defs.h"
#include "bfd.h"
#include "gdbarch.h"
#include "symtab.h"
#include "gdbtypes.h"
#include "dwarf2.h"
#include "value.h"
#include "xml-support.h"
#include "valprint.h"
#include "vec.h"
#include "hashtab.h"
#include "objfiles.h"
#include "complaints.h"
#include "language.h"
#include "buildsym.h"
#include "block.h"
#include "xml-source-to-source.h"
#ifndef HAVE_LIBEXPAT
#error No expat
#endif

struct caps_baton
{
  char *location;
};

#ifndef CAPS_FIELD_ALLOC_CHUNK
#define CAPS_FIELD_ALLOC_CHUNK 4
#endif

#define AT_NULL { NULL, GDB_XML_AF_NONE, NULL, NULL }

#define AT_name "at_name"
#define AT_producer "at_producer"
#define AT_language "at_language"
#define AT_location "at_location"
#define AT_data_member_location "at_data_member_location"
#define AT_ordering "at_ordering"
#define AT_byte_size "at_byte_size"
#define AT_const_value "at_const_value"
#define AT_is_optional "at_is_optional"
#define AT_lower_bound "at_lower_bound"
#define AT_upper_bound "at_upper_bound"
#define AT_count "at_count"
#define AT_encoding "at_encoding"
#define AT_type "at_type"
#define AT_allocated "at_allocated"
#define AT_associated "at_associated"
#define AT_data_location "at_data_location"
#define AT_byte_stride "at_byte_stride"
#define AT_typeof_location "at_typeof_location"

#define TAG_compile_unit "tag_compile_unit"
#define TAG_lexical_block "tag_lexical_block"
#define TAG_subprogram "tag_subprogram"
#define TAG_array_type "tag_array_type"
#define TAG_enumeration_type "tag_enumeration_type"
#define TAG_pointer_type "tag_pointer_type"
#define TAG_string_type "tag_string_type"
#define TAG_structure_type "tag_structure_type"
#define TAG_subroutine_type "tag_subroutine_type"
#define TAG_typedef "tag_typedef"
#define TAG_union_type "tag_union_type"
#define TAG_subrange_type "tag_subrange_type"
#define TAG_base_type "tag_base_type"
#define TAG_const_type "tag_const_type"
#define TAG_constant "tag_constant"
#define TAG_enumerator "tag_enumerator"
#define TAG_variable "tag_variable"
#define TAG_formal_parameter "tag_formal_parameter"
#define TAG_member "tag_member"
#define TAG_unknown_type "tag_unknown_type"


enum element_type
{
  AT_NAME,
  AT_PRODUCER,
  AT_LANGUAGE,
  AT_ORDERING,
  AT_TYPE,
  AT_DATA_LOCATION,
  AT_ASSOCIATED,
  AT_ENCODING,
  AT_ALLOCATED,
  AT_BYTE_SIZE,
  AT_LOCATION,
  AT_LOWER_BOUND,
  AT_UPPER_BOUND,
  AT_BYTE_STRIDE,
  AT_COUNT,
  AT_DATA_MEMBER_LOCATION,
  AT_CONST_VALUE,
  TAG_COMPILE_UNIT,
  TAG_LEXICAL_BLOCK,
  TAG_SUBPROGRAM,
  TAG_ARRAY_TYPE,
  TAG_POINTER_TYPE,
  TAG_STRING_TYPE,
  TAG_STRUCTURE_TYPE,
  TAG_SUBROUTINE_TYPE,
  TAG_TYPEDEF,
  TAG_UNION_TYPE,
  TAG_SUBRANGE_TYPE,
  TAG_BASE_TYPE,
  TAG_CONST_TYPE,
  TAG_CONSTANT,
  TAG_ENUMERATOR,
  TAG_VARIABLE,
  TAG_FORMAL_PARAMETER,
  TAG_MEMBER,
  TAG_UNKNOWN_TYPE,
  ERROR
};



enum element_type
lookup_name (const char *name)
{
  if (strcmp (name, AT_name) == 0)
    return AT_NAME;
  if (strcmp (name, AT_producer) == 0)
    return AT_PRODUCER;
  if (strcmp (name, AT_language) == 0)
    return AT_LANGUAGE;
  if (strcmp (name, AT_ordering) == 0)
    return AT_ORDERING;
  if (strcmp (name, AT_type) == 0)
    return AT_TYPE;
  if (strcmp (name, AT_data_location) == 0)
    return AT_DATA_LOCATION;
  if (strcmp (name, AT_associated) == 0)
    return AT_ASSOCIATED;
  if (strcmp (name, AT_encoding) == 0)
    return AT_ENCODING;
  if (strcmp (name, AT_allocated) == 0)
    return AT_ALLOCATED;
  if (strcmp (name, AT_byte_size) == 0)
    return AT_BYTE_SIZE;
  if (strcmp (name, AT_location) == 0)
    return AT_LOCATION;
  if (strcmp (name, AT_lower_bound) == 0)
    return AT_LOWER_BOUND;
  if (strcmp (name, AT_upper_bound) == 0)
    return AT_UPPER_BOUND;
  if (strcmp (name, AT_byte_stride) == 0)
    return AT_BYTE_STRIDE;
  if (strcmp (name, AT_count) == 0)
    return AT_COUNT;
  if (strcmp (name, AT_data_member_location) == 0)
    return AT_DATA_MEMBER_LOCATION;
  if (strcmp (name, AT_const_value) == 0)
    return AT_CONST_VALUE;
  if (strcmp (name, TAG_compile_unit) == 0)
    return TAG_COMPILE_UNIT;
  if (strcmp (name, TAG_lexical_block) == 0)
    return TAG_LEXICAL_BLOCK;
  if (strcmp (name, TAG_subprogram) == 0)
    return TAG_SUBPROGRAM;
  if (strcmp (name, TAG_array_type) == 0)
    return TAG_ARRAY_TYPE;
  if (strcmp (name, TAG_pointer_type) == 0)
    return TAG_POINTER_TYPE;
  if (strcmp (name, TAG_string_type) == 0)
    return TAG_STRING_TYPE;
  if (strcmp (name, TAG_structure_type) == 0)
    return TAG_STRUCTURE_TYPE;
  if (strcmp (name, TAG_subroutine_type) == 0)
    return TAG_SUBROUTINE_TYPE;
  if (strcmp (name, TAG_typedef) == 0)
    return TAG_TYPEDEF;
  if (strcmp (name, TAG_union_type) == 0)
    return TAG_UNION_TYPE;
  if (strcmp (name, TAG_subrange_type) == 0)
    return TAG_SUBRANGE_TYPE;
  if (strcmp (name, TAG_base_type) == 0)
    return TAG_BASE_TYPE;
  if (strcmp (name, TAG_const_type) == 0)
    return TAG_CONST_TYPE;
  if (strcmp (name, TAG_constant) == 0)
    return TAG_CONSTANT;
  if (strcmp (name, TAG_enumerator) == 0)
    return TAG_ENUMERATOR;
  if (strcmp (name, TAG_variable) == 0)
    return TAG_VARIABLE;
  if (strcmp (name, TAG_formal_parameter) == 0)
    return TAG_FORMAL_PARAMETER;
  if (strcmp (name, TAG_member) == 0)
    return TAG_MEMBER;
  if (strcmp (name, TAG_unknown_type) == 0)
    return TAG_UNKNOWN_TYPE;
  else
    return ERROR;
}

static struct gdb_xml_attribute tag_compile_unit_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_subprogram_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_formal_parameter_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_lexical_block_attributes[] = {
  {"id", GDB_XML_AF_OPTIONAL, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_array_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_enumeration_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_pointer_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_string_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_structure_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_subroutine_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_typedef_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_formal_parameter_attribute[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_union_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_subrange_type_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_member_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_base_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_variable_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_const_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

static const struct gdb_xml_attribute tag_constant_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_enumerator_attributes[] = {
  AT_NULL
};

static const struct gdb_xml_attribute tag_unknown_type_attributes[] = {
  {"id", GDB_XML_AF_NONE, NULL, NULL},
  AT_NULL
};

struct caps_compile_unit
{
  char *unit_name;
  char *producer;
  enum language language;

  htab_t object_hash;

  struct objfile *objfile;
};

struct caps_variable
{
  char *name;
  int type;
  char *location;
};

struct caps_base_type
{
  char *name;
  int encoding;
  int size;
};


enum form_type
{
  FORM_SDATA,
  FORM_UDATA,
  FORM_FLOAT,
  FORM_COMPLEX,
  FORM_STRING,
  FORM_EXPR,
  FORM_ADDR,
  FORM_REF,
  FORM_FLAG
};

struct caps_element
{
  enum element_type type;

  struct caps_element *parent;
  struct caps_element *sibling;
  struct caps_element *children;

  unsigned int id;		/* id */

  enum form_type form;

  union
  {
    struct caps_variable variable;
    struct caps_compile_unit cu;
    struct caps_base_type base_type;
    char *cdata;
  } contents;

  union
  {
    struct type *type;
    struct symbol *symbol;
  } gdb_object;
};

struct caps_user_data
{
  struct caps_element *current_element;
  struct caps_element *cu;
  struct objfile *objfile;
};

struct caps_element *
xalloc_caps_element ()
{
  struct caps_element *r = xmalloc (sizeof (struct caps_element));
  memset (r, 0, sizeof (struct caps_element));
  return r;
}

static struct type *lookup_type (int id, struct caps_compile_unit *cu);

/* Parent a new item, fix up its siblings */

static void
insert_child (struct caps_element *parent, struct caps_element *child)
{
  struct caps_element *first = parent->children;

  child->sibling = first;
  child->parent = parent;
  parent->children = child;
}

enum form_type
read_form (const char *form)
{
  if (strcmp (form, "sdata") == 0)
    return FORM_SDATA;
  if (strcmp (form, "udata") == 0)
    return FORM_UDATA;
  if (strcmp (form, "float") == 0)
    return FORM_FLOAT;
  if (strcmp (form, "complex") == 0)
    return FORM_COMPLEX;
  if (strcmp (form, "string") == 0)
    return FORM_STRING;
  if (strcmp (form, "expr") == 0)
    return FORM_EXPR;
  if (strcmp (form, "addr") == 0)
    return FORM_ADDR;
  if (strcmp (form, "ref") == 0)
    return FORM_REF;
  if (strcmp (form, "flag") == 0)
    return FORM_FLAG;
  return 0;
}

static void
stos_start_generic_element (struct gdb_xml_parser *parser,
			    const struct gdb_xml_element *element,
			    void *user_data,
			    VEC (gdb_xml_value_s) * attributes)
{
  struct caps_user_data *d = (struct caps_user_data *) user_data;
  struct caps_element *child = xalloc_caps_element ();
  int i;
  int length = VEC_length (gdb_xml_value_s, attributes);
  struct gdb_xml_value *attrs = VEC_address (gdb_xml_value_s, attributes);

  insert_child (d->current_element, child);
  child->type = lookup_name (element->name);
  d->current_element = child;
  for (i = 0; i < length; ++i)
    {
      if (strcmp (attrs[i].name, "form") == 0)
	{
	  child->form = read_form ((char *) attrs[i].value);
	}
      if (strcmp (attrs[i].name, "id") == 0)
	{
	  child->id = atoi ((char *) attrs[i].value);
	}
    }
}



static int
ce_offset_and_element_eq (const void *item_lhs, const void *item_rhs)
{
  const struct caps_element *ofs_lhs = item_lhs;
  const struct caps_element *ofs_rhs = item_rhs;
  return ofs_lhs->id == ofs_rhs->id;
}

static hashval_t
ce_offset_and_element_hash (const void *item)
{
  const struct caps_element *ofs = item;
  return ofs->id;
}


static void
stos_start_compile_unit (struct gdb_xml_parser *parser,
			 const struct gdb_xml_element *element,
			 void *user_data, VEC (gdb_xml_value_s) * attributes)
{
  struct caps_user_data *d = (struct caps_user_data *) user_data;
  d->current_element = xalloc_caps_element ();
  d->cu = d->current_element;
  d->cu->contents.cu.objfile = d->objfile;

  d->current_element->type = lookup_name (element->name);

  d->cu->contents.cu.object_hash = htab_create_alloc_ex (100,
							 ce_offset_and_element_hash,
							 ce_offset_and_element_eq,
							 NULL,
							 &d->objfile->
							 objfile_obstack,
							 hashtab_obstack_allocate,
							 dummy_obstack_deallocate);

}

static struct caps_element *
insert_ce (struct caps_element *ce, struct caps_compile_unit *cu)
{

  struct caps_element **slot = (struct caps_element **)
    htab_find_slot_with_hash (cu->object_hash, ce, ce->id, INSERT);
  if (*slot)
    complaint (&symfile_complaints,
	       _("A problem internal to GDB: DIE 0x%x has type already set"),
	       ce->id);
  *slot = ce;
  return ce;
}

static struct type *
set_ce_type (struct caps_element *ce, struct type *type,
	     struct caps_compile_unit *cu)
{
  struct caps_element **slot, ofs;

  ce->gdb_object.type = type;
  return type;
}

static struct type *
caps_create_unknown_type (struct caps_element *ce,
			  struct caps_compile_unit *cu)
{
  struct type *type = alloc_type (cu->objfile);
  return set_ce_type (ce, type, cu);
}

static struct type *
caps_create_pointer_type (struct caps_element *ce,
			  struct caps_compile_unit *cu)
{
  struct type *type;
  struct type *target_type;
  struct caps_element *child;
  int target_typeid = 0;
  int byte_size = 0;
  gdb_assert (ce != NULL);
  child = ce->children;
  for (; child != NULL; child = child->sibling)
    {
      switch (child->type)
	{
	case AT_TYPE:
	  target_typeid = atoi (child->contents.cdata);
	  break;
	case AT_BYTE_SIZE:
	  byte_size = atoi (child->contents.cdata);	/* XXX ignored */
	  break;
	default:
	  break;
	}
    }
  target_type = lookup_type (target_typeid, cu);
  type = lookup_pointer_type (target_type);
  return set_ce_type (ce, type, cu);
}

static struct type *
caps_create_base_type (struct caps_element *ce, struct caps_compile_unit *cu)
{
  int encoding = ce->contents.base_type.encoding;
  int size = ce->contents.base_type.size;
  int code = TYPE_CODE_INT;
  char *name = ce->contents.base_type.name;
  struct type *type;
  struct type *target_type = NULL;
  struct objfile *objfile = cu->objfile;
  int type_flags = 0;

  switch (encoding)
    {
    case DW_ATE_address:
      /* Turn DW_ATE_address into a void * pointer.  */
      code = TYPE_CODE_PTR;
      type_flags |= TYPE_FLAG_UNSIGNED;
      target_type = init_type (TYPE_CODE_VOID, 1, 0, NULL, objfile);
      break;
    case DW_ATE_boolean:
      code = TYPE_CODE_BOOL;
      type_flags |= TYPE_FLAG_UNSIGNED;
      break;
    case DW_ATE_complex_float:
      code = TYPE_CODE_COMPLEX;
      target_type = init_type (TYPE_CODE_FLT, size / 2, 0, NULL, objfile);
      break;
    case DW_ATE_decimal_float:
      code = TYPE_CODE_DECFLOAT;
      break;
    case DW_ATE_float:
      code = TYPE_CODE_FLT;
      break;
    case DW_ATE_signed:
      break;
    case DW_ATE_unsigned:
      type_flags |= TYPE_FLAG_UNSIGNED;
      break;
    case DW_ATE_signed_char:
      break;
    case DW_ATE_unsigned_char:
      type_flags |= TYPE_FLAG_UNSIGNED;
      break;
    case DW_ATE_UTF:
      /* We just treat this as an integer and then recognize the
         type by name elsewhere.  */
      break;

    default:
      complaint (&symfile_complaints, _("unsupported DW_AT_encoding: '%d'"),
		 encoding);
      break;
    }

  type = init_type (code, size, type_flags, NULL, objfile);
  TYPE_NAME (type) = name;
  TYPE_TARGET_TYPE (type) = target_type;

  if (name && strcmp (name, "char") == 0)
    TYPE_NOSIGN (type) = 1;

  return set_ce_type (ce, type, cu);
}

LONGEST
caps_eval_int (struct caps_baton * baton, struct value * obj, void *frame)
{
  LONGEST addr;
  enum language prev_language = set_language (language_c);

  addr = parse_and_eval_long (baton->location);
  set_language (prev_language);

  return addr;
}

static int
get_ordering (const char *text)
{
  if (strcmp ("ATE_col_major", text) == 0)
    return DW_ORD_col_major;
  else
    return DW_ORD_row_major;
}


static struct type *
caps_create_array_type (struct caps_element *ce,
			struct caps_compile_unit *unit)
{
  struct caps_element *child;
  struct type *range_type;
  LONGEST low = 0;
  LONGEST high = -1;
  int stride = 1;
  int soffset = 0;
  int lstride = 0;
  struct type *type = 0;
  struct type **range_types = NULL;
  int ndim = 0;
  int order = DW_ORD_col_major;

  gdb_assert (ce != NULL);
  child = ce->children;
  for (; child != NULL; child = child->sibling)
    {
      char *typenum;
      char *ordstr;
      struct caps_baton *low_compute = 0;
      struct caps_baton *high_compute = 0;
      struct caps_baton *count_compute = 0;
      struct caps_baton *stride_compute = 0;
      struct caps_baton *soffset_compute = 0;
      struct caps_baton *lstride_compute = 0;
      struct caps_element *elt;
      switch (child->type)
	{
	case TAG_SUBRANGE_TYPE:
	  for (elt = child->children; elt != NULL; elt = elt->sibling)
	    {

	      if (elt->type == AT_LOWER_BOUND)
		{
		  low_compute = (struct caps_baton *)
		    obstack_alloc (&(unit->objfile->objfile_obstack),
				   sizeof (struct caps_baton));
		  low_compute->location = elt->contents.cdata;
		}
	      else if (elt && elt->type == AT_UPPER_BOUND)
		{
		  high_compute = (struct caps_baton *)
		    obstack_alloc (&(unit->objfile->objfile_obstack),
				   sizeof (struct caps_baton));
		  high_compute->location = elt->contents.cdata;
		}
	      if ((ndim % CAPS_FIELD_ALLOC_CHUNK) == 0)
		{
		  range_types = (struct type **)
		    xrealloc (range_types, (ndim + CAPS_FIELD_ALLOC_CHUNK)
			      * sizeof (struct type *));
		  if (ndim == 0)
		    make_cleanup (free_current_contents, &range_types);
		}
	    }
	  range_types[ndim++] =
	    create_range_type_d_pgi (NULL,
				     objfile_type (unit->objfile)->
				     builtin_int, low, high, stride, soffset,
				     lstride, low_compute, high_compute,
				     count_compute, stride_compute,
				     soffset_compute, lstride_compute,
				     (LONGEST (*)(void *, CORE_ADDR, void *))
				     caps_eval_int);
	  break;
	case AT_TYPE:
	  /* target type */
	  typenum = child->contents.cdata;
	  type = lookup_type (atoi (typenum), unit);
	  break;
	case AT_ORDERING:
	  ordstr = child->contents.cdata;
	  if (ordstr)
	    order = get_ordering (ordstr);
	  break;
	default:
	  break;
	}
    }

  while (ndim-- > 0)
    type = create_array_type (NULL, type, range_types[ndim]);

  set_ce_type (ce, type, unit);
  return type;
}

static struct type *
caps_create_const_type (struct caps_element *ce, struct caps_compile_unit *cu)
{
  struct type_quals type_quals = null_type_quals;

  struct caps_element *child;
  const char *typenum = 0;
  int typeId;
  gdb_assert (ce != NULL);
  child = ce->children;

  TYPE_QUAL_FLAGS (type_quals) |= TYPE_INSTANCE_FLAG_CONST;

  for (; child != NULL; child = child->sibling)
    {
      switch (child->type)
	{
	case AT_TYPE:
	  typenum = child->contents.cdata;
	  break;
	default:
	  break;
	}
    }
  if (typenum)
    {
      struct type *qual_type;
      struct type *base_type;
      typeId = atoi (typenum);
      base_type = lookup_type (typeId, cu);
      if (base_type == NULL)
	complaint (&symfile_complaints,
		   _("Base_type not already read: %d already exists"),
		   typeId);


      type_quals = merge_type_quals (TYPE_QUALS (base_type), type_quals);
      qual_type = make_qual_variant_type (type_quals, base_type, 0);
      set_ce_type (ce, qual_type, cu);
      return qual_type;
    }
  return NULL;
}


static struct type *
lookup_type (int id, struct caps_compile_unit *cu)
{
  struct caps_element *elt, ofs;
  ofs.id = id;
  elt = (struct caps_element *) htab_find (cu->object_hash, &ofs);
  if (!elt)
    {
      complaint (&symfile_complaints, _("type not found %d"), id);
      /* Return an undefined type (TYPE_CODE(type) == TYPE_CODE_UNDEF). */
      return alloc_type (cu->objfile);
    }
  if (elt->gdb_object.type == NULL)
    {
      /* time to flesh out the object */
      switch (elt->type)
	{
	case TAG_POINTER_TYPE:
	  caps_create_pointer_type (elt, cu);
	  break;
	case TAG_ARRAY_TYPE:
	  caps_create_array_type (elt, cu);
	  break;
	case TAG_BASE_TYPE:
	  caps_create_base_type (elt, cu);
	  break;
	case TAG_CONST_TYPE:
	  caps_create_const_type (elt, cu);
	  break;
	case TAG_STRUCTURE_TYPE:
	case TAG_SUBROUTINE_TYPE:
	case TAG_UNION_TYPE:
	case TAG_STRING_TYPE:
	  error (_("To do: element %d"), id);
	case TAG_UNKNOWN_TYPE:
	  caps_create_unknown_type (elt, cu);
	  break;
	default:
	  error (_("Not a valid type: element %d"), id);
	  break;
	}
    }
  return elt->gdb_object.type;
}

static struct caps_element *
lookup_element (int id, struct caps_compile_unit *cu)
{
  struct caps_element *elt, ofs;
  ofs.id = id;
  elt = (struct caps_element *) htab_find (cu->object_hash, &ofs);
  if (!elt)
    {
      complaint (&symfile_complaints, _("Type not found %d"), id);
      return NULL;
    }
  return elt;
}


/* Return the value of SYMBOL in FRAME using the expression
   evaluator to calculate the location.  */
static struct value *
caps_locexpr_read_variable (struct symbol *symbol, struct frame_info *frame)
{
  struct value *val;
  struct caps_baton *baton = SYMBOL_LOCATION_BATON (symbol);
  struct value *addr_val;
  struct type *val_type;
  struct type *type;
  char *location = baton->location;

  enum language prev_language = set_language (language_c);

  addr_val = parse_and_eval (location);	/* Need type - to get storage class */
  set_language (prev_language);

  /* should be a pointer -- now find pointer to what type: */

  type = SYMBOL_TYPE (symbol);
  while (type)
    {
      TYPE_INSTANCE_FLAGS (type) =
	TYPE_INSTANCE_FLAGS (TYPE_TARGET_TYPE ((value_type (addr_val))));
      if (TYPE_CODE (type) == TYPE_CODE_ARRAY
	  || TYPE_CODE (type) == TYPE_CODE_PTR)
	type = TYPE_TARGET_TYPE (type);
      else
	type = NULL;
    }

  if (TYPE_TARGET_TYPE (SYMBOL_TYPE (symbol)))	/* is this a pointer to something - if so, it's GPU stuff */
    TYPE_INSTANCE_FLAGS (TYPE_TARGET_TYPE (SYMBOL_TYPE (symbol))) =
      TYPE_INSTANCE_FLAGS (TYPE_TARGET_TYPE ((value_type (addr_val))));

  val = value_at_lazy (SYMBOL_TYPE (symbol), value_as_address (addr_val));

  return val;
}

/* Return non-zero iff we need a frame to evaluate SYMBOL.  */
static int
caps_locexpr_read_needs_frame (struct symbol *symbol)
{
  return -1;
}

/* Print a natural-language description of SYMBOL to STREAM.  This
   version applies when there is a list of different locations, each
   with a specified address range.  */
static void
caps_locexpr_describe_location (struct symbol *symbol, CORE_ADDR addr,
				struct ui_file *stream)
{
  /* not supported */
}

/* Describe the location of SYMBOL as an agent value in VALUE, generating
   any necessary bytecode in AX.  */
static void
caps_locexpr_tracepoint_var_ref (struct symbol *symbol,
				 struct gdbarch *gdbarch,
				 struct agent_expr *ax,
				 struct axs_value *value)
{
  /* not supported */
}

const struct symbol_computed_ops caps_locexpr_funcs = {
  caps_locexpr_read_variable,
  caps_locexpr_read_needs_frame,
  caps_locexpr_describe_location,
  caps_locexpr_tracepoint_var_ref
};

static struct caps_element *
caps_create_variable (struct caps_element *ce, struct caps_compile_unit *cu)
{
  struct caps_variable **slot;
  struct caps_baton *baton;
  struct symbol *sym;
  struct caps_variable *variable = xmalloc (sizeof (struct caps_variable));
  struct caps_element *child;

  char *typenum = NULL;
  char *name = NULL;
  char *location = NULL;

  gdb_assert (ce != NULL);
  child = ce->children;

  for (; child != NULL; child = child->sibling)
    {
      switch (child->type)
	{
	case AT_NAME:
	  name = child->contents.cdata;
	  break;
	case AT_TYPE:
	  typenum = child->contents.cdata;
	  break;
	case AT_LOCATION:
	  location = child->contents.cdata;
	  break;
	default:
	  break;
	}
    }
  ce->contents.variable.name = name;
  ce->contents.variable.type = atoi (typenum);
  ce->contents.variable.location = location;

  sym =
    obstack_alloc (&(cu->objfile->objfile_obstack), sizeof (struct symbol));
  OBJSTAT (cu->objfile, n_syms++);
  memset (sym, 0, sizeof (struct symbol));
  ce->gdb_object.symbol = sym;

  SYMBOL_LANGUAGE (sym) = cu->language;
  SYMBOL_SET_NAMES (sym, name, strlen (name),
		    (cu->objfile->flags & OBJF_MAPPED) != 0, cu->objfile);
  SYMBOL_DOMAIN (sym) = VAR_DOMAIN;
  SYMBOL_CLASS (sym) = LOC_COMPUTED;
  SYMBOL_TYPE (sym) = lookup_type (ce->contents.variable.type, cu);
  SYMBOL_COMPUTED_OPS (sym) = &caps_locexpr_funcs;
  baton =
    obstack_alloc (&(cu->objfile->objfile_obstack),
		   sizeof (struct caps_baton));
  SYMBOL_LOCATION_BATON (sym) = baton;
  baton->location = xstrdup (ce->contents.variable.location);
  return ce;
  /* push var list?? */
}


static enum language
parse_language (const char *language)
{
  if (language && strcmp (language, "LANG_Fortran90") == 0)
    return language_fortran;
  else
    return language_c;
}

static void
caps_create_lexical_block (struct caps_element *ce,
			   struct caps_compile_unit *cu)
{
  struct caps_element *child;
  gdb_assert (ce != NULL);
  child = ce->children;

  for (; child != NULL; child = child->sibling)
    {
      switch (child->type)
	{
	case AT_NAME:
	  cu->unit_name = child->contents.cdata;
	  break;
	case AT_PRODUCER:
	  cu->producer = child->contents.cdata;
	  break;
	case AT_LANGUAGE:
	  cu->language = parse_language (child->contents.cdata);
	  break;
	case TAG_BASE_TYPE:
	  caps_create_base_type (child, cu);
	  break;
	case TAG_VARIABLE:
	  caps_create_variable (child, cu);
	  break;
	case TAG_CONST_TYPE:
	  caps_create_const_type (child, cu);
	  break;
	case TAG_LEXICAL_BLOCK:
	  caps_create_lexical_block (child, cu);
	  break;
	case TAG_UNKNOWN_TYPE:
	  caps_create_unknown_type (child, cu);
	  break;
	default:
	  break;
	}
    }
}

static void
stos_end_compile_unit (struct gdb_xml_parser *parser,
		       const struct gdb_xml_element *element,
		       void *user_data, const char *body_text)
{
  struct caps_user_data *d = (struct caps_user_data *) user_data;

  struct caps_element *child = d->cu->children;
  struct caps_compile_unit *cu = &(d->cu->contents.cu);

  for (; child != NULL; child = child->sibling)
    {
      switch (child->type)
	{
	case AT_NAME:
	  cu->unit_name = child->contents.cdata;
	  break;
	case AT_PRODUCER:
	  cu->producer = child->contents.cdata;
	  break;
	case AT_LANGUAGE:
	  cu->language = parse_language (child->contents.cdata);
	  break;
	case TAG_BASE_TYPE:
	  caps_create_base_type (child, cu);
	  break;
	case TAG_VARIABLE:
	  caps_create_variable (child, cu);
	  break;
	case TAG_CONST_TYPE:
	  caps_create_const_type (child, cu);
	  break;
	case TAG_LEXICAL_BLOCK:
	  caps_create_lexical_block (child, cu);
	  break;
	case TAG_UNKNOWN_TYPE:
	  caps_create_unknown_type (child, cu);
	  break;
	default:
	  break;
	}
    }
  d->current_element = d->current_element->parent;
}

static int
get_encoding (const char *text)
{
  if (strcmp (text, "ATE_address") == 0)
    return DW_ATE_address;
  if (strcmp (text, "ATE_boolean") == 0)
    return DW_ATE_boolean;
  if (strcmp (text, "ATE_complex_float") == 0)
    return DW_ATE_complex_float;
  if (strcmp (text, "ATE_float") == 0)
    return DW_ATE_float;
  if (strcmp (text, "ATE_signed") == 0)
    return DW_ATE_signed;
  if (strcmp (text, "ATE_signed_char") == 0)
    return DW_ATE_signed_char;
  if (strcmp (text, "ATE_unsigned") == 0)
    return DW_ATE_unsigned;
  if (strcmp (text, "ATE_unsigned_char") == 0)
    return DW_ATE_unsigned_char;
  return 0;
}


static void
stos_end_generic_element (struct gdb_xml_parser *parser,
			  const struct gdb_xml_element *element,
			  void *user_data, const char *body_text)
{
  struct caps_user_data *d = (struct caps_user_data *) user_data;
  if (d->current_element->id > 0)
    insert_ce (d->current_element, &(d->cu->contents.cu));

  d->current_element = d->current_element->parent;
}


static void
stos_end_base_type (struct gdb_xml_parser *parser,
		    const struct gdb_xml_element *element,
		    void *user_data, const char *body_text)
{
  struct caps_user_data *d = (struct caps_user_data *) user_data;
  struct caps_element *cur_element = d->current_element;
  struct caps_element *child = cur_element->children;
  int size = 0;
  int encoding = 0;
  char *name = NULL;
  for (; child != NULL; child = child->sibling)
    {
      switch (child->type)
	{
	case AT_NAME:
	  name = child->contents.cdata;
	  break;
	case AT_BYTE_SIZE:
	  size = atoi (child->contents.cdata);
	  break;
	case AT_ENCODING:
	  encoding = get_encoding (child->contents.cdata);
	  break;
	default:
	  break;
	}
    }
  cur_element->contents.base_type.name = name;
  cur_element->contents.base_type.size = size;
  cur_element->contents.base_type.encoding = encoding;

  stos_end_generic_element (parser, element, user_data, body_text);
}


static void
stos_end_cdata (struct gdb_xml_parser *parser,
		const struct gdb_xml_element *element,
		void *user_data, const char *body_text)
{

  struct caps_user_data *d = (struct caps_user_data *) user_data;
  struct caps_element *parent = d->current_element;
  struct caps_element *elt = xalloc_caps_element ();

  insert_child (parent, elt);

  elt->type = lookup_name (element->name);
  elt->contents.cdata = xstrdup (body_text);
}


const static struct gdb_xml_element tag_member_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_subroutine_type_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_formal_parameter_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_variable_children[] = {
  {AT_type, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_name, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_location, NULL, NULL, 0, NULL, stos_end_cdata},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_enumerator_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_constant_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_const_type_children[] = {
  {AT_type, NULL, NULL, 0, NULL, stos_end_cdata},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};


const static struct gdb_xml_element tag_base_type_children[] = {
  {AT_name, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_byte_size, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_encoding, NULL, NULL, 0, NULL, stos_end_cdata},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_typedef_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_subrange_type_children[] = {
  {AT_upper_bound, NULL, NULL, GDB_XML_EF_OPTIONAL, NULL, stos_end_cdata},
  {AT_lower_bound, NULL, NULL, GDB_XML_EF_OPTIONAL, NULL, stos_end_cdata},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_union_type_children[] = {
  {TAG_member, tag_member_attributes, tag_member_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_structure_type_children[] = {
  {TAG_member, tag_member_attributes, tag_member_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_pointer_type_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_string_type_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_enumeration_type_children[] = {
  {TAG_enumerator, tag_enumerator_attributes, tag_enumerator_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_array_type_children[] = {
  {AT_ordering, NULL, NULL, GDB_XML_EF_OPTIONAL, NULL, stos_end_cdata},
  {AT_type, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_data_location, NULL, NULL, GDB_XML_EF_OPTIONAL, NULL, stos_end_cdata},
  {AT_associated, NULL, NULL, GDB_XML_EF_OPTIONAL, NULL, stos_end_cdata},
  {AT_allocated, NULL, NULL, GDB_XML_EF_OPTIONAL, NULL, stos_end_cdata},
  {TAG_subrange_type, tag_subrange_type_attributes,
   tag_subrange_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

const static struct gdb_xml_element tag_unknown_type_children[] = {
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};



#define tag_lexical_block   { TAG_lexical_block,        \
      tag_lexical_block_attributes,                     \
      tag_lexical_block_children,                       \
      GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE,      \
      stos_start_generic_element,                       \
      stos_end_generic_element                            \
      }

const static struct gdb_xml_element tag_lexical_block_children[] = {
  tag_lexical_block,
  {TAG_array_type, tag_array_type_attributes, tag_array_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_enumeration_type, tag_enumeration_type_attributes,
   tag_enumeration_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_pointer_type, tag_pointer_type_attributes, tag_pointer_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_string_type, tag_string_type_attributes, tag_string_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_structure_type, tag_structure_type_attributes,
   tag_structure_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_subroutine_type, tag_subroutine_type_attributes,
   tag_subroutine_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_typedef, tag_typedef_attributes, tag_typedef_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_union_type, tag_union_type_attributes, tag_union_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_base_type, tag_base_type_attributes, tag_base_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_base_type},
  {TAG_const_type, tag_const_type_attributes, tag_const_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_constant, tag_constant_attributes, tag_constant_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_variable, tag_variable_attributes, tag_variable_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_unknown_type, tag_unknown_type_attributes, tag_unknown_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};


const static struct gdb_xml_element tag_subprogram_children[] = {
  {AT_name, NULL, NULL, 0, NULL, stos_end_cdata},
  {TAG_array_type, tag_array_type_attributes, tag_array_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_enumeration_type, tag_enumeration_type_attributes,
   tag_enumeration_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_pointer_type, tag_pointer_type_attributes, tag_pointer_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_string_type, tag_string_type_attributes, tag_string_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_structure_type, tag_structure_type_attributes,
   tag_structure_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_subroutine_type, tag_subroutine_type_attributes,
   tag_subroutine_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_typedef, tag_typedef_attributes, tag_typedef_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_union_type, tag_union_type_attributes, tag_union_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_base_type, tag_base_type_attributes, tag_base_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_base_type},
  {TAG_const_type, tag_const_type_attributes, tag_const_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_constant, tag_constant_attributes, tag_constant_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_variable, tag_variable_attributes, tag_variable_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_unknown_type, tag_unknown_type_attributes, tag_unknown_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};


const static struct gdb_xml_element tag_compile_unit_children[] = {
  {AT_name, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_producer, NULL, NULL, 0, NULL, stos_end_cdata},
  {AT_language, NULL, NULL, 0, NULL, stos_end_cdata},
  {TAG_subprogram, tag_subprogram_attributes, tag_subprogram_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  tag_lexical_block,
  {TAG_array_type, tag_array_type_attributes, tag_array_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_enumeration_type, tag_enumeration_type_attributes,
   tag_enumeration_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_pointer_type, tag_pointer_type_attributes, tag_pointer_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_string_type, tag_string_type_attributes, tag_string_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_structure_type, tag_structure_type_attributes,
   tag_structure_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_subroutine_type, tag_subroutine_type_attributes,
   tag_subroutine_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_typedef, tag_typedef_attributes, tag_typedef_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_union_type, tag_union_type_attributes, tag_union_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_base_type, tag_base_type_attributes, tag_base_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_base_type},
  {TAG_const_type, tag_const_type_attributes, tag_const_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_constant, tag_constant_attributes, tag_constant_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_variable, tag_variable_attributes, tag_variable_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {TAG_unknown_type, tag_unknown_type_attributes, tag_unknown_type_children,
   GDB_XML_EF_OPTIONAL | GDB_XML_EF_REPEATABLE, stos_start_generic_element,
   stos_end_generic_element},
  {NULL, NULL, NULL, GDB_XML_EF_NONE, NULL, NULL}
};

static struct gdb_xml_element tag_compile_unit = {
  TAG_compile_unit,
  tag_compile_unit_attributes,
  tag_compile_unit_children,
  0,
  stos_start_compile_unit,
  stos_end_compile_unit
};

static struct gdb_xml_element tag_array_type = {
  TAG_array_type,
  tag_array_type_attributes,
  tag_array_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_enumeration_type = {
  TAG_enumeration_type,
  tag_enumeration_type_attributes,
  tag_enumeration_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_formal_parameter = {
  TAG_formal_parameter,
  tag_formal_parameter_attributes,
  tag_formal_parameter_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_member = {
  TAG_member,
  tag_member_attributes,
  tag_member_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_pointer_type = {
  TAG_pointer_type,
  tag_pointer_type_attributes,
  tag_pointer_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_string_type = {
  TAG_string_type,
  tag_string_type_attributes,
  tag_string_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_structure_type = {
  TAG_structure_type,
  tag_structure_type_attributes,
  tag_structure_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_subroutine_type = {
  TAG_subroutine_type,
  tag_subroutine_type_attributes,
  tag_subroutine_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_typedef = {
  TAG_typedef,
  tag_typedef_attributes,
  tag_typedef_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_union_type = {
  TAG_union_type,
  tag_union_type_attributes,
  tag_union_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_subrange_type = {
  TAG_subrange_type,
  tag_subrange_type_attributes,
  tag_subrange_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_base_type = {
  TAG_base_type,
  tag_base_type_attributes,
  tag_base_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_const_type = {
  TAG_const_type,
  tag_const_type_attributes,
  tag_const_type_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_constant = {
  TAG_constant,
  tag_constant_attributes,
  tag_constant_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_enumerator = {
  TAG_enumerator,
  tag_enumerator_attributes,
  tag_enumerator_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_variable = {
  TAG_variable,
  tag_variable_attributes,
  tag_variable_children,
  0,
  NULL,
  NULL
};

static struct gdb_xml_element tag_unknown_type = {
  TAG_unknown_type,
  tag_unknown_type_attributes,
  tag_unknown_type_children,
  0,
  NULL,
  NULL
};


static struct caps_compile_unit *
parse_xml (const char *buffer, struct objfile *objfile)
{
  struct cleanup *back_to, *result_cleanup;
  struct gdb_xml_parser *parser;
  struct caps_user_data data;

  parser =
    gdb_xml_create_parser_and_cleanup (_("CAPS-XML"), &tag_compile_unit,
				       &data);
  gdb_xml_use_dtd (parser, "caps.dtd");

  memset (&data, 0, sizeof (struct caps_user_data));

  data.objfile = objfile;

  if (gdb_xml_parse (parser, buffer) == 0)
    {
      return &(data.cu->contents.cu);
    }
  else
    {
      warning (_("Failed to parser CAPS XML data"));
    }
  return NULL;
}


void
read_xml_source_to_source (struct symbol *sym, struct objfile *objfile)
{
  struct value *val;
  const gdb_byte *buffer = NULL;
  int bytes_read = 0;

  int save_smax;
  struct value_print_options opts;
  get_user_print_options (&opts);
  save_smax = opts.print_smax;
  user_print_options.print_smax = TYPE_LENGTH (SYMBOL_TYPE (sym));
  val = read_var_value (sym, NULL);

  if (TYPE_CODE (value_type (val)) != TYPE_CODE_ARRAY)
    {
      warning (_("Unexpected value type %d for XML feed variable"),
	       TYPE_CODE (value_type (val)));
      user_print_options.print_smax = save_smax;
      return;
    }
  else
    {
      struct gdbarch *gdbarch =
	get_type_arch (TYPE_TARGET_TYPE (value_type (val)));
      enum bfd_endian byte_order = gdbarch_byte_order (gdbarch);
      /* read the thing */
      buffer = value_contents_for_printing (val);
      user_print_options.print_smax = save_smax;
      if (value_length (val) <= 0)
	return;
    }
  /* now parse the xml */

  SYMBOL_XML_CU (sym) = parse_xml (buffer, objfile);
  /* add to symbol table */
}


/* create a block from the specified CAPS block  */
struct block *
xml_source_to_source_block (int scopeid, const struct block *parent)
{

  struct caps_element *child;
  struct context_stack *new;
  struct caps_element *scope_element;
  struct block *block;
  struct caps_compile_unit *cu;
  struct symbol *sym =
    lookup_symbol ("__hmpp_dwarf_xml", (struct block *) parent,
		   VAR_DOMAIN, (int *) NULL);

  if (sym == NULL)
    {
      warning (_("Cannot find __hmpp_dwarf_xml in current block %d"),
	       scopeid);
      return NULL;
    }

  if (scopeid <= 0)
    {
      warning (_("Bad scope element %d"), scopeid);
      return NULL;
    }

  cu = SYMBOL_XML_CU (sym);

  if (!cu)			/* failed to read */
    return NULL;

  scope_element = lookup_element (scopeid, cu);

  if (scope_element == NULL)
    {
      warning (_("No scope element found for block %d"), scopeid);
      return NULL;
    }

  new = push_context (0, 0);

  for (; scope_element != NULL; scope_element = scope_element->parent)
    {
      /* look up the block */
      child = scope_element->children;
      for (; child != NULL; child = child->sibling)
	{
	  switch (child->type)
	    {
	    case TAG_VARIABLE:
	      {
		struct symbol *newsym =
		  caps_create_variable (child, cu)->gdb_object.symbol;
		SYMBOL_SYMTAB (newsym) = SYMBOL_SYMTAB (sym);
		add_symbol_to_list (newsym, &local_symbols);
		break;
	      }
	    default:
	      break;
	    }
	}
    }

  new = pop_context ();
  block =
    finish_block (0, &local_symbols, new->old_blocks, 0, 0, cu->objfile);
  /* The new block will be a child of pblock->block.  */
  BLOCK_SUPERBLOCK (block) = (struct block *) parent;

  /* find and its parents */

  return block;
}

int
is_caps_v32_symbols_enabled (const struct block *block)
{
  static int looking = 0;	/* re-entrant check */
  struct symbol *sym;
  if (looking)
    return 0;
  looking = 1;

  sym = lookup_symbol_aux_local ("__hmpp_dwarf_xml", (struct block *) block,
		       VAR_DOMAIN, language_c);

  looking = 0;
  if (sym)
    return -1;
  else
    return 0;

}

struct symbol *
caps_lookup_symbol (const char *name, const struct block *block,
		    const domain_enum domain,
		    enum language language, int *is_a_field_of_this)
{
  struct value *val;
  struct symbol *scope_sym;
  struct symbol *sym;
  int ignore;
  LONGEST scope;
  const struct block *new_block;

  static int looking = 0;

  if (looking)
    return NULL;

  if (strcmp ("__hmpp_scope", name) == 0 ||
      strcmp ("__hmpp_dwarf_xml", name) == 0)
    {
      return NULL;
    }

  looking = -1;
  scope = caps_v32_current_scope (block);
  if (scope <= 0)
    {
      looking = 0;
      return NULL;
    }

  new_block = xml_source_to_source_block (scope, block);
  if (new_block == block)
    {
      looking = 0;
      return NULL;
    }

  sym = lookup_symbol (name, new_block, domain, is_a_field_of_this);

  looking = 0;
  return sym;
}

LONGEST
caps_v32_current_scope (const struct block * block)
{
  int ignore = 0;
  struct value *val;
  struct symbol *scope_sym =
    lookup_symbol ("__hmpp_scope", (struct block *) block,
		   VAR_DOMAIN, &ignore);

  if (!scope_sym)
    return 0;
  val = read_var_value (scope_sym, NULL);
  return value_as_long (val);
}

enum language
caps_v32_language (const struct block *block)
{
  enum language lang;
  int none = 0;
  struct caps_compile_unit *cu;
  struct symbol *sym = lookup_symbol ("__hmpp_dwarf_xml", block,
				      VAR_DOMAIN, &none);

  if (sym == NULL)
    return current_language->la_language;

  cu = SYMBOL_XML_CU (sym);

  if (cu == NULL)
    return current_language->la_language;

  return cu->language;
}
