#include <stdio.h>	
#include <stdlib.h>	
#include <string.h>	
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <pthread.h>
#include <semaphore.h> 

typedef struct thread_args{
    char file_txt_name[20];
    char fifo_name[20];
}T_ARGS;

void read_file(char* file_name);
void* write_fifo_thread(void* file_name);

int main(){	
    pthread_t t1, t2;
    int t1_id, t2_id;
    
    char file[20] = "votantes_0.txt";
    char fifo_in[20] = "FIFO-IN";
    

    T_ARGS* a1 = (T_ARGS*)calloc(1,sizeof(T_ARGS));
    strcat(a1->file_txt_name,"");
    strcat(a1->file_txt_name,file);
    strcat(a1->fifo_name,"");
    strcat(a1->fifo_name,fifo_in);
    
    t1_id = pthread_create(&t1, NULL, write_fifo_thread, (void*)a1);
    pthread_join(t1, NULL);
    
    exit(0);
}

void* write_fifo_thread(void* t_args){
    T_ARGS* args = (T_ARGS*) t_args;
       
    FILE* file;
    char* file_name = (char* )args->file_txt_name; 
    file = fopen((char* )args->file_txt_name,"r");
    if(!file){
        printf("Unable to open : %s ", (char* )args->file_txt_name);
        exit(1);
    }
    mkfifo((char*)args->fifo_name, 0666);
    int fd;
    
    char line[500];
    while (fgets(line, sizeof(line), file)){    
        fd = open((char*)args->fifo_name, O_WRONLY); 
        if(!fd){
            printf("Unable to open fifo");
            exit(1);
        }
        write(fd, line, strlen(line)+1); 
        close(fd);
        sleep(2);
        //puts(line);
    }
    close(fd);
    fclose(file); 
}





