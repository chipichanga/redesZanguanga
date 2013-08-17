/* FICHERO: slr.c */

#include "common.h"

#define PUERTO_REMOTO PUERTO /* puerto remoto en el servidor al que se envian los mensajes */

int main (int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in their_addr;  /* informacion de la direccion del servidor */
  char buf[MAXDATASIZE];          /* buffer de recepcion */
  struct common_slr_peticion peticion;     /* mensaje de operacion enviado */
  struct common_slr_concesion *concesion;    /* mensaje de concesion recibido */
  int numbytes;                   /* numero de bytes recibidos o enviados */
  size_t sin_size;
  int len;

  /* obtiene parametros */
  if (argc != 5)
  {
    fprintf (stderr, "uso: cliente -q PSPR -l SLR\n");
    exit (1);
  }

  /* crea el socket */
  if ((sockfd = socket (AF_INET, SOCK_DGRAM, 0)) == -1)
  {
    perror ("socket");
    exit (1);
  }

  their_addr.sin_family = AF_INET;  /* Familia: ordenacion de bytes de la maquina */
  their_addr.sin_port =
  htons (atoi(argv[2]));    /* Puerto: ordenacion de bytes de la red */
  their_addr.sin_addr.s_addr =             /* IP: ordenacion de bytes de la red */
  inet_addr (argv[4]);
  memset (&(their_addr.sin_zero), 0, 8);  /* Pone a cero el resto de la estructura */

  /* envia mensaje de peticion al servidor */
  peticion.op = (short int) htons(PETICION_ID);   /* op */
  strcpy(peticion.d1, "23288228s");        /* Belen's dni */
  strcpy(peticion.d2, "00000010X");        /* JC1's dni */ 
  strcpy(peticion.ip, "155.54.204.49\0\0");  /* SLR's IP */
  strcpy(peticion.ts, "Peticion message");  /* data */
  peticion.ts[strlen(peticion.ts)] = DF;    /* Add delimiter */

  len = sizeof(short int) + 9 + 9 + 15 + strlen(peticion.ts);	

  if ((numbytes = sendto (sockfd, (char *) &peticion,
      len, 0,
      (void*)&their_addr,
      sizeof(struct sockaddr_in))) == -1)
  {
    perror ("send");
    exit (1);
  }
  else
  printf ("(SPR) mensaje enviado al SLR [longitud %d]\n", numbytes);

  /* espera resultado de la operacion */        
  memset (buf, '\0', MAXDATASIZE); /* Pone a cero el buffer inicialmente */
  sin_size = sizeof(struct sockaddr_in);

  if ((numbytes = recvfrom (sockfd, buf, MAXDATASIZE, 0 /* flags */,
            (void*)&their_addr, &sin_size)) == -1)
  {
    perror ("recv");
    exit (1);
  }
  printf ("(SPR) mensaje recibido del SLR [longitud %d]\n", numbytes);

  /* Preventing buffer overflow */
  if (numbytes <= MAXDATASIZE)
  {
    memset(&concesion, 0, sizeof(concesion));
    concesion = (struct common_slr_concesion *) &buf;
  }
  else
  {
    perror ("Too much data received");
    exit (1);
  }

  printf("Concesion:\n");        
  printf("\tOp: %d\n", ntohs(concesion->op));        
  printf("\tId: %d\n", ntohl(concesion->id));
  printf("\tMensaje: %s\n", concesion->men);

  /* cierra el socket */
  close (sockfd);

  return 0;
}
