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

### **NUMBER 2**
**a. Input Matrix A (4x3) and Matrix B (3x6) then multiply it**</br>
First We need to input both Matrix
```c
int main(){
    
    int a[4][3],b[3][6];
    printf("---Input Matrix a and b---\n");
    for(int i=0; i<4;i++){
    	for(int j=0;j<3;j++){
    		scanf("%d",&a[i][j]);
    	}
    }
    
    for(int i=0; i<3;i++){
    	for(int j=0;j<6;j++){
    		scanf("%d",&b[i][j]);
    	}
    }
   ```
  We directly use the loop limit because the question wants us only to input 4x3 matrix and 3x6 matrix</br>
  Next we find the multiply of both matrix.
```c
void Multiply(int arr1[4][3], int arr2[3][6]){
    int sum=0;
    
    key_t key=1234;
    int *value;
    int shmid = shmget(key, sizeof(int)*24, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);
    /*shared memory*/
    int multiply[4][6];

    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            for(int k=0; k<3; k++){
                sum += arr1[i][k] * arr2[k][j];
            }
            multiply[i][j] = sum;
            sum = 0;
        }
    }/*calculate the result a*b*/

    for(int i=0; i<4; i++){
        for(int j=0; j<6; j++){
            value[i*6+j] = multiply[i][j]; 
            printf("%d\t", multiply[i][j]);
        } 
        printf("\n");
    }/*print result in 1 matrix*/

    shmdt(value);
}
   ```
   In This function we not only calculate the multiply matrix but also save the multiply matrix into 1 array dimension called value then use shared memory so the next program can use the result from this `soal2a.c` program
  
**b. Input New matrix 4x6 then calculate the factorial of it**</br>
First we need to declare all we need such as the `shared memory` , `matrix` and other stuff
```c
void main(){
    key_t key=1234;
    int *value;
    int shmid = shmget(key, sizeof(int)*24, IPC_CREAT | 0666);
    value = shmat(shmid, NULL, 0);

    int New_mat[24];
    unsigned long long result[24];

    // Input New Matrix
    printf("---Input New Mat---\n");
    for(int i=0; i<24; i++){
        scanf("%d", &New_mat[i]);
    }

    printf("\n");

    printf("---Matrix from Soal2a.c---");
    for(int i=0; i<24; i++){
        if(i%6 == 0){
            printf("\n");
        }
        printf("%d ", value[i]);
    }//print value from soal2a.c

    printf("\n\n");
```
   After we input new matrix which is 4x6 we print the shared matrix from `soal2a.c`.</br>
   Next step is we calculate the factorial using:
   ```c
   for(unsigned long long i=0; i<24; i++){
       if(New_mat[i] == 0 || value[i] == 0){
           result[i]=0;
       }
       else if(value[i] == 1){
           result[i] = 1; 
       }
       else if(value[i] == 2){
           result[i] = 2;
       }
       else{
           if(value[i] >= New_mat[i]){
               result[i] = value[i];
               for(unsigned long long a=1; a<New_mat[i]; a++){
                   result[i] = result[i] * (value[i]-a); 
               }
           }
           else{
               result[i] = value[i];
               for(unsigned long long a=1; a<value[i]; a++){
                   result[i] = result[i] * (value[i]-a);  
               }
           }
       }
    }
```
Next we Just Print the matrix after we find all the factorial using.
```c
 printf("---Result after factorial---");
    for(int i=0; i<24; i++){
        if(i%6 == 0){
            printf("\n");
        }
        printf("%llu ", result[i]);
    }
```
**c. Run ps aux | sort -nrk 3,3 | head -5 in C**</br>
First we need to check the pipe then run the ps aux command first
```c
void call_ls() {
    pid_t child_id;
    int status;
    int file_descriptor_1[2], file_descriptor_2[2];

    /*periksa pipe*/
    if(pipe(file_descriptor_1) == -1) {
        fprintf(stderr, "Pipe Failed");
        return;
    }

    if(pipe(file_descriptor_2) == - 1) {
        fprintf(stderr, "Pipe Failed");
        return;
    }

    /*jalankan command ps aux | sort -nrk 3,3 | head -5*/
    child_id = fork();
    if(child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if(child_id == 0) {
        close(file_descriptor_1[0]);
        dup2(file_descriptor_1[1], STDOUT_FILENO);

        /*exec ps aux*/
        char *argv[] = {"ps", "aux", NULL};
        execv("/bin/ps", argv);
    }
    else {
        /*tunggu child process selesai*/
        while ((wait(&status)) > 0);
        call_sort_and_head(file_descriptor_1, file_descriptor_2);
    }
}
```
Then We Need to run the sort nrk -3,3 and head -5 command</br>

```c
void call_sort_and_head(int file_descriptor_1[2], int file_descriptor_2[2]) {
    pid_t child_id;
    child_id = fork();
    int status;

    if(child_id < 0) {
        exit(EXIT_FAILURE);
    }

    if(child_id == 0) {
        close(file_descriptor_1[1]);
        dup2(file_descriptor_1[0], STDIN_FILENO);

        close(file_descriptor_2[0]);
        dup2(file_descriptor_2[1], STDOUT_FILENO);

        /*exec sort -nrk 3,3*/
        char *argv[] = {"sort", "-nrk", "3,3", NULL};
        execv("/usr/bin/sort", argv);
    } else {
        /*tutup pipe 1*/
        close(file_descriptor_1[0]);
        close(file_descriptor_1[1]);

        /*tunggu child process selesai*/
        while((wait(&status)) > 0);

        close(file_descriptor_2[1]);
        dup2(file_descriptor_2[0], STDIN_FILENO);

        /*exec head -5*/
        char *argv[] = {"head", "-5", NULL};
        execv("/usr/bin/head", argv);
    }
}
```
Obstacles:
- hard to understand
- In the beginning, The shared result from `soal2a.c` not sended to `soal2b.c` so the result 0.

### **Number 3**
**A. Program accepting `-f` argument, then the program will categorize file as much as the user input**</br>
First, we will make `*checkName` function to make the program to identify the file name.
```c
char *checkName(char *dir){
char *name = strrchr(dir, '/');
if(name == dir) return "";
return name + 1;
}
Then, we will make *lowercase function to make the procedure is not case sensitive by checking the small characters
char *lowercase(char *str){
unsigned char *temp = (unsigned char *)str;
while(*temp){
*temp = tolower(*temp);
temp++;
}
return str;
}
```
After that, we will check the extension, and classify the types of the extension given. If its `unknown` or `hidden`, the file will be specially categorized. The function used is `cat`
```c
void* cat(void *arg){
char *src = (char *)arg;
char srcP[150];
memcpy(srcP, (char*)arg, 400);
char *srcExt = cekExt(src);
char ext[400];
strcpy(ext, srcExt);

DIR *dir = opendir(srcExt);
if(dir) closedir(dir);
else if(ENOENT == errno) mkdir(srcExt, 0755);

char *srcName = checkName(srcP);
char *curr = getenv("PWD");

char destP[512];
sprintf(destP, "%s/%s/%s", curr, ext, srcName);

rename(srcP, destP);
}
```
Lastly, in the main program, it will identify the `-f` argument and access the functions below, resulting the designated outputs.
```c
if(argc > 2 && strcmp(argv[1], "-f") == 0){
pthread_t tid[argc-2];
int count = 0;
for(int i=2; i<argc; i++){
if(access(argv[i], F_OK) == 0){
pthread_create(&tid[count], NULL, cat, (void *)argv[i]);
count++;
printf("File %d : Berhasil Dikategorikan\n", i-1);
}
else printf("File %d : Sad, gagal :(\n", i-1);
}
for(int i=0; i<count; i++) pthread_join(tid[i], NULL);
return 0;
}
```
**B. Program accepting -d argument, then the program will categorize the file but only with 1 input**</br>
First we will make `cat2` function then it will accept the file only once. After identifying the file, we will call `cat` function to categorize it.
```c
void cat2(char *folderPath, int threadSize){
DIR *fd = opendir(folderPath);
struct dirent *dp;
pthread_t tid[threadSize];
int count = 0;
char fileName[400][400];

while((dp = readdir(fd)) != NULL){
if(dp->d_type == DT_REG){
sprintf(fileName[count], "%s/%s", folderPath, dp->d_name);
pthread_create(&tid[count], NULL, cat, (void *)fileName[count]);
count++;
}
else if((dp->d_type == DT_DIR) && strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0) {
            char folderPath2[400];
            sprintf(folderPath2, "%s/%s", folderPath, dp->d_name);
            DIR *fd2 = opendir(folderPath2);
            struct dirent *dp2;
int threadSize2 = 0;
while((dp2 = readdir(fd2)) != NULL){
if(dp2->d_type == DT_REG){
threadSize2++;
}
}
cat2(folderPath2, threadSize2);
closedir(fd2);
        }
}

for(int i=0; i<threadSize; i++) pthread_join(tid[i], NULL);
    closedir(fd);
}
```
After the function is done, it will receive `-d` function and calling the `cat2` function to process the categorization. The ouput will be the same as the designated by the problem.
```c
else if(argc == 3 && strcmp(argv[1], "-d") == 0){
DIR *fd = opendir(argv[2]);
if(fd){
struct dirent *dp;
int threadSize = 0;
while((dp = readdir(fd)) != NULL){
if(dp->d_type == DT_REG){
threadSize++;
}
}
cat2(argv[2], threadSize);
closedir(fd);
printf("Direktori sukses disimpan!\n");
}
else if(ENOENT == errno) printf("Yah, gagal disimpan :(\n");
}
```
**C. By inputting `*` argument, the program will categorize ALL files in the working directory**</br>
First, the program the `*` argument, then the directory that has been inputted will be organized using `cat2` function.
```c
else if(argc == 2 && strcmp(argv[1], "*") == 0){
char *curr = getenv("PWD");
DIR *dir = opendir(curr);
struct dirent *dp;
int threadSize = 0;
while((dp = readdir(dir)) != NULL){
if(dp->d_type == DT_REG){
threadSize++;
}
}
cat2(curr, threadSize);
closedir(dir);
}
else{
printf("Format input salah\n");
return 0;
}
```
**D. If there is a file that its name is peculiar, such as `unknown` file type or `hidden` file, it will be categorize to a special folder**</br>
We will use the `cekExt` function before to identify it, then make some condition if there is `unknown` file types or `hidden` files. Also we will need the `lowercase` function to help the identification more detailed.
```c
char *cekExt(char *dir){
char *unk = {"Unknown"};
char *hid = {"Hidden"};
char *tmp = strrchr(dir, '/');
if(tmp[1] == '.') return hid;

int i = 0;
while(i < strlen(tmp) && tmp[i] != '.') i++;
if(i == strlen(tmp)) return unk;

char ext[400];
int j = i;
while(i < strlen(tmp)) ext[i-j] = tmp[i], i++;
return lowercase(ext + 1);
}
```
**E. Every 1 file that is categorized is operated by 1 thread so that it can run in parallel so that the category process can run faster**</br>
The functions needed in this program are already implement the `pthread` function so that the program will run as thread goes.

Obstacles:
- Hard to understand
