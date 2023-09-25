//
// Created by 王童 on 2023/9/24.
//

#ifndef WTDB_INDEX_H
#define WTDB_INDEX_H

/**
 * HASH_ADD_INT() 表示添加的键值为int类型
 * HASH_ADD_STR() 表示添加的键值为字符串类型
 * HASH_ADD_PTR() 表示添加的键值为指针类型
 * HASH_ADD() 表示添加的键值可以是任意类型
 */

#include <stdbool.h>
#include "uthash.h"


struct index {
    char key[128];             /* key */
    int offset;
    UT_hash_handle hh;         /* makes this structure hashable */
};
/*声明哈希为NULL指针*/
struct index *wtdb = NULL;     /* important! initialize to NULL */


/**
 * 描述: 往哈希表中添加元素
 * @param key 需要添加的键
 * @param offset 键对应的值
 */
void addIndex(char *key, int offset) {
    struct index *s;
    /*重复性检查，当把两个相同key值的结构体添加到哈希表中时会报错*/
    HASH_FIND_STR(wtdb, key, s);  /* id already in the hash? */
    /*只有在哈希中不存在ID的情况下，我们才创建该项目并将其添加。否则，我们只修改已经存在的结构。*/
    if (s==NULL) {
        s = (struct index *)malloc(sizeof *s);
        strcpy(s->key, key);
        HASH_ADD_STR(wtdb , key, s);  /* key: name of key field */
    }
    s->offset = offset;
}


/**
 * 描述: 在哈希表中查找元素
 * @param key 要查询的值对应的键
 * @return
 */
struct index *findIndex(char *key) {
    struct index *s;
    HASH_FIND_STR(wtdb, key, s );  /* s: output pointer */
    return s;
}


/**
 * 描述: 删除指定元素
 * @param child 待删除的键值对结构
 */
void deleteIndex(struct index *child) {
    HASH_DEL(wtdb, child);          /* user: pointer to deletee */
    free(wtdb);             /* optional; it's up to you! */
}


/**
 * 功能描述: 将索引存入磁盘
 * @param key
 * @param offset
 */
void storageIndex(char *key, int offset) {
    FILE *fp = fopen("indexes", "a+");
    fprintf(fp, "%s=%d\n", key, offset);
    fclose(fp);
}


/**
 * 功能描述: 将字符串转为int整数
 * @param str
 * @return
 */
int parseInt(char *str) {
    int len = (int)strlen(str), res = 0;
    for (int i = 0; i < len-1; i++) {
        res *= 10;
        res += str[i] - '0';
    }
    return res;
}


/**
 * 功能描述: 从指定文件中加载索引
 * @param fileName
 * @return
 */
_Bool loadIndex(char *fileName) {
    FILE *fp = fopen(fileName, "r");
    char buf[ABUP_JSON_KEY_SIZE];

    if (fp == NULL) {
        printf("------文件不存在------");
        return false;
    }

    while (fgets(buf, ABUP_JSON_KEY_SIZE, fp)) {
        size_t len = strlen(buf);
        char *key;

        int offset = parseInt(strstr(buf, "=") + 1);
        for (int i = 0; i < len; i++) {
            if (buf[i] == '=') {
                buf[i] = '\0';
                break;
            }
        }
        key = buf;
        addIndex(key, offset);
    }
    return true;
}

#endif //WTDB_INDEX_H
