// #include <inttypes.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>

// #include <mbedtls/error.h>
// #include <mbedtls/entropy.h>
// #include <mbedtls/ctr_drbg.h>
// #include <mbedtls/net_sockets.h>
// #include <mbedtls/ssl.h>

#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <sys/random.h>

#include <applibs/log.h>
#include <applibs/networking.h>
#include <applibs/storage.h>
#include "mbedtls_socket.h"


static int entropy_pluton_source(void* data, unsigned char* output,
	size_t len, size_t* olen);


#if !defined(MBEDTLS_NET_POLL_READ)
/* compat for older mbedtls */
#define	MBEDTLS_NET_POLL_READ	1
#define	MBEDTLS_NET_POLL_WRITE	1

int
mbedtls_net_poll(mbedtls_net_context * ctx, uint32_t rw, uint32_t timeout)
{
	/* XXX this is not ideal but good enough for an example */
	usleep(300);
	return 1;
}
#endif

// struct mbedtls_context {
//     mbedtls_net_context net_ctx;
//     mbedtls_ssl_context ssl_ctx;
//     mbedtls_ssl_config ssl_conf;
//     mbedtls_x509_crt ca_crt;
//     mbedtls_entropy_context entropy;
//     mbedtls_ctr_drbg_context ctr_drbg;
// };

void failed(const char *fn, int rv) {
    char buf[100];
    mbedtls_strerror(rv, buf, sizeof(buf));
    Log_Debug("%s failed with %x (%s)\n", fn, -rv, buf);
    // exit(1);
}

void cert_verify_failed(uint32_t rv) {
    char buf[512];
    mbedtls_x509_crt_verify_info(buf, sizeof(buf), "\t", rv);
    Log_Debug("Certificate verification failed (%0" PRIx32 ")\n%s\n", rv, buf);
    // exit(1);
}

/*
    A template for opening a non-blocking mbed TLS connection.
*/
void mbedtls_open_nb_socket(struct mbedtls_context *ctx,
                    const char *hostname,
                    const char *port,
                    const char *ca_file) {
    const unsigned char *additional = (const unsigned char *)"MQTT-C";
    size_t additional_len = 6;
    int rv;
    int ret;
    const char* pers = "azure sphere SSL client";
    mbedtls_net_context *net_ctx = &ctx->net_ctx;
    mbedtls_ssl_context *ssl_ctx = &ctx->ssl_ctx;
    mbedtls_ssl_config *ssl_conf = &ctx->ssl_conf;
    mbedtls_x509_crt *ca_crt = &ctx->ca_crt;
    mbedtls_entropy_context *entropy = &ctx->entropy;
    mbedtls_ctr_drbg_context *ctr_drbg = &ctx->ctr_drbg;

    mbedtls_entropy_init(entropy);
    mbedtls_ctr_drbg_init(ctr_drbg);

#if 1
	mbedtls_entropy_add_source(entropy, entropy_pluton_source, NULL, MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);
	if ((ret = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy,
		(const unsigned char*)pers,
		strlen(pers))) != 0)
	{
		Log_Debug(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
		// goto exit;
	}

	Log_Debug(" ok\n");
#else
    rv = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy,
                               additional, additional_len);
    if (rv != 0) {
        failed("mbedtls_ctr_drbg_seed", rv);
    }
#endif

    mbedtls_x509_crt_init(ca_crt);

#if 1
	/*
	 * 0. Initialize certificates
	 */
	Log_Debug("  . Loading the CA root certificate ...");
	fflush(stdout);

	// ret = Storage_OpenFileInImagePackage("GlobalSignRootCA.crt");
	// ret = Storage_OpenFileInImagePackage("GlobalSignRootCA_new.crt");
	ret = Storage_OpenFileInImagePackage(ca_file);
	if (ret < 0) {
		Log_Debug(" failed\n  !  Storage_OpenFileInImagePackage fail\n\n");
		// goto exit;
	}

	int cert_fd = ret;

	char *p_cert_file = malloc(8192); // certificate will not exceed 8KB
	if (p_cert_file == NULL) {
		Log_Debug(" failed\n  !  malloc fail\n\n");
		close(cert_fd);
		// goto exit;
	}
	ret = read(cert_fd, p_cert_file, 8192);
	if (ret < 0) {
		Log_Debug(" failed\n  !  read fail\n\n");
		close(cert_fd);
		free(p_cert_file);
		// goto exit;
	}

	ret = mbedtls_x509_crt_parse_der(ca_crt, p_cert_file, ret);
	if (ret < 0) {
		Log_Debug(" failed\n  !  mbedtls_x509_crt_parse_der returned -0x%x\n\n", -ret);
		close(cert_fd);
		free(p_cert_file);
		// goto exit;
	}

	free(p_cert_file);
	close(cert_fd);

	Log_Debug(" ok (%d skipped)\n", ret);
#else
    rv = mbedtls_x509_crt_parse_file(ca_crt, ca_file);
    if (rv != 0) {
        failed("mbedtls_x509_crt_parse_file", rv);
    }
#endif

    mbedtls_ssl_config_init(ssl_conf);
    rv = mbedtls_ssl_config_defaults(ssl_conf,  MBEDTLS_SSL_IS_CLIENT,
                                     MBEDTLS_SSL_TRANSPORT_STREAM,
                                     MBEDTLS_SSL_PRESET_DEFAULT);
    if (rv != 0) {
        failed("mbedtls_ssl_config_defaults", rv);
    }
    mbedtls_ssl_conf_ca_chain(ssl_conf, ca_crt, NULL);
    mbedtls_ssl_conf_authmode(ssl_conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_rng(ssl_conf, mbedtls_ctr_drbg_random, ctr_drbg);

    mbedtls_net_init(net_ctx);
    rv = mbedtls_net_connect(net_ctx, hostname, port, MBEDTLS_NET_PROTO_TCP);
    if (rv != 0) {
        failed("mbedtls_net_connect", rv);
    }
    rv = mbedtls_net_set_nonblock(net_ctx);
    if (rv != 0) {
        failed("mbedtls_net_set_nonblock", rv);
    }

    mbedtls_ssl_init(ssl_ctx);
    rv = mbedtls_ssl_setup(ssl_ctx, ssl_conf);
    if (rv != 0) {
        failed("mbedtls_ssl_setup", rv);
    }
    rv = mbedtls_ssl_set_hostname(ssl_ctx, hostname);
    // rv = mbedtls_ssl_set_hostname(ssl_ctx, "192.168.45.197");
    if (rv != 0) {
        failed("mbedtls_ssl_set_hostname", rv);
    }
    mbedtls_ssl_set_bio(ssl_ctx, net_ctx,
                        mbedtls_net_send, mbedtls_net_recv, NULL);

    Log_Debug("  . Performing the SSL/TLS handshake...");

    for (;;) {
        rv = mbedtls_ssl_handshake(ssl_ctx);
        uint32_t want = 0;
        if (rv == MBEDTLS_ERR_SSL_WANT_READ) {
            want |= MBEDTLS_NET_POLL_READ;
        } else if (rv == MBEDTLS_ERR_SSL_WANT_WRITE) {
            want |= MBEDTLS_NET_POLL_WRITE;
        } else {
            break;
        }
        rv = mbedtls_net_poll(net_ctx, want, -1);
        if (rv < 0) {
            failed("mbedtls_net_poll", rv);
        }
    }
    if (rv != 0) {
        failed("mbedtls_ssl_handshake", rv);
    }
    uint32_t result = mbedtls_ssl_get_verify_result(ssl_ctx);
    if (result != 0) {
        if (result == (uint32_t)-1) {
            failed("mbedtls_ssl_get_verify_result", result);
        } else {
            cert_verify_failed(result);
        }
    }
}


static int entropy_pluton_source(void* data, unsigned char* output,
	size_t len, size_t* olen)
{
	ssize_t bytes_copied;
	((void)data);

	// getrandom is added after 19.05 to levearge pluton's TRNG
	bytes_copied = getrandom(output, len, 0);
	if (bytes_copied >= 0) {
		*olen = bytes_copied;
		return 0;
	} else {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}
}