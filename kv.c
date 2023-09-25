//
// Created by 王童 on 2023/9/21.
//
#include "kv.h"
#include "index.h"
#include "windows.h"
#include "time.h"


/**
  * 函数名 : RemoveKeyValue
  * 函数功能描述 ：如果指定文件存在key就删除key及value
  * 函数入参:  char *fileName 文件名,char *key 键名
**/
void RemoveKeyValue(char *fileName, char *key) {
    char buf[ABUP_JSON_VALUE_SIZE] = "";
    int i = 0;

    FILE *fp, *temp;
    fp = fopen(fileName, "r");
    temp = fopen("temp", "w"); // 临时文件用于保存要删除数据之外的其它数据

    if (fp == NULL) { // 文件不存在
        printf("--------该文件不存在--------");
        return ;
    }

    while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) { // 将键值不为key的kv对写入temp.txt文件中
        while (buf[i] == ' ') { // 跳过空格
            i++;
        }

        // 将删除后的所有数据存入temp临时文件中
        if (strncmp(&buf[i], key, strlen(key)) == 0) {
            continue;
        } else {
            fputs(buf, temp);
        }
    }

    fclose(fp);
    fclose(temp);
    remove("test");
    rename("temp", fileName);
}


/**
  * 函数名 : WriteKeyValue
  * 函数功能描述 ：如果指定文件存在key就修改其value，如果不存在就在文件尾部追加key = value
  * 函数入参:   char *fileName 文件名,
  *            char *key 键名,
  *            char *value 值
**/
void WriteKeyValue(char *fileName, char *key, char *value) {
    char buf[ABUP_JSON_KEY_SIZE] = "";
    int i = 0;
    int offset = 0;

    struct index *targetIndex = findIndex(key);

    FILE *fp, *temp;
    fp = fopen(fileName, "r+");                       // 以读写的模式打开要写入的文件

    if (fp == NULL) {                                 // 文件不存在
        fp = fopen(fileName, "w+");                   // "w+"模式 -> 如果文件不存在，则创建一个文件
        fprintf(fp, "%s\n", value);                   // 将value写入文件
        fclose(fp);
    } else if (targetIndex != NULL) {                 // key存在
        temp = fopen("temp", "w+");                   // 创建一个temp.txt文件
        offset = targetIndex->offset;                 // value的偏移量

        //将数据写入temp文件
        while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
            while (buf[i] == ' ') { // 跳过空字符
                i++;
            }

            // 将更新后的所有数据保存在临时文件temp中
            if (ftell(fp) == offset) {
                fprintf(temp, "%s\n", value);
            } else {
                fputs(buf, temp);
            }
        }
        fclose(temp);
        fclose(fp);
        remove("test");                             // 删除原来的数据文件
        rename("temp", fileName);                   // 将临时文件命名为数据据文件
    } else {                                        // 在数据文件中没有找到对应的key
        fseek(fp,0, SEEK_END);                      // 定位到文件末尾
        offset = ftell(fp);
        fprintf(fp, "%s\n", value);
        fclose(fp);
    }

    storageIndex(key, offset);
    addIndex(key, offset);                          // 添加索引
}


/**
  * 函数名 : ReadValue
  * 函数功能描述 ：查找指定文件key对于的value 存在返回其value，不存在返回NULL
  * 函数入参:   char *fileName 文件名,
  *           char *key 键名
**/
char* ReadValue(char *fileName,char *key) {
    char buf[ABUP_JSON_VALUE_SIZE] = "";
    int i = 0;
    int valueLen; // value长度
    char *valueBuf = NULL;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) { // 文件不存在
        return NULL;
    }

    struct index *index = findIndex(key);
    if (index == NULL) {                        // 判断key是否存在
        printf("------key不存在------");
        return NULL;
    }

    int offset = index->offset;                 // 偏移量
    fseek(fp, offset, SEEK_SET);                // 定位数据
    fgets(buf, ABUP_JSON_VALUE_SIZE, fp);       // 读出数据
    valueLen = strlen(buf);
    valueBuf = (char*) malloc(sizeof (char) * (valueLen + 1));
    while (i < valueLen) {
        valueBuf[i] = buf[i];
        i++;
    }
    valueBuf[i] = '\0';

    fclose(fp);
    return valueBuf;
}


unsigned int seed = 1;
char* genRandomString(int length) { // 生成length长度的随机字符串
    int flag, i;
    char* string;
//    unsigned int seed = (unsigned) time(NULL);
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

int main(int argc, char const *argv[]) {
    char fileName[50] = "test";
    char *key1 = NULL;
    char *value1 = NULL;
    int i = 0;

// 写
//    clock_t start, stop;
//    start = clock();
//    for(i = 1; i < 10000; i++) {
//        key1 = genRandomString(8);
//        value1 = genRandomString(10);
//        WriteKeyValue(fileName,key1,value1);
//    }
//    WriteKeyValue(fileName,"wt","hello");
//    stop = clock();
//    //计算以秒为单位的运行时间，（结束时间-开始时间）/CLK_TCK
//    printf("耗费时间为%f秒\n", ((double)(stop - start)) / CLK_TCK);


// 读
    loadIndex("indexes");
    LARGE_INTEGER freq;
    LARGE_INTEGER start, stop;
    double exe_time;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);

    char *string = ReadValue("test", "wt");
    
    QueryPerformanceCounter(&stop);
    exe_time = 1e3*(stop.QuadPart - start.QuadPart) / freq.QuadPart;
    printf(string);
    printf("耗费时间为%lf秒\n", exe_time);

    unsigned int len = HASH_COUNT(wtdb);
    printf("索引长度: %d", len);

    return 0;
}