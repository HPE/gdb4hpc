/* THIS FILE IS GENERATED.  Original: k1om.xml */

#include "defs.h"
#include "osabi.h"
#include "target-descriptions.h"

struct target_desc *tdesc_k1om;
static void
initialize_tdesc_k1om (void)
{
  struct target_desc *result = allocate_target_description ();
  struct tdesc_feature *feature;
  struct tdesc_type *field_type, *type;

  set_tdesc_architecture (result, bfd_scan_arch ("k1om"));

  feature = tdesc_create_feature (result, "org.gnu.gdb.i386.core");
  field_type = tdesc_create_flags (feature, "i386_eflags", 4);
  tdesc_add_flag (field_type, 0, "CF");
  tdesc_add_flag (field_type, 1, "");
  tdesc_add_flag (field_type, 2, "PF");
  tdesc_add_flag (field_type, 4, "AF");
  tdesc_add_flag (field_type, 6, "ZF");
  tdesc_add_flag (field_type, 7, "SF");
  tdesc_add_flag (field_type, 8, "TF");
  tdesc_add_flag (field_type, 9, "IF");
  tdesc_add_flag (field_type, 10, "DF");
  tdesc_add_flag (field_type, 11, "OF");
  tdesc_add_flag (field_type, 14, "NT");
  tdesc_add_flag (field_type, 16, "RF");
  tdesc_add_flag (field_type, 17, "VM");
  tdesc_add_flag (field_type, 18, "AC");
  tdesc_add_flag (field_type, 19, "VIF");
  tdesc_add_flag (field_type, 20, "VIP");
  tdesc_add_flag (field_type, 21, "ID");

  tdesc_create_reg (feature, "rax", 0, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rbx", 1, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rcx", 2, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rdx", 3, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rsi", 4, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rdi", 5, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rbp", 6, 1, NULL, 64, "data_ptr");
  tdesc_create_reg (feature, "rsp", 7, 1, NULL, 64, "data_ptr");
  tdesc_create_reg (feature, "r8", 8, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r9", 9, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r10", 10, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r11", 11, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r12", 12, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r13", 13, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r14", 14, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "r15", 15, 1, NULL, 64, "int64");
  tdesc_create_reg (feature, "rip", 16, 1, NULL, 64, "code_ptr");
  tdesc_create_reg (feature, "eflags", 17, 1, NULL, 32, "i386_eflags");
  tdesc_create_reg (feature, "cs", 18, 1, NULL, 32, "int32");
  tdesc_create_reg (feature, "ss", 19, 1, NULL, 32, "int32");
  tdesc_create_reg (feature, "ds", 20, 1, NULL, 32, "int32");
  tdesc_create_reg (feature, "es", 21, 1, NULL, 32, "int32");
  tdesc_create_reg (feature, "fs", 22, 1, NULL, 32, "int32");
  tdesc_create_reg (feature, "gs", 23, 1, NULL, 32, "int32");
  tdesc_create_reg (feature, "st0", 24, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st1", 25, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st2", 26, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st3", 27, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st4", 28, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st5", 29, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st6", 30, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "st7", 31, 1, NULL, 80, "i387_ext");
  tdesc_create_reg (feature, "fctrl", 32, 1, "float", 32, "int");
  tdesc_create_reg (feature, "fstat", 33, 1, "float", 32, "int");
  tdesc_create_reg (feature, "ftag", 34, 1, "float", 32, "int");
  tdesc_create_reg (feature, "fiseg", 35, 1, "float", 32, "int");
  tdesc_create_reg (feature, "fioff", 36, 1, "float", 32, "int");
  tdesc_create_reg (feature, "foseg", 37, 1, "float", 32, "int");
  tdesc_create_reg (feature, "fooff", 38, 1, "float", 32, "int");
  tdesc_create_reg (feature, "fop", 39, 1, "float", 32, "int");

  feature = tdesc_create_feature (result, "org.gnu.gdb.i386.k1om");
  field_type = tdesc_named_type (feature, "ieee_single");
  tdesc_create_vector (feature, "v16f", field_type, 16);

  field_type = tdesc_named_type (feature, "ieee_double");
  tdesc_create_vector (feature, "v8d", field_type, 8);

  field_type = tdesc_named_type (feature, "int8");
  tdesc_create_vector (feature, "v64i8", field_type, 64);

  field_type = tdesc_named_type (feature, "int16");
  tdesc_create_vector (feature, "v32i16", field_type, 32);

  field_type = tdesc_named_type (feature, "int32");
  tdesc_create_vector (feature, "v16i32", field_type, 16);

  field_type = tdesc_named_type (feature, "int64");
  tdesc_create_vector (feature, "v8i64", field_type, 8);

  field_type = tdesc_named_type (feature, "uint128");
  tdesc_create_vector (feature, "v4ui128", field_type, 4);

  type = tdesc_create_union (feature, "vec512");
  field_type = tdesc_named_type (feature, "v16f");
  tdesc_add_field (type, "v16_float", field_type);
  field_type = tdesc_named_type (feature, "v8d");
  tdesc_add_field (type, "v8_double", field_type);
  field_type = tdesc_named_type (feature, "v64i8");
  tdesc_add_field (type, "v64_int8", field_type);
  field_type = tdesc_named_type (feature, "v32i16");
  tdesc_add_field (type, "v32_int16", field_type);
  field_type = tdesc_named_type (feature, "v16i32");
  tdesc_add_field (type, "v16_int32", field_type);
  field_type = tdesc_named_type (feature, "v8i64");
  tdesc_add_field (type, "v8_int64", field_type);
  field_type = tdesc_named_type (feature, "v4ui128");
  tdesc_add_field (type, "v4_uint128", field_type);

  field_type = tdesc_create_flags (feature, "k1om_mxcsr", 4);
  tdesc_add_flag (field_type, 0, "IE");
  tdesc_add_flag (field_type, 1, "DE");
  tdesc_add_flag (field_type, 2, "ZE");
  tdesc_add_flag (field_type, 3, "OE");
  tdesc_add_flag (field_type, 4, "UE");
  tdesc_add_flag (field_type, 5, "PE");
  tdesc_add_flag (field_type, 6, "DAZ");
  tdesc_add_flag (field_type, 13, "RC");
  tdesc_add_flag (field_type, 15, "FZ");
  tdesc_add_flag (field_type, 20, "DDO");
  tdesc_add_flag (field_type, 21, "DUE");

  tdesc_create_reg (feature, "k0", 40, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k1", 41, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k2", 42, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k3", 43, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k4", 44, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k5", 45, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k6", 46, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "k7", 47, 1, NULL, 16, "uint16");
  tdesc_create_reg (feature, "zmm0", 48, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm1", 49, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm2", 50, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm3", 51, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm4", 52, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm5", 53, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm6", 54, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm7", 55, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm8", 56, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm9", 57, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm10", 58, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm11", 59, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm12", 60, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm13", 61, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm14", 62, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm15", 63, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm16", 64, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm17", 65, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm18", 66, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm19", 67, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm20", 68, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm21", 69, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm22", 70, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm23", 71, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm24", 72, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm25", 73, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm26", 74, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm27", 75, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm28", 76, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm29", 77, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm30", 78, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "zmm31", 79, 1, NULL, 512, "vec512");
  tdesc_create_reg (feature, "mxcsr", 80, 1, "vector", 32, "k1om_mxcsr");

  tdesc_k1om = result;
}
