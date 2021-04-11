/*$Id: iceb_tu_ekrs.c,v 5.5 2013/05/17 14:56:40 sasa Exp $*/
/*17.02.2009	20.09.2006	Белых А.И.	iceb_tu_ekrs.c
Экранирование символов в строке
*/
#include <string.h>

void iceb_tu_ekrs(char *stroka,int razmer,char ekran,const char *simv)
{
char stroka_tmp[razmer];
strncpy(stroka_tmp,stroka,razmer);
int dlina_simv=strlen(simv);
int i_stroka=0;

for(int i=0; i < razmer; i++)
 {
  for(int ii=0; ii < dlina_simv; ii++)
    if(simv[ii] == stroka_tmp[i])
     {
      stroka[i_stroka++]=ekran;

      if(i_stroka+1 >= razmer)
        return;

      stroka[i_stroka++]=stroka_tmp[i];

      if(i_stroka+1 >= razmer)
        return;
      
     }
    else
      stroka[i_stroka++]=stroka_tmp[i];

  if(i_stroka+1 >= razmer)
    return;
 }




}
