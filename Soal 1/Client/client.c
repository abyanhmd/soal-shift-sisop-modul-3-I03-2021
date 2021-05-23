#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
char buffer[1024] = {0};
int sock;

void reglog(char choice[], char id[], char pass[], char passid[]);
void addFile(char choice[], char passid[], int sock);
void seeFile();

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char choice[1001], passid[1001];
    memset(passid, 0, sizeof(passid));
    char id[1001], pass[1001];
    
    // while(1) {
        read(sock, buffer, sizeof(buffer)); // Type one of the following command
        printf("%s", buffer);

        scanf("%s", choice);
        send(sock, choice, sizeof(choice), 0);

        if(strcmp(choice, "login") == 0 || strcmp(choice, "register") == 0)
            reglog(choice, id, pass, passid);

        else if(strcmp(choice, "add") == 0)
            addFile(choice, passid, sock);

        else if(strcmp(choice, "quit") == 0)
            exit(EXIT_SUCCESS);

        else if(strcmp(choice, "see") == 0)
            seeFile();

        else {
            read(sock, buffer, sizeof(buffer));
            printf("%s", buffer);
        }
    // }
    
    return 0;
}

void reglog(char choice[], char id[], char pass[], char passid[]) {
    char strtemp[1001];
    read(sock, strtemp, sizeof(strtemp)); // Input id and pass in one line
    printf("%s", strtemp);

    // To change the id and password into id:password format in the akun.txt
    scanf("%s %s", id, pass);
    strcpy(passid, id);
    strcat(passid, ":");
    strcat(passid, pass);
    strcat(passid, "\n");
    
    int len = strlen(passid);
    send(sock, passid, len, 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
}

void addFile(char choice[], char id[], int sock) {
    char publisher[1001], year[1001], path[1001];
    
    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
    scanf("%s", publisher);
    send(sock, publisher, sizeof(publisher), 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
    scanf("%s", year);
    send(sock, year, sizeof(year), 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
    scanf("%s", path);
    send(sock, path, sizeof(path), 0);
}

void seeFile() {
    char strtemp[1024];
    read(sock, strtemp, sizeof(strtemp));
    printf("%s", strtemp);
    printf("\n");
}
