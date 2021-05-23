#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

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
    Multiply(a, b);

}
