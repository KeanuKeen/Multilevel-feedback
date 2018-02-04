#include<stdio.h>
#include <time.h>


int main()
{

    srand(time(NULL));
    int arrival_time,service_time,ready_queue,time_slice,num,x,y,r,count,choice;
    int AT_array[10],ST_array[10];
    char pid='A';
    char pid2='A';

    //AT_Array and ST_Array are substitutes for CPU_PROCESS.PID or something

    printf("Round Robin\n\n");

    printf("Enter Number of Processes [Max 10]: ");
    scanf("%d",&num);

    printf("Manual or Random? [1] or [2]: ");
    scanf("%d",&choice);

    if(choice==1){
         printf("\nEnter Arrival Time for Process\n" );

        for(x=0; x<num; x++){

        printf("[%c]: ", pid+x);
        scanf("%d",&arrival_time);
        AT_array[x]=arrival_time;
        }

        printf("\nEnter Service Time for Process\n" );

        for (x=0; x<num; x++){
            printf("[%c]: ", pid+x);
            scanf("%d",&service_time);
            ST_array[x]=service_time;
        }

    }
    //Random Choice
    else {
        AT_array[0]=0;
        for (x=0; x<num; x++){

            AT_array[x+1]=random(r)+AT_array[x];
            ST_array[x]=random2(r);

        }

    }

    printf("\n|PROCESS|\t|ARRIVAL TIME|\t|SERVICE TIME|\n");

    for (x=0; x<num;x++){
            printf("\n[%c]:\t\t     %d\t\t     %d\n",pid+x,AT_array[x],ST_array[x]);

    }

    printf("\n\n");
    for (count=0; count<30; count++){

        printf("%d  ", count);

    }
}

int random(){ //para 1 to 6


    int r=rand()%6+1;


    return r;
}
int random2(){ //para 1 to 10

    int j=rand()%10+1;


    return j;
}


