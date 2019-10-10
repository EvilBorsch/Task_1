/*
 * Создать структуру для хранения информации о небоскребах мира: количестве этажей, общей высоте,
 * высоте шпиля и назначении здания,
 * а также регионе расположения. Составить с ее использованием программу вывода информации
 * обо всех зданиях с группировкой сначала по назначению (офисные, гостиничные и т.д.),
 * затем — по региону расположения (Европа, Ближний Восток, Юго-Восточная Азия и т.д.).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

typedef struct {
    int floors;
    float height;
    float spire;
    char *target;
    char *region;
} Tower;

typedef struct {
    Tower *arr;
    size_t real_size;              //Динамический массив расширяемый
    size_t buffer_size;
} List;


void add_to_list(List *li,const Tower* el);
void print_Tower(const Tower* tw);
void print_List(const List *li);
bool compare(Tower a,Tower b);
void insertion_sort(List* mass,bool (compare)(Tower,Tower));
void test();


int main() {
    test();
}



void add_to_list(List *li,const Tower* el) {
    if (li==NULL){
        perror("Database is not created");
        errno=EDOM;
        return;
    }
    if ((li->real_size == 0 && li->buffer_size == 0)) {      //если массив пустой, то нужно вставить первый элемент
        Tower* temp= (Tower *) malloc(sizeof(Tower));
        if (temp==NULL) {
            perror("OS didnt give memory");
            errno=EDOM;
            return;
        }
        else li->arr=temp;
        li->buffer_size = 1;
        li->arr[0] = *el;
        li->real_size = 1;
        return;
    }

    if (li->real_size == li->buffer_size) {
        li->buffer_size *= 2;
        Tower* temp= (Tower*) realloc(li->arr, li->buffer_size * sizeof(Tower));
        if (temp==NULL){
            perror("OS didnt give memory");
            free(temp);
            errno=EDOM;
            return;
        }
        else {
            li->arr = temp;
        }

    }

    li->real_size++;
    li->arr[li->real_size-1] = *el;
    insertion_sort(li,compare);

}

void print_List(const List *li) {
    if (li==NULL){
        perror("Database is not created");
        return;
    }
    for (size_t i = 0; i < li->real_size; i++) {      //если массив пустой то мы ничего не запринтим
        print_Tower(&li->arr[i]);
    }
}


List* create_list(const size_t arr_size,Tower* arr) {
    List* temp=(List*) malloc(sizeof(List));
    temp->buffer_size=arr_size;
    temp->real_size=arr_size;
    temp->arr=arr;
    insertion_sort(temp,compare);
    return temp;
}


void del_list(List* li){
    free(li->arr);
    free(li);
}


bool compare(Tower a, Tower b) {
    int result=strcasecmp(a.target,b.target);
    if (result==0){
        return strcasecmp(a.region,b.region)>0;
    }
    else{
        return result>0;
    }
}

void insertion_sort(List *mass, bool (*compare)(Tower, Tower)) {
    if (mass==NULL){
        perror("mass is empty");
        return;
    }

    Tower temp;
    for (size_t counter = 1; counter < mass->real_size; counter++){
        temp = mass->arr[counter];
        int item =(int) counter-1;
        while(item >= 0 && compare(mass->arr[item],temp)){
            mass->arr[item + 1] = mass->arr[item]; // перестановка элементов массива
            mass->arr[item] = temp;
            item--;
        }
    }
}

void print_Tower(const Tower *tw) {
    if (tw==NULL){
        perror("Empty Tower");
        return;
    }
    printf("%d\n", tw->floors);
    printf("%f\n", tw->height);
    printf("%f\n", tw->spire);
    printf("%s\n", tw->target);
    printf("%s\n", tw->region);
    printf("%s\n", " ");
}

List* input_List_from_keyboard(){
    List* temp_li=(List*)malloc(sizeof(List));
    int arr_size;
    scanf("%100d",&arr_size);

    char buffer[1024];
    for (int i=0;i<arr_size;i++) {
        Tower temp;
        scanf("%100d", &temp.floors);
        scanf("%100f", &temp.height);
        scanf("%100f", &temp.spire);
        scanf("%1023s", buffer);
        temp.target = (char*) malloc(sizeof(char)*strlen(buffer));
        strcpy(temp.target, buffer);
        scanf("%1023s", buffer);
        temp.region = (char*) malloc(sizeof(char)*strlen(buffer));
        strcpy(temp.region, buffer);
        add_to_list(temp_li, &temp);
    }
    return temp_li;
}


void test() {
    
    Tower test = {1, 2, 3, "abc", "cus"};
    Tower test2 = {3, 2, 2, "b", "Alu"};
    Tower test3 = {3, 2, 5, "abc", "cus"};
    Tower test4 = {3, 2, 5, "b", "Blu"};
    Tower test5 = {1, 2, 3, "abc", "q"};
    Tower test6 = {3, 2, 123123, "office", "EU"};
    Tower test7 = {3, 2, 5, "abc", "als"};
    Tower test8 = {3, 2, 5, "asdasdas", "qqqq"};
    Tower test9 = {3, 2, 5, "asdasdas", "SWA"};
    size_t a=3;
    Tower* arr=(Tower*) malloc(sizeof(Tower)*a);
    arr[0]=test;
    arr[1]=test2;
    arr[2]=test3;
    List* li=create_list(a,arr);
    add_to_list(li,&test4);
    add_to_list(li,&test5);
    add_to_list(li,&test6);
    add_to_list(li,&test7);
    add_to_list(li,&test8);
    add_to_list(li,&test9);
    print_List(li);
    del_list(li);
    /*
    List* li2=input_List_from_keyboard();
    print_List(li2);
    del_list(li2);
    */
}

