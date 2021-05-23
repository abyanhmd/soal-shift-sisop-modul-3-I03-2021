#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>

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
    
    printf("---Result after factorial---");
    for(int i=0; i<24; i++){
        if(i%6 == 0){
            printf("\n");
        }
        printf("%llu ", result[i]);
    }

    printf("\n");
    shmdt(value);
}
