
//MOHAMED BELTAGY
/* includes */

#include "vxWorks.h"
#include "stdio.h"
#include "stdlib.h"
#include "semLib.h"
#include "taskLib.h"


/* defines */

#define DELAY_TICKS     20

#define STACK_SIZE	20000




int i ;
int i1 ;
int number_phil;
int think_time;
int wait_time;
int eat_time;
int simulation_time;
eat_time=10;
think_time=50;
i=0;
i1=0;

SEM_ID array_sema_fork [20];
int array_tidphil [20];

void phil1 (void);


int count[20];



/*************************************************************************
*
* progStart - start the sample program.
*
* RETURNS: OK
*/

STATUS progStart (void)
	{
		int x = 0;
	/* get the input data from the user  */
	printf("enter wait time ");
	scanf("%d",&wait_time);
	printf("enter number of philospher  ");
	scanf("%d",&number_phil);
	printf("enter the simulation time in seconds   ");
	scanf("%d",&simulation_time);
	
	
	for (i=0 ;i <number_phil ;i++)
		{
		array_sema_fork [i] = semBCreate (SEM_Q_FIFO, SEM_FULL);
		count[i] = 0;
	
		}

 
	/* create and start tasks */
	for (i1=0; i1<number_phil ; i1++)
		{
		array_tidphil[i1]=taskSpawn (NULL, 200, 0, STACK_SIZE,
		(FUNCPTR) phil1, 0,0,0,0,0,0,0,0,0,0);
		taskDelay (6);
		}
	taskDelay (simulation_time*60);// kabarok khalih mablagh
		
	for (x = 0; x < number_phil;x++)
		{
			printf ("Phil %d eated %d \n",x,count[x]);
		} 
	printf("\n\ndone.\n");
	//taskDelete ();
	for (i1=0; i1<number_phil ; i1++)
		{
		taskDelete (array_tidphil[i1]);
		}
	return (OK);
	}

//Task one
void phil1 (void)   {
	int m ;
	int l ;
	int i2;
	int last_phil;
	
	m=0;
	l=0;
	i2=0;
	while (1) 
	{
		while(i2<number_phil)
		//for (i2=0 ; i2<number_phil ; i2++)
		{
			if(array_tidphil[i2]== taskIdSelf() )
			{ 
				printf("phil %d think \n ", i2 );
				taskDelay (think_time);
				printf("phil %d FINISHED thinking \n ", i2 );
				printf("phil %d NEED  the %d fork!! \n  ", i2 ,i2 );
				m= rand() % (number_phil - 3) + 2;
				if (i2==m) 
					{
						l=i2+1;// start with the left fork not the right as the rest 			
						semTake (array_sema_fork[l], WAIT_FOREVER);
						semTake (array_sema_fork [i2], WAIT_FOREVER);//taking the second fork start with the left one 
						count[i2]++;
						printf("phil %d eats   \n ", i2 );
						taskDelay (eat_time);
						semGive (array_sema_fork [i2]);
						semGive (array_sema_fork [l]);	
					}
				
				else 
					semTake (array_sema_fork [i2], WAIT_FOREVER);//taking the first fork start with the right one 
					printf("phil %d fork one   \n  ", i2  );
					taskDelay (wait_time);
					last_phil=0;
					last_phil=number_phil-1;
					if (i2 == last_phil ) // this if condition for the last philospher  
						{ 
						semTake (array_sema_fork[0], WAIT_FOREVER);
						printf("last phil takes the  %d and fork zero  \n ", i2 );
						printf("phil %d last one eats \n ", i2 );
						count[i2]++;
						taskDelay (eat_time);
						semGive (array_sema_fork [i2]);
						semGive (array_sema_fork [0]);
						}
				else
					{		
					l=i2+1;				
					semTake (array_sema_fork[l], WAIT_FOREVER);
					count[i2]++;
					printf("phil %d eats   \n ", i2 );
					taskDelay (eat_time);
					semGive (array_sema_fork [i2]);
					semGive (array_sema_fork [l]);											
					}	
			}
			else 
			{
				i2++;
			
			}
		//continue ;
		
		}
	}
}
    







