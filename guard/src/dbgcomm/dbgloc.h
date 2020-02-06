/*********************************************************************************\
 * Program location.
 *
 * Copyright (c) 1996-2008 by Guardsoft Pty Ltd.
 *
 * © 2010-2013 Cray Inc.  All Rights Reserved.
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

#ifndef _DBGLOC_H
#define _DBGLOC_H

struct dbgloc {
    char* loc_file;
    char* loc_func;
    char* loc_addr;
    int loc_line;
};
typedef struct dbgloc dbgloc_t;

#endif /* !_DBGLOC_H */
