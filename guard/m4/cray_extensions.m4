#
# cray_extensions.m4
# Cray configure extensions.
#
# Copyright 2019 Cray Inc. All Rights Reserved
# Copyright (c) 1996-2010 by Guardsoft Pty Ltd.
#
# Unpublished Proprietary Information.
# This unpublished work is protected to trade secret, copyright and other laws.
# Except as permitted by contract or express written permission of Cray Inc.,
# no part of this work or its content may be used, reproduced or disclosed
# in any form.
#

AC_DEFUN([CRAY_PARSE_VERSION], [
    m4_if(m4_eval([$# > 2]), [1], [
        m4_if(m4_normalize(m4_argn([$#], $*)), [], [],
            [m4_bregexp([$1], [$2], [m4_define(m4_normalize(m4_argn([$#], $*)), \]m4_if([$#], [3], [&], m4_eval([$# - 3]))[)])])
        m4_if(m4_eval([$# > 3]), [1], [CRAY_PARSE_VERSION(m4_reverse(m4_shift(m4_reverse($@))))])
    ])
    ]
)
