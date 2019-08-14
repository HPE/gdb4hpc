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

dnl  CRAY_DEFINE_SUBSTRINGS_AS(string, regexp, macro0[, macro1[, ... macroN ]])
dnl  ***************************************************************************
dnl
dnl  Searches for the first match of `regexp` in `string`. For both the entire
dnl  regular expression `regexp` (`\0`) and each sub-expression within capturing
dnl  parentheses (`\1`, `\2`, `\3`, ... , `\N`) a macro expanding to the
dnl  corresponding matching text will be created, named according to the
dnl  argument `macroN` passed. If a `macroN` argument is omitted or empty, the
dnl  corresponding parentheses in the regular expression will be considered as
dnl  non-capturing. If `regexp` cannot be found in `string` no macro will be
dnl  defined. If `regexp` can be found but some of its capturing parentheses
dnl  cannot, the macro(s) corresponding to the latter will be defined as empty
dnl  strings.
dnl
dnl  Source: https://github.com/madmurphy/not-autotools
dnl
dnl  ***************************************************************************
AC_DEFUN([CRAY_DEFINE_SUBSTRINGS_AS], [
    m4_if(m4_eval([$# > 2]), [1], [
        m4_if(m4_normalize(m4_argn([$#], $*)), [], [],
            [m4_bregexp([$1], [$2], [m4_define(m4_normalize(m4_argn([$#], $*)), \]m4_if([$#], [3], [&], m4_eval([$# - 3]))[)])])
        m4_if(m4_eval([$# > 3]), [1], [CRAY_DEFINE_SUBSTRINGS_AS(m4_reverse(m4_shift(m4_reverse($@))))])
    ])
])

AC_DEFUN([CRAY_PARSE_VERSION], [
    CRAY_DEFINE_SUBSTRINGS_AS(
        m4_include([$1]),
        [revision="\([0-9]+\)"\s*\n*\s*\n*major="\([0-9]+\)"\s*\n*\s*\n*minor="\([0-9]+\)"],
        [], [CRAY_REVISION], [CRAY_MAJOR], [CRAY_MINOR]
    )
    m4_ifndef([CRAY_REVISION], [AC_MSG_ERROR([invalid version format in release_versioning file])])
    m4_ifndef([CRAY_MAJOR], [AC_MSG_ERROR([invalid version format in release_versioning file])])
    m4_ifndef([CRAY_MINOR], [AC_MSG_ERROR([invalid version format in release_versioning file])])

    AC_MSG_NOTICE([Building with version CRAY_MAJOR.CRAY_MINOR.CRAY_REVISION])
])
