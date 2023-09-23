//
// Created by ��ͯ on 2023/9/21.
//
#include "kv.h"


/**
  * ������ : RemoveKeyValue
  * ������������ �����ָ���ļ�����key��ɾ��key��value
  * �������:  char *fileName �ļ���,char *key ����
**/
void RemoveKeyValue(char *fileName, char *key) {
    char buf[ABUP_JSON_VALUE_SIZE] = "";
    int i = 0;

    FILE *fp, *temp;
    fp = fopen(fileName, "r");
    temp = fopen("temp", "w"); // ��ʱ�ļ����ڱ���Ҫɾ������֮�����������

    if (fp == NULL) { // �ļ�������
        printf("--------���ļ�������--------");
        return ;
    }

    while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) { // ����ֵ��Ϊkey��kv��д��temp.txt�ļ���
        while (buf[i] == ' ') { // �����ո�
            i++;
        }

        // ��ɾ������������ݴ���temp��ʱ�ļ���
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
  * ������ : WriteKeyValue
  * ������������ �����ָ���ļ�����key���޸���value����������ھ����ļ�β��׷��key = value
  * �������:   char *fileName �ļ���,
  *            char *key ����,
  *            char *value ֵ
**/
void WriteKeyValue(char *fileName, char *key, char *value) {
    char buf[ABUP_JSON_KEY_SIZE] = "";
    int i = 0;
    int isExist = 0;

    FILE *fp, *temp;
    fp = fopen(fileName, "r+");                      // �Զ�д��ģʽ��Ҫд����ļ�

    if (fp == NULL) {                                // �ļ�������
        fp = fopen(fileName, "w+");                  // "w+"ģʽ -> ����ļ������ڣ��򴴽�һ���ļ�
        fprintf(fp, "%s = %s\n", key, value);        // ��kv���Ը�ʽ������ʽд���ļ�
        fclose(fp);
    } else {                                         // �ļ��Ѿ�����
        temp = fopen("temp", "w+");                  // ����һ��temp.txt�ļ�

        //���Ҷ�Ӧ��key
        while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
            while (buf[i] == ' ') { // �������ַ�
                i++;
            }

            // �����º���������ݱ�������ʱ�ļ�temp��
            if (strncmp(&buf[i], key, strlen(key)) == 0) {
                fprintf(temp, "%s = %s\n", key, value);
                isExist = 1;
            } else {
                fputs(buf, temp);
            }

        }

        fclose(temp);

        if (isExist == 0) {            // �������ļ���û���ҵ���Ӧ��key
            fprintf(fp, "%s = %s\n", key, value);
            fclose(fp);
            remove("temp");
        } else {
            fclose(fp);
            remove("test");            // ɾ��ԭ���������ļ�
            rename("temp", fileName);  // ����ʱ�ļ�����Ϊ���ݾ��ļ�
        }
    }
}


/**
  * ������ : ReadValue
  * ������������ ������ָ���ļ�key���ڵ�value ���ڷ�����value�������ڷ���NULL
  * �������:   char *fileName �ļ���,
  *           char *key ����
**/
char* ReadValue(char *fileName,char *key) {
    char buf[ABUP_JSON_VALUE_SIZE] = "";
    int i = 0;
    char *value = NULL;
    int valueLen; // value����
    char *valueBuf = NULL;
    int isExist = 0;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) { // �ļ�������
        return NULL;
    }

    while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
        while (buf[i] == ' ') { // �����ո�
            i++;
        }

        if (strncmp(&buf[i], key, strlen(key)) == 0) { // ƥ���Ӧ��keyֵ
            // strstr(const char *s1, const char *s2) -> ����ָ��s1�ַ�����s2�ַ������ֵ���λ�ã����򷵻�NULL
            value = strstr(&buf[i], "=") + 1;
            valueLen = strlen(value); // ��ȡֵ�ĳ���
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