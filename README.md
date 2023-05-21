# Сравнение распределения хэш-функций для различных типов данных и сравнение времени работы хэш-таблиц с закрытой и открытой адресацией

## Сравнение распределения хэш-функций 

***В этой части работы мы рассмотрели распределение значений типа int, double, char * различными хэш-функциями.*** 

Несколько важных замечаний:
- В качестве хэш-таблицы использовалась хэш-таблица методом цепочек.
- Во всех случаях размер хэш-таблицы был равен 1009 (простое число), а на вход подавался миллион уникальных элементов. 
- В рамках одной хэш-таблицы используется только одна хэш-функция;
- На вход различным хэш-функциям одного типа подавались одинаковые элементы.

### Хэш-функции для работы с типом int

Рассмотренные хэш-функции:
- size_t hash_modulo (const int x) - хэш-функция, берущая остаток от деления на число;
- size_t hash_unsigned (const int x) - хэш-функция, возвращающая битовое представление числа в unsigned;
- size_t hash_multiplicative (const int x) - мультипликативная хэш-функция.

Результаты распределений представлены на рис.1.

![](/data/int_hash_functions.png?raw=true)

### Хэш-функции для работы с типом dbl

Рассмотренные хэш-функции:
- size_t hash_double_to_int (const double x) - хэш-функция, преобразующая double в int;
- size_t hash_interpret (const double x) - хэш-функция, возвращающая битовое представление числа в unsigned.

Результаты распределений представлены на рис.2.

![](/data/dbl_hash_functions.png?raw=true)

### Хэш-функции для работы с типом chr

Рассмотренные хэш-функции:
- size_t hash_strlen (const char *string) - хэш-функция, возвращающая длину строки;
- size_t hash_ch_sum (const char *string) - хэш-функция, возврающая сумму ASCII кодов строки;
- size_t hash_crc64 (const char *string) - crc-32 хэш-функция, реализованная с 64-битной маской;
- size_t hash_poly (const char *string) - хэш-функция, преобразующая строку согласно выбранному полиному.

Результаты распределений представлены на рис.3.

![](/data/chr_hash_functions.png?raw=true)


### Выводы 

int: 
- Распределение всех хэш-функцией практически одинаковое и его можно назвать хорошим, так как количество коллизий мало. Наилучшими являются hash_unsigned и hash_multiplicative.

dbl:
- Распределение hash_double_to_int плохо, так как все значения распределены в малой области (так как случайные значения были малы, но это не должно ухудшать хорошую хэш-функцию). 
- Функция hash_interpret показала хорошое распределение с большим, но не критичным, количеством коллизий. Она лучшая из рассмотренных.

char *:
- Функция hash_strlen показала плохое распределение с огромным количеством коллизий и неравномерным графиком.
- Функция hash_ch_sum также имеет неравномерный график и приводит к большому числу коллизий.
- Функции hash_poly и hash_crc64 показали наилучшие результаты с очень малым количеством коллизий. Они наилучшие из рассмотренных.

Мы подтвердили, что очень важно с умом подходить к выбору хэш-функции, так как ее распределение может быть плохим, а значит, будет замедлять всю программу.

## Сравнение хэш-таблиц с открытой и закрытой адресацией

***В данной части исследования измерялось время работы хэш-функций с открытой и закрытой адресацией при проведении операций insert, remove, find с ними.***

- В рамках данной работы использовались хэш-таблица основанная на массиве элементов (открытая адресация) и основанная на односвязном списке (закрытая адресация);
- Все тесты производились на типе данных int;
- Выполнение rehash было исключено путем увеличения начальной вместимости хэш-таблицы;
- Тесты проводились с изменением количества запросов на проведение операции: начиная от 10000 и до 1000000 с шагом 10000.

Сначала сравним работу хэш-таблиц при равновероятном проведении всех трех операций с ними (рис.4).

![](/data/equ_op_chance_hts.png?raw=true)

Из графика видно, что вне зависимости от количества запросов хэш-таблица методом цепочек (закрытая адресация) работает быстрее.

Теперь же увеличим количество добавлений в хэш-таблицу, сделав вероятность проведения этой операции, равной 0,5, а остальных - 0,25 (рис. 5).

![](/data/increased_insert_hts.png?raw=true)

В случае увеличения запросов на добавление в хэш-таблицу графики обоих хэш-таблиц практически не изменились и закрытая адресация все также выигрывает по времени, причем очевидно, что с увеличением количества запросов выигрыш по времени только увеличивается.


## Выводы

Хэш-таблица, использующая закрытую адресацию работает быстрее чем использующая открытую адресацию, так как коллизии в первом случае складываются только из элементов, имеющих один хэш, а во втором он могут складываться из элементов, имеющих близкое значение хэша. 