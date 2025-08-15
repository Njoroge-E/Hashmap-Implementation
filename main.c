#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct StudentDetails {
    unsigned int id;
    char name[50];
} StudentDetails;

StudentDetails* create_students(unsigned int max) {
    StudentDetails* students = calloc(max, sizeof(StudentDetails));
    if (!students) {
        perror("Malloc Failed!\n");
        return NULL;
    }
    return students;
}

unsigned int hash(unsigned int key) {
    if (key > 0 && key < 11)
        return (key - 1) % 10;
    return UINT32_MAX;
}

// what exactly is _Bool(whatever bool evalueates too) like int returns a num from x to y, what does bool return or do; like how is it defined or rather implemented?
bool input(unsigned int key, char* name, StudentDetails* students) {
    if (!students) {
        fprintf(stderr, "Students array is NULL!\n"); 
        return false;
    }
    unsigned int index = hash(key);
    if (index == UINT32_MAX) {
        fprintf(stderr, "Key should be > than 0 and < than 11!\n");
        return false;
    }
    char null_str[50]; 
    // can you do: char null_str[sizeof(students[index].name)] or it can't evaluate this?; So the use of a function in the [] of a stacck array?
    memset(null_str, 0, sizeof(null_str));

    // what do you think of the function below for validity?
    if (students[index].id == 0 && strcmp(students[index].name, null_str) == 0) {
        students[index].id = key;
        strncpy(students[index].name, name, sizeof(null_str) - 1);
        // does strcpy only copy the length of the src string(\n excluded) then \n included later on, and strncpy ensure n chars of src are copied and \n includeed later on or what?
        return true;
    }
    fprintf(stderr, "Key %d is in the hashmap!\n", key);
    // what's the difference between a hashmap and a hashtable?
    return false;
}

bool search(unsigned int key, StudentDetails* students) {
    if (!students) {
        fprintf(stderr, "Students array is NULL!\n"); 
        return false;
    }

    unsigned int index = hash(key);
    if (index == UINT32_MAX) {
        fprintf(stderr, "Key should be > than 0 and < than 11!\n");
        return false;
    }
    char null_str[50];
    memset(null_str, 0, sizeof(null_str));
    if (memcmp(students[index].name, null_str, sizeof(null_str)) != 0) {
    // index is valid
        return true;
    }
    fprintf(stderr, "Key %d is not in the hashmap!\n", key);
    return false;
}


int main() {
    StudentDetails* students = create_students(10);
    // array for hashmap


    input(1, "David", students);
    input(2, "John", students);
    input(3, "Abby", students);
    input(4, "Dave", students);
    input(5, "Michael", students);
    input(6, "Ryan", students);
    input(7, "Angela", students);
    input(8, "Mustafa", students);
    input(9, "Maria", students);
    input(10, "Rama", students);

    // trials to see if input() works?
    input(11, "David", students);
    input(5, "Maslow", students);
    // what happens if I enter a negative number as the key like below or key 0?
    input(-5, "blahblahblah", students);
    input(0, "blahblahblah", students);

    for (int i = 1; i <= 15; i++) {
        if (search(i, students)) {
            printf("Key: %d;\tValue: %s\n", i, students[hash(i)].name);
            continue;
        }
        printf("Key: %d is not in the hashmap\n", i);
    }

    return 0;
}