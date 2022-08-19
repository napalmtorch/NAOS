#include <ubsan.h>
#include <core/kernel.h>

#define is_aligned(value, alignment) !(value & (alignment - 1))

#ifdef __cplusplus
EXTC
{
#endif

void __ubsan_handle_out_of_bounds(ubsan_outofbounds_t* info, uintptr_t pointer)
{
    printf("%s Out of Bounds Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_type_mismatch(ubsan_typemismatch_t* info, uintptr_t pointer)
{
    if (pointer == 0) { printf("%s Type Mismatch Exception\n", DEBUG_ERROR); printf("Null pointer access\n"); }
    else if (info->alignment != 0 && is_aligned(pointer, info->alignment)) { printf("%s Type Mismatch Exception\n", DEBUG_ERROR); printf("Unaligned memory access\n"); }   
    else { return; } 
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_add_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    printf("%s Add Overflow Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_sub_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    printf("%s Sub Overflow Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_mul_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    printf("%s Mul Overflow Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_negate_overflow(ubsan_overflow_t* info, uint32_t old_val)
{
    printf("%s Negate Overflow Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_divrem_overflow(ubsan_overflow_t* info, uint32_t lhs, uint32_t rhs)
{
    printf("%s Divrem Overflow Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_missing_return(ubsan_unreachabledata_t* info)
{
    printf("%s Missing Return Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_load_invalid_value(ubsan_invalidvalue_t* info, uint32_t val)
{
    printf("%s Invalid Value Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_shift_out_of_bounds(ubsan_shift_outofbounds_t* info, uint32_t lhs, uint32_t rhs)
{
    printf("%s Shift Out of Bounds Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

void __ubsan_handle_vla_bound_not_positive(ubsan_vla_bound_data_t* info, uint32_t bound)
{
    printf("%s VLA Bound Not Positive Exception\n", DEBUG_ERROR);
    perror("LINE: %d COL: %d FILE: '%s'\n", info->location.line, info->location.column, info->location.file);
}

#ifdef __cplusplus
}
#endif