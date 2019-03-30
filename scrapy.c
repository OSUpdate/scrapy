#include "scrapy.h"

void crawling(char *header, char* body) {
        int i = 0, status, fd;
	int port = 80;
        char req[1024], *type;
        char *rheader, *rbody, *name;
        char host[100], path[512], *message;
        char *response;

        if (strstr(body, "<a href=\"") != NULL) {

                //printf("%s",test);
                getLink(body);
                for (i = 0; i < pindex; i++) {
                        memset(host, 0, sizeof(host));
                        memset(path, 0, sizeof(path));
                        memset(header, 0, sizeof(header));
                        //memset(message, 0, sizeof(message));
                        //printf("current url : %s\n",parray[i]);
                        if (!parseUrl(parray[i], host, path, &port)) {
                                perror("\nurl error");
				free(parray[i]);
                        }
                        else {
                                printf("\nurl : %s host : %s path : %s\n", parray[i], host, path);

                                sprintf(req, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);

                                message = getResponse(req, host,port);
                                if ((response = strstr(message, "\r\n\r\n")) != NULL) {
                                        rheader = getRheader(message, response);
                                        rbody = getBody(response);
                                        status = getStatus(rheader);
                                        type = getType(rheader);
                                        printf("status : %d\ntype : %s\n", status, type);
                                        if (strstr(type, "html"))
                                        {
                                                name = getName(path);
                                                printf("file name : %s\n", name);
                                                fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                                if (fd == -1) {
                                                        perror("file open error");
                                                        exit(1);
                                                }
                                                write(fd, body, strlen(body));
                                                close(fd);
						free(name);
                                        }
					free(type);
                                }
                        }
		free(message);
		free(rbody);
		free(rheader);
		free(type);

                }
                for (i = 0; i < pindex; i++) {
                        free(parray[i]);
                }
        }
}



int main(int argc, char *argv[]) {

        int fd, opt;
        int i = 0;
        int mlen, port = 0;
        int status;

        char ch;

        char host[100], path[512];
        char header[1024];
        char url[200];

        char *message, *type, *name;
        char *response, *rheader, *body;
        char *test, *location;

        pindex = 0;
        get = "GET";
        post = "POST";
        extern char *optarg;
        extern int optind;
        memset(url, 0, sizeof(url));
        //parseUrl. ... url... ..
        strcpy(url, argv[argc - 1]);
        if (!parseUrl(url, host, path, &port)) {
                perror("url error");
                return 0;
        }
        if(argc == 1){

        }
        if (argc == 2) {

                sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);

                message = getResponse(header, host, port);
                if(response = strstr(message, "\r\n\r\n")) {
                        rheader = getRheader(message, response);
                        body = getBody(response);
                        status = getStatus(rheader);
                        type = getType(rheader);

                        printf("\ntype : %s\nstatus : %d\n", type, status);
                        printf("\n\n%s\n\n",rheader);
                        printf("%s\n\n", body);
			free(type);
                }
        }

        //printf("url : %s host: %s optarg : %s\n", url, host, optarg);
        while ((opt = getopt(argc, argv, "h:po:a:lsd:L")) != -1)// getopt. .... .. ..
        {
                switch (opt)
                {
                case 'L':

			sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);
			message = getResponse(header, host, port);
                        if(response = strstr(message, "\r\n\r\n")) {
                            
				rheader = getRheader(message, response);
				status = getStatus(rheader);
                            	type = getType(rheader);

                            	printf("\ntype : %s\nstatus : %d\n", type, status);
                            	printf("\n\n%s\n\n",rheader);

				if (status > 300 && status < 400) {
                            		location = getLocation(rheader);
                            		printf("\nlocation : %s\n", location);
                            		memset(host,0,sizeof(host));
                            		memset(header,0,sizeof(header));
                            		memset(path,0,sizeof(path));
                                
                            		free(message);
                            		free(body);
					free(type);
                            		free(rheader);
					
					sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);

                            		parseUrl(location,host,path, &port);
                            		message = getResponse(header,host, port);
                            		if(response = strstr(message, "\r\n\r\n")) {
                                		rheader = getRheader(message, response);
                                		body = getBody(response);
                                		status = getStatus(rheader);
                                		type = getType(rheader);

                                		printf("\ntype : %s\nstatus : %d\n", type, status);
                                		printf("\n\n%s\n\n",rheader);
                                		printf("%s\n\n", body);
						free(location);
						free(type);
                            		}
                        	}
			}
                        break;
                case 'l':
                        sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);

                        message = getResponse(header, host, port);
                        if(response = strstr(message, "\r\n\r\n")) {
                            	rheader = getRheader(message, response);
                            	status = getStatus(rheader);
                            	type = getType(rheader);

                            	printf("\ntype : %s\nstatus : %d\n", type, status);
                            	printf("\n\n%s\n\n",rheader);
				free(type);
                        }
                        break;
                case 's':
                    sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", post, path, host);

                    message = getResponse(header, host, port);
                    if(response = strstr(message, "\r\n\r\n")) {
                        rheader = getRheader(message, response);
                        status = getStatus(rheader);
                        type = getType(rheader);

                        printf("\ntype : %s\nstatus : %d\n", type, status);
			free(type);
                    }
                    break;
                case 'h':
                        //.. ... ... ..
                    if (optarg)
                    {
                        sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n", get, path, host);
                        strcat(header, strcat(optarg, "\r\n"));//... ..
                        strcat(header, "\r\n");
                        message = getResponse(header, host, port);
                        if(response = strstr(message, "\r\n\r\n")) {
                           	rheader = getRheader(message, response);
                            	body = getBody(response);
                            	status = getStatus(rheader);
                            	type = getType(rheader);

                            	printf("\ntype : %s\nstatus : %d\n", type, status);
                            	printf("\n\n%s\n\n", rheader);
                            	printf("%s\n\n", body);
				free(type);
                        }

                    }
                    break;
                case 'p':
                        //http body... .... .... ... ..
                    sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", post, path, host);

                    message = getResponse(header, host, port);
                    if(response = strstr(message, "\r\n\r\n")) {
                        rheader = getRheader(message, response);
                        body = getBody(response);
                        status = getStatus(rheader);
                        type = getType(rheader);

                        printf("status : %d\ntype : %s\n", status, type);
                        printf("\n\n%s\n\n", rheader);
                        printf("%s\n\n", body);
			free(type);
                    }

                    break;
                case 'd':
                    sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", post, path, host);

                    if (optarg)
                        strcat(header, optarg); // .... ..
                    message = getResponse(header, host,port);
                    if(response = strstr(message, "\r\n\r\n")) {
                        rheader = getRheader(message, response);
                        body = getBody(response);
                        status = getStatus(rheader);
                        type = getType(rheader);

                        printf("status : %d\ntype : %s\n", status, type);
                        printf("\n\n%s\n\n", rheader);
                        printf("%s\n\n", body);
			free(type);
                    }
                    break;
                case 'o':
                    if (optarg)
                    {
                        sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);
                        message = getResponse(header, host, port);
                            if(response = strstr(message, "\r\n\r\n")) {
                                rheader = getRheader(message, response);
                                body = getBody(response);
                                type = getType(rheader);
                                status = getStatus(rheader);

                                printf("\nurl : %s\n", url);
                                printf("status : %d\ntype : %s\n", status, type);
				name = getName(path);
				printf("file name : %s\n",name);
                                fd = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                                if (fd == -1) {
                                    perror("file open error");
                                    exit(1);
                                }
                            	write(fd, message, strlen(message));
                            	close(fd);
				free(type);
				free(name);
                        }
                    }
                    else {
                        perror("please enter a file name");
                        exit(1);
                    }
                    break;
                case 'a':
                    if (optarg) {
                        sprintf(header, "%s %s HTTP/1.1\r\nHost: %s\r\nContent-type: application/x-www-form-urlencodede\r\n\r\n", get, path, host);
                        message = getResponse(header, host, port);
                        if (response = strstr(message, "\r\n\r\n")) {
                            body = getBody(response);
                            rheader = getRheader(message, response);
                            if (strstr(body, "<a href=\"") != NULL) {
                                crawling(rheader, body);
                            }
                        }
                    }
                    else {
                        perror("please enter url");
                        exit(1);
                    }
                    break;
                }
        }
        free(body);
        free(rheader);
        if (mUrl != NULL)
                free(mUrl);

        free(message);
        return 0;
}

