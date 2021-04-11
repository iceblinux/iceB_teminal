/*$Id: iceb_t_sortkk.c,v 5.9 2013/08/13 05:50:23 sasa Exp $*/
/*19.04.2011	17.09.2009	Белых А.И.	iceb_t_sortkk.c
Сортировка списка контрагентов
*/
#include "buhl.h"

extern SQL_baza bd;


int iceb_t_sortkk(int metkasort, /*0-без сортировки 1-по коду в символьном виде 2-по коду в цифровом виде 3-по наименованию 4 - группе контрагента*/
class SPISOK *skontr, /*Список контрагентов для сортировки. Если второй список не задан то здесь будет отсортировонный список контрагентов*/
class SPISOK *skontr_sort) /*Если не NULL - отсортированный список котнтрагентов*/
{
class SPISOK skontr_tmp;

int kolih_kontr=skontr->kolih();

if(metkasort != 0)
 {
  SQL_str rowtmp;
  class SQLCURSOR curtmp;
  
  class iceb_t_tmptab tabtmp;
  const char *imatmptab={"icebsortkk"};

  char zaprostmp[512];
  memset(zaprostmp,'\0',sizeof(zaprostmp));

  sprintf(zaprostmp,"CREATE TEMPORARY TABLE %s (\
kontr char(24) not null,\
kontri int not null,\
grup int not null,\
naim char(255) not null) ENGINE = MYISAM",imatmptab);

  if(tabtmp.create_tab(imatmptab,zaprostmp) != 0)
   {
    return(0);
   }  

/*Делаем список контрагентов отсортированный*/
  char strsql[512];
  class SQLCURSOR cur;
  SQL_str row;
  for(int nomer_kontr=0; nomer_kontr < kolih_kontr; nomer_kontr++)
   {
    sprintf(strsql,"select naikon,grup from Kontragent where kodkon='%s'",skontr->ravno(nomer_kontr));
    
    if(readkey(strsql,&row,&cur) != 1)
     {

      sprintf(strsql,"iceb_sortkk:%s %s %s !",
      gettext("Не найден код контрагента"),row[0],
      gettext("в общем списке контрагентов"));
      iceb_t_soob(strsql);
      continue;
     }
//    fprintf(ff_tmp,"%s|%-2s|%s|\n",skontr->ravno(nomer_kontr),row[1],row[0]);  

    sprintf(strsql,"insert into %s values ('%s',%d,%d,'%s')",
    imatmptab,
    skontr->ravno(nomer_kontr),
    skontr->ravno_atoi(nomer_kontr),
    atoi(row[1]),iceb_tu_sqlfiltr(row[0]));  

    sql_zapis(strsql,1,1);    
    
   }



  if(metkasort == 1)
    sprintf(strsql,"select * from %s order by kontr asc",imatmptab);
  if(metkasort == 2)
    sprintf(strsql,"select * from %s order by kontri asc,kontr asc",imatmptab);
  if(metkasort == 3)
    sprintf(strsql,"select * from %s order by naim asc",imatmptab);
  if(metkasort == 4)
    sprintf(strsql,"select * from %s order by grup asc,naim asc",imatmptab);

  int kolstrtmp=0;
  if((kolstrtmp=curtmp.make_cursor(&bd,strsql)) < 0)
   {
    msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
    return(0);
   }


  while(curtmp.read_cursor(&rowtmp) != 0)
   {
    if(skontr_sort != NULL)
      skontr_sort->plus(rowtmp[0]);
    else
      skontr_tmp.plus(rowtmp[0]);
   }

  if(skontr_sort == NULL)
   {
    skontr->free_class();
    for(int nomer_kontr=0; nomer_kontr < kolih_kontr; nomer_kontr++)
     skontr->plus(skontr_tmp.ravno(nomer_kontr));
   }  
 }
else
 {
  if(skontr_sort != NULL)
   for(int nomer=0; nomer < kolih_kontr; nomer++)
      skontr_sort->plus(skontr->ravno(nomer));
 }



return(0);
}
