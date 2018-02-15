#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_PROC 999
#define MAX_TIME 999
#define MAX_QUEUE 10

void push_array(struct process this_proc, int max_index, struct process array[]);
// void printAtArrival();
struct process pull_out_ready_queue();
void serving(int time_slice);
int probability_rand();
int get_array_slot(struct process array[], int max_index);
int isTimeChartDone(struct process timechart[], int i, int end);
void printAtArrival(int print, char arrived, char notArrived, struct process cpu[]);
int sumOfService();

struct process {
    char pid;
    int arrivalTime, serviceTime, remainingTime;
    int startTime, finishTime, turnaroundTime, waitingTime;
};

int i = 0,
    n = 0,
    num_proc,
    curr_second = 0,
    is_done = 0,
    proc_pointer = 0,
    temp_proc_pointer = 0,
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

char user_ans;

struct process timechart_process[MAX_TIME];
struct process ready_queue[MAX_QUEUE][MAX_PROC+1];
struct process serve_process; // +1 is to have a empty element used to copy into elements to be removed.

void printTimeChart(int start_time, int end_time, struct process timechart[], struct process cpu_process[]);

int main(){
    printf("\nHow many processes to simulate? [MAX=999] : ");
    scanf(" %d", &num_proc);
    printf("\nWould you like to [R]andomize data or [M]anual input? : ");
    scanf(" %c", &user_ans);

    struct process cpu_process[num_proc];

    for(i = 0; i < num_proc; i++){
        cpu_process[i].pid = (char)65+i;
    }

    srand (time(NULL));

    for(n = 0; n < MAX_QUEUE; n++){
        memset(ready_queue[n], '\0', sizeof(ready_queue[n]));
    }
    for(n = 0; n < MAX_TIME; n++){
        memset(timechart_process, '\0', sizeof(timechart_process));
    }

    // if(user_ans == 'R'){
    //     cpu_process[0].arrivalTime = 0;
    //     for(i = 1; i < num_proc; i++){
    //         gap_arrival = probability_rand();
    //         cpu_process[i].arrivalTime = gap_arrival + cpu_process[i-1].arrivalTime;
    //         cpu_process[i].serviceTime = probability_rand();
    //         printf("");
    //     }
    // // }else if(user_ans == 'M'){
    // //     for(i = 0; i < num_proc; i++){
    // //         printf("\n\nProcess %c:", cpu_process[i].pid);
    // //         printf("\nArrival time: ");
    // //         scanf(" %d", &cpu_process[i].arrivalTime);
    // //         printf("\nService time: ");
    // //         scanf(" %d", &cpu_process[i].serviceTime);
    // //     }
    // }

    cpu_process[0].pid = 'A';
    cpu_process[1].pid = 'B';
    cpu_process[2].pid = 'C';
    // cpu_process[3].pid = 'D';
    // cpu_process[4].pid = 'E';
    // cpu_process[5].pid = 'F';
    // cpu_process[6].pid = 'G';
    // cpu_process[7].pid = 'H';
    // cpu_process[8].pid = 'I';

    cpu_process[0].arrivalTime = 0;
    // for(i = 1; i < num_proc; i++){
    //     gap_arrival = probability_rand();
    //     cpu_process[i].arrivalTime = gap_arrival + cpu_process[i-1].arrivalTime;
    // }
    cpu_process[1].arrivalTime = 1;
    cpu_process[2].arrivalTime = 2;
    // cpu_process[3].arrivalTime = 7;
    // cpu_process[4].arrivalTime = 9;
    // cpu_process[5].arrivalTime = 11;
    // cpu_process[6].arrivalTime = 14;
    // cpu_process[7].arrivalTime = 15;
    // cpu_process[8].arrivalTime = 21;

    cpu_process[0].serviceTime = 3;
    cpu_process[1].serviceTime = 5;
    cpu_process[2].serviceTime = 5;
    // cpu_process[3].serviceTime = 8;
    // cpu_process[4].serviceTime = 3;
    // cpu_process[5].serviceTime = 5;
    // cpu_process[6].serviceTime = 1;
    // cpu_process[7].serviceTime = 2; 
    // cpu_process[8].serviceTime = 6; 

    printf("\nPID\tArrival\t Service\n");
    for(i = 0; i < num_proc; i++){
        printf(" %c\t", cpu_process[i].pid);
        printf("  %d\t", cpu_process[i].arrivalTime);
        printf("    %d \n", cpu_process[i].serviceTime);
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

    printf("\nFinished at %d!\n", curr_second);
    for( start_time=0, end_time=20;isTimeChartDone(timechart_process,start_time,end_time)!=1;){
        printTimeChart(start_time, end_time, timechart_process, cpu_process);
    }

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
// int probability_set = {{0,4,80}, {5,8,10}}
int probability_rand(){
    rand_val = rand()%9;
    if(rand_val >= 6 && rand_val <= 8 && rand()%100 < 10){
        return rand_val;
    }else{
        return ((rand()%5) + 1);
    }
}

void printAtArrival(int print, char arrived, char notArrived, struct process cpu[]){
    proc_pointer = temp_proc_pointer;
    printf("\n");
    // printf("\n(%d<%d)&&(%d<%d)", proc_pointer, num_proc, start_time, end_time );
    for(i = start_time; (proc_pointer < num_proc) && (i < end_time); i++){
        // printf("\n%d == %d", proc_pointer, i);
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
        if(i >= 10){
            printf(" ");
        }
    }
}

void printTimeChart(int start, int end, struct process timechart[], struct process cpu_process[]){
    if(isTimeChartDone(timechart, start, end) == 1){
        printf("\nDone!");
    }
    printf("\n\n");
    for(i = start; (timechart[i].pid != 0) && (i < end); i++){
        if(i < 10){
            printf("%3c", timechart[i].pid);
        }else{
            printf("%4c", timechart[i].pid);
        }
    }
    printf("\n");
    for(i = start; isTimeChartDone(timechart,i,end) == 0;){
        i++;
        printf("%d  ", i-1);
        if(isTimeChartDone(timechart,i,end) == 1){
            printf("%d", i);
        }
    }
    if((proc_pointer < num_proc) && (isTimeChartDone(timechart,start,end) == 0)){
        // printAtArrival(0, '^',' ', cpu_process);
        temp_proc_pointer = proc_pointer;
        printAtArrival(0, '|',' ', cpu_process);
        printAtArrival(1, ' ',' ', cpu_process);
        printAtArrival(2, ' ',' ', cpu_process);
    }
    start_time = end_time;
    end_time += 20;
    // printf("\nStart next: %d\nEnd next: %d", start_time, end_time);
}

int isTimeChartDone(struct process timechart[], int i, int end){
    for(;(timechart[i].pid != 0) && (i < end);){
        return 0;
    }
    return 1;
}

int sumOfService(){
    return 0;
}
