/*$Id: dpm.c,v 5.5 2013/05/17 14:56:40 sasa Exp $*/
/*16.02.2009    21.04.1997      Белых А.И.      dpm.c
Вычисление нужной даты
*/
#include	"util.h"

void            dpm(short *d,short *m,short *g,short mt)
/*
short           *d,*m,*g;
short           mt; 1-увеличить на день 2-уменьшить на день
		      3-увеличить на месяц 4-уменьшить на месяц
		      5-получить последний день текущего месяца
		      */
{

if(mt == 1)
 {
   *d=*d+1;
   if( *m == 1 || *m == 3 || *m == 5 || *m == 7 || *m == 8 || *m == 10 || *m == 12)
    if(*d > 31)
     {
      *d=1; *m=*m+1;
     }
   if( *m == 4 || *m == 6 || *m == 9 || *m == 11)
    if(*d > 30)
     {
      *d=1; *m=*m+1;
     }

   if(*m == 2)
    {
     int metka_vis_god=0;
//    if(*g % 4 == 0 && *g % 100 != 0 || *g % 400 == 0)
    if(*g % 4 == 0 && *g % 100 != 0)
     {
      metka_vis_god=1;
     }

    if( *g % 400 == 0)
      metka_vis_god=1;

    if(metka_vis_god == 1)
     {
      if(*d > 29)   /*Год високосный*/
       {
	*d=1; *m=*m+1;
       }
     }
    else
     {
      if(*d > 28)   /*Год не високосный*/
       {
	*d=1; *m=*m+1;
       }
     }
    }
   if(*m > 12)
    {
     *m=1;
     *g=*g+1;
    }
 }

if(mt == 2)
 {

   *d=*d-1;
   if(*d <= 0)
    {
     if(*m == 0 || *m == 1 || *m == 2 || *m == 4 || *m == 6 || *m == 8 || *m == 9 || *m == 11)
      {
       *d=31;
      }
     if( *m == 5 || *m == 7 || *m == 10 || *m == 12)
      {
       *d=30;
      }
     if( *m == 3)
      {
       int metka_vis_god=0;      
//        if(*g % 4 == 0 && *g % 100 != 0 || *g % 400 == 0)
        if(*g % 4 == 0 && *g % 100 != 0)
         {
          metka_vis_god=1;
         }
        if(*g % 400 == 0)
         metka_vis_god=1;
        if(metka_vis_god == 1)
         {
          /*Год високосный*/
          *d=29;
         }
        else
         {
          /*Год не високосный*/
          *d=28;
         }
      }
     *m=*m-1;
     if(*m <= 0)
      {
       *m=12; *g=*g-1;
      }

    }
 }

if(mt == 3)
 {
  *m=*m+1;
  if(*m > 12)
   {
    *m=1;
    *g=*g+1;
   }
 }

if(mt == 4)
 {
  *m=*m-1;
  if(*m <= 0 )
   {
    *m=12;
    *g=*g-1;
   }
 }
if(mt == 5)
 {
   if( *m == 1 || *m == 3 || *m == 5 || *m == 7 || *m == 8 || *m == 10 || *m == 12)
     *d=31;
   if( *m == 4 || *m == 6 || *m == 9 || *m == 11)
     *d=30;
   if(*m == 2)
    {
     int metka_vis_god=0;
//      if(*g % 4 == 0 && *g % 100 != 0 || *g % 400 == 0)
      if(*g % 4 == 0 && *g % 100 != 0)
       {
        metka_vis_god=1;
       }
      if(*g % 400 == 0)
       metka_vis_god=1;
       
      if(metka_vis_god == 1)
       *d=29;
      else
        *d=28;
    }
 }
}






