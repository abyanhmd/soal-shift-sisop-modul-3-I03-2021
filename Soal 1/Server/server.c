#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define PORT 8080
bool isLogin = false;
char strtemp[1001];

void login(char choice[], char passid[], int new_socket);
void addFile(char passid[], int new_socket);
void seeFile(int new_socket);

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char choice[1001], passid[1001];
    memset(passid, 0, sizeof(passid));
    // while(1) {
        strcpy(strtemp, "\nPlease type one of the following command\n1) register\n2) login\n3) quit\n= ");
        send(new_socket, strtemp, sizeof(strtemp), 0);
        
        read(new_socket, choice, sizeof(choice));
        
        if(strcmp(choice, "login") == 0 || strcmp(choice, "register") == 0)
            login(choice, passid, new_socket);

        if(strcmp(choice, "add") == 0)
            addFile(passid, new_socket);

        else if(strcmp(choice, "quit"))
            exit(EXIT_SUCCESS);

        else if(strcmp(choice, "see"))
            seeFile(new_socket);

        else {
            strcpy(strtemp, "Invalid Input\n");
            send(new_socket, strtemp, sizeof(strtemp), 0);
        }
    // }
    return 0;
}

void login(char choice[], char passid[], int new_socket) {
    FILE *akun = fopen("akun.txt", "a+"); // a+ because the file is already made
    char str[1001];    
    bool check = true;

    strcpy(strtemp, "Input id and password in one line :\n= ");
    send(new_socket, strtemp, sizeof(strtemp), 0);

    int idread = read(new_socket, passid, 1024);
    if(strcmp(choice, "register") == 0) {
        // To check whether the id and password is already listed in the system
        while(fgets(str, 100, akun)) { 
            if(strcmp(str, passid) == 0) {
                strcpy(strtemp, "The inputted id and password is already listed in the system\n");
                send(new_socket, strtemp, sizeof(strtemp), 0);
                check = false;
                fclose(akun);
            }
        }
        if(check) {
            fprintf(akun, "%s", passid);
            strcpy(strtemp, "The account is successfully created in the system\n");
            send(new_socket, strtemp, sizeof(strtemp), 0);
        }
    }
    else if(strcmp(choice, "login") == 0) {
        while(fgets(str, 100, akun) != NULL ) {
            if(strcmp(str, passid) == 0) {
                strcpy(strtemp, "Log in success\n");
                send(new_socket, strtemp, sizeof(strtemp), 0);
                isLogin = true;
                fclose(akun);
            }
        }
        if(!isLogin) {
            strcpy(strtemp, "Id and password is not listed in the system\n");
            send(new_socket, strtemp, sizeof(strtemp), 0);
        }
    }  
}

void addFile(char passid[], int new_socket) {
    char str[1001];
    FILE *FILES = fopen("file.tsv", "a");
    
    strcpy(strtemp, "Publisher: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
    read(new_socket, str, sizeof(str));
    fprintf(FILES, "Publisher: %s\n", str);

    strcpy(strtemp, "Publication year: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
    read(new_socket, str, sizeof(str));
    fprintf(FILES, "Publication year: %s\n", str);

    strcpy(strtemp, "Filepath: ");
    send(new_socket, strtemp, sizeof(strtemp), 0);
    read(new_socket, str, sizeof(str));
    fprintf(FILES, "Filepath: %s\n\n", str);

    fclose(FILES);
}

void seeFile(int new_socket) {
    FILE *tsv = fopen("file.tsv", "r");
    char str[1024];
    fseek(tsv, 0, SEEK_SET);
    fread(str, sizeof(str), 1, tsv);
    send(new_socket, str, sizeof(str), 0);

    fclose(tsv);
}