#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

struct process {
    char pid;
    int arrivaltime, servicetime, remainingtime;
    int starttime, finishtime, turnaroundtime, waitingtime;
};



int main(){
    int i = 0, max_proc  = 10, max_time = 100;
    int num_proc = 4, ready_queue[max_proc], curr_second = 0, is_done = 0;
    int proc_pointer = 0, time_table[max_time];
    struct process cpu_process[num_proc];

    cpu_process[0].pid = 'A';
    cpu_process[1].pid = 'B';
    cpu_process[2].pid = 'C';
    cpu_process[3].pid = 'D';

    cpu_process[0].arrivaltime = 0;
    cpu_process[1].arrivaltime = 3;
    cpu_process[2].arrivaltime = 5;
    cpu_process[3].arrivaltime = 6;

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
        char temp;

        if(curr_second == cpu_process[proc_pointer].arrivaltime){
            printf("\nProcess %c at %d secs", cpu_process[proc_pointer].pid,  curr_second);
            proc_pointer++;
        } else{
            //printf("\nNo process arrival at %d ", curr_second);
        }

        if(proc_pointer - 1 == num_proc){
            is_done = 1;
        }

    }

    printf("\nFinished at %d!", curr_second - 1);

    return 0;
}

