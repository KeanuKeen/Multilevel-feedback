#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_PROC 10
#define MAX_TIME 99
#define MAX_QUEUE 10


void push_ready_queue();
struct process pull_out_ready_queue();
void serving();
int probability_rand();
int get_array_slot();

struct process {
    char pid;
    int arrivaltime, servicetime, remainingtime;
    int starttime, finishtime, turnaroundtime, waitingtime;
};

int i = 0,
    n = 0,
    num_proc = 4,
    curr_second = 0,
    is_done = 0,
    proc_pointer = 0,
    is_serving = 0,
    done_serving = 0,
    slice_counter = 0,
    gap_arrival = 0,
    time_slice,
    rand_val,
    next_ready_queue,
    curr_ready_queue = -1;

char timeable_process[MAX_TIME];

struct process ready_queue[MAX_QUEUE][MAX_PROC+1];
struct process serve_process; // +1 is to have a empty element used to copy into elements to be removed.


int main(){
    struct process cpu_process[num_proc];

    srand (time(NULL));

    for(n = 0; n < MAX_QUEUE; n++){
        memset(ready_queue[n], '\0', sizeof(ready_queue[n]));
    }

    cpu_process[0].pid = 'A';
    cpu_process[1].pid = 'B';
    cpu_process[2].pid = 'C';
    cpu_process[3].pid = 'D';

    cpu_process[0].arrivaltime = 0;
    // for(i = 1; i < num_proc; i++){
    //     gap_arrival = probability_rand();
    //     cpu_process[i].arrivaltime = gap_arrival + cpu_process[i-1].arrivaltime;
    // }
    cpu_process[1].arrivaltime = 1;
    cpu_process[2].arrivaltime = 3;
    cpu_process[3].arrivaltime = 5;

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

        printf("\n\nt: %d", curr_second);

        if((curr_second == cpu_process[proc_pointer].arrivaltime) && (proc_pointer < num_proc)){
            printf("\nProcess %c arrived", cpu_process[proc_pointer].pid);
            push_ready_queue(cpu_process[proc_pointer], 1);
            proc_pointer++;
        }

        if(serve_process.servicetime == 0){
            is_serving = 0;
            slice_counter = 0;
        }else if((slice_counter == time_slice) && (serve_process.servicetime != 0)){
            push_ready_queue(serve_process, 0);
            is_serving = 0;
            slice_counter = 0;
        }

        if(is_serving == 1){            
            serving(time_slice);
        }else if(is_serving != 1){
            serve_process = pull_out_ready_queue();
            is_serving = 1;
            serving(time_slice);
        }

        printf("\nServing process: %c", serve_process.pid);

        for(n = 0; (n < MAX_QUEUE) && (ready_queue[n][0].pid == 0); n++){}
        if((serve_process.servicetime == 0) && (n == MAX_QUEUE)){
            is_done = 1;
            printf("\n\nMulti-level feedback Done!");
        }

    }

    printf("\nFinished at %d!", curr_second - 1);
    printf("\nRandom(1-8): ");
    for(i = 0; i < 100; i++){
        printf("%d, ", probability_rand());
    }

    return 0;
}

void push_ready_queue(struct process this_proc, int arrived){
    int index, array_index;
    next_ready_queue = curr_ready_queue + 1;

    if(arrived == 1){
        array_index = 0;
    }else{
        array_index = next_ready_queue;
    }
    
    index = get_array_slot(ready_queue[array_index]);
    if(index != -1){
        ready_queue[array_index][index] = this_proc;
    }
}

int get_array_slot(struct process array[]){
    int index;

    for(index = 0; (array[index].pid) != 0 && (index < MAX_PROC); index++){}

    if(index != MAX_PROC){
        return index;
    }else{
        return -1;
    }

}

struct process pull_out_ready_queue(){
    int found = 0,
        index = 0;
    struct process this_proc;

    for(n = 0; (found == 0) && (n < MAX_QUEUE); n++){
        if(ready_queue[n][0].pid != 0){
            found = 1;
        }
    }
    n--;
    curr_ready_queue = n;

    this_proc = ready_queue[curr_ready_queue][0];
    time_slice = pow(2, curr_ready_queue);

    for(; index < MAX_PROC; index++){
        ready_queue[n][index] = ready_queue[n][index+1];
    }

    return this_proc;
}

void serving(int time_slice){
    if(slice_counter != time_slice && (serve_process.servicetime != 0)){
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