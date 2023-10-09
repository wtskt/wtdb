//
// Created by 王童 on 2023/9/21.
//
#include "kv.h"
#include "index.h"
#include "windows.h"
#include "time.h"

unsigned int seed = 1;

char* genRandomString(int length); /* 生成长度为length的随机字符 */

void testWrite(char * fileName);   /* 写入测试 */

void testUpdate(char * fileName, char * key, char * value); /* 测试更新 */

void testRead(char * fileName, char * key); /* 测试读 */

int main(int argc, char const *argv[]) {
    initKVDB(); /* 初始化数据库 */

// // 写
//    testWrite("test");
//
// // 读
//    testRead("test", "wt");

// 改
//    testUpdate("test", "wt", "NB");

// 追加写
//    WriteKeyValue("test", "gha", "408");
//    WriteKeyValue("test", "ghg", "408");
//    WriteKeyValue("test", "ghd", "408");
//    testRead("test", "gha");

    closeKVDB(); /* 关闭数据库 */
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
    //计算以秒为单位的运行时间，（结束时间-开始时间）/CLK_TCK
    printf("耗费时间为%f秒\n", ((double)(stop - start)) / CLK_TCK);
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
    printf("耗费时间为%lf秒\n", exe_time);

    unsigned int len = HASH_COUNT(kvDB);
    printf("索引长度: %d", len);
}

void testUpdate(char * fileName, char * key, char * value) {
    WriteKeyValue(fileName, key, value);
    unsigned int len = HASH_COUNT(kvDB);
    char *string = ReadValue(fileName, key);
    printf("%s = %s", key, string);
}

char* genRandomString(int length) { // 生成length长度的随机字符串
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