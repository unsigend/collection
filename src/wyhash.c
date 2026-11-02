/* collection - A high-performance C data structure library
 * Copyright (C) 2026-2027 Yixiang Qiu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <hash.h>
#include <string.h>

/*
 * Derived from wyhash 4.3, Wang Yi <godspeed_china@yeah.net>
 * https://github.com/wangyi-fudan/wyhash
 *
 * Unlicense. 64-bit hash with the default secret only.
 */

static const uint64_t wyp[4] = {
    0x2d358dccaa6c78a5ull,
    0x8bb84b93962eacc9ull,
    0x4b33a62ed433d4a3ull,
    0x4d5a2da51de1aa47ull,
};

static void wymum(uint64_t *a, uint64_t *b)
{
#if defined(__SIZEOF_INT128__)
    __uint128_t r = *a;

    r *= *b;
    *a = (uint64_t)r;
    *b = (uint64_t)(r >> 64);
#elif defined(_MSC_VER) && defined(_M_X64)
    *a = _umul128(*a, *b, b);
#else
    uint64_t ha = *a >> 32, hb = *b >> 32;
    uint64_t la = (uint32_t)*a, lb = (uint32_t)*b;
    uint64_t rh = ha * hb, rm0 = ha * lb, rm1 = hb * la, rl = la * lb;
    uint64_t t = rl + (rm0 << 32), c = t < rl;
    uint64_t lo = t + (rm1 << 32);

    c += lo < t;
    *a = lo;
    *b = rh + (rm0 >> 32) + (rm1 >> 32) + c;
#endif
}

static uint64_t wymix(uint64_t a, uint64_t b)
{
    wymum(&a, &b);
    return a ^ b;
}

static uint64_t wyr8(const uint8_t *p)
{
    uint64_t v;

    memcpy(&v, p, 8);
    return v;
}

static uint64_t wyr4(const uint8_t *p)
{
    uint32_t v;

    memcpy(&v, p, 4);
    return v;
}

static uint64_t wyr3(const uint8_t *p, size_t k)
{
    return ((uint64_t)p[0] << 16) | ((uint64_t)p[k >> 1] << 8) | p[k - 1];
}

uint64_t hash_wy(const void *data, size_t len, uint64_t seed)
{
    const uint8_t *p = data;
    uint64_t a, b;

    seed ^= wymix(seed ^ wyp[0], wyp[1]);
    if (len <= 16) {
        if (len >= 4) {
            a = (wyr4(p) << 32) | wyr4(p + ((len >> 3) << 2));
            b = (wyr4(p + len - 4) << 32) |
                wyr4(p + len - 4 - ((len >> 3) << 2));
        } else if (len > 0) {
            a = wyr3(p, len);
            b = 0;
        } else
            a = b = 0;
    } else {
        size_t i = len;

        if (i >= 48) {
            uint64_t see1 = seed, see2 = seed;

            do {
                seed = wymix(wyr8(p) ^ wyp[1], wyr8(p + 8) ^ seed);
                see1 = wymix(wyr8(p + 16) ^ wyp[2], wyr8(p + 24) ^ see1);
                see2 = wymix(wyr8(p + 32) ^ wyp[3], wyr8(p + 40) ^ see2);
                p += 48;
                i -= 48;
            } while (i >= 48);
            seed ^= see1 ^ see2;
        }
        while (i > 16) {
            seed = wymix(wyr8(p) ^ wyp[1], wyr8(p + 8) ^ seed);
            i -= 16;
            p += 16;
        }
        a = wyr8(p + i - 16);
        b = wyr8(p + i - 8);
    }
    a ^= wyp[1];
    b ^= seed;
    wymum(&a, &b);
    return wymix(a ^ wyp[0] ^ len, b ^ wyp[1]);
}
