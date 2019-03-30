#include "scrapy.h"


void timeout(int sig){
	if(sig == SIGALRM){
		puts("\n");
	}
}
char *getName(char *path) {
	int i = 0;
	int size = strlen(path);
	char *name;
	if (strlen(path) == 1) {
		name = (char*)malloc(sizeof(char)*strlen("index.html"));
		memset(name,0,sizeof(char)* strlen("index.html"));
		strcpy(name, "index.html");
	}
	else {
		if(path[size - 1] == '/'){
			while(path[size-i-2] != '/'){
				i++;
			}
			name = (char*)malloc(sizeof(char) * (i+6));
			memset(name,0,sizeof(char)*(i+6));
			strncpy(name,(path + size -i-1), i);
			strcat(name,".html");
		}
		else{		
			while (path[size - i] != '/') {
				i++;
			}
			name = (char*)malloc(sizeof(char)*(i+6));
			
			memset(name,0,sizeof(char)*(i+6));
			strncpy(name, (path + size - i + 1), i);
			strcat(name, ".html");
		}
	}
	return name;

}
void getLink(char *body) {
	int i = 0;
	char *link = body;
	while ((link = strstr(link, "<a href=\""))) {
		if (pindex < 20) {
			while (link[i + 9] != '"') {
				i++;
			}

			parray[pindex] = (char*)malloc(sizeof(char)*i);
			strncpy(parray[pindex], link + 9, i);
		}
		else
			break;

		pindex++;
		link++;
		i = 0;
	}
}
char *getBody(char *response) {
	int i = 0;
	int size = strlen(response);
	char *body;
	while(response[i] != '<'){
		i++;
	}
	body = (char*)malloc(sizeof(char)*size);
	memset(body, 0, sizeof(char)*size);
	strncpy(body, response+i, size);
	return body;
}

char *getRheader(char *message, char *response) {
	int size = strlen(message) - strlen(response);
	char *header = (char*)malloc(sizeof(char)*size);
	memset(header, 0, sizeof(char)*size);
	strncpy(header, message, size);
	return header;
}

char *getResponse(char *header, char *host, int port)
{
	int sock;
	struct timeval tv;
	struct sockaddr_in sockinfo;
	struct hostent *test;
	int len = 0, total = BUF, received = 0;
	int j = 0;
	char *temp;
	char buf[1024];
	
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	signal(SIGALRM, timeout);
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		perror("socket() error");

	if ((test = gethostbyname(host)) == NULL)
		perror("gethostbyname() error");
	
	memset(&sockinfo, 0, sizeof(sockinfo));

	sockinfo.sin_family = AF_INET;
	sockinfo.sin_port = htons(port);
	sockinfo.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)test->h_addr_list[0]));
	

	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(struct timeval));
	if (connect(sock, (struct sockaddr*)&sockinfo, sizeof(sockinfo)) == -1)
		printf(" .. .. .. ");

	temp = (char*)malloc(sizeof(char)*BUF);
	memset(temp, 0, sizeof(char)*BUF);
	send(sock, header, strlen(header), 0);
	/*
	if(len = read(sock,response,BUF-1) != 0){

	}
	*/
	printf("port : %d\n",port);
	alarm(5);	
	do {
		len = recv(sock, buf, 1024,0);
		
			
		if (len == 0)
			break;
		
		if (len < 0){
			break;
			//perror("error");
		}	
		received += len;
		strcat(temp, buf);

	} while ((strstr(buf, "</html>") == NULL) && (strstr(buf,"</HTML>") == NULL));
	close(sock);
	return temp;
}
int getStatus(char *header) {
	char *temp = strdup(header);
	char status[3];
	strtok(temp, " ");
	strcpy(status, strtok(NULL, " "));
	free(temp);
	return atoi(status);
}
char *getLocation(char *header) {
	int i = 0;

	char *find;
	char *location;
	if (!(find = strstr(header, "Location:"))) {
		return NULL;
	}
	else {
		find = strchr(find, ' ');
		while ((find[i + 1] != '\r') && (find[i + 1] != ' ')) {
			i++;
		}
		location = (char*)malloc(sizeof(char)*strlen(find));
		memset(location,0,sizeof(location));
		strncpy(location, find + 1, i);
	}
	return location;

}
char *getType(char *header) {
	int i = 0;

	char *find;
	char *type = (char*)malloc(sizeof(char) * 200);
	memset(type, 0, sizeof(char) * 200);
	if (!(find = strstr(header, "Content-Type:"))) {
		return NULL;
	}
	else {
		find = strchr(find, ' ');
		while ((find[i + 1] != '\r') && (find[i + 1] != ' ')) {
			i++;
		}
		strncpy(type, find + 1, i);
	}
	return type;


}
int parseUrl(char* url, char *serverName, char *path, int *port)
{
	char *http = "http://";
	char *https = "https://";
	char *find;
	char temp[200];

	memset(temp, 0, sizeof(temp));
	if (!strstr(url, ".com") || !strstr(url, ".kr") || !strstr(url, ".org")) {

		if (find = strstr(url, http)) {
			strcpy(url, find + 7);
			*port = 80;
		}
		if (find = strstr(url, https)) {
			strcpy(url,find+8);
			*port = 443;
			//perror("HTTPS protocol is not supported");
			//return 0;
		}
		if (!strstr(url, "/"))
		{
			strcpy(serverName, url);
			strcpy(path, "/");
			if (mUrl == NULL) {
				mUrl = strdup(url);
			}
			if(*port == 0)
				*port = 80;
		}
		else
		{
			if(mUrl == NULL){
				mUrl = strdup(url);
			}
			if((url[0] == '/') && (mUrl != NULL)){
				strcpy(path, url);
                                strcpy(url, mUrl);
                                strcpy(serverName, mUrl);
                                return 1;
			}

			strcpy(temp, url);
			strcpy(serverName, strtok(temp, "/"));
			strcpy(path, strchr(url, '/'));
			if (mUrl == NULL) {
				mUrl = strdup(serverName);
			}
		}
		if (!strstr(serverName, ".com") && !strstr(serverName, ".kr")) {
			return 0;
		}
		if(strstr(path," ")){
			return 0;
		}
	}
	else {
		return 0;
	}
	return 1;

}


