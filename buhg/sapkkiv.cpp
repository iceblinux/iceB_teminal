/* $Id: sapkkiv.c,v 5.5 2013/05/17 14:56:11 sasa Exp $ */
/*20.07.2003    01.02.1994      Белых А.И.      sapkkiv.c
Шапка инвентразации и приема - передачи материалов
*/
#include	"buhg.h"


void		sapkkiv(int *kolstr,int kollist,FILE *ff2)
{

*kolstr+=5;

fprintf(ff2,"%100s %s%d\n\
---------------------------------------------------------------------------------------------------------------------------------\n",
" ",gettext("Лист N"),kollist);
fprintf(ff2,gettext("\
  N |            Наименование               |Едини.|  Цена    |Наличие по бух.учету | Фактическое наличие |     Разница         |\n"));
fprintf(ff2,gettext("\
    |                                       |измер.|          |Количество|  Сумма   |Количество|  Сумма   |Количество|  Сумма   |\n"));
fprintf(ff2,"\
---------------------------------------------------------------------------------------------------------------------------------\n");
 
}
