/*******************************************************************************
 *
 *		システム名：割込み用ライブラリヘッダファイル
 *
 *		Copyright (c) 2010 Morimoto Yasushi
 *
 *------------------------------------------------------------------------------
 *
 *		$RCSfile: interrupt.h,v $		$Revision: 1.1 $
 *
 *		$設　計: 森本泰司			$日付: 2007/10/13
 *		$Author: Y.Morimoto $		$Date: 2010-12-21 03:48:42+09 $
 *
 *******************************************************************************/

#ifndef _PRINT_H
#define _PRINT_H

#define DELAY_TIME 800

static char model[8] = "Start";
static int start_x=0;
static int start_y=0;
static int patient_x=0;
static int patient_y=0;
static int goal_x=0;
static int goal_y=0;
static char dir[2]="N";
static int run_time=0;

static int start=0;
void show(void);
void put_str_start(void);
void put_str_patient(void);
void put_str_goal(void);
void put_reset(void);
void put_alt(void);
void put_dir(void);
void put_x(void);
void put_y(void);
#endif	/* _PRINT_H */
/*------------------ end of interrupt.h ---------------------------------------*/
