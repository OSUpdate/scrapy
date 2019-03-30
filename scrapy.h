#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#define BUF 200000

char *parray[20];
int pindex;
char *mUrl;
char *get;
char *post;

void timeout(int sig);
void crawling(char *header, char* body);
char *getName(char *path);
void getLink(char *body);
char *getBody(char *response);
char *getRheader(char *message, char *response);
char *getResponse(char *header, char *host, int port);
int getStatus(char *header);
char *getLocation(char *header);
char *getType(char *header);
int parseUrl(char* url, char *serverName, char *path, int *port);
