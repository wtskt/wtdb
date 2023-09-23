//
// Created by 王童 on 2023/9/21.
//
#include "kv.h"


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
    int isExist = 0;

    FILE *fp, *temp;
    fp = fopen(fileName, "r+");                      // 以读写的模式打开要写入的文件

    if (fp == NULL) {                                // 文件不存在
        fp = fopen(fileName, "w+");                  // "w+"模式 -> 如果文件不存在，则创建一个文件
        fprintf(fp, "%s = %s\n", key, value);        // 将kv对以格式化的形式写入文件
        fclose(fp);
    } else {                                         // 文件已经存在
        temp = fopen("temp", "w+");                  // 创建一个temp.txt文件

        //查找对应的key
        while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
            while (buf[i] == ' ') { // 跳过空字符
                i++;
            }

            // 将更新后的所有数据保存在临时文件temp中
            if (strncmp(&buf[i], key, strlen(key)) == 0) {
                fprintf(temp, "%s = %s\n", key, value);
                isExist = 1;
            } else {
                fputs(buf, temp);
            }

        }

        fclose(temp);

        if (isExist == 0) {            // 在数据文件中没有找到对应的key
            fprintf(fp, "%s = %s\n", key, value);
            fclose(fp);
            remove("temp");
        } else {
            fclose(fp);
            remove("test");            // 删除原来的数据文件
            rename("temp", fileName);  // 将临时文件命名为数据据文件
        }
    }
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
    char *value = NULL;
    int valueLen; // value长度
    char *valueBuf = NULL;
    int isExist = 0;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) { // 文件不存在
        return NULL;
    }

    while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
        while (buf[i] == ' ') { // 跳过空格
            i++;
        }

        if (strncmp(&buf[i], key, strlen(key)) == 0) { // 匹配对应的key值
            // strstr(const char *s1, const char *s2) -> 返回指向s1字符串中s2字符串出现的首位置，否则返回NULL
            value = strstr(&buf[i], "=") + 1;
            valueLen = strlen(value); // 获取值的长度
            valueBuf = (char*) malloc(sizeof (char) * (valueLen + 1));
            i = 0;
            while (i < valueLen) {
                valueBuf[i] = value[i];
                i++;
            }
            valueBuf[i] = '\0';
            isExist = 1;
            break;
        }
    }

    if (isExist == 0) {
        return NULL;
    }

    fclose(fp);
    return valueBuf;
}