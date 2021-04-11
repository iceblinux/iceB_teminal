/* $Id: rasdoku.c,v 5.81 2014/06/30 06:36:34 sasa Exp $ */
/*21.01.2020	21.04.2000	Белых А.И.	rasdoku.c
Распечтка акта выполненых работ, счета на оплату
*/
#include        <errno.h>
#include	"buhg.h"

extern double	okrcn;  /*Округление цены*/
extern double   okrg1; /*Округление*/

void rasdoku(short dg,short mg,short gg, //Дата документа
int podr, //Подразделение
const char *nomdok, //Номер документа
short lnds, //Льготы по НДС
short vidr, //0-акт вып. работ 1-счет
FILE *ff,  //Указатель на файл
int tipz,  //1-приход 2-расход
int saldo) //0-без учета сальдо 1-с учетом
{
class iceb_tu_str nmo(""),nmo1("");  /*Наименование организации*/
class iceb_tu_str gor(""),gor1(""); /*Адрес*/
class iceb_tu_str pkod(""),pkod1(""); /*Почтовый код*/
class iceb_tu_str nmb(""),nmb1("");  /*Наименование банка*/
class iceb_tu_str mfo(""),mfo1("");  /*МФО*/
class iceb_tu_str nsh(""),nsh1("");  /*Номер счета*/
class iceb_tu_str inn(""),inn1("");  /*Индивидуальный налоговый номер*/
class iceb_tu_str grb(""),grb1("");  /*Город банка*/
class iceb_tu_str npnds(""),npnds1(""); /*Номер плательщика НДС*/
class iceb_tu_str gor_kontr("");
class iceb_tu_str gor_kontr1("");
class iceb_tu_str fio_otv(""); /*фамилия ответственног*/
class iceb_tu_str tel(""),tel1("");    /*Номер телефона*/
char		strsql[2048];
SQL_str         row,row1;
char		bros[1024],bros1[1024];
class iceb_tu_str kontr("");
int		metka,kodzap;
int		kolstr;
double		kolih,cena,cenasnd;
class iceb_tu_str naim("");
double		suma;
double		itogo=0.,itogonds=0.;
class iceb_tu_str naipod("");
int		podr1;
short		mvnpp;
class iceb_tu_str naimpr("");
class iceb_tu_str kodop("");
double		sumkor,bb;
class iceb_tu_str datop("");
short		d,m,g;
short		ds,ms,gs; //Дата сальдо
short		dlina;
class iceb_tu_str osnov("");
class iceb_tu_str dover("");
class iceb_tu_str datdov("");
class iceb_tu_str sherez("");
double		deb,kre;
class iceb_tu_str shetuslug("");
class iceb_tu_str kodkaz("");
class iceb_tu_str rrkaz("");
class iceb_tu_str naimnal("");
float		procent=0.;
class iceb_tu_str kodkontr00("00");
float pnds=0.;

SQLCURSOR curr;
/*
printw("\nrasdoku-Дата:%d.%d.%d Подр.:%d %s %d %d tipz=%d\n",
dg,mg,gg,podr,nomdok,lnds,vidr,tipz);
OSTANOV();
*/
class iceb_tu_str imaf_nast("uslnast.alx");
//imafn("uslnast.alx",&imaf_nast);

/*Читаем шапку документа*/
sprintf(strsql,"select kontr,kodop,sumkor,datop,osnov,pn,dover,datdov,sherez,k00 from Usldokum \
where datd='%04d-%02d-%02d' and podr=%d and nomd='%s' and tp=%d",
gg,mg,dg,podr,nomdok,tipz);

if(sql_readkey(&bd,strsql,&row,&curr) != 1)
 {
  VVOD SOOB(1);

  sprintf(strsql,"%s-%s!",__FUNCTION__,gettext("Не найден документ"));
  SOOB.VVOD_SPISOK_add_MD(strsql);

  sprintf(strsql,"%s:%d.%d.%d %s:%s %s:%d",
  gettext("Дата"),
  dg,mg,gg,
  gettext("Номер документа"),
  nomdok,
  gettext("Код подразделения"),
  podr);
  SOOB.VVOD_SPISOK_add_MD(strsql);

  soobshw(&SOOB,stdscr,-1,-1,0,1);
  return;
 }
pnds=atof(row[5]);
kontr.new_plus(row[0]);
kodop.new_plus(row[1]);
sumkor=atof(row[2]);

datop.new_plus(row[3]);

osnov.new_plus(row[4]);
dover.new_plus(row[6]);
datdov.new_plus(row[7]);
sherez.new_plus(row[8]);
kodkontr00.new_plus(row[9]);

usldopnal(tipz,kodop.ravno(),&procent,&naimnal);

if(tipz == 2)
 {
  if(SRAV("00",kodkontr00.ravno(),0) == 0)
    kodkontr00.new_plus(iceb_t_getk00(1,kodop.ravno()));
 }
//printw("\nkodkontr00=%s\n",kodkontr00);
//OSTANOV();

/*Читаем реквизиты организации свои */



sprintf(bros,"select * from Kontragent where kodkon='%s'",kodkontr00.ravno());
if(sql_readkey(&bd,bros,&row,&curr) != 1)
 {
  sprintf(strsql,"%s %s !",
  gettext("Не найден код контрагента"),kodkontr00.ravno());
  iceb_t_soob(strsql);
 }
else
 {
  if(row[16][0] == '\0')
   {
    if(polen(row[1],&nmo,1,'(') != 0)
     nmo.new_plus(row[1]);
   }
  else
    nmo.new_plus(row[16]);
    
  gor.new_plus(row[3]);
  pkod.new_plus(row[5]);
  nmb.new_plus(row[2]);
  mfo.new_plus(row[6]);
  nsh.new_plus(row[7]);
  inn.new_plus(row[8]);
  grb.new_plus(row[4]);
  npnds.new_plus(row[9]);
  tel.new_plus(row[10]);
  gor_kontr.new_plus(row[17]);
 } 

//Чтение настроек для бюджетных организаций

memset(bros,'\0',sizeof(bros));
if(iceb_t_poldan("Код казначейства",bros,imaf_nast.ravno()) == 0)
 if(bros[0] != '\0')
  {
   kodkaz.new_plus(bros);  
   iceb_t_poldan("Расчётный счёт казначейства",&rrkaz,imaf_nast.ravno());
   iceb_t_poldan("Регистрационный счёт в казначействе",&nsh,imaf_nast.ravno());
   iceb_t_poldan("МФО казначейства",&mfo,imaf_nast.ravno());
  }


/*Читаем реквизиты организации чужие*/
/*При внутреннем перемещении не читать*/
if(kontr.ravno_atoi() != 0 || kontr.ravno()[0] != '0')
 {

  sprintf(bros,"select * from Kontragent where kodkon='%s'",kontr.ravno());
  if(sql_readkey(&bd,bros,&row,&curr) != 1)
   {
    sprintf(strsql,"%s %s !",gettext("Не найден код контрагента"),kontr.ravno());
    iceb_t_soob(strsql);
   }
  else
   {
    if(row[16][0] == '\0')
      nmo1.new_plus(row[1]);
    else
      nmo1.new_plus(row[16]);
    gor1.new_plus(row[3]);
    pkod1.new_plus(row[5]);
    nmb1.new_plus(row[2]);
    mfo1.new_plus(row[6]);
    nsh1.new_plus(row[7]);
    inn1.new_plus(row[8]);
    grb1.new_plus(row[4]);
    npnds1.new_plus(row[9]);
    tel1.new_plus(row[10]);
    gor_kontr1.new_plus(row[17]);
    
   }
 }
SQLCURSOR cur;

sprintf(strsql,"select metka,kodzap,kolih,cena,ei,dnaim from Usldokum1 \
where datd='%d-%02d-%02d' and podr=%d and nomd='%s' and tp=%d order by metka,kodzap asc",
gg,mg,dg,podr,nomdok,tipz);
if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
  return;
 }

if(kolstr == 0)
  return;

int ots=4;
memset(bros,'\0',sizeof(bros)); 
if(iceb_t_poldan("Отступ от левого края",bros,imaf_nast.ravno()) == 0)
  ots=(short)ATOFM(bros);

fprintf(ff,"\x1b\x6C%c",ots); /*Установка левого поля*/
fprintf(ff,"\x1B\x33%c",30); /*Уменьшаем межстрочный интервал*/
fprintf(ff,"\x12"); /*Отмена ужатого режима*/
fprintf(ff,"\x1B\x4D"); /*12-знаков*/
if(vidr == 1)
  fprintf(ff,"\x1B\x45"); /*Включение режима выделенной печати*/


if(vidr == 0)
 {
  if(tipz == 1)
   fprintf(ff,gettext("\
    Кому                                 От кого\n"));
  if(tipz == 2)
   fprintf(ff,gettext("\
     От кого                             Кому\n"));
  fprintf(ff,"%-*.*s \x1B\x45N%s %-*.*s\x1B\x46\n",
  iceb_tu_kolbait(40,nmo.ravno()),iceb_tu_kolbait(40,nmo.ravno()),nmo.ravno(),
  kontr.ravno(),
  iceb_tu_kolbait(40,nmo1.ravno()),iceb_tu_kolbait(40,nmo1.ravno()),nmo1.ravno());


  for(int nom=40; nom < iceb_tu_strlen(nmo.ravno()) || nom < iceb_tu_strlen(nmo1.ravno()); nom+=40)
   {
    if(iceb_tu_strlen(nmo.ravno()) > nom)
     fprintf(ff,"%-*.*s ",
     iceb_tu_kolbait(40,iceb_tu_adrsimv(nom,nmo.ravno())),
     iceb_tu_kolbait(40,iceb_tu_adrsimv(nom,nmo.ravno())),
     iceb_tu_adrsimv(nom,nmo.ravno()));
    else
     fprintf(ff,"%40s ","");
     
    if(iceb_tu_strlen(nmo1.ravno()) > nom)
      fprintf(ff,"\x1B\x45%-.*s\x1B\x46",
      iceb_tu_kolbait(40,iceb_tu_adrsimv(nom,nmo1.ravno())),
      iceb_tu_adrsimv(nom,nmo1.ravno()));

    fprintf(ff,"\n");
   }

 }

if(vidr == 1)
 {
  if(tipz == 1)
    fprintf(ff,gettext("\
    Плательщик                            Получатель\n"));

  if(tipz == 2)
    fprintf(ff,gettext("\
    Получатель                            Плательщик\n"));

  fprintf(ff,"%-*.*s N%s %-*.*s\n",
  iceb_tu_kolbait(40,nmo.ravno()),iceb_tu_kolbait(40,nmo.ravno()),nmo.ravno(),
  kontr.ravno(),
  iceb_tu_kolbait(40,nmo1.ravno()),iceb_tu_kolbait(40,nmo1.ravno()),nmo1.ravno());

  for(int nom=40; nom < iceb_tu_strlen(nmo.ravno()) || nom < iceb_tu_strlen(nmo1.ravno()); nom+=40)
   {
    if(iceb_tu_strlen(nmo.ravno()) > nom)
     fprintf(ff,"%-*.*s ",
     iceb_tu_kolbait(40,iceb_tu_adrsimv(nom,nmo.ravno())),
     iceb_tu_kolbait(40,iceb_tu_adrsimv(nom,nmo.ravno())),
     iceb_tu_adrsimv(nom,nmo.ravno()));
    else
     fprintf(ff,"%40s ","");
     
    if(iceb_tu_strlen(nmo1.ravno()) > nom)
      fprintf(ff,"%-.*s",
      iceb_tu_kolbait(40,iceb_tu_adrsimv(nom,nmo1.ravno())),
      iceb_tu_adrsimv(nom,nmo1.ravno()));

    fprintf(ff,"\n");
   }

 }

memset(bros,'\0',sizeof(bros));
memset(bros1,'\0',sizeof(bros1));
sprintf(bros,"%s %s",gettext("Адрес"),gor.ravno());
sprintf(bros1,"%s %s",gettext("Адрес"),gor1.ravno());

fprintf(ff,"%-*.*s %-*.*s\n",
iceb_tu_kolbait(40,bros),iceb_tu_kolbait(40,bros),bros,
iceb_tu_kolbait(40,bros1),iceb_tu_kolbait(40,bros1),bros1);

if(iceb_tu_strlen(bros) > 40 || iceb_tu_strlen(bros1) > 40)
  fprintf(ff,"%-*.*s %-*.*s\n",
  iceb_tu_kolbait(40,iceb_tu_adrsimv(40,bros)),iceb_tu_kolbait(40,iceb_tu_adrsimv(40,bros)),iceb_tu_adrsimv(40,bros),
  iceb_tu_kolbait(40,iceb_tu_adrsimv(40,bros1)),iceb_tu_kolbait(40,iceb_tu_adrsimv(40,bros1)),iceb_tu_adrsimv(40,bros1));

sprintf(bros,"%s %s",gettext("Код ЕГРПOУ"),pkod.ravno());
sprintf(bros1,"%s %s",gettext("Код ЕГРПOУ"),pkod1.ravno());
fprintf(ff,"%-*s %s\n",iceb_tu_kolbait(40,bros),bros,bros1);
/***********************
sprintf(bros,"%s %s %s %s",gettext("Р/С"),nsh.ravno(),
gettext("МФО"),mfo.ravno());

sprintf(bros1,"%s %s %s %s",gettext("Р/С"),nsh1.ravno(),
gettext("МФО"),mfo1.ravno());
******************************/
sprintf(bros,"%s %s",gettext("Р/С"),nsh.ravno());

sprintf(bros1,"%s %s",gettext("Р/С"),nsh1.ravno());

fprintf(ff,"%-*s %s\n",iceb_tu_kolbait(40,bros),bros,bros1);

memset(bros,'\0',sizeof(bros));
memset(bros1,'\0',sizeof(bros1));

sprintf(bros,"%s %s",gettext("в"),nmb.ravno());
sprintf(bros1,"%s %s",gettext("в"),nmb1.ravno());

if(grb.getdlinna() > 1)
 {
  sprintf(strsql," %s",grb.ravno());
  strcat(bros,strsql);
 }

if(grb1.getdlinna() > 1)
 {
  sprintf(strsql," %s",grb1.ravno());
  strcat(bros1,strsql);
 }

//sprintf(bros,"%s %s %s %s",gettext("в"),nmb.ravno(),gettext("гор."),grb.ravno());
//sprintf(bros1,"%s %s %s %s",gettext("в"),nmb1.ravno(),gettext("гор."),grb1.ravno());

fprintf(ff,"%-*.*s %.*s\n",
iceb_tu_kolbait(40,bros),iceb_tu_kolbait(40,bros),bros,
iceb_tu_kolbait(45,bros1),bros1);

if(iceb_tu_strlen(bros) > 40 || iceb_tu_strlen(bros1) > 45)
 fprintf(ff,"%-*.*s %.*s\n",
 iceb_tu_kolbait(40,iceb_tu_adrsimv(40,bros)),iceb_tu_kolbait(40,iceb_tu_adrsimv(40,bros)),iceb_tu_adrsimv(40,bros),
 iceb_tu_kolbait(45,iceb_tu_adrsimv(45,bros1)),iceb_tu_adrsimv(45,bros1));
 
if(kodkaz.ravno()[0] != '\0')
 {

  fprintf(ff,"%s %s %s\n",
  gettext("Код"),
  gettext("казначейства"),kodkaz.ravno());
  

  fprintf(ff,"%s %s %s\n",gettext("Р/С"),
  gettext("казначейства"),rrkaz.ravno());
  
 }
sprintf(bros,"%s %s",
gettext("Налоговый номер"),inn.ravno());

sprintf(bros1,"%s %s",
gettext("Налоговый номер"),inn1.ravno());

fprintf(ff,"%-*s %s\n",iceb_tu_kolbait(40,bros),bros,bros1);

sprintf(bros,"%s %s",
gettext("Номер сви-ства плательщика НДС"),npnds.ravno());
sprintf(bros1,"%s %s",
gettext("Номер сви-ства плательщика НДС"),npnds1.ravno());
fprintf(ff,"%-*s %s\n",iceb_tu_kolbait(40,bros),bros,bros1);

sprintf(bros,"%s %s",gettext("Телефон"),tel.ravno());
sprintf(bros1,"%s %s",gettext("Телефон"),tel1.ravno());
fprintf(ff,"%-*s %s\n",iceb_tu_kolbait(40,bros),bros,bros1);

fprintf(ff,"\x1B\x50"); /*10-знаков*/

/*Читаем наименование операции*/
if(tipz == 1)
 strcpy(bros,"Usloper1");
if(tipz == 2)
 strcpy(bros,"Usloper2");

mvnpp=0;

sprintf(strsql,"select naik,vido from %s where kod='%s'",bros,kodop.ravno());
if(sql_readkey(&bd,strsql,&row,&curr) != 1)
 {
  sprintf(strsql,"%s-%s %s!",__FUNCTION__,gettext("Не найден код операции"),kodop.ravno());
  iceb_t_soob(strsql);
 }
else
 {

  mvnpp=atoi(row[1]);
  naimpr.new_plus(row[0]);
  
 }

/*Читаем наименование подразделения*/
sprintf(strsql,"select naik,np,fo from Uslpodr where kod=%d",podr);

if(sql_readkey(&bd,strsql,&row,&curr) != 1)
 {
  sprintf(strsql,"%s-%s %d !",__FUNCTION__,gettext("Не найден код подразделения"),podr);
  iceb_t_soob(strsql);
 }
else
 {
  naipod.new_plus(row[0]);
  fio_otv.new_plus(row[2]);
  if(row[1][0] != '\0')
   {
    gor_kontr.new_plus(row[1]);
    if(gor_kontr1.getdlinna() <= 1)
     gor_kontr1.new_plus(row[1]);
   }
 }

if(vidr == 0 || vidr == 2)
  fprintf(ff,"\n%s",naipod.ravno());

if(mvnpp == 1) /*Внутреннее перемещение*/
 {
  if(kontr.ravno_atoi() == 0 && POLE(kontr.ravno(),bros,2,'-') == 0 && kontr.ravno()[0] == '0')
   {
    POLE(kontr.ravno(),bros,2,'-');
    podr1=(int)ATOFM(bros);
    /*Читаем наименование склада*/
    sprintf(strsql,"select naik from Uslpodr where kod=%d",
    podr1);

    if(sql_readkey(&bd,strsql,&row,&curr) != 1)
     {
      sprintf(strsql,"%s %d !",gettext("Не найден код подразделения"),podr1);
      iceb_t_soob(strsql);
      naipod.new_plus("");
     }
    else
      naipod.new_plus(row[0]);

    if(tipz == 1)
       fprintf(ff,"\n%s: %s %d <= %s %d %s",
       naimpr.ravno(),gettext("Подразделение"),podr,
       gettext("Подразделение"),podr1,naipod.ravno());
    if(tipz == 2)
       fprintf(ff,"\n%s: %s %d => %s %d %s",
       naimpr.ravno(),gettext("Подразделение"),podr,
       gettext("Подразделение"),podr1,naipod.ravno());
   }
 }

fprintf(ff,"\n%s",naimpr.ravno());
fprintf(ff,"\n");

if(tipz == 1)
 fprintf(ff,"\n%s\n",gor_kontr1.ravno());
if(tipz == 2) 
 fprintf(ff,"\n%s\n",gor_kontr.ravno());

sprintf(bros,"N%s",nomdok);  

if(vidr == 0)
 {
  fprintf(ff,"\
                %s  %s\n",gettext("АКТ ВЫПОЛНЕНЫХ РАБОТ"),bros);
 }

if(vidr == 1)
  fprintf(ff,"\
                %s %s\n",
  gettext("С ч е т"),bros);

memset(bros,'\0',sizeof(bros));
mesc(mg,1,bros);
fprintf(ff,"\
                  %d %s %d%s\n",
dg,bros,gg,gettext("г."));

if(osnov.ravno()[0] != '\0')
  fprintf(ff,"%s: %s\n",gettext("Основание"),osnov.ravno());

if(dover.getdlinna() > 1)
  fprintf(ff,"%s: %s %s %s\n",
  gettext("Доверенность"),
  dover.ravno(),
  gettext("от"),
  datdov.ravno());

if(sherez.getdlinna() > 1)
  fprintf(ff,"%s: %s\n",gettext("Через кого"),sherez.ravno());
  
if(datop.ravno()[0] != '0')
 {
  rsdat(&d,&m,&g,datop.ravno(),2);
  if(d == 0 || SRAV1(g,m,d,gg,mg,dg) <= 0)
   {
    if(vidr == 0)
     fprintf(ff,"\x1B\x45%s: %d.%d.%d%s\x1B\x45\x1b\x46\n",
     gettext("Конечная дата расчёта"),d,m,g,
     gettext("г."));
    if(vidr == 1)
     fprintf(ff,"\x1B\x45%s %d.%d.%d%s\x1B\x45\x1b\x46\n",
     gettext("Счёт действителен до"),d,m,g,
     gettext("г."));
    
   }
 }
if(vidr == 1)
 {
  fprintf(ff,"\x1B\x46"); /*Выключение режима выделенной печати*/
  fprintf(ff,"\x1B\x47"); /*Включение режима двойного удара*/
 }

//fprintf(ff,"\x1B\x4D"); /*12-знаков*/
//fprintf(ff,"\x0F"); /*Включение ужатого режима печати*/
 
if(lnds == 0 )
 {
  fprintf(ff,"\x0F"); /*Включение ужатого режима печати*/
  fprintf(ff,"\
------------------------------------------------------------------------------------------------------\n");
  fprintf(ff,gettext("\
 Код |           Наименование услуг            |Едини.|Количество| Цена с   |  Цена без   |Сумма без |\n\
     |                                         |измер.|          |   НДС    |     НДС     |   НДС    |\n"));
  fprintf(ff,"\
------------------------------------------------------------------------------------------------------\n");
/********
12345 12345678901234567890123456789012345678901 123456 1234567890 1234567890 1234567890123 1234567890     
***********/
 }
else
 {
  fprintf(ff,"\x1B\x4D"); /*12-знаков*/
  fprintf(ff,"\
------------------------------------------------------------------------------------------\n");
  fprintf(ff,gettext("\
 Код |               Наименование услуг                |Един|Количе-| Цена без |Сумма без|\n"));
  fprintf(ff,gettext("\
     |                                                 |изм.|  ство |   НДС    |  НДС    |\n"));
  fprintf(ff,"\
------------------------------------------------------------------------------------------\n");
      
 }
itogo=0;

if(saldo == 1)
 {
  deb=kre=0.;
  ds=1; ms=mg; gs=gg;
  dpm(&ds,&ms,&gs,2);
  naim.new_plus("");    

  //Узнаем счёт учета операции
  sprintf(bros,"Usloper%d",tipz);
  sprintf(strsql,"select shet from %s where kod='%s'",bros,kodop.ravno());
  if(sql_readkey(&bd,strsql,&row,&curr) == 1)
    shetuslug.new_plus(row[0]);
  else
    shetuslug.new_plus("");

  if(shetuslug.ravno()[0] != '\0')
   {
    sprintf(strsql,"%s 1.%d.%d",gettext("Сальдо на"),mg,gg);
    naim.new_plus(strsql);
    memset(strsql,'\0',sizeof(strsql));
    salork(shetuslug.ravno(),kontr.ravno(),&deb,&kre,0,strsql,strsql,"\0",1,1,gs,ds,ms,gs,"\0",0,strsql,strsql);
    deb-=kre;
   }
  else
    naim.new_plus(gettext("Не введён счёт услуг"));
   
  bb=deb-deb*pnds/(100+pnds);
  suma=okrug(bb,okrg1);
  cena=okrug(bb,okrcn);


  if(lnds == 0)
   fprintf(ff,"%5s %-*.*s %-6s %10.10g %10.2f %13.13g %10.2f\n",
   " ",
   iceb_tu_kolbait(41,naim.ravno()),iceb_tu_kolbait(41,naim.ravno()),naim.ravno(),
   " ",1.,deb,cena,suma);
  else  
   fprintf(ff,"%5s %-*.*s %-4s %7.7g %10.10g %9.2f\n",
   " ",
   iceb_tu_kolbait(49,naim.ravno()),iceb_tu_kolbait(49,naim.ravno()),naim.ravno(),
   " ",1.,cena,suma);
  itogo+=suma;

  suma=kre=0.;

  naim.new_plus("");
  if(shetuslug.ravno()[0] != '\0')
   {
    sprintf(strsql,"%s %d.%d",gettext("Оплата в"),mg,gg);
    naim.new_plus(strsql);
    //Смотрим сумму оплат за месяц если она была
    sprintf(strsql,"select kre from Prov where datp>='%d-%d-01' and \
datp<='%d-%d-31' and kodkon='%s' and sh='%s' and kre <> 0.",
    gg,mg,gg,mg,kontr.ravno(),shetuslug.ravno());
    SQLCURSOR cur1;    
    if((kolstr=cur1.make_cursor(&bd,strsql)) < 0)
       msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
    while(cur1.read_cursor(&row1) != 0)
     kre+=atof(row1[0]);

   }
  bb=kre-kre*pnds/(100+pnds);
  suma=okrug(bb,okrg1)*-1;
  cena=okrug(bb,okrcn)*-1;


  if(lnds == 0)
   {
    fprintf(ff,"%5s %-*.*s %-6s %10.10g %10.2f %13.13g %10.2f\n",
    " ",
    iceb_tu_kolbait(41,naim.ravno()),iceb_tu_kolbait(41,naim.ravno()),naim.ravno(),
    " ",1.,kre*-1,cena,suma);
    if(iceb_tu_strlen(naim.ravno()) > 41)
      fprintf(ff,"%5s %s\n"," ",iceb_tu_adrsimv(41,naim.ravno()));
   }
  else  
   {
    fprintf(ff,"%5s %-*.*s %-4s %7.7g %10.10g %9.2f\n",
    " ",
    iceb_tu_kolbait(49,naim.ravno()),iceb_tu_kolbait(49,naim.ravno()),naim.ravno(),
    " ",1.,cena,suma);

    if(iceb_tu_strlen(naim.ravno()) > 49)
      fprintf(ff,"%5s %s\n",
      " ",iceb_tu_adrsimv(49,naim.ravno()));
    
   }
  itogo+=suma;

 }

while(cur.read_cursor(&row) != 0)
 {

  metka=atoi(row[0]);
  kodzap=atoi(row[1]);
  kolih=atof(row[2]);
  cena=atof(row[3]);
  cena=okrug(cena,okrcn);
  suma=cena*kolih;
  suma=okrug(suma,okrg1);
  itogo+=suma;
  cenasnd=cena+cena*pnds/100.;
  cenasnd=okrug(cenasnd,okrg1);
  
  /*Читаем наименование услуги*/
  naim.new_plus("");

  if(metka == 0)
   sprintf(strsql,"select naimat from Material where kodm=%d",
   kodzap);

  if(metka == 1)
   sprintf(strsql,"select naius from Uslugi where kodus=%d",
   kodzap);

  if(sql_readkey(&bd,strsql,&row1,&curr) != 1)
   {
    printw("%s\n",strsql);
    if(metka == 0)
     printw("%s %d !\n",gettext("Не найден код материалла"),kodzap);
    if(metka == 1)
     printw("%s %d !\n",gettext("Не найден код услуги"),kodzap);
    OSTANOV();
   }
  else
   {
    naim.new_plus(row1[0]);
    if(row[5][0] != '\0')
     {
      naim.plus(" ",row[5]);
     }    
   }
  if(lnds == 0)
   {
    fprintf(ff,"%5d %-*.*s %-*s %10.10g %10.2f %13.13g %10.2f\n",
    kodzap,
    iceb_tu_kolbait(41,naim.ravno()),iceb_tu_kolbait(41,naim.ravno()),naim.ravno(),
    iceb_tu_kolbait(6,row[4]),row[4],
    kolih,cenasnd,cena,suma);
    if(iceb_tu_strlen(naim.ravno()) > 41)
     fprintf(ff,"%5s %s\n"," ",iceb_tu_adrsimv(41,naim.ravno()));
   }
  else  
   {
    fprintf(ff,"%5d %-*.*s %-*s %7.7g %10.10g %9.2f\n",
    kodzap,
    iceb_tu_kolbait(49,naim.ravno()),iceb_tu_kolbait(49,naim.ravno()),naim.ravno(),
    iceb_tu_kolbait(4,row[4]),row[4],
    kolih,cena,suma);
    if(iceb_tu_strlen(naim.ravno()) > 49)
     fprintf(ff,"%5s %s\n"," ",iceb_tu_adrsimv(49,naim.ravno()));
   }
}


if(lnds == 0)
 {
  dlina=90;
  fprintf(ff,"\
------------------------------------------------------------------------------------------------------\n");
  fprintf(ff,"%*s:%10.2f\n",iceb_tu_kolbait(dlina,gettext("Итого")),gettext("Итого"),itogo);
 }
else
 {
  dlina=78;
  fprintf(ff,"\
------------------------------------------------------------------------------------------\n");
  fprintf(ff,"%*s:%9.2f\n",iceb_tu_kolbait(dlina+1,gettext("Итого")),gettext("Итого"),itogo);
 }
 
if(mvnpp == 0) /*Внешнее перемещение*/
 {
  itogonds=dok_end_usl(tipz,dg,mg,gg,nomdok,podr,dlina,sumkor,itogo,pnds,lnds,procent,naimnal.ravno(),ff);

 }

memset(strsql,'\0',sizeof(strsql));
preobr(itogonds,strsql,0);

fprintf(ff,"%s:%s\n",gettext("Сумма прописью"),strsql);
if(lnds != 0 && mvnpp == 0)
  fprintf(ff,"%s\n",gettext("Без НДС"));
  
fprintf(ff,"\x1B\x32"); /*Межстрочный интервал 1/6 дюйма*/

class iceb_t_fioruk_rk rukov;
class iceb_t_fioruk_rk glavbuh;
iceb_t_fioruk(1,&rukov);
iceb_t_fioruk(2,&glavbuh);

if(vidr == 1)
 {
  if(tipz == 2)
   fprintf(ff,"\n\
%*s________________%s\n\n\
%*s________________%s\n",
   iceb_tu_kolbait(20,gettext("Руководитель")),gettext("Руководитель"),
   rukov.famil_inic.ravno(),
   iceb_tu_kolbait(20,gettext("Головний бухгалтер")),gettext("Головний бухгалтер"),
   glavbuh.famil_inic.ravno());
  else
   fprintf(ff,"\n\
%*s________________\n\n\
%*s________________\n",
   iceb_tu_kolbait(20,gettext("Руководитель")),gettext("Руководитель"),
   iceb_tu_kolbait(20,gettext("Головний бухгалтер")),gettext("Головний бухгалтер"));
  
 }
 
if(vidr == 0)
 {
  fprintf(ff,"\x0F"); /*Включение ужатого режима печати*/
//  fprintf(ff,"\x1B\x4D"); //12-знаков
/***************
  memset(bros,'\0',sizeof(bros));
  iceb_t_poldan("В акте выполненых работ подписи только исполнителя и заказчика",bros,imaf_nast.ravno());
  if(SRAV(bros,"Вкл",1) != 0)
********************/
  if(iceb_t_poldan_vkl("В акте выполненых работ подписи только исполнителя и заказчика",imaf_nast.ravno()) != 0)
   {  
    fprintf(ff,"\n\
%*s________________%-*s %*s______________\n\n\
%*s________________%-*s %*s______________\n",
     iceb_tu_kolbait(20,gettext("Руководитель")),gettext("Руководитель"),
     iceb_tu_kolbait(20,rukov.famil_inic.ravno()),
     rukov.famil_inic.ravno(),
     iceb_tu_kolbait(20,gettext("Руководитель")),gettext("Руководитель"),
     iceb_tu_kolbait(20,gettext("Головний бухгалтер")),gettext("Головний бухгалтер"),
     iceb_tu_kolbait(20,glavbuh.famil_inic.ravno()),
     glavbuh.famil_inic.ravno(),
     iceb_tu_kolbait(20,gettext("Головний бухгалтер")),gettext("Головний бухгалтер"));
    }

  if(tipz == 1)
    fprintf(ff,"\n\
%*s________________%-*s %*s______________\n",
    iceb_tu_kolbait(20,gettext("Заказчик")),gettext("Заказчик"),
    iceb_tu_kolbait(20,sherez.ravno()),sherez.ravno(),
    iceb_tu_kolbait(20,gettext("Исполнитель")),gettext("Исполнитель"));

  if(tipz == 2)
    fprintf(ff,"\n\
%*.*s________________%-*.*s %*s______________%s\n",
    iceb_tu_kolbait(20,gettext("Исполнитель")),
    iceb_tu_kolbait(20,gettext("Исполнитель")),
    gettext("Исполнитель"),
    iceb_tu_kolbait(20,fio_otv.ravno()),
    iceb_tu_kolbait(20,fio_otv.ravno()),
    fio_otv.ravno(),
    iceb_tu_kolbait(20,gettext("Заказчик")),gettext("Заказчик"),sherez.ravno());
      
 }
 
fprintf(ff,"\x1B\x50"); /*10-знаков*/
fprintf(ff,"\x12"); /*Отмена ужатого режима печати*/

//Если есть приложение - распечатываем
sprintf(strsql,"select zapis from Usldokum3 where podr=%d and god=%d \
and tp=%d and nomd='%s'",podr,gg,tipz,nomdok);
if((kolstr=cur.make_cursor(&bd,strsql)) < 0)
 {
  msql_error(&bd,gettext("Ошибка создания курсора !"),strsql);
 }

if(kolstr != 0)
 fprintf(ff,"\n\n\n%s:\n",gettext("Приложение к документу"));

while(cur.read_cursor(&row) != 0)
  fprintf(ff,"%s\n",row[0]);

//if(vidr == 1)
 fprintf(ff,"\x1B\x48"); /*Выключение режима двойного удара*/


fprintf(ff,"\x12"); /*Отмена ужатого режима печати*/
if(mvnpp == 0 && tipz == 2) /*Внешнее перемещение*/
 {
  if(vidr != 0) //Для акта выполненых работ не надо
   if(nalndoh() == 0)
    fprintf(ff,"%s\n",gettext("Предприятие является плательшиком налога на доход на общих основаниях."));

  fprintf(ff,"\n\x0E"); /*Включение режима удвоенной ширины*/
 // fprintf(ff,"\n%7s%s\n"," ",gettext("Благодарим за сотрудничество !"));
  fprintf(ff,gettext("Благодарим за сотрудничество !"));
  fprintf(ff,"\x14"); /*Выключение режима удвоенной ширины*/
 }
  
podpis(ff);
fprintf(ff,"\x1b\x6C%c",1); /*Установка левого поля*/


}
