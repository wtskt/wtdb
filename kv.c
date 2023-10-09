//
// Created by ��ͯ on 2023/9/21.
//
#include "kv.h"
#include "index.h"
#include "windows.h"
#include "time.h"

unsigned int seed = 1;

char* genRandomString(int length); /* ���ɳ���Ϊlength������ַ� */

void testWrite(char * fileName);   /* д����� */

void testUpdate(char * fileName, char * key, char * value); /* ���Ը��� */

void testRead(char * fileName, char * key); /* ���Զ� */

int main(int argc, char const *argv[]) {
    initKVDB(); /* ��ʼ�����ݿ� */

// // д
//    testWrite("test");
//
// // ��
//    testRead("test", "wt");

// ��
//    testUpdate("test", "wt", "NB");

// ׷��д
//    WriteKeyValue("test", "gha", "408");
//    WriteKeyValue("test", "ghg", "408");
//    WriteKeyValue("test", "ghd", "408");
//    testRead("test", "gha");

    closeKVDB(); /* �ر����ݿ� */
    return 0;
}


void testWrite(char * fileName) {
    char *key;
    char *value;

    clock_t start, stop;
    start = clock();

    for(int i = 1; i < 10000; i++) {
        key = genRandomString(8);
        value = genRandomString(10);
        WriteKeyValue(fileName,key,value);
    }
    WriteKeyValue(fileName,"wt","hello");
    stop = clock();
    //��������Ϊ��λ������ʱ�䣬������ʱ��-��ʼʱ�䣩/CLK_TCK
    printf("�ķ�ʱ��Ϊ%f��\n", ((double)(stop - start)) / CLK_TCK);
}

void testRead(char * fileName, char * key) {
    LARGE_INTEGER freq;
    LARGE_INTEGER start, stop;
    double exe_time;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    char *string = ReadValue(fileName, key);

    QueryPerformanceCounter(&stop);
    exe_time = 1e3*(stop.QuadPart - start.QuadPart) / freq.QuadPart;
    printf( "%s = %s\n", key, string);
    printf("�ķ�ʱ��Ϊ%lf��\n", exe_time);

    unsigned int len = HASH_COUNT(kvDB);
    printf("��������: %d", len);
}

void testUpdate(char * fileName, char * key, char * value) {
    WriteKeyValue(fileName, key, value);
    unsigned int len = HASH_COUNT(kvDB);
    char *string = ReadValue(fileName, key);
    printf("%s = %s", key, string);
}

char* genRandomString(int length) { // ����length���ȵ�����ַ���
    int flag, i;
    char* string;
    srand(seed++);
    if ((string = (char*) malloc(length)) == NULL ) {
        return NULL ;
    }

    for (i = 0; i < length - 1; i++) {
        flag = rand() % 3;
        switch (flag) {
            case 0:
                string[i] = 'A' + rand() % 26;
                break;
            case 1:
                string[i] = 'a' + rand() % 26;
                break;
            case 2:
                string[i] = '0' + rand() % 10;
                break;
            default:
                string[i] = 'x';
                break;
        }
    }
    string[length - 1] = '\0';
    return string;
}