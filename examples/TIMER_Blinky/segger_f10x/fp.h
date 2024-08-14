#ifndef _FIXEDPTC_H_
#define _FIXEDPTC_H_


#define fp_BITS    32

typedef int32_t fp;
typedef int64_t fpd;
typedef uint32_t fpu;
typedef uint64_t fpud;

#define fp_WBITS   24

#define fp_FBITS   (fp_BITS - fp_WBITS)
#define fp_FMASK   (((fp)1 << fp_FBITS) - 1)

#define fp_rconst(R) ((fp)((R) * (((fpd)1 << fp_FBITS) \
    + ((R) >= 0 ? 0.5 : -0.5))))
#define fp_fromint(I) ((fpd)(I) << fp_FBITS)
#define fp_toint(F) ((F) >> fp_FBITS)
#define fp_add(A,B) ((A) + (B))
#define fp_sub(A,B) ((A) - (B))
#define fp_xmul(A,B)                       \
    ((fp)(((fpd)(A) * (fpd)(B)) >> fp_FBITS))
#define fp_xdiv(A,B)                       \
    ((fp)(((fpd)(A) << fp_FBITS) / (fpd)(B)))
#define fp_fracpart(A) ((fp)(A) & fp_FMASK)


#define fp_abs(A) ((A) < 0 ? -(A) : (A))


static inline fp
fp_mul(fp A, fp B)
{
    return (((fpd)A * (fpd)B) >> fp_FBITS);
}


static inline fp
fp_div(fp A, fp B)
{
    return (((fpd)A << fp_FBITS) / (fpd)B);
}


static inline void
fp_str(fp A, char *str)
{
    int ndec = 0, slen = 0;
    char tmp[12] = {0};
    fpud fr, ip;
    const fpud one = (fpud)1 << fp_BITS;
    const fpud mask = one - 1;
    int max_dec = 2;


    if (A < 0) {
        str[slen++] = '-';
        A *= -1;
    }

    ip = fp_toint(A);
    do {
        tmp[ndec++] = '0' + ip % 10;
        ip /= 10;
    } while (ip != 0);

    while (ndec > 0)
        str[slen++] = tmp[--ndec];
    str[slen++] = '.';

    fr = (fp_fracpart(A) << fp_WBITS) & mask;
    do {
        fr = (fr & mask) * 10;

        str[slen++] = '0' + (fr >> fp_BITS) % 10;
        ndec++;
    } while (fr != 0 && ndec < max_dec);

    if (ndec > 1 && str[slen-1] == '0')
        str[slen-1] = '\0'; /* cut off trailing 0 */
    else
        str[slen] = '\0';
}

static inline char*
fp_cstr(const fp A)
{
    static char str[25];

    fp_str(A, str);
    return (str);
}


#endif
