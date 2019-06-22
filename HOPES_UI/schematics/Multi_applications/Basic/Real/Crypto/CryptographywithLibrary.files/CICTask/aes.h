// AES Implementation by X-N2O
// Started:  15:41:35 - 18 Nov 2009
// Finished: 20:03:59 - 21 Nov 2009
// Logarithm, S-Box, and RCON tables are not hardcoded
// Instead they are generated when the program starts
// All of the code below is based from the AES specification
// You can find it at http://csrc.nist.gov/publications/fips/fips197/fips-197.pdf
// You may use this code as you wish, but do not remove this comment
// This is only a proof of concept, and should not be considered as the most efficient implementation

#ifndef __aes_h
#define __aes_h

#define AES_RPOL      0x011b // reduction polynomial (x^8 + x^4 + x^3 + x + 1)
#define AES_GEN       0x03   // gf(2^8) generator    (x^4 + 1)
#define AES_SBOX_CC   0x63   // S-Box C constant
#define AES_SBOX_MT   0x8f   // S-Box initial matrix row

#define KEY_128 (128/8)
#define KEY_192 (192/8)
#define KEY_256 (256/8)

#define aes_mul(a, b) ((a)&&(b)?g_aes_ilogt[(g_aes_logt[(a)]+g_aes_logt[(b)])%0xff]:0)
#define aes_inv(a)    ((a)?g_aes_ilogt[0xff-g_aes_logt[(a)]]:0)

extern unsigned char g_aes_logt[256], g_aes_ilogt[256];
extern unsigned char g_aes_sbox[256], g_aes_isbox[256];

typedef struct {
	unsigned char state[4][4];
	int kcol;
	unsigned int rounds;
	unsigned int *keysched;
} aes_ctx_t;

void aes_init(void);
aes_ctx_t *aes_alloc_ctx(unsigned char *key, unsigned int keyLen);
unsigned int aes_subword(unsigned int w);
unsigned int aes_rotword(unsigned int w);
void aes_keyexpansion(aes_ctx_t *ctx);
unsigned char aes_mul_manual(unsigned char a, unsigned char b); // use aes_mul instead

void aes_subbytes(aes_ctx_t *ctx);
void aes_shiftrows(aes_ctx_t *ctx);
void aes_mixcolumns(aes_ctx_t *ctx);
void aes_addroundkey(aes_ctx_t *ctx, int round);
void aes_encrypt(aes_ctx_t *ctx, unsigned char input[16], unsigned char output[16]);

void aes_invsubbytes(aes_ctx_t *ctx);
void aes_invshiftrows(aes_ctx_t *ctx);
void aes_invmixcolumns(aes_ctx_t *ctx);
void aes_decrypt(aes_ctx_t *ctx, unsigned char input[16], unsigned char output[16]);

void aes_free_ctx(aes_ctx_t *ctx);

#endif //__aes_h
