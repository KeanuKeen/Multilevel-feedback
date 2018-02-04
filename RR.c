#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_PROC 10 
#define MAX_TIME 99
#define TIME_SLICE 3



void push_ready_queue();
struct process pull_out_ready_queue(int array_index);
void serving();
int probability_rand();

struct process {
    char pid;
    int arrivaltime, servicetime, remainingtime;
    int starttime, finishtime, turnaroundtime, waitingtime;
};

int i = 0, 
    num_proc = 4,
    curr_second = 0,  
    is_done = 0,
    proc_pointer = 0,
    curr_proc_pointer = 0,
    top_stack = 0,
    is_serving = 0,
    done_serving = 0,
    slice_counter = 0,
    gap_arrival = 0,
    final_val,
    rand_val;

char timeable[MAX_TIME];

struct process ready_queue[MAX_PROC+1];
struct process serve_process; // +1 is to have a empty element used to copy into elements to be removed.


int main(){
    struct process cpu_process[num_proc];

    srand (time(NULL));

    memset(ready_queue, '\0', sizeof(ready_queue)); // set all ready_queue elements to '0' to define a element is empty

    cpu_process[0].pid = 'A';
    cpu_process[1].pid = 'B';
    cpu_process[2].pid = 'C';
    cpu_process[3].pid = 'D';

    cpu_process[0].arrivaltime = 0;
    for(i = 1; i < num_proc; i++){
        gap_arrival = probability_rand();
        cpu_process[i].arrivaltime = gap_arrival + cpu_process[i-1].arrivaltime;
    }


    // cpu_process[0].arrivaltime = 0;
    // cpu_process[1].arrivaltime = 3;
    // cpu_process[2].arrivaltime = 5;
    // cpu_process[3].arrivaltime = 6;

    cpu_process[0].servicetime = 5;
    cpu_process[1].servicetime = 7;
    cpu_process[2].servicetime = 3;
    cpu_process[3].servicetime = 4;


    for(i = 0; i < num_proc; i++){
        printf("%c ", cpu_process[i].pid);
        printf("%d ", cpu_process[i].arrivaltime);
        printf("%d \n", cpu_process[i].servicetime);
    }

    for(;is_done != 1; curr_second++){

        printf("\n-----------");
        if((curr_second == cpu_process[proc_pointer].arrivaltime) && (proc_pointer < num_proc)){
            printf("\nProcess %c arrived", cpu_process[proc_pointer].pid,  curr_second);
            push_ready_queue(cpu_process[proc_pointer]);
            printf("\n%c pushed to ready queue from arrival", cpu_process[proc_pointer].pid);
            proc_pointer++;
        }
        
        if(serve_process.servicetime == 0){
            is_serving = 0;
            slice_counter = 0;
        }else if(slice_counter == 3 && serve_process.servicetime != 0){
            push_ready_queue(serve_process);
            printf("\n%c pushed back to ready queue from cpu process", serve_process.pid);
            is_serving = 0;
            slice_counter = 0;
        }

        if(is_serving == 1){
           serving();
        }else if(is_serving != 1){
            // printf("\nqueue[0]: %c ", ready_queue[0].pid);
            serve_process = pull_out_ready_queue(0);
            // printf("pid: %c time: %d", serve_process.pid, serve_process.servicetime);
            is_serving = 1;
            serving(); 
        }

        printf("\n%d: ", curr_second);
        for(i = 0; ready_queue[i].pid != 0; i++){
            printf("[%d]%c:%d, ", i, ready_queue[i].pid,ready_queue[i].servicetime);
        }
        printf("\nServe process: %c", serve_process.pid);

        if((serve_process.servicetime == 0) && (ready_queue[0].pid == 0)){
            is_done = 1;
            printf("\n\nRound Robin Done!");
        }

    }
    // push_ready_queue(ready_queue[3]);
    // serve_process = pull_out_ready_queue(0);
    // printf("\n%c", serve_process.pid);
    // printf("\nReady Queue: ");
    // for(i = 0; ready_queue[i].pid != 0; i++){
    //     printf("%c", ready_queue[i].pid);
    // }

    printf("\nFinished at %d!", curr_second - 1);
    printf("\nRandom(1-8): ");
    for(i = 0; i < 100; i++){
        printf("%d, ", probability_rand());
    }
    
    return 0;
}

void push_ready_queue(struct process this_proc){

    if(top_stack == MAX_PROC - 1){
        printf("Stack overflow!");
    }
    
    ready_queue[top_stack] = this_proc;

    top_stack++;

}

struct process pull_out_ready_queue(int array_index){
    struct process this_proc;
    this_proc = ready_queue[array_index];

    for(; array_index < MAX_PROC; array_index++){
        ready_queue[array_index] = ready_queue[array_index+1];
    }

    top_stack--;

    return this_proc;
}

void serving(){

    if(slice_counter != 3 && (serve_process.servicetime != 0)){
        serve_process.servicetime--;
        is_serving = 1;
        slice_counter++;
    }
    
}

int probability_rand(){
    
    rand_val = rand()%9;
    if(rand_val >= 6 && rand_val <= 8 && rand()%100 < 10){
        return rand_val;
    }else{
        return ((rand()%5) + 1);
    }
}   