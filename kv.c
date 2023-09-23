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
    int isExist = 0;
    int i = 0;

    FILE *fp, *temp;
    fp = fopen(fileName, "r");
    temp = fopen("temp.txt", "w"); // 临时文件用于保存要删除数据之外的其它数据

    if (fp == NULL) { // 文件不存在
        printf("--------该文件不存在--------");
        return ;
    }

    fgets(buf, ABUP_JSON_VALUE_SIZE, fp);

    do { // 将键值不为key的kv对写入temp.txt文件中
        while (buf[i] == ' ') { // 跳过空格
            i++;
        }

        if (strncmp(&buf[i], key, strlen(key)) == 0) {
            continue;
        } else {
            fputs(buf, temp);
        }
    } while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp));

    fclose(fp);
    fclose(temp);

    fp = fopen(fileName, "w");
    temp = fopen("temp.txt", "r");

    // 将temp.txt中的数据写入fileName对应的文件中
    while (1) {
        fgets(buf, sizeof(buf), temp);
        if (feof(temp)) {
            printf("删除成功\n");
            break;
        }
        fputs(buf, fp);
    }

    fclose(fp);
    fclose(temp);
    remove("temp.txt");
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
    fp = fopen(fileName, "r"); // 以只读的模式打开要写入的文件

    if (fp == NULL) {  // 文件不存在
        fp = fopen(fileName, "w"); // "w"模式 -> 如果文件不存在，则创建一个文件
        fprintf(fp, "%s = %s\n", key, value); // 将kv对以格式化的形式写入文件
        fclose(fp);
        printf("--------写入成功-------");
    } else {           // 文件已经存在
        temp = fopen("temp.txt", "w"); // 创建一个temp.txt文件
        fgets(buf, ABUP_JSON_VALUE_SIZE, fp); // 从文件中读取数据

        //查找对应的key
        do {
            while (buf[i] == ' ') { // 跳过空字符
                i++;
            }

            if (strncmp(&buf[i], key, strlen(key)) == 0) {
                fprintf(temp, "%s = %s\n", key, value); // 将kv对写入temp.txt
                isExist = 1;
            } else {
                fputs(buf, temp); // 将字符串buf写入temp文件中
            }

        } while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp));

        if (isExist == 0) {
            fprintf(temp, "%s = %s\n", key, value);
        }

        fclose(fp);
        fclose(temp);

        fp = fopen(fileName, "w");
        temp = fopen("temp.txt", "r");

        while (1) {
            fgets(buf, sizeof(buf), temp);
            if (feof(temp)) {
                printf("---------写入成功--------");
                break;
            }
            fputs(buf, fp);
        }
        fclose(fp);
        fclose(temp);
        remove("temp.txt");
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
    int valueLen = 0; // value长度
    char *valueBuf = NULL;
    int isExist = 0;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) { // 文件不存在
        return NULL;
    }

    fgets(buf, ABUP_JSON_VALUE_SIZE, fp); // 从文件中读取数据到buf数组中
    do {
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
    } while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp));

    if (isExist == 0) {
        return NULL;
    }

    fclose(fp);
    return valueBuf;
}