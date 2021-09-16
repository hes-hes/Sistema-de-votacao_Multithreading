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
void* report_fifo_thread(void* file_name);

int main(){	
    pthread_t t1, t2;
    int t1_id, t2_id;
    
    char file[20] = "eventos.txt";
    char fifo[20] = "FIFO-OUT";

    T_ARGS* a1 = (T_ARGS*)calloc(1,sizeof(T_ARGS));
    strcat(a1->file_txt_name,"");
    strcat(a1->file_txt_name,file);
    strcat(a1->fifo_name,"");
    strcat(a1->fifo_name,fifo);

    t1_id = pthread_create(&t1, NULL, report_fifo_thread, (void*)a1);
    pthread_join(t1, NULL);
    
    exit(0);
}


void* report_fifo_thread(void* t_args){
    T_ARGS* args = (T_ARGS*) t_args;
    char content[2056] = "";

    mkfifo((char*)args->fifo_name, 0666);
    FILE* file;
    while(1){
        int fd;
        fd = open((char*)args->fifo_name, O_RDONLY);
        if(!fd){
            printf("Unable to open fifo");
            exit(1);
        }
        int n;
        while ((n = read(fd, content, 1024)) > 0){
        }
        file = fopen((char*)args->file_txt_name,"w");
        fprintf(file,"%s",content);
        fclose(file);

        puts(content);
        strcpy(content,"");
        close(fd);
    }
          
}

