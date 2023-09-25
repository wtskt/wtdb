//
// Created by ��ͯ on 2023/9/21.
//
#include "kv.h"
#include "index.h"
#include "windows.h"
#include "time.h"


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
    int offset = 0;

    struct index *targetIndex = findIndex(key);

    FILE *fp, *temp;
    fp = fopen(fileName, "r+");                       // �Զ�д��ģʽ��Ҫд����ļ�

    if (fp == NULL) {                                 // �ļ�������
        fp = fopen(fileName, "w+");                   // "w+"ģʽ -> ����ļ������ڣ��򴴽�һ���ļ�
        fprintf(fp, "%s\n", value);                   // ��valueд���ļ�
        fclose(fp);
    } else if (targetIndex != NULL) {                 // key����
        temp = fopen("temp", "w+");                   // ����һ��temp.txt�ļ�
        offset = targetIndex->offset;                 // value��ƫ����

        //������д��temp�ļ�
        while (fgets(buf, ABUP_JSON_VALUE_SIZE, fp)) {
            while (buf[i] == ' ') { // �������ַ�
                i++;
            }

            // �����º���������ݱ�������ʱ�ļ�temp��
            if (ftell(fp) == offset) {
                fprintf(temp, "%s\n", value);
            } else {
                fputs(buf, temp);
            }
        }
        fclose(temp);
        fclose(fp);
        remove("test");                             // ɾ��ԭ���������ļ�
        rename("temp", fileName);                   // ����ʱ�ļ�����Ϊ���ݾ��ļ�
    } else {                                        // �������ļ���û���ҵ���Ӧ��key
        fseek(fp,0, SEEK_END);                      // ��λ���ļ�ĩβ
        offset = ftell(fp);
        fprintf(fp, "%s\n", value);
        fclose(fp);
    }

    storageIndex(key, offset);
    addIndex(key, offset);                          // �������
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
    int valueLen; // value����
    char *valueBuf = NULL;
    FILE *fp;

    fp = fopen(fileName, "r");
    if (fp == NULL) { // �ļ�������
        return NULL;
    }

    struct index *index = findIndex(key);
    if (index == NULL) {                        // �ж�key�Ƿ����
        printf("------key������------");
        return NULL;
    }

    int offset = index->offset;                 // ƫ����
    fseek(fp, offset, SEEK_SET);                // ��λ����
    fgets(buf, ABUP_JSON_VALUE_SIZE, fp);       // ��������
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
char* genRandomString(int length) { // ����length���ȵ�����ַ���
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

// д
//    clock_t start, stop;
//    start = clock();
//    for(i = 1; i < 10000; i++) {
//        key1 = genRandomString(8);
//        value1 = genRandomString(10);
//        WriteKeyValue(fileName,key1,value1);
//    }
//    WriteKeyValue(fileName,"wt","hello");
//    stop = clock();
//    //��������Ϊ��λ������ʱ�䣬������ʱ��-��ʼʱ�䣩/CLK_TCK
//    printf("�ķ�ʱ��Ϊ%f��\n", ((double)(stop - start)) / CLK_TCK);


// ��
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
    printf("�ķ�ʱ��Ϊ%lf��\n", exe_time);

    unsigned int len = HASH_COUNT(wtdb);
    printf("��������: %d", len);

    return 0;
}