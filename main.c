#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define possible_target_count 3 //офисные,гостиничные, другие(другие это 3 тип)
#define possible_region_count 4  //Европа,ближний восток, юго восточная азия, другие(другие это 4 тип)


typedef struct {
    int floors;
    float height;
    float spire;
    const char *target;
    const char *region;
} Tower;

typedef struct {
    Tower *arr;
    size_t real_size;              //Динамический массив расширяемый
    size_t buffer_size;
} List;

typedef struct{
    List *list[possible_target_count][possible_region_count];         //двумерный массив,внутри каждой ячейки которого наши продвинутые List'ы
} Data_base;

void __add_to_List(List *li,const Tower* el);
void __print_Tower(const Tower* tw);
void __print_List(const List *li);
void add_to_DB(const Tower* tw, Data_base* db);
void create_DB(Data_base *db);
void delDB(Data_base *db);
void print_DB(const Data_base* db);



void __print_Tower(const Tower* tw) {
    printf("%d\n", tw->floors);
    printf("%f\n", tw->height);
    printf("%f\n", tw->spire);
    printf("%s\n", tw->target);
    printf("%s\n", tw->region);
    printf("%s\n", " ");         //перевод на новую строку
}

void __add_to_List(List *li,const Tower* el) {
    if (li==NULL){
        perror("Database is not created");
        return;
    }
    if ((li->real_size == 0 && li->buffer_size == 0)) {      //если массив пустой, то нужно воткнуть первый элемент
        Tower* temp= (Tower *) malloc(sizeof(Tower));
        if (temp==NULL) {
            perror("OS didnt give memory");               //Память не выделилась- запишем что была ошибка + старые данные спасём(они не обнулятся)
            return;
        }
        else li->arr=temp;
        li->buffer_size = 1;
        li->arr[0] = *el;
        li->real_size = 1;
        return;
    }

    if (li->real_size == li->buffer_size) {            //если буффер заполнился увеличиваем размер в два раза
        li->buffer_size *= 2;
        Tower* temp= (Tower*) realloc(li->arr, li->buffer_size * sizeof(Tower));
        if (temp==NULL){
            perror("OS didnt give memory");
            free(temp);
            return;
        }                                         //Если память не выделилась, спасаем старые данные
        else {
            li->arr = temp;
        }

    }
    li->real_size++;
    li->arr[li->real_size-1] = *el;


}

void __print_List(const List *li) {
    if (li==NULL){
        perror("Database is not created");
        return;
    }
    for (size_t i = 0; i < li->real_size; i++) {      //если массив пустой то мы ничего не запринтим
        __print_Tower(&li->arr[i]);
    }
}


void print_DB(const Data_base* db) {
    if (db==NULL){
        perror("Database is not created");
        return;
    }
    for (size_t i = 0; i < possible_target_count; i++) {
        for (size_t j = 0; j < possible_region_count; j++) {
            __print_List(db->list[i][j]);     //принтим все массивы внутри матрицы
        }
    }
}

void create_DB(Data_base *db) {

    for (size_t i = 0; i < possible_target_count; i++) {
        for (size_t j = 0; j < possible_region_count; j++) {
            List *temp = (List *) malloc(sizeof(Tower));
            if (temp==NULL) {
                perror("OS didnt give memory");                //Выводим ошибку если память не выделилась
                return;
            }
            temp->arr=NULL;
            temp->buffer_size = 0;
            temp->real_size = 0;                                      //заполняем матрицу для хранения небоскребов пустыми массивами
            db->list[i][j] = temp;

        }
    }
}

void delDB(Data_base *db) {
    if (db==NULL) {
        perror("DB IS NOT CREATED");
        return;
    }
    for (size_t i = 0; i < possible_target_count; i++) {
        for (size_t j = 0; j < possible_region_count; j++) {
            free(db->list[i][j]->arr);
            free(db->list[i][j]);
        }
    }
}

void add_to_DB(const Tower *tw, Data_base *db) {
    ////Выбираем место в базе данных для того чтобы вставить в зависимости от региона и типа
    size_t other_targets_index=2;
    size_t other_regions_index=3;

    size_t target_purpose = other_targets_index; //двойка для не оффисных и не отельных зданий(если след проверки не пройдут, значит здание другое)
    if (strncmp(tw->target, "office", 6) == 0) target_purpose = 0;
    else if (strncmp(tw->target, "hotel", 5) == 0) target_purpose = 1;

    size_t region_purpose = other_regions_index; //тройка если другой регион (например RU)
    if (strncmp(tw->region, "EU", 2) == 0) region_purpose = 0;
    else if (strncmp(tw->region, "ME", 2) == 0) region_purpose = 1;
    else if (strncmp(tw->region, "SWA", 3) == 0) region_purpose = 2;
    ///////

    __add_to_List(db->list[target_purpose][region_purpose],tw);              //вставляем Tower в необходимое место, найденное раньше


}


void test(){
    Data_base db;
    create_DB(&db);

    Tower test = {1, 2, 3, "hotel", "EU"};
    Tower test2 = {3, 2, 2, "office", "asd"};
    Tower test3 = {3, 2, 5, "office", "ME"};
    Tower test4 = {3, 2, 5, "office", "EU"};
    Tower test5 = {1, 2, 3, "hotel", "dasdasd"};
    Tower test6 = {3, 2, 123123, "office", "EU"};
    Tower test7 = {3, 2, 5, "hotel", "SWA"};
    Tower test8 = {3, 2, 5, "asdasdas", "qqqq"};
    Tower test9 = {3, 2, 5, "asdasdas", "SWA"};
    add_to_DB(&test, &db);
    add_to_DB(&test2, &db);
    add_to_DB(&test3, &db);
    add_to_DB(&test4, &db);
    add_to_DB(&test5, &db);
    add_to_DB(&test6, &db);
    add_to_DB(&test7, &db);
    add_to_DB(&test8, &db);
    add_to_DB(&test9, &db);
    print_DB(&db);
    delDB(&db);

}



int main() {
    test();
}
