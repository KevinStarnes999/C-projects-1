#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "net.h"
#include "jbod.h"

/* the client socket descriptor for the connection to the server */
int cli_sd = -1;

/* attempts to read n bytes from fd; returns true on success and false on
 * failure */
static bool nread(int fd, int len, uint8_t *buf) {
  
  int n = 0;
  while(n  < len){
     int fileRead = read(fd, &buf[n], len - n);
      if(fileRead < 0){
	return false;
      }
	else
	  n += fileRead;
	    
  }
  return true;
}
/* attempts to write n bytes to fd; returns true on success and false on
 * failure */
static bool nwrite(int fd, int len, uint8_t *buf) {
  int n = 0;
  while(n < len){
     int fileWrite = write(fd, &buf[n], len - n);
      if(fileWrite < 0){
	return false;
      }
	else
	  n += fileWrite;
	    
	    }
  return true;
}

/* attempts to receive a packet from fd; returns true on success and false on
 * failure */
static bool recv_packet(int fd, uint32_t *op, uint16_t *ret, uint8_t *block) {
  uint8_t header[8];
  if(nread(fd, 8, header) == false){
    return false;
  }
  memcpy(op, header, 5);
  memcpy(ret, header + 6, 1);
  

  uint32_t len = ntohs(*(uint16_t *)header);

  if(1 <= len){
    if(nread(fd, len, block) == false){
      return false;
    }
  }
  return true;
	       }	     

/* attempts to send a packet to sd; returns true on success and false on
 * failure */
static bool send_packet(int sd, uint32_t op, uint8_t *block) {
  uint8_t send[263];
  send = ntohl(send);
  memcpy(send, &op, 6);
  memcpy(&send[6], block, 256);
  }
  int send_bytes = nwrite(sd, sizeof(send), send);
  if(send_bytes == -1) {
    return false;
  }
    else {
      return true;
    }
  free(send);
}





/* attempts to connect to server and set the global cli_sd variable to the
 * socket; returns true if successful and false if not. */
bool jbod_connect(const char *ip, uint16_t port) {
  struct sockaddr_in v4;
  int socket_fd;
  socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  cli_sd = socket_fd;
  if(inet_aton(ip, &(v4.sin_addr))== -1){
    return false;
  }
  v4.sin_family = AF_INET;
  v4.sin_port = htons(port);
  if(connect(socket_fd, (const struct sockaddr *)&v4, sizeof(v4)) == -1){
    return false;
  }
  return true;
  
}

/* disconnects from the server and resets cli_sd */
void jbod_disconnect(void) {
  
  close(cli_sd);
  cli_sd = -1;
}

/* sends the JBOD operation to the server and receives and processes the
 * response. */
int jbod_client_operation(uint32_t op, uint8_t *block) {
  if(send_packet(cli_sd, op, block) == false) {
    return -1;
  }
  uint16_t ret = -1;

  if(recv_packet(cli_sd, &op, &ret, block) == false){
     return -1;
  }
  return 0;
}
