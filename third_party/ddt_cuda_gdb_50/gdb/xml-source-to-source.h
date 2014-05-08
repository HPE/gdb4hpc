/* CAPS HMPP debugging format support for GDB.

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

#ifndef XML_SOURCE_TO_SOURCE_H
#define XML_SOURCE_TO_SOURCE_H

void read_xml_source_to_source (struct symbol *, struct objfile *);

struct block *xml_source_to_source_block (int block,
					  const struct block *parent);

int is_caps_v32_symbols_enabled ();

struct symbol *caps_lookup_symbol (const char *name,
				   const struct block *block,
				   const domain_enum domain,
				   enum language language,
				   int *is_a_field_of_this);

enum language caps_v32_language (const struct block *block);

LONGEST caps_v32_current_scope (const struct block *block);

#endif
