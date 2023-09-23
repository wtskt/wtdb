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

#ifdef __cplusplus
}
#endif

#endif // __KV_H__