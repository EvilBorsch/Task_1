/*Условие ИЗ№1:
Создать структуру для хранения информации о небоскребах мира: количестве этажей, общей высоте, 
высоте шпиля и назначении здания, а также регионе расположения. Составить с ее использованием программу 
вывода информации обо всех зданиях с группировкой сначала по назначению (офисные, гостиничные и т.д.), 
затем — по региону расположения (Европа, Ближний Восток, Юго-Восточная Азия и т.д.).
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>


#define Tower struct Tower
#define __Carr struct __Carr
#define DataBase struct DataBase
//офисные,гостиничные, другие
//Европа,ближний восток, юго восточная азия, другие

__Carr {
    Tower *arr;
    size_t real_size;              //Динамический массив расширяемый
    size_t buffer_size;
};

Tower {
    int floors;
    float height;
    float spire;
    const char *target;
    const char *region;
};

DataBase {
    __Carr *list[3][4];         //массив длинны 3 на 4 т.к у нас 3 параметра для цели и 4 для расположения
    // (внутри каждой ячеки матрицы динамические массивы)
};

void __print(const Tower tw) {
    printf("%d\n", tw.floors);
    printf("%f\n", tw.height);
    printf("%f\n", tw.spire);
    printf("%s\n", tw.target);
    printf("%s\n", tw.region);
    printf("%s\n", " ");         //перевод на новую строку
}


void __add_to_Carr(__Carr *cr, Tower el) {
    if ((cr->real_size == 0 && cr->buffer_size == 0)) {      //если массив пустой, то нужно воткнуть первый элемент
        Tower* temp= (Tower *) malloc(sizeof(Tower));
        if (temp==NULL) {
            errno = EDOM;               //Выводим ошибку если память не выделилась
            assert( 1 == 0);
        }
        else cr->arr=temp;
        cr->buffer_size = 1;
        cr->arr[0] = el;
        cr->real_size = 1;
        return;
    }

    if (cr->real_size == cr->buffer_size) {            //если буффер заполнился увеличиваем размер в два раза
        cr->buffer_size *= 2;
        Tower* temp= realloc(cr->arr, cr->buffer_size * sizeof(Tower));
        if (temp!=NULL)                                         //Если память не выделилась, спасаем старые данные
            cr->arr = temp;
        else {
            errno=EDOM;
            free(temp);
            assert(0==1);
        }

    }
    cr->real_size++; // и начинаем мы с 0 (поэтому если нужно запихнуть первый элемент будет 0++)
    cr->arr[cr->real_size-1] = el;                           //запихиваем в следующее место


}


void __printCarr(const __Carr *bd) {
    for (size_t i = 0; i < bd->real_size; i++) {      //если массив пустой то мы ничего не запринтим
        __print(bd->arr[i]);
    }
}

void add_to_DB(Tower tw, DataBase db) {
    size_t target_purpose = 2; //двойка для другого
    if (strncmp(tw.target, "office", 6) == 0) target_purpose = 0;
    else if (strncmp(tw.target, "hotel", 5) == 0) target_purpose = 1;
    size_t region_purpose = 3; //тройка для другого
    if (strncmp(tw.region, "EU", 2) == 0) region_purpose = 0;
    else if (strncmp(tw.region, "ME", 2) == 0)
        region_purpose = 1;                //Выбираем место для того чтобы запихнуть в зависимости от региона и цели
    else if (strncmp(tw.region, "SWA", 3) == 0) region_purpose = 2;


    __add_to_Carr(db.list[target_purpose][region_purpose],tw);              //засовываем Tower в необходимое место, найденное раньше


}

void printDB(const DataBase db) {
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            __printCarr(db.list[i][j]);     //принтим все массивы внутри матрицы
        }
    }
}

void createDB(DataBase *db) {

    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            __Carr *temp = (__Carr *) malloc(sizeof(Tower)*1);
            if (temp==NULL) {
                errno = EDOM;               //Выводим ошибку если память не выделилась
                assert(1 == 0);
            }
            temp->arr=NULL;
            temp->buffer_size = 0;
            temp->real_size = 0;                                      //заполняем матрицу для хранения небоскребов пустыми массивами
            db->list[i][j] = temp;

        }
    }
}

void delDB(DataBase *db){
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 4; j++) {
            free(db->list[i][j]->arr);
            free(db->list[i][j]);
        }
    }
}

int main() {
    DataBase db;
    createDB(&db);

    Tower test = {1, 2, 3, "hotel", "EU"};
    Tower test2 = {3, 2, 2, "office", "asd"};
    Tower test3 = {3, 2, 5, "office", "ME"};
    Tower test4 = {3, 2, 5, "office", "EU"};
    Tower test5 = {1, 2, 3, "hotel", "dasdasd"};
    Tower test6 = {3, 2, 123123, "office", "EU"};
    Tower test7 = {3, 2, 5, "hotel", "SWA"};
    Tower test8 = {3, 2, 5, "asdasdas", "qqqq"};
    Tower test9 = {3, 2, 5, "asdasdas", "SWA"};
    add_to_DB(test, db);
    add_to_DB(test2, db);
    add_to_DB(test3, db);
    add_to_DB(test4, db);
    add_to_DB(test5, db);
    add_to_DB(test6, db);
    add_to_DB(test7, db);
    add_to_DB(test8, db);
    add_to_DB(test9, db);
    printDB(db);

    delDB(&db);
    /*char a[1024];
    char* b;
    print(test);
    scanf("%s",a);
    b=a;
    printf("%s",b);
     */
}
