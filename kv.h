//
// Created by 王童 on 2023/9/21.
//

#ifndef __KV_H__
#define __KV_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"


#define ABUP_JSON_VALUE_SIZE  1024
#define ABUP_JSON_KEY_SIZE    128

/**
 * 函数功能描述 ：如果指定文件存在key就修改其value，如果不存在就在文件尾部追加key = value
 * @param fileName 文件名
 * @param key       键
 * @param value     值
 */
void WriteKeyValue(char *fileName,char *key,char *value);

/**
 * 函数功能描述 ：查找指定文件key对于的value 存在返回其value，不存在返回NULL
 * @param fileName
 * @param key
 * @return
 */
char* ReadValue(char *fileName,char *key);

/**
 * 函数功能描述 ：如果指定文件存在key就删除key及value
 * @param fileName
 * @param key
 */
void RemoveKeyValue(char *fileName,char *key);

/**
 * 函数功能描述 ：初始化数据库
 */
void initKVDB();

/**
 * 函数功能描述 ：关闭数据库
 */
void closeKVDB();




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

        addNewIndex(key, offset);                                       // 记录本次新增的索引
    } else if (targetIndex != NULL) {                 // key存在
        temp = fopen("temp", "w+");                   // 创建一个临时文件
        offset = targetIndex->offset;                 // value的偏移量

        // 将更新后的所有数据保存在临时文件temp中
        while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
            while (buf[i] == ' ') { // 跳过空字符
                i++;
            }

            if (ftell(fp) - strlen(buf) - 1 == offset) { /* windows中的文件换行符占两个字节 */
                fprintf(temp, "%s\n", value);
            } else {
                fputs(buf, temp);
            }
        }
        fclose(temp);
        fclose(fp);
        remove("test");                                             // 删除原来的数据文件
        rename("temp", fileName);                   // 将临时文件命名为数据据文件
    } else {                                                                // 在数据文件中没有找到对应的key
        fseek(fp,0, SEEK_END);                              // 定位到文件末尾
        offset = ftell(fp);
        fprintf(fp, "%s\n", value);
        fclose(fp);

        addNewIndex(key, offset);                                       // 记录本次新增的索引
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

/**
 * 初始化数据库
 */
void initKVDB() {
    loadIndex("indexes"); /* 从文件中加载索引 */
}


/**
 * 关闭数据库
 */
void closeKVDB() {
    flushIndex();   /* 将索引加载到磁盘 */
}
#ifdef __cplusplus
}
#endif

#endif // __KV_H__