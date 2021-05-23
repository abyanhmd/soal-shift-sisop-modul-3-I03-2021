## Modul 3 - Group I03

### **Number 1**
The problem asks us to make a server and a client system that can communicate each other via socket. The first thing to do is to connect the client to the server and register the account to proceed the next command.

**- Register or Login to the server**

Before storing the data to the database, client should register and login to the server. Before getting into the function, we made if statements in the server and the client code whether the command that the client typed is matched with the server command. </br>
__*Server*__
```c
void login(char choice[], char passid[], int new_socket) {
    FILE *akun = fopen("akun.txt", "a+"); // a+ because the file is exist
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
```
The first step is to check the input is either register or login. If the input is register, it will check id and password in `akun.txt` and match it so that there is no duplication in the `akun.txt`. It will send warning if there is duplication. Otherwise, it will print the id and password in the `akun.txt`. If the input is login, it will match inputted id and password with `akun.txt`. 

__*Client*__
```c
void reglog(char choice[], char id[], char pass[], char passid[]) {
    char strtemp[1001];
    // Input id and pass in one line
    read(sock, strtemp, sizeof(strtemp)); 
    printf("%s", strtemp);

    // To change the id and password into id:password format in the akun.txt
    scanf("%s %s", id, pass);
    strcpy(passid, id);
    strcat(passid, ":");
    strcat(passid, pass);
    strcat(passid, "\n");
    
    // Send the id and password with the format to the server
    int len = strlen(passid);
    send(sock, passid, len, 0);

    read(sock, buffer, sizeof(buffer));
    printf("%s", buffer);
}
```
In the client code, we change the id and password to the id:password format to make an easy access to the server because we can search in the `akun.txt` with this format, not id and password individually. We need to make a new variable to store the id and password with `strcat` then we can send it to the server.

**- Insert new information of the book**

In order to add data to the `file.tsv`, we need to access the file stream and print the information from the client input to the server database.

__*Server*__
```c
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
```

__*Client*__
```c
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
```
It basically just send the string to the client and the client will read it. After that, the client will send the input to the server and the server will store it to the `file.tsv`.
