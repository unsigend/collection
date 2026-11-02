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

/*
 * SipHash reference C implementation
 *
 * Copyright (c) 2012-2022 Jean-Philippe Aumasson
 * <jeanphilippe.aumasson@gmail.com>
 * Copyright (c) 2012-2014 Daniel J. Bernstein <djb@cr.yp.to>
 *
 * To the extent possible under law, the author(s) have dedicated all copyright
 * and related and neighboring rights to this software to the public domain
 * worldwide. This software is distributed without any warranty.
 *
 * You should have received a copy of the CC0 Public Domain Dedication along
 * with this software. If not, see
 * <http://creativecommons.org/publicdomain/zero/1.0/>.
 *
 * Derived from https://github.com/veorq/SipHash
 * SipHash-1-3, 64-bit output only.
 */

#include <hash.h>

#define ROTL(x, b) (((x) << (b)) | ((x) >> (64 - (b))))

#define SIPROUND                                                               \
    do {                                                                       \
        v0 += v1;                                                              \
        v1 = ROTL(v1, 13);                                                     \
        v1 ^= v0;                                                              \
        v0 = ROTL(v0, 32);                                                     \
        v2 += v3;                                                              \
        v3 = ROTL(v3, 16);                                                     \
        v3 ^= v2;                                                              \
        v0 += v3;                                                              \
        v3 = ROTL(v3, 21);                                                     \
        v3 ^= v0;                                                              \
        v2 += v1;                                                              \
        v1 = ROTL(v1, 17);                                                     \
        v1 ^= v2;                                                              \
        v2 = ROTL(v2, 32);                                                     \
    } while (0)

static uint64_t load64le(const uint8_t *p)
{
    return (uint64_t)p[0] | ((uint64_t)p[1] << 8) | ((uint64_t)p[2] << 16) |
           ((uint64_t)p[3] << 24) | ((uint64_t)p[4] << 32) |
           ((uint64_t)p[5] << 40) | ((uint64_t)p[6] << 48) |
           ((uint64_t)p[7] << 56);
}

uint64_t hash_sip(const void *data, size_t len, const uint8_t k[16])
{
    const uint8_t *in = data;
    const uint8_t *end = in + len - (len % 8);
    uint64_t v0 = 0x736f6d6570736575ull;
    uint64_t v1 = 0x646f72616e646f6dull;
    uint64_t v2 = 0x6c7967656e657261ull;
    uint64_t v3 = 0x7465646279746573ull;
    uint64_t k0 = load64le(k);
    uint64_t k1 = load64le(k + 8);
    uint64_t b = (uint64_t)len << 56;
    uint64_t m;
    unsigned left = (unsigned)(len & 7);

    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    for (; in != end; in += 8) {
        m = load64le(in);
        v3 ^= m;
        SIPROUND;
        v0 ^= m;
    }

    switch (left) {
    case 7:
        b |= (uint64_t)in[6] << 48;
        /* fall through */
    case 6:
        b |= (uint64_t)in[5] << 40;
        /* fall through */
    case 5:
        b |= (uint64_t)in[4] << 32;
        /* fall through */
    case 4:
        b |= (uint64_t)in[3] << 24;
        /* fall through */
    case 3:
        b |= (uint64_t)in[2] << 16;
        /* fall through */
    case 2:
        b |= (uint64_t)in[1] << 8;
        /* fall through */
    case 1:
        b |= (uint64_t)in[0];
        break;
    }

    v3 ^= b;
    SIPROUND;
    v0 ^= b;
    v2 ^= 0xff;
    SIPROUND;
    SIPROUND;
    SIPROUND;

    return v0 ^ v1 ^ v2 ^ v3;
}
