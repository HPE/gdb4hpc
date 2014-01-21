/******************************************************************************\
 * <varstr.h>
 *
 * Copyright (c) 1996-2002, 2010 by Guardsoft Pty Ltd and others.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 * 	Guardsoft Pty Ltd - initial API and implementation (from libaif)
 * 	Greg Watson - stand alone implementation
 *
 * © 2011-2013 Cray Inc.  All Rights Reserved.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Cray Inc.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#ifndef _VARSTR_H_
#define _VARSTR_H_

struct varstr {
	char *	str_buf;
	char *	str_pos;
	int		buf_len;
};
typedef struct varstr	varstr;

varstr *	varstr_new(int initial_size);
varstr *	varstr_fromstr(char *str);
varstr *	varstr_cat(varstr *v, char *str);
varstr *	varstr_add(varstr *v, char ch);
varstr *	varstr_sprintf(varstr *v, char *fmt, ...);
char *		varstr_tostr(varstr *v);
void		varstr_free(varstr *v);

#endif /* !_VARSTR_H_ */
