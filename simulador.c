#include <stdio.h>	
#include <stdlib.h>	
#include <string.h>	
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>


#define MAX (16)

typedef struct thread_args{
    char fifo_name[20];
}T_ARGS;

typedef struct votante{
    char id[40];
    char assembly[20];
    char table[5];
    long time_in;
    char voto[10];
}VT;

VT* queue[MAX];
int in = 0; // beginning 
int out = 0; // end
int vt_count = 0;

void* read_fifo_thread(void* file_name);
void create_VT(VT* vt_struct, char* vt_string);
void* votation_thread();
void get_random_vote(char* str);

//Queue funcs...
void push(VT* vt);
VT* pop();
void display();
void clean(int i);

int main(){
    char fifoin[20] = "FIFO-IN";
        
    pthread_t t1, t2, vote_t;
    int t1_id, t2_id, vote_tid;

    T_ARGS* a1 = (T_ARGS*)calloc(1,sizeof(T_ARGS));
    strcat(a1->fifo_name,"");
    strcat(a1->fifo_name,fifoin);
    
    t1_id = pthread_create(&t1, NULL, read_fifo_thread, (void*)a1); 
    vote_tid = pthread_create(&vote_t, NULL, votation_thread, (void*)NULL);
    
    pthread_join(t1, NULL);
    pthread_join(vote_t, NULL);

    exit(0);
}

void* read_fifo_thread(void* t_args){
    T_ARGS* args = (T_ARGS*) t_args;
    char content[2056];

    mkfifo((char*)args->fifo_name, 0666);

    while(1){
        int fd;
        fd = open((char*)args->fifo_name, O_RDONLY);
        if(!fd){
            printf("Unable to open fifo");
            exit(1);
        }
        int n;
        while ((n = read(fd, content, 64)) > 0){
        }
        close(fd);
        VT* vt = (VT*) malloc(sizeof(VT));
        create_VT(vt,content);
        push(vt);
    }    
}

void* votation_thread(){
    char fifo_out[20] = "FIFO-OUT";
    while(1){
        sleep(3);
        char vt_str[256];
        strcpy(vt_str,"");
        
        VT* vt = pop();
        if(vt == NULL){
            continue;
        }

        /*time_t time_out;
        do{
            time_out = time(NULL);
        }while(time_out == -1);
        char time[50] = "";
        strftime(time, 20, "%Y-%m-%d %H:%M:%S", localtime(&time_out));
        puts(time);*/

        sprintf(vt_str,"%s,%s,%s,%s,%ld,%ld\n",
            vt->id, vt->assembly, vt->table, vt->voto, vt->time_in,time(NULL));

        mkfifo(fifo_out, 0666);
        int fd = open(fifo_out, O_WRONLY); 
        if(!fd){
            printf("Unable to open fifo");
            exit(1);
        }
        //write(fd, vt_str, strlen(vt_str)+1); 
        write(fd, vt_str, 256); 
        //puts(vt_str);

        close(fd);
    }
}

void create_VT(VT* vt_struct, char* vt_string){
	int x = 0;

	char uuid[40];
    char assembly[40];
    char table[40];

    /*time_t now;
    do{
        now = time(NULL);
    }while(now == -1);
    strftime(vt_struct->time_in, 20, "%Y-%m-%d %H:%M:%S",localtime(&now));*/
	
	const char s[3] = ", ";
	char* token;
	char* rest = vt_string;   
		
	while ((token = strtok_r(rest, s, &rest)) != NULL){     
		//printf("%s\n", token);
		if(x==0){
			strcpy(uuid,token);	
		}
		else if(x==1){
			strcpy(assembly,token);
		}
		else if(x==2){
			strcpy(table,token);
		}
		x++;
	}
    
    strcat(vt_struct->id,"");
    strcat(vt_struct->id,uuid);

    strcat(vt_struct->assembly,"");
    strcat(vt_struct->assembly,assembly);

    strcat(vt_struct->table,"");
    strcat(vt_struct->table,table);

    vt_struct->time_in = time(NULL);

    get_random_vote(vt_struct->voto);
}

//Queue funcs...
/* insert item */
void push(VT* vt){
    int index;
	index = ((in++) & (MAX-1));
    queue[index] = vt;
    ++vt_count;
}

/* remove item */
VT* pop(){
	int index;
	index = ((out++) & (MAX-1));
	VT* r;
  	r = queue[index];
  	clean(index);
    ++vt_count;
  	return r;
}

/* print queue */
void display() {
    int i, next;
    for (i=0; i < MAX; i++){
		printf("queue[%d] = %s, %s\n", i, queue[i]->id, queue[i]->table);
        next = i+1;
        if(queue[next]==NULL ){
            if(i == vt_count - 1){

                break;
            }
            else{
                continue;
            }
        }
    }
	printf("\n");
} 

/* clean item */
void clean(int i) {
	queue[i] = NULL;
}

void get_random_vote(char* str){
	char party_1[8] = "ADDE";
	char party_2[8] = "AEMN";
	char party_3[8] = "ACRE";
	char party_4[8] = "APF";
	char party_5[8] = "EFA";
	char party_6[8] = "EUD";
	char party_7[8] = "ECPM";
	char party_8[8] = "ELEFT";
	char party_9[8] = "PDE";
	char party_10[8] = "PES";
	char party_11[8] = "EPP";
	char party_12[8] = "EGREEN";
	char party_13[8] = "VOLT";

	time_t t;
	srand((unsigned) time(&t));
	int num = 0;
	do{
		num = rand() % 15;
	}while(num < 1 || num > 13);
	
	if(num == 1){
		strcpy(str,party_1);;
	}
	if(num == 2){
		strcpy(str,party_2);;
	}
	if(num == 3){
		strcpy(str,party_3);;
	}
	if(num == 4){
		strcpy(str,party_4);;
	}
	if(num == 5){
		strcpy(str,party_5);;
	}
	if(num == 6){
		strcpy(str,party_6);;
	}
	if(num == 7){
		strcpy(str,party_7);;
	}
	if(num == 8){
		strcpy(str,party_8);;
	}
	if(num == 9){
		strcpy(str,party_9);;
	}
	if(num == 10){
		strcpy(str,party_10);;
	}
	if(num == 11){
		strcpy(str,party_11);;
	}
	if(num == 12){
		strcpy(str,party_12);;
	}
	if(num == 13){
		strcpy(str,party_13);;
	}
}
