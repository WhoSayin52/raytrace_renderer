#ifndef CORE_HPP
#define CORE_HPP

#include <cstdint>
#include <cstddef>

// types
using s32 = int32_t;
using s64 = int64_t;

using u32 = uint32_t;
using u64 = uint64_t;

using wchar = wchar_t;

using bool32 = s32;

// math
#pragma warning(push)
#pragma warning(disable: 4201)
struct Vector3 {
        union {
                struct { int x, y, z; };
                struct { int r, g, b; };
                int data[3];
        };
};

struct Vector4 {
        union {
                struct { int x, y, z, w; };
                struct { int r, g, b, a; };
                int data[4];
        };
};
#pragma warning(pop)

// memory
constexpr int bits_per_byte = 8;

constexpr u64 kilobytes(u64 bytes) { return bytes * 1024LL; }
constexpr u64 megabytes(u64 bytes) { return kilobytes(bytes) * 1024LL; }
constexpr u64 gigabytes(u64 bytes) { return megabytes(bytes) * 1024LL; }
constexpr u64 terabytes(u64 bytes) { return gigabytes(bytes) * 1024LL; }

#define ALIGN4(bytes) ((bytes) + 3) & ~3;

// debugging
#if !defined(NDEBUG)
#define ASSERT(x)               \
        if(!(x)){               \
                *(int*)0 = 0;   \
        }
#else
#define ASSERT(x)
#endif // !defined(NDEBUG)

#endif // CORE_HPP
