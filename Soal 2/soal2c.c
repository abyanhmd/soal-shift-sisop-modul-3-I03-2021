#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

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

int main() {
    call_ls();
}
