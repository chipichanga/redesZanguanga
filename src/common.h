#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <ctype.h>

#define PUERTO 3490     /* puerto en el servidor */
#define MAXDATASIZE 296 /* Max buffer size */
#define DF 0x0A
/* OP codes */
#define PETICION_ID 0x0001
#define PETICION_REG 0x0002
#define CONCESION_REG_REC 0x0004
#define PETICION_REC 0x0005
#define LOCALIZADOR_REC 0x0006
#define ERROR 0xFFFF


/* formato del mensaje de peticion del slr */
struct common_slr_peticion
{
        short int op;                     /* codigo de operacion */
        char d1 [9];     /* DNI 1 */
        char d2 [9];     /* DNI 2 */
        char ip [15];    /* 155.54.204.49\0\0 */
        char ts [255];   /* TS */
} __attribute__((packed));

/* formato del mensaje de concesion del slr */
struct common_slr_concesion
{
        short int op;                        /* codigo de operacion */
        long int id;                        /* identificador */
        char men [255];   /* TS */
} __attribute__((packed));
