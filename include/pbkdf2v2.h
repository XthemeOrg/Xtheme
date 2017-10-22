/*
 * Copyright (c) 2017 Atheme Development Group
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Data structures and macros for the PBKDF2v2 crypto module.
 */

#ifndef PBKDF2V2_H
#define PBKDF2V2_H

#include <openssl/evp.h>

#define PBKDF2_PRF_HMAC_SHA1        4U
#define PBKDF2_PRF_HMAC_SHA2_256    5U
#define PBKDF2_PRF_HMAC_SHA2_512    6U

#define PBKDF2_PRF_SCRAM_SHA1       44U
#define PBKDF2_PRF_SCRAM_SHA2_256   45U
#define PBKDF2_PRF_SCRAM_SHA2_512   46U     /* Not currently specified */

struct pbkdf2v2_parameters
{
	const EVP_MD    *md;                    // OpenSSL EVP(3) Digest Method
	unsigned char    cdg[EVP_MAX_MD_SIZE];  // PBKDF2 Digest (Computed)
	unsigned char    sdg[EVP_MAX_MD_SIZE];  // PBKDF2 Digest (Stored)
	unsigned char    ssk[EVP_MAX_MD_SIZE];  // SCRAM-SHA ServerKey (Stored)
	unsigned char    shk[EVP_MAX_MD_SIZE];  // SCRAM-SHA StoredKey (Stored)
	char             salt[0x2000];          // PBKDF2 Salt
	size_t           dl;                    // Digest Length
	size_t           sl;                    // Salt Length
	unsigned int     a;                     // PRF ID (one of the macros above)
	unsigned int     c;                     // PBKDF2 Iteration Count
	bool             scram;                 // Whether to use HMAC-SHA or SCRAM-SHA
};

#endif /* !PBKDF2V2_H */
