#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <nss.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <dirent.h>

static char* default_tld = ".dev";
static char* default_services = "files dns";

static char* get_tld()
{
    char* configured_tld = getenv("TLD_MAPPER_TLD");
    return configured_tld ? configured_tld : default_tld;
}

char* get_services()
{
    char* configured_services = getenv("TLD_MAPPER_SERVICES");
    return configured_services ? configured_services : default_services;
}

void __attribute__((constructor)) nss_tld_mapper_init()
{
    char* services = get_services();
    char* service_line = malloc((strlen(services) + strlen("tld_mapper") + 2) * sizeof(char));
    sprintf(service_line, "%s %s", services, "tld_mapper");
    __nss_configure_lookup("hosts", service_line);
    free(service_line);
}

static int _fill_hostent (const char *name, int af, struct hostent *result)
{
    result->h_name = malloc((strlen(name) + 1) * sizeof(char));
    strcpy(result->h_name, name);

    result->h_aliases = malloc(sizeof(char *));
    *result->h_aliases = NULL;

    switch (af)
        {
        case AF_INET:
            result->h_addrtype = AF_INET;
            result->h_length = INADDRSZ;
            break;
        case AF_INET6:
            result->h_addrtype = AF_INET6;
            result->h_length = IN6ADDRSZ;
        }

    result->h_addr_list = malloc(sizeof(char *) * 2);
    *result->h_addr_list = malloc(sizeof(in_addr_t));
    in_addr_t addr = inet_addr("127.0.0.1");
    memcpy(*result->h_addr_list, &addr, sizeof(in_addr_t));
    *(result->h_addr_list + 1) = NULL;

    return 0;
}

enum nss_status
_nss_tld_mapper_gethostbyname2_r (const char *name, int af, struct hostent *result,
                                  char *buffer, size_t buflen, int *errnop,
                                  int *h_errnop)
{
    printf("Called our resolver with name %s\n", name);
    char* tld = get_tld();
    char* tld_part = strstr(name, tld);
    if(tld_part != NULL && strlen(tld_part) == strlen(tld))
        {
            _fill_hostent(name, af, result);
            return NSS_STATUS_SUCCESS;
        }
    else
        {
            return NSS_STATUS_NOTFOUND;
        }
}

enum nss_status
_nss_tld_mapper_gethostbyname_r (const char *name, struct hostent *result,
              char *buffer, size_t buflen, int *errnop,
              int *h_errnop)
{
  return _nss_tld_mapper_gethostbyname2_r(name, AF_INET, result, buffer, buflen, errnop, h_errnop);
}
