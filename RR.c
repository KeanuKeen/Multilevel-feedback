#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_PROC 10
#define MAX_TIME 99
#define MAX_QUEUE 10


// void push_array(struct process this_proc, int max_index, struct process array[]);
// struct process pull_out_ready_queue();
// void serving(int time_slice);
// int probability_rand();
// int get_array_slot(struct process array[], int max_index);

void push_array();
// void printAtArrival();
struct process pull_out_ready_queue();
void serving();
int probability_rand();
int get_array_slot();
int isTimeChartDone();

struct process {
    char pid;
    int arrivalTime, serviceTime, remainingTime;
    int startTime, finishTime, turnaroundTime, waitingTime;
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
    curr_ready_queue = -1,
    start_time,
    end_time;

struct process timechart_process[MAX_TIME];
struct process ready_queue[MAX_QUEUE][MAX_PROC+1];
struct process serve_process; // +1 is to have a empty element used to copy into elements to be removed.

void printTimeChart();

int main(){
    struct process cpu_process[num_proc];

    srand (time(NULL));

    for(n = 0; n < MAX_QUEUE; n++){
        memset(ready_queue[n], '\0', sizeof(ready_queue[n]));
    }
    for(n = 0; n < MAX_TIME; n++){
        memset(timechart_process, '\0', sizeof(timechart_process));
    }

    cpu_process[0].pid = 'A';
    cpu_process[1].pid = 'B';
    cpu_process[2].pid = 'C';
    cpu_process[3].pid = 'D';

    cpu_process[0].arrivalTime = 0;
    // for(i = 1; i < num_proc; i++){
    //     gap_arrival = probability_rand();
    //     cpu_process[i].arrivalTime = gap_arrival + cpu_process[i-1].arrivalTime;
    // }
    cpu_process[1].arrivalTime = 1;
    cpu_process[2].arrivalTime = 4;
    cpu_process[3].arrivalTime = 7;

    cpu_process[0].serviceTime = 2;
    cpu_process[1].serviceTime = 4;
    cpu_process[2].serviceTime = 6;
    cpu_process[3].serviceTime = 8;

    for(i = 0; i < num_proc; i++){
        printf("%c ", cpu_process[i].pid);
        printf("%d ", cpu_process[i].arrivalTime);
        printf("%d \n", cpu_process[i].serviceTime);
    }

    for(;is_done != 1; curr_second++){

        if((curr_second == cpu_process[proc_pointer].arrivalTime) && (proc_pointer < num_proc)){
            push_array(cpu_process[proc_pointer], MAX_PROC+1, ready_queue[0]);
            proc_pointer++;
        }

        if(serve_process.serviceTime == 0){
            is_serving = 0;
            slice_counter = 0;
        }else if((slice_counter == time_slice) && (serve_process.serviceTime != 0)){
            next_ready_queue = curr_ready_queue + 1;
            push_array(serve_process, MAX_PROC+1, ready_queue[next_ready_queue]);
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

        push_array(serve_process, MAX_TIME, timechart_process);

        for(n = 0; (n < MAX_QUEUE) && (ready_queue[n][0].pid == 0); n++){}
        if((serve_process.serviceTime == 0) && (n == MAX_QUEUE)){
            is_done = 1;
            printf("\nMulti-level feedback Done!\n");
        }

    }

    proc_pointer = 0;

    printf("\nFinished at %d!\n", curr_second - 1);

    start_time = 0;
    end_time = 20;

    printTimeChart(0, 20, timechart_process, cpu_process);
    printTimeChart(20, 40, timechart_process, cpu_process);

    printf("\n\n");
    return 0;
}

void push_array(struct process this_proc, int max_index, struct process array[]){
    int index;
    next_ready_queue = curr_ready_queue + 1;

    index = get_array_slot(array, max_index);
    if(index != -1){
        array[index] = this_proc;
    }
}

int get_array_slot(struct process array[], int max_index){
    int index;

    for(index = 0; (array[index].pid) != 0 && (index < max_index); index++){}
    if(index != max_index){
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
    if(slice_counter != time_slice && (serve_process.serviceTime != 0)){
        serve_process.serviceTime--;
        is_serving = 1;
        slice_counter++;
    }
    if(serve_process.serviceTime == 0){
        serve_process.finishTime = curr_second-1;
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

void printAtArrival(int print, char arrived, char notArrived, struct process cpu[]){
    printf("\n");
    for(i = 0, proc_pointer = 0; proc_pointer < num_proc; i++){
        if(cpu[proc_pointer].arrivalTime == i){
            if(print == 1){
                printf("%c  ", cpu[proc_pointer].pid);
            }else if(print == 2){
                printf("%d  ", cpu[proc_pointer].serviceTime);
            }else{
                printf("%c  ", arrived);
            }
            
            proc_pointer++;
        }else{
            printf("%c  ", notArrived);
        }
    }
}

void printTimeChart(int start_time, int end_time, struct process timechart[], struct process cpu_process[]){
    printf("\n");
    for(i = start_time; (timechart[i].pid != 0) && (i < end_time); i++){
        if(i < 10){
            printf("%3c", timechart[i].pid);
        }else{
            printf("%4c", timechart[i].pid);
        }
    }
    printf("\n");
    for(i = start_time; isTimeChartDone(timechart,i,end_time);i++){
        printf("%d  ", i);
    }
    if(isTimeChartDone(timechart,i,end_time) == 1){
        printf("%d", i);
    }
    // for(i = start_time; ((timechart[i].pid != 0)||((timechart[i-1].pid != 0)&&(timechart[i+1].pid != 0))) && (i < end_time); i++){
    //     printf("%d  ", i);
    // }
    if(proc_pointer < num_proc){
        printAtArrival(0, '^',' ', cpu_process);
        printAtArrival(0, '|',' ', cpu_process);
        printAtArrival(1, ' ',' ', cpu_process);
        printAtArrival(2, ' ',' ', cpu_process);
    }
}

int isTimeChartDone(struct process timechart[], int i, int end_time){
    for(;(timechart[i].pid != 0) && (i < end_time);){
        return 1;
    }
    return 0;
}

