#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#define MAX_PROC 999
#define MAX_TIME 999
#define MAX_QUEUE 10

void push_array(struct process this_proc, int max_index, struct process array[]);
struct process pull_out_ready_queue();
void serving(int time_slice);
int probability_rand(int config[]);
int get_array_slot(struct process array[], int max_index);
int isTimeChartDone(struct process timechart[], int i, int end);
void printAtArrival(int print, char arrived, char notArrived, struct process cpu[]);
int foundRQ();
int counter = 0;

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
    end_time,
    found = 0;

char user_ans;

struct process timechart_process[MAX_TIME];
struct process ready_queue[MAX_QUEUE][MAX_PROC+1];
struct process serve_process; // +1 is to have a empty element used to copy into elements to be removed.

void printTimeChart(int start_time, int end_time, struct process timechart[], struct process cpu_process[]);

int main(){
    srand(time(NULL));
    printf("\nHow many processes to simulate? [MAX=999] : ");
    scanf(" %d", &num_proc);
    printf("\nWould you like to [R]andomize data or [M]anual input? : ");
    scanf(" %c", &user_ans);

    struct process cpu_process[num_proc];

    for(n = 0; n < MAX_QUEUE; n++){
        memset(ready_queue[n], '\0', sizeof(ready_queue[n]));
    }
    memset(timechart_process, '\0', sizeof(timechart_process));

    if(user_ans == 'R'){
        cpu_process[0].arrivalTime = 0;
        for(i = 0; i < num_proc; i++){
            int config_1[] = {80,15,5};
            gap_arrival = probability_rand(config_1);
            cpu_process[i].pid = (char)65+i;
            cpu_process[i+1].arrivalTime = gap_arrival + cpu_process[i].arrivalTime;
            int config[] = {5,60,35};
            cpu_process[i].serviceTime = probability_rand(config);
        }
    }else if(user_ans == 'M'){
        for(i = 0; i < num_proc; i++){
            printf("\n\nProcess %c:", cpu_process[i].pid);
            printf("\nArrival time: ");
            scanf(" %d", &cpu_process[i].arrivalTime);
            printf("Service time: ");
            scanf(" %d", &cpu_process[i].serviceTime);
        }
    }

    for(i = 0; i < num_proc; i++){
        printf(" %c\t", cpu_process[i].pid);
        printf("  %d\t", cpu_process[i].arrivalTime);
        printf("    %d\n", cpu_process[i].serviceTime);
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
        }else if(is_serving != 1 && (foundRQ() != -1)){
            serve_process = pull_out_ready_queue();
            is_serving = 1;
            serving(time_slice);
        }else if(foundRQ() == -1){
            serve_process.pid = ' ';
            is_serving = 0;
        }

        push_array(serve_process, MAX_TIME, timechart_process);
        for(n = 0; (n < MAX_QUEUE) && (ready_queue[n][0].pid == 0); n++){}
        if((serve_process.serviceTime == 0) && (n == MAX_QUEUE) && (proc_pointer == num_proc)){
            is_done = 1;
            printf("\nMulti-level feedback Done!\n");
        }
    }
    proc_pointer = 0;
    printf("\nFinished at %d!\n", curr_second);
    for( start_time=0, end_time=20;isTimeChartDone(timechart_process,start_time,end_time)!=1;){
        printTimeChart(start_time, end_time, timechart_process, cpu_process);
    }
    printf("\n");
    for(i = 0; i < num_proc; i++){
        printf(" %c\t", cpu_process[i].pid);
        printf("  %d\t", cpu_process[i].arrivalTime);
        printf("   %d", cpu_process[i].serviceTime);
        printf("  %d \n", cpu_process[i].startTime);
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
    n = foundRQ();
    n--;
    curr_ready_queue = n;
    this_proc = ready_queue[curr_ready_queue][0];
    time_slice = pow(2, curr_ready_queue);

    for(; index < MAX_PROC; index++){
        ready_queue[n][index] = ready_queue[n][index+1];
    }
    return this_proc;
}

int foundRQ(){
    for(n = 0, found = 0; (found == 0) && (n < MAX_QUEUE); n++){
        if(ready_queue[n][0].pid != 0){
            found = 1;
        }
    }
    if(found == 0){
        n = -1;   
    }
    return n;
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
int probability_rand(int config[]){
    rand_val = rand()%9;
    if(rand_val >= 6 && rand_val <= 8 && rand()%100 < config[2]){
        return rand_val;
    }else if(rand_val >= 1 && rand_val <= 2 && rand()%100 < config[1]){
        return rand_val;
    }else if(rand_val >= 3 && rand_val <= 5 && rand()%100 < config[0]){
        return rand_val;
    }else{
        return ((rand()%5) + 1);
    }
}

void printAtArrival(int print, char arrived, char notArrived, struct process cpu[]){
    proc_pointer = temp_proc_pointer;
    printf("\n");
    for(i = start_time; (proc_pointer < num_proc) && (i < end_time); i++){
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
        if(timechart[i].pid == cpu_process[counter].pid){
            printf("\n%c == %c : %d, %d\n", timechart[i].pid, cpu_process[counter].pid, i, counter);
            cpu_process[counter].startTime = curr_second;
            counter++;
        }
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
        temp_proc_pointer = proc_pointer;
        printAtArrival(0, '|',' ', cpu_process);
        printAtArrival(1, ' ',' ', cpu_process);
        printAtArrival(2, ' ',' ', cpu_process);
    }
    start_time = end_time;
    end_time += 20;
}

int isTimeChartDone(struct process timechart[], int i, int end){
    for(;(timechart[i].pid != 0) && (i < end);){
        return 0;
    }
    return 1;
}