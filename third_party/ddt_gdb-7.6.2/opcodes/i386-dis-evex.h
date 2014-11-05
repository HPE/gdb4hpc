#ifdef NEED_PREFIX_TABLE

  /* PREFIX_EVEX_0F18_R_0_M_0_P_0 */
  {
    { "vprefetchnta",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_1_M_0_P_0 */
  {
    { "vprefetch0",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_2_M_0_P_0 */
  {
    { "vprefetch1",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_3_M_0_P_0 */
  {
    { "vprefetch2",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_4_M_0_P_0 */
  {
    { "vprefetchenta",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_5_M_0_P_0 */
  {
    { "vprefetche0",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_6_M_0_P_0 */
  {
    { "vprefetche1",	{ MVec } },
  },

  /* PREFIX_EVEX_0F18_R_7_M_0_P_0 */
  {
    { "vprefetche2",	{ MVec } },
  },

  /* PREFIX_EVEX_0F28 */
  {
    { VEX_W_TABLE (EVEX_W_0F28_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F28_P_2) },
    { Bad_Opcode },
  },

  /* PREFIX_EVEX_0F29 */
  {
    { VEX_W_TABLE (EVEX_W_0F29_P_0) },
    { VEX_W_TABLE (EVEX_W_0F29_P_1) },
    { VEX_W_TABLE (EVEX_W_0F29_P_2) },
    { VEX_W_TABLE (EVEX_W_0F29_P_3) }
  },

  /* PREFIX_EVEX_0F58 */
  {
    { VEX_W_TABLE (EVEX_W_0F58_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F58_P_2) },
  },

  /* PREFIX_EVEX_0F59 */
  {
    { VEX_W_TABLE (EVEX_W_0F59_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F59_P_2) },
  },

  /* PREFIX_EVEX_0F5A */
  {
    { VEX_W_TABLE (EVEX_W_0F5A_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F5A_P_2) },
  },

  /* PREFIX_EVEX_0F5C */
  {
    { VEX_W_TABLE (EVEX_W_0F5C_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F5C_P_2) },
  },

  /* PREFIX_EVEX_0F66 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F66_P_2) },
  },

  /* PREFIX_EVEX_0F6F */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vmovdqa%LN", { XMEVexUpWdq, EXxEVexUpNoBcstWdq } },
  },

  /* PREFIX_EVEX_0F70 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F70_P_2) },
  },

  /* PREFIX_EVEX_0F72_REG_2 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F72_R_2_P_2) },
  },

  /* PREFIX_EVEX_0F72_REG_4 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F72_R_4_P_2) },
  },

  /* PREFIX_EVEX_0F72_REG_6 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F72_R_6_P_2) },
  },

  /* PREFIX_EVEX_0F76 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F76_P_2) },
  },

  /* PREFIX_EVEX_0F7A */
  {
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F7A_P_1) },
  },

  /* PREFIX_EVEX_0F7F */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vmovdqa%LN", { MEVexDownNoBcstWpX, XMEVexDownWpX } },
  },

  /* PREFIX_EVEX_0FAE_R_7_M_0 */
  {
    { Bad_Opcode },
    { "clevict1", { MVec } },
    { Bad_Opcode },
    { "clevict0", { MVec } },
  },

  /* PREFIX_EVEX_0FC2 */
  {
    { VEX_W_TABLE (EVEX_W_0FC2_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0FC2_P_2) },
  },

  /* PREFIX_EVEX_0FDB */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpand%LW", { XM, Vex, EXxEVexWdq } },
  },

  /* PREFIX_EVEX_0FDF */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpandn%LW", { XM, Vex, EXxEVexWdq } },
  },

  /* PREFIX_EVEX_0FE6 */
  {
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0FE6_P_1) },
  },

  /* PREFIX_EVEX_0FEB */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpor%LW", { XM, Vex, EXxEVexWdq } },
  },

  /* PREFIX_EVEX_0FEF */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpxor%LW", { XM, Vex, EXxEVexWdq } },
  },

  /* PREFIX_EVEX_0FFA */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0FFA_P_2) },
  },

  /* PREFIX_EVEX_0FFE */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0FFE_P_2) },
  },

  /* PREFIX_EVEX_0F3818 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3818_P_2) },
  },

  /* PREFIX_EVEX_0F3819 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3819_P_2) },
  },

  /* PREFIX_EVEX_0F381A */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F381A_P_2) },
  },

  /* PREFIX_EVEX_0F381B */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F381B_P_2) },
  },

  /* PREFIX_EVEX_0F3827 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3827_P_2) },
  },

  /* PREFIX_EVEX_0F3836 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3836_P_2) },
  },

  /* PREFIX_EVEX_0F3839 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3839_P_2) },
  },

  /* PREFIX_EVEX_0F383B */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F383B_P_2) },
  },

  /* PREFIX_EVEX_0F383D */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F383D_P_2) },
  },

  /* PREFIX_EVEX_0F383F */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F383F_P_2) },
  },

  /* PREFIX_EVEX_0F3840 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3840_P_2) },
  },

  /* PREFIX_EVEX_0F3842 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vgetexpp%XW", { XM, EXxEVexWpXR, EXxEVexS } },
  },

  /* PREFIX_EVEX_0F3845 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3845_P_2) },
  },

  /* PREFIX_EVEX_0F3846 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3846_P_2) },
  },

  /* PREFIX_EVEX_0F3847 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3847_P_2) },
  },

  /* PREFIX_EVEX_0F3850 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vaddnp%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F3851 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3851_P_2) },
  },

  /* PREFIX_EVEX_0F3852 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vgminp%XW", { XM, Vex, EXxEVexWpXR, EXxEVexS } },
  },

  /* PREFIX_EVEX_0F3853 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vgmaxp%XW", { XM, Vex, EXxEVexWpXR, EXxEVexS } },
  },

  /* PREFIX_EVEX_0F3855 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfixupnanp%XW", { XM, Vex, EXxEVexWdqR, EXxEVexS } },
  },

  /* PREFIX_EVEX_0F3858 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3858_P_2) },
  },

  /* PREFIX_EVEX_0F3859 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3859_P_2) },
  },

  /* PREFIX_EVEX_0F385A */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F385A_P_2) },
  },

  /* PREFIX_EVEX_0F385B */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F385B_P_2) },
  },

  /* PREFIX_EVEX_0F385C */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F385C_P_2) },
  },

  /* PREFIX_EVEX_0F385D */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F385D_P_2) },
  },

  /* PREFIX_EVEX_0F385E */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F385E_P_2) },
  },

  /* PREFIX_EVEX_0F385F */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F385F_P_2) },
  },

  /* PREFIX_EVEX_0F3864 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpblendm%LW", { XM, Vex, EXxEVexWdq } },
  },

  /* PREFIX_EVEX_0F3865 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vblendmp%XW", { XM, Vex, EXxEVexWpX } },
  },

  /* PREFIX_EVEX_0F386C */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F386C_P_2) },
  },

  /* PREFIX_EVEX_0F386D */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vsubrp%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F386E */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F386E_P_2) },
  },

  /* PREFIX_EVEX_0F386F */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F386F_P_2) },
  },

  /* PREFIX_EVEX_0F3874 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3874_P_2) },
  },

  /* PREFIX_EVEX_0F3884 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3884_P_2) },
  },

  /* PREFIX_EVEX_0F3886 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3886_P_2) },
  },

  /* PREFIX_EVEX_0F3887 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3887_P_2) },
  },

  /* PREFIX_EVEX_0F3890 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpgatherd%LW", { XMEVexUpWdq, MEVexUpVSIBWdq } },
  },

  /* PREFIX_EVEX_0F3892 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vgatherdp%XW", { XMEVexUpWpX, MEVexUpVSIBWpX } },
  },

  /* PREFIX_EVEX_0F3898 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfmadd132p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F389A */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfmsub132p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F389C */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfnmadd132p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F389E */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfnmsub132p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38A0 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vpscatterd%LW", { MEVexDownVSIBWdq, XMEVexDownWdq } },
  },

  /* PREFIX_EVEX_0F38A2 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vscatterdp%XW", { MEVexDownVSIBWpX, XMEVexDownWpX } },
  },

  /* PREFIX_EVEX_0F38A4 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38A4_P_2) },
  },

  /* PREFIX_EVEX_0F38A8 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfmadd213p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38AA */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfmsub213p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38AC */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfnmadd213p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38AE */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfnmsub213p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38B4 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38B4_P_2) },
  },

  /* PREFIX_EVEX_0F38B5 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38B5_P_2) },
  },

  /* PREFIX_EVEX_0F38B8 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfmadd231p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38BA */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfmsub231p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38BC */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfnmadd231p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38BE */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vfnmsub231p%XW", { XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },

  /* PREFIX_EVEX_0F38C6_REG_0 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vgatherpf0hintdp%XW", { MEVexPFVSIBWpX } },
  },

  /* PREFIX_EVEX_0F38C6_REG_1 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38C6_R_1_P_2) },
  },

  /* PREFIX_EVEX_0F38C6_REG_2 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38C6_R_2_P_2) },
  },

  /* PREFIX_EVEX_0F38C6_REG_4 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vscatterpf0hintdp%XW", { MEVexPFVSIBWpX } },
  },

  /* PREFIX_EVEX_0F38C6_REG_5 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38C6_R_5_P_2) },
  },

  /* PREFIX_EVEX_0F38C6_REG_6 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38C6_R_6_P_2) },
  },

  /* PREFIX_EVEX_0F38C8 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38C8_P_2) },
  },

  /* PREFIX_EVEX_0F38C9 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38C9_P_2) },
  },

  /* PREFIX_EVEX_0F38CA */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38CA_P_2) },
  },

  /* PREFIX_EVEX_0F38CB */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38CB_P_2) },
  },

  /* PREFIX_EVEX_0F38CC */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38CC_P_2) },
  },

  /* PREFIX_EVEX_0F38CD */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F38CD_P_2) },
  },

  /* PREFIX_EVEX_0F38D0 */
  {
    { "vloadunpackl%LW", { XMEVexUpWdq, MEVexUpWdq } },
    { Bad_Opcode },
    { "vpackstorel%LW", { MEVexDownWdq, XMEVexDownWdq } },
  },

  /* PREFIX_EVEX_0F38D1 */
  {
    { "vloadunpacklp%XW", { XMEVexUpWpX, MEVexUpWpX } },
    { Bad_Opcode },
    { "vpackstorelp%XW", { MEVexDownWpX, XMEVexDownWpX } },
  },

  /* PREFIX_EVEX_0F38D4 */
  {
    { "vloadunpackh%LW", { XMEVexUpWdq, MEVexUpWdq } },
    { Bad_Opcode },
    { "vpackstoreh%LW", { MEVexDownWdq, XMEVexDownWdq } },
  },

  /* PREFIX_EVEX_0F38D5 */
  {
    { "vloadunpackhp%XW", { XMEVexUpWpX, MEVexUpWpX } },
    { Bad_Opcode },
    { "vpackstorehp%XW", { MEVexDownWpX, XMEVexDownWpX } },
  },

  /* PREFIX_EVEX_0F3A03 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3A03_P_2) },
  },

  /* PREFIX_EVEX_0F3A07 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3A07_P_2) },
  },

  /* PREFIX_EVEX_0F3A1E */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3A1E_P_2) },
  },

  /* PREFIX_EVEX_0F3A1F */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3A1F_P_2) },
  },

  /* PREFIX_EVEX_0F3A26 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vgetmantp%XW", { XM, EXxEVexWpXR, Ib, EXxEVexS } },
  },

  /* PREFIX_EVEX_0F3A52 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { "vrndfxpntp%XW", { XM, EXxEVexWpXR, Ib, EXxEVexS } },
  },

  /* PREFIX_EVEX_0F3ACA */
  {
    { VEX_W_TABLE (EVEX_W_0F3ACA_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3ACA_P_2) },
    { VEX_W_TABLE (EVEX_W_0F3ACA_P_3) },
  },

  /* PREFIX_EVEX_0F3ACB */
  {
    { VEX_W_TABLE (EVEX_W_0F3ACB_P_0) },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3ACB_P_2) },
  },

  /* PREFIX_EVEX_0F3AE6 */
  {
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { VEX_W_TABLE (EVEX_W_0F3AE6_P_3) },
  },

#endif /* NEED_PREFIX_TABLE */

#ifdef NEED_EVEX_EH_TABLE
  {
    /* EVEX_EH_0F29_P_1_W1 */
    { "vmovnrapd",	{ MEVexDownNoBcstWpX, XMEVexDownWpX } },
    { "vmovnrngoapd",	{ MEVexDownNoBcstWpX, XMEVexDownWpX } },
  },
  {
    /* EVEX_EH_0F29_P_3_W0 */
    { "vmovnraps",	{ MEVexDownNoBcstWpX, XMEVexDownWpX } },
    { "vmovnrngoaps",	{ MEVexDownNoBcstWpX, XMEVexDownWpX } },
  }
#endif /* NEED_EVEX_EH_TABLE */

#ifdef NEED_VEX_W_TABLE

  {
    /* EVEX_W_0F28_P_0 */
    { "vmovaps",	{ XMEVexUpWpX, EXxEVexUpNoBcstWpX } },
  },
  {
    /* EVEX_W_0F28_P_2 */
    { Bad_Opcode },
    { "vmovapd",	{ XMEVexUpWpX, EXxEVexUpNoBcstWpX } },
  },
  {
    /* EVEX_W_0F29_P_0 */
    { "vmovaps",	{ MEVexDownNoBcstWpX, XMEVexDownWpX } },
  },
  {
    /* EVEX_W_0F29_P_1 */
    { Bad_Opcode },
    { EVEX_EH_TABLE (EVEX_EH_0F29_P_1_W1) },
  },
  {
    /* EVEX_W_0F29_P_2 */
    { Bad_Opcode },
    { "vmovapd",	{ MEVexDownNoBcstWpX, XMEVexDownWpX } },
  },
  {
    /* EVEX_W_0F29_P_3 */
    { EVEX_EH_TABLE (EVEX_EH_0F29_P_3_W0) }
  },
  {
    /* EVEX_W_0F58_P_0 */
    { "vaddps",		{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F58_P_2 */
    { Bad_Opcode },
    { "vaddpd",		{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F59_P_0 */
    { "vmulps",		{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F59_P_2 */
    { Bad_Opcode },
    { "vmulpd",		{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F5A_P_0 */
    { "vcvtps2pd",	{ XM, EXxEVexBcstOnlyHalfW } },
  },
  {
    /* EVEX_W_0F5A_P_2 */
    { Bad_Opcode },
    { "vcvtpd2ps",	{ XM, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F5C_P_0 */
    { "vsubps",		{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F5C_P_2 */
    { Bad_Opcode },
    { "vsubpd",		{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F66_P_2 */
    { "vpcmpgtd",	{ XMask, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F70_P_2 */
    { "vpshufd",	{ XM, EXxEVexW, Ib } },
  },
  {
    /* EVEX_W_0F72_R_2_P_2 */
    { "vpsrld",		{ VexMask, EXxEVexWdq, Ib } },
  },
  {
    /* EVEX_W_0F72_R_4_P_2 */
    { "vpsrad",		{ VexMask, EXxEVexWdq, Ib } },
  },
  {
    /* EVEX_W_0F72_R_6_P_2 */
    { "vpslld",		{ VexMask, EXxEVexWdq, Ib } },
  },
  {
    /* EVEX_W_0F76_P_2 */
    { "vpcmpeqd",	{ XMask, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F7A_P_1 */
    { "vcvtudq2pd",	{ XM, EXxEVexBcstOnlyHalfW } },
  },
  {
    /* EVEX_W_0FC2_P_0 */
    { "vcmpps",		{ XMask, Vex, EXxEVexWpXR, VCMP, EXxEVexS } },
  },
  {
    /* EVEX_W_0FC2_P_2 */
    { Bad_Opcode },
    { "vcmppd",		{ XMask, Vex, EXxEVexWpXR, VCMP, EXxEVexS } },
  },
  {
    /* EVEX_W_0FE6_P_1 */
    { "vcvtdq2pd",	{ XM, EXxEVexBcstOnlyHalfW } },
  },
  {
    /* EVEX_W_0FFA_P_2 */
    { "vpsubd",		{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0FFE_P_2 */
    { "vpaddd",		{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3818_P_2 */
    { "vbroadcastss",	{ XMEVexUpWpX, MEVexUpWpX } },
  },
  {
    /* EVEX_W_0F3819_P_2 */
    { Bad_Opcode },
    { "vbroadcastsd",	{ XMEVexUpWpX, MEVexUpWpX } },
  },
  {
    /* EVEX_W_0F381A_P_2 */
    { "vbroadcastf32x4", { XMEVexUpWpX, MEVexUpBcstX4WpX } },
  },
  {
    /* EVEX_W_0F381B_P_2 */
    { Bad_Opcode },
    { "vbroadcastf64x4", { XMEVexUpWpX, MEVexUpBcstX4WpX } },
  },
  {
    /* EVEX_W_0F3827_P_2 */
    { "vptestmd",	{ XMask, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3836_P_2 */
    { "vpermd",		{ XM, Vex, EXxEVexW } },
  },
  {
    /* EVEX_W_0F3839_P_2 */
    { "vpminsd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F383B_P_2 */
    { "vpminud",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F383D_P_2 */
    { "vpmaxsd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F383F_P_2 */
    { "vpmaxud",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3840_P_2 */
    { "vpmulld",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3845_P_2 */
    { "vpsrlvd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3846_P_2 */
    { "vpsravd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3847_P_2 */
    { "vpsllvd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3851_P_2 */
    { "vgmaxabsps",	{ XM, Vex, EXxEVexWpXR, EXxEVexS } },
  },
  {
    /* EVEX_W_0F3858_P_2 */
    { "vpbroadcastd",	{ XMEVexUpWdq, MEVexUpWdq } },
  },
  {
    /* EVEX_W_0F3859_P_2 */
    { Bad_Opcode },
    { "vpbroadcastq",	{ XMEVexUpWdq, MEVexUpWdq } },
  },
  {
    /* EVEX_W_0F385A_P_2 */
    { "vbroadcasti32x4", { XMEVexUpWdq, MEVexUpBcstX4Wdq } },
  },
  {
    /* EVEX_W_0F385B_P_2 */
    { Bad_Opcode },
    { "vbroadcasti64x4", { XMEVexUpWdq, MEVexUpBcstX4Wdq } },
  },
  {
    /* EVEX_W_0F385C_P_2 */
    { "vpadcd",		{ XM, MaskVexZMM, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F385D_P_2 */
    { "vpaddsetcd",	{ XM, MaskVexZMM, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F385E_P_2 */
    { "vpsbbd",		{ XM, MaskVexZMM, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F385F_P_2 */
    { "vpsubsetbd",	{ XM, MaskVexZMM, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F386C_P_2 */
    { "vpsubrd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F386E_P_2 */
    { "vpsbbrd",	{ XM, MaskVexZMM, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F386F_P_2 */
    { "vpsubrsetbd",	{ XM, MaskVexZMM, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3874_P_2 */
    { "vpcmpltd",	{ XMask, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3884_P_2 */
    { "vscaleps",	{ XM, Vex, EXxEVexWdqR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F3886_P_2 */
    { "vpmulhud",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3887_P_2 */
    { "vpmulhd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F38A4_P_2 */
    { "vfmadd233ps",	{ XM, Vex, EXxEVexBcstOnly2WR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F38B4_P_2 */
    { "vpmadd233d",	{ XM, Vex, EXxEVexBcstOnly2W } },
  },
  {
    /* EVEX_W_0F38B5_P_2 */
    { "vpmadd231d",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F38C6_R_1_P_2 */
    { "vgatherpf0dps",	{ MEVexPFVSIBWpX } },
  },
  {
    /* EVEX_W_0F38C6_R_2_P_2 */
    { "vgatherpf1dps",	{ MEVexPFVSIBWpX } },
  },
  {
    /* EVEX_W_0F38C6_R_5_P_2 */
    { "vscatterpf0dps",	{ MEVexPFVSIBWpX } },
  },
  {
    /* EVEX_W_0F38C6_R_6_P_2 */
    { "vscatterpf1dps",	{ MEVexPFVSIBWpX } },
  },
  {
    /* EVEX_W_0F38C8_P_2 */
    { "vexp223ps",	{ XM, EXxEVexWR, EXxEVexS } },
  },
  {
    /* EVEX_W_0F38C9_P_2 */
    { "vlog2ps",	{ XM, EXxEVexWR, EXxEVexS } },
  },
  {
    /* EVEX_W_0F38CA_P_2 */
    { "vrcp23ps",	{ XM, EXxEVexWR, EXxEVexS } },
  },
  {
    /* EVEX_W_0F38CB_P_2 */
    { "vrsqrt23ps",	{ XM, EXxEVexWR, EXxEVexS } },
  },
  {
    /* EVEX_W_0F38CC_P_2 */
    { "vaddsetsps",	{ XM, Vex, EXxEVexWpXR, EXxEVexR } },
  },
  {
    /* EVEX_W_0F38CD_P_2 */
    { "vpaddsetsd",	{ XM, Vex, EXxEVexWdq } },
  },
  {
    /* EVEX_W_0F3A03_P_2 */
    { "valignd",	{ XM, Vex, EXxEVexW, Ib } },
  },
  {
    /* EVEX_W_0F3A07_P_2 */
    { "vpermf32x4",	{ XM, EXxEVexW, Ib } },
  },
  {
    /* EVEX_W_0F3A1E_P_2 */
    { "vpcmpud",	{ XMask, Vex, EXxEVexWdq, Ib } },
  },
  {
    /* EVEX_W_0F3A1F_P_2 */
    { "vpcmpd",		{ XMask, Vex, EXxEVexWdq, VPCMP } },
  },
  {
    /* EVEX_W_0F3ACA_P_0 */
    { "vcvtfxpntudq2ps", { XM, EXxEVexWdqR, EXxEVexR, Ib } },
  },
  {
    /* EVEX_W_0F3ACA_P_2 */
    { "vcvtfxpntps2udq", { XM, EXxEVexWpXR, Ib, EXxEVexS } },
  },
  {
    /* EVEX_W_0F3ACA_P_3 */
    { Bad_Opcode },
    { "vcvtfxpntpd2udq", { XM, EXxEVexWpXR, Ib, EXxEVexS } },
  },
  {
    /* EVEX_W_0F3ACB_P_0 */
    { "vcvtfxpntdq2ps",	{ XM, EXxEVexWdqR, EXxEVexR, Ib } },
  },
  {
    /* EVEX_W_0F3ACB_P_2 */
    { "vcvtfxpntps2dq",	{ XM, EXxEVexWpXR, Ib, EXxEVexS } },
  },
  {
    /* EVEX_W_0F3AE6_P_3 */
    { Bad_Opcode },
    { "vcvtfxpntpd2dq",	{ XM, EXxEVexWpXR, Ib, EXxEVexS } },
  },

#endif /* NEED_VEX_W_TABLE */

#ifdef NEED_OPCODE_TABLE

static const struct dis386 evex_table[][256] = {
  /* VEX_0F */
  {
    /* 00 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 08 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 10 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 18 */
    { REG_TABLE (REG_EVEX_0F18) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 20 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 28 */
    { PREFIX_TABLE (PREFIX_EVEX_0F28) },
    { PREFIX_TABLE (PREFIX_EVEX_0F29) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 30 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 38 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 40 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 48 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 50 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 58 */
    { PREFIX_TABLE (PREFIX_EVEX_0F58) },
    { PREFIX_TABLE (PREFIX_EVEX_0F59) },
    { PREFIX_TABLE (PREFIX_EVEX_0F5A) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F5C) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 60 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F66) },
    { Bad_Opcode },
    /* 68 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F6F) },
    /* 70 */
    { PREFIX_TABLE (PREFIX_EVEX_0F70) },
    { Bad_Opcode },
    { REG_TABLE (REG_EVEX_0F72) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F76) },
    { Bad_Opcode },
    /* 78 */
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F7A) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F7F) },
    /* 80 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 88 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 90 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 98 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* a0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* a8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { REG_TABLE (REG_EVEX_0FAE) },
    { Bad_Opcode },
    /* b0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* b8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* c0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FC2) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* c8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* d0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* d8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FDB) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FDF) },
    /* e0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FE6) },
    { Bad_Opcode },
    /* e8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FEB) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FEF) },
    /* f0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* f8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FFA) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0FFE) },
    { Bad_Opcode },
  },
  /* VEX_0F38 */
  {
    /* 00 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 08 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 10 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 18 */
    { PREFIX_TABLE (PREFIX_EVEX_0F3818) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3819) },
    { PREFIX_TABLE (PREFIX_EVEX_0F381A) },
    { PREFIX_TABLE (PREFIX_EVEX_0F381B) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 20 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3827) },
    /* 28 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 30 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3836) },
    { Bad_Opcode },
    /* 38 */
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3839) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F383B) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F383D) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F383F) },
    /* 40 */
    { PREFIX_TABLE (PREFIX_EVEX_0F3840) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3842) },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3845) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3846) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3847) },
    /* 48 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 50 */
    { PREFIX_TABLE (PREFIX_EVEX_0F3850) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3851) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3852) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3853) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3855) },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 58 */
    { PREFIX_TABLE (PREFIX_EVEX_0F3858) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3859) },
    { PREFIX_TABLE (PREFIX_EVEX_0F385A) },
    { PREFIX_TABLE (PREFIX_EVEX_0F385B) },
    { PREFIX_TABLE (PREFIX_EVEX_0F385C) },
    { PREFIX_TABLE (PREFIX_EVEX_0F385D) },
    { PREFIX_TABLE (PREFIX_EVEX_0F385E) },
    { PREFIX_TABLE (PREFIX_EVEX_0F385F) },
    /* 60 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3864) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3865) },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 68 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F386C) },
    { PREFIX_TABLE (PREFIX_EVEX_0F386D) },
    { PREFIX_TABLE (PREFIX_EVEX_0F386E) },
    { PREFIX_TABLE (PREFIX_EVEX_0F386F) },
    /* 70 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3874) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 78 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 80 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3884) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3886) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3887) },
    /* 88 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 90 */
    { PREFIX_TABLE (PREFIX_EVEX_0F3890) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3892) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 98 */
    { PREFIX_TABLE (PREFIX_EVEX_0F3898) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F389A) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F389C) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F389E) },
    { Bad_Opcode },
    /* a0 */
    { PREFIX_TABLE (PREFIX_EVEX_0F38A0) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38A2) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38A4) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* a8 */
    { PREFIX_TABLE (PREFIX_EVEX_0F38A8) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38AA) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38AC) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38AE) },
    { Bad_Opcode },
    /* b0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38B4) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38B5) },
    { Bad_Opcode },
    { Bad_Opcode },
    /* b8 */
    { PREFIX_TABLE (PREFIX_EVEX_0F38B8) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38BA) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38BC) },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38BE) },
    { Bad_Opcode },
    /* c0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { REG_TABLE (REG_EVEX_0F38C6) },
    { Bad_Opcode },
    /* c8 */
    { PREFIX_TABLE (PREFIX_EVEX_0F38C8) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38C9) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38CA) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38CB) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38CC) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38CD) },
    { Bad_Opcode },
    { Bad_Opcode },
    /* d0 */
    { PREFIX_TABLE (PREFIX_EVEX_0F38D0) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38D1) },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F38D4) },
    { PREFIX_TABLE (PREFIX_EVEX_0F38D5) },
    { Bad_Opcode },
    { Bad_Opcode },
    /* d8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* e0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* e8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* f0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* f8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
  },
  /* VEX_0F3A */
  {
    /* 00 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3A03) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3A07) },
    /* 08 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 10 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 18 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3A1E) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3A1F) },
    /* 20 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3A26) },
    { Bad_Opcode },
    /* 28 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 30 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 38 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 40 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 48 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 50 */
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3A52) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 58 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 60 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 68 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 70 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 78 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 80 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 88 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 90 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* 98 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* a0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* a8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* b0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* b8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* c0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* c8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3ACA) },
    { PREFIX_TABLE (PREFIX_EVEX_0F3ACB) },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* d0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* d8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* e0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { PREFIX_TABLE (PREFIX_EVEX_0F3AE6) },
    { Bad_Opcode },
    /* e8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* f0 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    /* f8 */
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
    { Bad_Opcode },
  },
};

#endif /* NEED_OPCODE_TABLE */
