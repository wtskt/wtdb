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
    int isExist = 0;
    int i = 0;

    FILE *fp, *temp;
    fp = fopen(fileName, "r");
    temp = fopen("temp.txt", "w"); // ��ʱ�ļ����ڱ���Ҫɾ������֮�����������

    if (fp == NULL) { // �ļ�������
        printf("--------���ļ�������--------");
        return ;
    }

    fgets(buf, ABUP_JSON_VALUE_SIZE, fp);

    do { // ����ֵ��Ϊkey��kv��д��temp.txt�ļ���
        while (buf[i] == ' ') { // �����ո�
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

    // ��temp.txt�е�����д��fileName��Ӧ���ļ���
    while (1) {
        fgets(buf, sizeof(buf), temp);
        if (feof(temp)) {
            printf("ɾ���ɹ�\n");
            break;
        }
        fputs(buf, fp);
    }

    fclose(fp);
    fclose(temp);
    remove("temp.txt");
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
    fp = fopen(fileName, "r"); // ��ֻ����ģʽ��Ҫд����ļ�

    if (fp == NULL) {  // �ļ�������
        fp = fopen(fileName, "w"); // "w"ģʽ -> ����ļ������ڣ��򴴽�һ���ļ�
        fprintf(fp, "%s = %s\n", key, value); // ��kv���Ը�ʽ������ʽд���ļ�
        fclose(fp);
        printf("--------д��ɹ�-------");
    } else {           // �ļ��Ѿ�����
        temp = fopen("temp.txt", "w"); // ����һ��temp.txt�ļ�
        fgets(buf, ABUP_JSON_VALUE_SIZE, fp); // ���ļ��ж�ȡ����

        //���Ҷ�Ӧ��key
        do {
            while (buf[i] == ' ') { // �������ַ�
                i++;
            }

            if (strncmp(&buf[i], key, strlen(key)) == 0) {
                fprintf(temp, "%s = %s\n", key, value); // ��kv��д��temp.txt
                isExist = 1;
            } else {
                fputs(buf, temp); // ���ַ���bufд��temp�ļ���
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
                printf("---------д��ɹ�--------");
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
  * ������ : ReadValue
  * ������������ ������ָ���ļ�key���ڵ�value ���ڷ�����value�������ڷ���NULL
  * �������:   char *fileName �ļ���,
  *           char *key ����
**/
char* ReadValue(char *fileName,char *key) {
    char buf[ABUP_JSON_VALUE_SIZE] = "";
    int i = 0;
    char *value = NULL;
    int valueLen = 0; // value����
    char *valueBuf = NULL;
    int isExist = 0;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) { // �ļ�������
        return NULL;
    }

    fgets(buf, ABUP_JSON_VALUE_SIZE, fp); // ���ļ��ж�ȡ���ݵ�buf������
    do {
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
    } while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp));

    if (isExist == 0) {
        return NULL;
    }

    fclose(fp);
    return valueBuf;
}