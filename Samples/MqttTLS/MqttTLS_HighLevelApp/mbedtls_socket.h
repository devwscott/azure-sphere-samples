#pragma once

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <mbedtls/error.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>

struct mbedtls_context {
    mbedtls_net_context net_ctx;
    mbedtls_ssl_context ssl_ctx;
    mbedtls_ssl_config ssl_conf;
    mbedtls_x509_crt ca_crt;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
};

void mbedtls_open_nb_socket(struct mbedtls_context *ctx,
                    const char *hostname,
                    const char *port,
                    const char *ca_file);