/*******************************************************************************
 *
 *		システム名：　リアルタイムOS基礎演習（MN101C49K用）
 *						演習４：英文字・数字出力タスクと出力関数
 *
 *		Copyright (c) Panasonic Corporation 2003-2009
 *		Copyright (c) 2010 Morimoto Yasushi
 *
 *------------------------------------------------------------------------------
 *
 *		$RCSfile: print.c,v $		$Revision: 1.1 $
 *
 *		$設　計: 森本泰司			$日付: 2003/10/ 3
 *		$Author: Y.Morimoto $		$Date: 2010-12-21 04:33:04+09 $
 *
 *******************************************************************************/
#include <string.h>

#include <itron.h>
#include <kernel.h>
#include <kernel_id.h>
#include "interrupt.h"
#include "general.h"
#include "trainer.h"
#include "define_objs.h"
#include "lcd.h"
#include "print.h"
#include <limits.h>
#include <alib.h>

/***** <ice> 1 ****/
#include "Demo.h"
#include "Stack.h"
#include "Position.h"
#include "Action.h"
#include "Direction.h"
#include "Map.h"
#include "VisitLog.h"
#include "PriorityQueue.h"
#include "Dictionary.h"
#include "Path.h"
#include "GameDict.h"
#include "GameQueue.h"
#include "GameTrack.h"
/****************/

void put_time(int ini_time)
{

    wai_sem(SEM_ID_DISPLAY);
    char sentence[100]="\x1b[2J"" ";
    char x1[20]="10";
    run_time=get_time()-ini_time;
    itoa(run_time,x1);
    strcat( sentence,x1);
    lcd_puts (sentence);
   	sig_sem(SEM_ID_DISPLAY);
}

void put_final_time(void)
{

    wai_sem(SEM_ID_DISPLAY);
    char sentence[100]="\x1b[2J"" ";
    char x1[20]="10";

    itoa(run_time,x1);
    strcat( sentence,x1);
    lcd_puts (sentence);
   	sig_sem(SEM_ID_DISPLAY);
}
void tsk_time(VP_INT exinf)
{
		int flg=0;
		int ini_time=0;
	     FLGPTN flgptn;
		 wai_flg(FLG_ID_TIME, 1, TWF_ORW, &flgptn);		  
	
	    FOREVER
	    {   
           slp_tsk();
		   if (flg==0)
	       {
	    	    ini_time=get_time(); 
	    	    set_flg(FLG_ID_RUN,1);
	       }
	         put_time(ini_time);	
	         flg++;

          }
          
}
void show(void)
{
		
	wai_sem(SEM_ID_DISPLAY);	/* 	 資源獲得 */ 
    char sentence[100]="\x1b[2J"" ";
    char x1[3]="10";
    char x2[3]="10";
    char x3[3]="10";
    char y1[3]="10";
    char y2[3]="10";
    char y3[3]="10";
    
    itoa(start_x,x1);
    itoa(patient_x,x2);
    itoa(goal_x,x3);
    itoa(start_y,y1);
    itoa(patient_y,y2);
    itoa(goal_y,y3);
    
    strcat( sentence,dir);
    strcat( sentence,";");
    strcat( sentence,x1);
    strcat( sentence,",");
    strcat( sentence,y1);
    strcat( sentence,";");
    strcat( sentence,x2);
    strcat( sentence,",");
    strcat( sentence,y2);
    strcat( sentence,";");
    strcat( sentence,x3);
    strcat( sentence,",");
    strcat( sentence,y3);

   lcd_puts (sentence);
   	sig_sem(SEM_ID_DISPLAY);
}
void put_reset(void)
{
	wai_sem(SEM_ID_DISPLAY);
	strcpy(model,"Start");
     start_x=0;
     start_y=0;
     patient_x=0;
     patient_y=0;
     goal_x=0;
     goal_y=0;
	strcpy(dir,"N");
    put_str_start();
    
   	sig_sem(SEM_ID_DISPLAY);

     
}
void put_str_start(void)
{
	wai_sem(SEM_ID_DISPLAY);
	
    char sentence[100]="\x1b[2J""Start ";
    char x1[3]="10";
    char y1[3]="10";
    
    itoa(start_x,x1);
    itoa(start_y,y1);

    
    strcat( sentence,x1);
    strcat( sentence,",");
    strcat( sentence,y1);
    strcat( sentence,";D=");
    strcat( sentence,dir);

   lcd_puts (sentence);

   	sig_sem(SEM_ID_DISPLAY);

     
}
static const char rcsid[] = "$Id: print.c,v 1.1 2010-12-21 04:33:04+09 Y.Morimoto Exp $";
void put_str_patient(void)
{
	 
	wai_sem(SEM_ID_DISPLAY);
	
    char sentence[100]="\x1b[2J""Patient ";
    char x1[3]="10";
    char y1[3]="10";

    
    itoa(patient_x,x1);
    itoa(patient_y,y1);

    
    strcat( sentence,x1);
    strcat( sentence,",");
    strcat( sentence,y1);
    strcat( sentence,";D=");
    strcat( sentence,dir);


   lcd_puts (sentence);

   	sig_sem(SEM_ID_DISPLAY);

     
}
void put_str_goal(void)
{
	wai_sem(SEM_ID_DISPLAY);
	
    char sentence[100]="\x1b[2J""Goal ";
    char x1[3]="10";
    char y1[3]="10";
    itoa(goal_x,x1);
    itoa(goal_y,y1);
    strcat( sentence,x1);
    strcat( sentence,",");
    strcat( sentence,y1);
    strcat( sentence,";D=");
    strcat( sentence,dir);


   lcd_puts (sentence);

   	sig_sem(SEM_ID_DISPLAY);   
}


void put_alt(void)
{
           
        if (strcmp(model,"Start")==0)
        {
		    strcpy(model,"Patient");
		    put_str_patient();
        }
		else if (strcmp(model,"Patient")==0)
		{
		    strcpy(model,"Goal");
		    put_str_goal();
		}
		else if (strcmp(model,"Goal")==0)
		{
		    strcpy(model,"Show");
		    show();
		}
		else
        {
		    strcpy(model,"Start");
		    put_str_start();
        }
}

void put_dir(void)
{
		if (strcmp(dir,"N")==0)
		  strcpy(dir,"E");
		else if (strcmp(dir,"E")==0)
		  strcpy(dir,"S");
		else if (strcmp(dir,"S")==0)
		  strcpy(dir,"W");
		else
	      strcpy(dir,"N");
        if (strcmp(model,"Start")==0)
        {
		    put_str_start();
        }
		else if (strcmp(model,"Patient")==0)
		{
		    put_str_patient();
		}
		else if (strcmp(model,"Goal")==0)
		{
		    put_str_goal();
		}
		else
}


/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *	関数名 :    tsk_put_alpha		英文字出力タスク
 *
 *	書　式 :    void tsk_put_alpha(VP_INT exinf)
 *
 *	機　能 :    1.TSW_0フラグが立つのを待って英文字を出力する
 *
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/



/*------------------ end of tsk_put_alpha() -----------------------------------*/

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *	関数名 :    tsk_put_digit		数字出力タスク
 *
 *	書　式 :    void tsk_put_digit(VP_INT exinf)
 *
 *	機　能 :    1.TSW_7フラグが立つのを待って数字を出力する
 *
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

void put_x(void)
{
		
		int div=6;

		if (strcmp(model,"Start")==0)
	     {
		     start_x=(start_x+1);
		     if (start_x>=div)
		        start_x=0;
		     put_str_start();
	     }
		  
		else if (strcmp(model,"Patient")==0)
    	{
		     patient_x=(patient_x+1);
		     if (patient_x>=div)
		        patient_x=0;
		     put_str_patient();
    	}
		else if (strcmp(model,"Goal")==0)
	    {
		     goal_x=(goal_x+1);
	         if (goal_x>=div)
		        goal_x=0;
		     put_str_goal();
	    }
	    else
       	    
}


void put_y(void)
{
		
		int div=4;

		if (strcmp(model,"Start")==0)
	     {
		     start_y=(start_y+1);
	         if (start_y>=div)
		        start_y=0;
		     put_str_start();
	     }
		  
		else if (strcmp(model,"Patient")==0)
    	{
		     patient_y=(patient_y+1);
		    if (patient_y>=div)
		        patient_y=0;
		     put_str_patient();
    	}
		else if (strcmp(model,"Goal")==0)
	    {
		     goal_y=(goal_y+1);
		     if (goal_y >=div)
		        goal_y=0;
		     put_str_goal();
	    }
       	else

}

unsigned char rotate_r(unsigned char data);
unsigned char rotate_l(unsigned char data);

unsigned char buf_l = 0x99;
unsigned char buf_r = 0x99;

void start_wall(void);
void forward_wall(void);
void turn_left(void);
void turn_right(void);
void turn_back(void);
void turn_round(void);
void push_back(void);

/*********** <ice> 2 *******/
/* an 2D array for encoding map information*/
unsigned short int map[N_ROW][N_COL][N_WALL];
unsigned short int visited[N_ROW][N_COL];

unsigned int sensors[N_SENSORS];

/* position of the robot*/
Position startPos, robotPos, nextPos, humanPos, goalPos;

/* stacks*/
Stack posStack;

/* robot direction*/
unsigned int robotDir;

int stepN = 0;

/* for phase 2 */
Position pushTarget;
Position moveGoal;
PosTrack pathTrack;
Stack path;
int action;
/* program status */
int phase = 1;	/* current phase */
int hasSolution;
RescueSolution solution;

/* functions declaration */
int getDeepestPos(void);
void getNextPos();
short int getTransitionCost(unsigned short int map[N_ROW][N_COL][N_WALL], GameState startState, GameState endState);
int findSolutionAstar(unsigned short int map[N_ROW][N_COL][N_WALL], unsigned short int corners[N_ROW][N_COL], GameTrack * gt, Position rbPos, Position hmPos, Position gPos, RescueSolution * sol);

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *	関数名 :    tsk_put_alpha		英文字出力タスク
 *
 *	書　式 :    void tsk_put_alpha(VP_INT exinf)
 *
 *	機　能 :    1.TSW_0フラグが立つのを待って英文字を出力する
 *
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


void tsk_run(VP_INT exinf)
{
	int flg=0;
	FLGPTN flgptn;
	wai_flg(FLG_ID_RUN, 1, TWF_ORW, &flgptn);	
	FOREVER {
		/* PHASE 1: Get map information */
		if (phase == 1)
		{
			dly_tsk(1000);
			/********** INITIALIZATION ***********/
			/* initialize map */
			mapInit(map);

			/* initialize visiting log */
			visitLogInit(visited);

			/* set robot position*/
			/**/
			setPos(&startPos, start_y, start_x);
			setPos(&robotPos, start_y, start_x);
			/**
			setPos(&startPos, 3, 0);
			setPos(&robotPos, 3, 0);
			/**/
			
			/* set robot direction */
			if (strcmp(dir, "N") == 0)
				robotDir = NORTH;
			else if (strcmp(dir, "E") == 0)
				robotDir = EAST;
			else if (strcmp(dir, "W") == 0)
				robotDir = WEST;
			else
				robotDir = SOUTH;

			/* set human position */
			/**/
			setPos(&humanPos, patient_y, patient_x);
			/**
			setPos(&humanPos, 2, 2);
			/**/
			setVisited2(visited, humanPos); /* ok */

			/* set goal position */
			/**/
			setPos(&goalPos, goal_y, goal_x);
			/**
			setPos(&goalPos, 0, 2);
			/**/
			/* initialize stacks */
			stackInit(&posStack);	/* ok */
			push(&posStack, robotPos); /* ok */
			
			for (int i = 0; i < N_SENSORS; i++)
				sensors[i] = 0;

			/********** GET SENSORS AND MOVE ***********/
			/* get sensors from start position */
			start_wall();
			wait_100us(DELAY_TIME);
			
			while (TRUE) {
				stepN++;

				/* mark current position as visited */
				setVisited2(visited, robotPos); /* ok */

				/* update wall information */
				updateWall(map, sensors, robotPos, robotDir);	/* ok */

				/* decide next position to go */
				getNextPos();	/* ok */

				/* get action */
				action = getAction(robotPos, nextPos, robotDir); /* ok */

				/* update robot status */
				if (isValidPos2(nextPos)) {
					move(&robotPos, &nextPos, &robotDir);
				}
				else
					break;
					
				/* Actually take action */
				switch (action) {
					case MOVE_FORWARD:
						/* move forward */
						forward_wall();
						break;
					case MOVE_LEFT:
						/* turn left + move forward */
						turn_left();
						forward_wall();
						break;
					case MOVE_RIGHT:
						/* turn right + move forward */
						turn_right();
						forward_wall();
						break;
					case MOVE_BACKWARD:
						/* turn 180 + move forward */
						turn_back();
						forward_wall();
						break;
					default:
						break;
				}
				wait_100us(DELAY_TIME);
			}
			phase = 2;
			put_final_time();
		}
		else /* PHASE 2 */
		{
			/********** INITIALIZATION ***********/
			/* mark the corner positions in the visited array */
			getCornersPos(map, visited);	/*ok*/
			
			int is_corner = isCornerPos2(visited, humanPos);
			int has_path1 = hasPath2(map, humanPos, goalPos, newPosition(N_ROW, N_COL));
			int has_path2 = hasPath2(map, humanPos, startPos, newPosition(N_ROW, N_COL));
			
			GameTrack gt;
			if (!is_corner && has_path1 && has_path2) {
				/**/ hasSolution = findSolutionAstar(map, visited, &gt, startPos, humanPos, goalPos, &solution); /**/
				/* hasSolution = findSolutionAstar2(startPos, humanPos, goalPos, &solution); */
			} else {
				hasSolution = FALSE;
			}

			/* if there exists a solution */
			if (hasSolution) {
				/* start moving */
				popSolution(&solution);
				while (!isEmptySolution(solution)) {
					/* get current target to push human */
					pushTarget = popSolution(&solution);

					/* get the current goal to move to */
					moveGoal.row = 2 * humanPos.row - pushTarget.row;
					moveGoal.col = 2 * humanPos.col - pushTarget.col;

					/* find path from current robot position to the current goal */
					findPath2(map, &pathTrack, robotPos, moveGoal, humanPos, &path);
					/* getPath(pathTrack, robotPos, moveGoal, &path); */

					/* go to current goal */
					while (!isEmptyStack(path)) {
						/* get next position */
						nextPos = pop(&path);

						/* decide action */
						action = getAction(robotPos, nextPos, robotDir);

						/* update robot status */
						move(&robotPos, &nextPos, &robotDir);

						/* take real action */
						switch (action) {
							case MOVE_FORWARD:
								/* move forward */
								forward_wall();
								break;
							case MOVE_LEFT:
								/* turn left + move forward */
								turn_left();
								forward_wall();
								break;
							case MOVE_RIGHT:
								/* turn right + move forward */
								turn_right();
								forward_wall();
								break;
							case MOVE_BACKWARD:
								/* turn 180 + move forward */
								turn_back();
								forward_wall();
								break;
							default:
								break;
						}
						wait_100us(DELAY_TIME);
					}

					/* push human to the target */
					action = getAction(robotPos, humanPos, robotDir);

					/* update robot status taking action */
					move(&robotPos, &humanPos, &robotDir);

					/* update human position */
					humanPos = pushTarget;

					/* take real action */
					switch (action) {
						case MOVE_FORWARD:
							/* move forward + push */
							forward_wall();
							break;
						case MOVE_LEFT:
							/* turn left + move forward + push */
							turn_left();
							forward_wall();
							break;
						case MOVE_RIGHT:
							/* turn right + move forward + push  */
							turn_right();
							forward_wall();
							break;
						case MOVE_BACKWARD:
							/* turn 180 + move forward + push  */
							turn_back();
							forward_wall();
							break;
						default:
							break;
					}
					
					wait_100us(DELAY_TIME);
					
					/* TODO push action */
					push_back();
				}
			} else {
				/* do nothing */
			}
		}
	}
	
}

/*------------------ end of tsk_put_digit() -----------------------------------*/
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 *	関数名 :    putstring		文字列表示
 *
 *	書　式 :    void putstring(const char *s)
 *
 *	機　能 :    1.与えられた文字列をLCD表示タスクにデータキューで送る
 *
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

unsigned char rotate_r(unsigned char data)
{
	unsigned char msb2lsb;

	msb2lsb = data << CHAR_BIT - 1;
	data >>= 1;
	data |= msb2lsb;
	
	return data;
}

unsigned char rotate_l(unsigned char data)
{
	unsigned char msb2lsb;

	msb2lsb = data >> CHAR_BIT - 1;
	data <<= 1;
	data |= msb2lsb;

	return data;
}

void start_wall(void)
{
	for (int i = 0; i < N_SENSORS; i++)
		sensors[i] = 0;
		
	unsigned int  buf;
	if((inp(SENL)|0xFB)==251||(inp(SENR)|0x9F)==159){sensors[0]=1;}
	for(int temp=0;temp<20;temp++)
	{		
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
  		dly_tsk(10);
		buf_l = rotate_l(buf_l);
		buf_r = rotate_l(buf_r);
	}
	if(inp(SENL)!=0xFF&&inp(SENR)!=0xFF){sensors[1]=1;sensors[2]=1;}
	if(inp(SENL)!=0xFF&&inp(SENR)==0xFF){sensors[1]=1;sensors[2]=0;}
  	if(inp(SENL)==0xFF&&inp(SENR)!=0xFF){sensors[1]=0;sensors[2]=1;}
  	for(int temp=0;temp<20;temp++)
	{				
		/*sensorl=inp(SENL);*/
		/*sensorr=inp(SENR);*/
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
  		
  		dly_tsk(10);
		buf_l = rotate_r(buf_l);
		buf_r = rotate_r(buf_r);
	}
	if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}	
}
/*壁情報収集と直行*/
void forward_wall(void)
{
	for (int i = 0; i < N_SENSORS; i++)
		sensors[i] = 0;
		
	unsigned int buf;
	
	for(int otemp=0;otemp<12;otemp++)
	{
		for(int temp=0;temp<10;temp++)
		{	
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
  		
  			
  		dly_tsk(3);
			buf_l = rotate_r(buf_l);
			buf_r = rotate_r(buf_r);
		}
		if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
		if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}
	}	
	if(inp(SENL)!=0xFF&&inp(SENR)!=0xFF){sensors[1]=1;sensors[2]=1;}
	if(inp(SENL)!=0xFF&&inp(SENR)==0xFF){sensors[1]=1;sensors[2]=0;}
  	if(inp(SENL)==0xFF&&inp(SENR)!=0xFF){sensors[1]=0;sensors[2]=1;}
  	/*if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}*/
	for(int otemp=0;otemp<12;otemp++)
	{
		for(int temp=0;temp<10;temp++)
		{	
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	

  			
  		dly_tsk(3);
			buf_l = rotate_r(buf_l);
			buf_r = rotate_r(buf_r);
		}
		if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
		if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}	
   		if((inp(SENL)|0x7F)==127||(inp(SENR)|0x7F)==127){sensors[0]=1;}
	}
	if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}		
}
/*左回り９０度*/
void turn_left(void)
{
	unsigned int buf;
	for(int temp=0;temp<104;temp++)
	{
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
   	
   		
  		dly_tsk(3);
  		buf_l = rotate_l(buf_l);
   		buf_r = rotate_r(buf_r);
	}
	if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}		
}
/*右回り９０度*/
void turn_right(void)
{
	unsigned int buf;
	for(int temp=0;temp<104;temp++)
	{
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
  		dly_tsk(3);
   		buf_l = rotate_r(buf_l);
   		buf_r = rotate_l(buf_r);
	}
	if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}	
}
/*現地180度*/
void turn_back(void)
{
	unsigned int buf;
	for(int temp=0;temp<210;temp++)
	{
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
   		
  		dly_tsk(3);
   		buf_l = rotate_r(buf_l);
   		buf_r = rotate_l(buf_r);
	}
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
	buf_l = rotate_l(buf_l);
	buf_r = rotate_l(buf_r);
	if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}	
	
}
/*現地360度*/
void turn_round(void)
{
	unsigned int buf;
	for(int temp=0;temp<429;temp++)
	{
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	
 
  		dly_tsk(3);
   		buf_l = rotate_r(buf_l);
   		buf_r = rotate_l(buf_r);
	}
	    buf=inp(P6OUT_8);
		outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
		outp(MOTL,buf_l);
  	

  		dly_tsk(3);
	buf_l = rotate_r(buf_l);
	buf_r = rotate_r(buf_r);
	if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}	
}
/*push and backward*/
void push_back(void)
{
	unsigned int buf;
	for(int temp=0;temp<70;temp++)
	{	
    buf=inp(P6OUT_8);
	outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
	outp(MOTL,buf_l);
	dly_tsk(3);
		buf_l = rotate_r(buf_l);
		buf_r = rotate_r(buf_r);
	}
		if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_r(buf_r);wait_100us(20);}
		if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_r(buf_l);wait_100us(20);}	
	for(int out=0;out<10;out++)
	{
		for(int temp=0;temp<9;temp++)
		{	
			buf=inp(P6OUT_8);
			outp(MOTR,(buf&LCD_USE4)|(buf_r&LCD_USE3));
			outp(MOTL,buf_l);

		
  			dly_tsk(3);
			buf_l = rotate_l(buf_l);
			buf_r = rotate_l(buf_r);
		}
		if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTR,buf_r);buf_r = rotate_l(buf_r);wait_100us(20);}
		if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTL,buf_l);buf_l = rotate_l(buf_l);wait_100us(20);}
	}
	/*if((inp(SENL)|0xEF)==239||(inp(SENL)|0xCF)==207||(inp(SENL)|0xDF)==223||(inp(SENL)|0x9F)==159||(inp(SENL)|0xBF)==191||(inp(SENR)|0xEF)==239||(inp(SENR)|0xCF)==207||(inp(SENR)|0xDF)==223||(inp(SENR)|0x9F)==159||(inp(SENR)|0xBF)==191){outp(MOTL,buf_l);buf_l = rotate_l(buf_r);wait_100us(20);}
	if((inp(SENL)|0xF7)==247||(inp(SENL)|0xF3)==243||(inp(SENL)|0xFB)==251||(inp(SENL)|0xF9)==249||(inp(SENL)|0xFD)==253||(inp(SENR)|0xF7)==247||(inp(SENR)|0xF3)==243||(inp(SENR)|0xFB)==251||(inp(SENR)|0xF9)==249||(inp(SENR)|0xFD)==253){outp(MOTR,buf_r);buf_r = rotate_l(buf_l);wait_100us(20);}*/
}

/* find the deepest position in the stack which has unvisited neighbour */
int getDeepestPos(void) {
    int i;

    for (i = posStack.top; i > -1; i--) {
        if (hasUnvisitedNeighbour2(map, visited, posStack.s[i]))
            return i;
    }
    return -1;
}

void getNextPos() {
    /* check if there is any unvisited neighbour (order: front -> left -> right) */
    unsigned int wallInfo[N_SENSORS];
    getWallInfo(wallInfo, map, robotPos, robotDir); /* ok */

    short int testRow, testCol;

    switch (robotDir) {
    case NORTH:
        /* test front position */
        if (wallInfo[FRONT]==0) {
            testRow = robotPos.row - 1;
            testCol = robotPos.col;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test left position */
        if (wallInfo[LEFT]==0) {
            testRow = robotPos.row;
            testCol = robotPos.col - 1;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test right position */
        if (wallInfo[RIGHT]==0) {
            testRow = robotPos.row;
            testCol = robotPos.col + 1;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        break;
    case EAST:
        /* test front position */
        if (wallInfo[FRONT]==0) {
            testRow = robotPos.row;
            testCol = robotPos.col + 1;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test left position */
        if (wallInfo[LEFT]==0) {
            testRow = robotPos.row - 1;
            testCol = robotPos.col;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test right position */
        if (wallInfo[RIGHT]==0) {
            testRow = robotPos.row + 1;
            testCol = robotPos.col;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        break;
    case WEST:
        /* test front position */
        if (wallInfo[FRONT]==0) {
            testRow = robotPos.row;
            testCol = robotPos.col - 1;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test left position */
        if (wallInfo[LEFT]==0) {
            testRow = robotPos.row + 1;
            testCol = robotPos.col;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test right position */
        if (wallInfo[RIGHT]==0) {
            testRow = robotPos.row - 1;
            testCol = robotPos.col;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        break;
    case SOUTH:
        /* test front position */
        if (wallInfo[FRONT]==0) {
            testRow = robotPos.row + 1;
            testCol = robotPos.col;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test left position */
        if (wallInfo[LEFT]==0) {
            testRow = robotPos.row;
            testCol = robotPos.col + 1;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        /* test right position */
        if (wallInfo[RIGHT]==0) {
            testRow = robotPos.row;
            testCol = robotPos.col - 1;
            if (isValidPos(testRow, testCol)) {
                if (isVisited(visited, testRow, testCol) == FALSE) {
                    setPos(&nextPos, testRow, testCol);
                    push(&posStack, nextPos);
                    return;
                }
            }
        }
        break;
    }

    /* if robot has just started, set the position behind the robot as the next position */
    if (stepN == 1) {
        switch (robotDir) {
        case NORTH:
            setPos(&nextPos, robotPos.row + 1, robotPos.col);
            break;
        case EAST:
            setPos(&nextPos, robotPos.row, robotPos.col - 1);
            break;
        case WEST:
            setPos(&nextPos, robotPos.row, robotPos.col + 1);
            break;
        case SOUTH:
            setPos(&nextPos, robotPos.row - 1, robotPos.col);
            break;
        }
        push(&posStack, nextPos);
        return;
    }

    /* backtrack */
    pop(&posStack);

    /* check if stack is empty */
    if (isEmptyStack(posStack)) {
        setPos(&nextPos, -1, -1);
        return;
    }

    /* check if previous position has unvisited neighbour */
    Position prevPos = head(posStack);
    if (hasUnvisitedNeighbour2(map, visited, prevPos)) {
        nextPos = prevPos;
        return;
    }

    /* find the deepest position in the stack which has unvisited neighbour */
    int deepestIndex = getDeepestPos();

    /* test if one of the neighbours of current position is in stack */
    int index = 1000;
    int sid;
	short int i, r, c;
    /* for each neighbour */
    for (i = -2; i < 3; i++) {
        if (i != 0) {
            r = robotPos.row + i/2;
            c = robotPos.col + i%2;
            if (r != prevPos.row || c != prevPos.col) {
                if (isValidPos(r, c) && !checkWall(map, r, c, robotPos.row, robotPos.col)) {
                    sid = isInStack(posStack, r, c);
                    if (sid > -1 && sid >= deepestIndex && sid < index) {
                        index = sid;
                    }
                }
            }
        }
    }

    if (index < 1000) {
        popToIndex(&posStack, index);
    }

    nextPos = head(posStack);
}

short int getTransitionCost(unsigned short int map[N_ROW][N_COL][N_WALL], GameState startState, GameState endState) {

    /* get the position X to which robot has to reach in order to move human */
    short int xRow = startState.hmPos.row + startState.hmPos.row - endState.hmPos.row;
    short int xCol = startState.hmPos.col + startState.hmPos.col - endState.hmPos.col;

    /* check and get path cost from robot position in start state to position X */
    Position xPos = newPosition(xRow, xCol);
    short int cost = getMoveCost2(map, startState.rbPos, xPos, startState.hmPos);

    /** plan A: not consider the path robot has to move **/
    if (cost != MAX_VALUE) {
        return 1;
    }
    else
        return MAX_VALUE;
    /**/
    /** plan B: consider the path robot has to move **
    return cost;
    /**/
}

int findSolutionAstar(unsigned short int map[N_ROW][N_COL][N_WALL], unsigned short int corners[N_ROW][N_COL], GameTrack * gt, Position rbPos, Position hmPos, Position gPos, RescueSolution * sol) {
    /* initialize queue */
    GameQueue gq;
    gameQueueInit(&gq);

    /* initialize cost dictionary */
    GameDict stateCost;
    initGameDict(&stateCost);

    /* initialize game track */
    initGameTrack(gt);

    /* put the initial game state to queue */
    enGameQueue(&gq, newGameQueueElement2(rbPos, hmPos, 0));

    /* save cost of initial state to dictionary */
    addState2(&stateCost, rbPos, hmPos, 0);

    while (!isEmptyGameQueue(&gq)) {
        /* get state to consider from game queue */
        GameQueueElement curGameE = deGameQueue(&gq);
        GameState curState = curGameE.gs;
		
        /* check if the human has been moved to the goal in this state */
        if (isSamePos2(curState.hmPos, gPos)) {
			/* go here */
			sol->top = -1;
			GameState startState = newGameState(rbPos, hmPos);
			GameState curState;
			for (int j = 0; j < gt->gtSize; j++) {
				if (isSamePos2(gt->gsL[j].hmPos, gPos)) {
					/* ok run to here */
					curState = newGameState(gt->gsL[j].rbPos, gt->gsL[j].hmPos);
					break;
				}
			}

			
			/**/
			while (!isSameGameState(curState, startState)) {
				sol->top++;
				sol->rs[sol->top] = curState.hmPos;
				/* curState = getFromState(*gt, curState); */
				for (int j = 0; j < gt->gtSize; j++) {
					if (isSameGameState(gt->gsL[j], curState)) {
						curState = newGameState(gt->fromL[j].rbPos, gt->fromL[j].hmPos);
					}
				}
			}
			
			/**/
			
            return TRUE;
        }

        /* otherwise */
        short int i;
		short int nextHmRow, nextHmCol, newcost, transCost;
        unsigned short int priority;
		
        /* for each neighbor of considered state */
        for (i = -2; i < 3; i++) {
            if (i != 0) {
                nextHmRow = curState.hmPos.row + i/2;
                nextHmCol = curState.hmPos.col + i%2;
				
                if (isValidPos(nextHmRow, nextHmCol)
                     && !checkWall(map, nextHmRow, nextHmCol, curState.hmPos.row, curState.hmPos.col)
                     && (!isCornerPos(corners, nextHmRow, nextHmCol) || isSamePos2(gPos, newPosition(nextHmRow, nextHmCol)))
                     && isMovable(map, curState.hmPos.row, curState.hmPos.col, nextHmRow, nextHmCol)) {
                        GameState state = newGameState(curState.hmPos, newPosition(nextHmRow, nextHmCol));
                        transCost = getTransitionCost(map, curState, state);
                        if (transCost != MAX_VALUE) {
                            newcost = getStateCost2(stateCost, curState.rbPos, curState.hmPos) + transCost;
                            if (newcost < getStateCost2(stateCost, state.rbPos, state.hmPos)) {
                                addState2(&stateCost, state.rbPos, state.hmPos, newcost);
                                priority = newcost + estimateCost2(state.hmPos, gPos);
                                enGameQueue(&gq, newGameQueueElement2(state.rbPos, state.hmPos, priority));
                                addStateTrack2(gt, state, curState);
                            }
                        }
                }
            }
        }
    }
	
    return FALSE;
}

/*------------------ end of putstring() ---------------------------------------*/
/*------------------ end of print.c -------------------------------------------*/
