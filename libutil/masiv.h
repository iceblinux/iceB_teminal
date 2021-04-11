/*$Id: masiv.h,v 5.8 2013/08/13 05:50:32 sasa Exp $*/
/*14.01.2019 	02.02.2001	Белых А.И.	masiv.h
описания классов для работы с динамическими массивами
*/

class masiv_din_double
 {
private:
  int    optimiz;  //0-записывать в масив без оптимизации 1-с оптимизацией
  int	 kolz;    //Количество элементов массива
  double *masiv_double;  //Массив 
  
public:

  masiv_din_double(int); //Конструктор c созданием массива указанного размера
  masiv_din_double(); //Конструктор
  ~masiv_din_double();      //Деструктор

  int find(double); //Найти в какой ячейке искомое значение
  int find(const char*); //Найти в какой ячейке искомое значение

  int plus(double,int);      //Записать значение просумировав его с предыдущим и если нужно увеличить размер массива
  int plus_str(int);  /*Увеличить размер массива на заданное количество елементов*/
  int plus(double);      //Записать значение в добавленную ячейку массива
  int plus(const char*);      //Записать значение в добавленную ячейку массива
  int new_plus(double,int);      //Записать значение не просумировав его с предыдущим и если нужно увеличить размер массива

  int make_class(int);     //создать массив нужного размера
  void print_class(); //вывести содержимое массива для контроля

  double ravno(int elem); //вернуть значение нужного елемента

  void clear_class(); //Обнулить массив
  void free_class(); //Удалить массив
  double suma(); //Вернуть сумму элементов массива
  int kolih(); //Вернуть количество элементов в массиве

  int prov_kol(int kol_str,int kol_kol,int nom_kol); /*проверить есть ли не нулевые значения в колоке двумерного массива*/
  double suma_kol(int kol_str,int kol_kol,int nom_kol); /*получить сумму по колонке двухмерного массива*/
 };

class masiv_din_char
 {
private:
  int  	 kol;     //Количество элементов массива
  int    razmer;  //Размер массива
  char   *masiv_char;  //Массив 
  char	 razdel;   //Символ разделитель элементов массива  
public:

  masiv_din_char(char); //Конструктор
  ~masiv_din_char();      //Деструктор

  int find_masiv_char(char*); //Найти в какой ячейке искомое значение

  int add_masiv_char(char*);  //Записать новый элемент и если нужно увеличить размер массива

  int pol_masiv_char(int,char*);     //Получить значение элемента массива

  int make_masiv_char(int);     //создать массив нужного размера
  void print_masiv_char(); //вывести содержимое массива для контроля
  void free_masiv_char(); //Уничтожить массив
  int kolelem_masiv_char(); //Получить количество элементов массива
 };

class masiv_din_int
 {
private:
  int    optimiz;  //0-записывать в масив без оптимизации 1-с оптимизацией
  int	 kolz;    //Количество элементов массива
  int *masiv_int;  //Массив 
  
public:

  masiv_din_int(int); //Конструктор
  masiv_din_int(); //Конструктор
  ~masiv_din_int();      //Деструктор

  int find(int); //Найти в какой ячейке искомое значение
  int find(char*); //Найти в какой ячейке искомое значение в символьном виде
  int find(const char*); //Найти в какой ячейке искомое значение в символьном виде

  int plus(int,int);      //Записать новый элемент и если нужно увеличить размер массива
  int new_plus(int,int);  //Записать новый элемент не просумировав его с существующим и если нужно увеличить размер массива
  int plus(const char*,int);   //Записать новый элемент в символьном виде и если нужно увеличить размер массива
  int plus(char*,int);   //Записать новый элемент в символьном виде и если нужно увеличить размер массива
  int plus(int);   //Записать в добавленный элемент массива
  int plus(char*);   //Записать в добавленный элемент массива
  int plus(const char*);   //Записать в добавленный элемент массива

  int make_class(int);     //создать массив нужного размера
  void print_class(); //вывести содержимое массива для контроля
  int ravno(int elem); //вернуть значение нужного елемента
  char *ravno_char(int elem); //вернуть значение нужного елемента в символьном виде
  int ravno_max(); //вернуть максимальное значение елемента
  int kolih(); //вернуть количество элементов массива
  void clear_class(); //Обнулить массив
  void free_class();
  int suma(); /*Получить сумму всех елементов*/
 };
