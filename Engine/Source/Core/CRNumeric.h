#pragma once


#include "CRTypes.h"


static constexpr i8  min_i8  = -0x7F;
static constexpr i8  max_i8  =  0x7F;
static constexpr i16 min_i16 = -0x7FFF;
static constexpr i16 max_i16 =  0x7FFF;
static constexpr i32 min_i32 = -0x7FFFFFFF;
static constexpr i32 max_i32 =  0x7FFFFFFF;
static constexpr i64 min_i64 = -0x7FFFFFFFFFFFFFFF;
static constexpr i64 max_i64 =  0x7FFFFFFFFFFFFFFF;

static constexpr u8  min_u8  = 0;
static constexpr u8  max_u8  = 0xFF;
static constexpr u16 min_u16 = 0;
static constexpr u16 max_u16 = 0xFFFF;
static constexpr u32 min_u32 = 0;
static constexpr u32 max_u32 = 0xFFFFFFFF;

static constexpr f32 min_f32 = -3.402823466e+38f;
static constexpr f32 max_f32 =  3.402823466e+38f;
static constexpr f64 min_f64 = -1.7976931348623158e+308;
static constexpr f64 max_f64 =  1.7976931348623158e+308;
