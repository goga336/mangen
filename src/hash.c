/**
 * @file hash.c
 * @brief Реализация вычисления SHA-256 хэша.
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../inc/hash.h"
#include "rfc6234/sha.h"

/**
 * @copydoc hash_file
 */
int hash_file(const char *path, const char *display_path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "mangen: %s: %s\n", path, strerror(errno));
        return -1;
    }



    SHA256Context ctx;
    if (SHA256Reset(&ctx) != shaSuccess) {
        fclose(f);
        fprintf(stderr, "mangen: %s: SHA256Reset failed\n", path);
        return -1;
    }

    uint8_t buf[8192];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), f)) > 0) {
        if (SHA256Input(&ctx, buf, (unsigned int)n) != shaSuccess) {
            fclose(f);
            fprintf(stderr, "mangen: %s: SHA256Input failed\n", path);
            return -1;
        }
    }
    if (ferror(f)) {
        fclose(f);
        fprintf(stderr, "mangen: %s: read error\n", path);
        return -1;
    }
    fclose(f);

    uint8_t digest[SHA256HashSize];
    if (SHA256Result(&ctx, digest) != shaSuccess) {
        fprintf(stderr, "mangen: %s: SHA256Result failed\n", path);
        return -1;
    }

    printf("%s : ", display_path);
    for (int i = 0; i < SHA256HashSize; i++)
        printf("%02x", digest[i]);
    printf("\n");

    return 0;
}
