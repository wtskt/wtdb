#include "kv.h"
#include <time.h>
#include <windows.h>

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


//    LARGE_INTEGER freq;
//    LARGE_INTEGER start, stop;
//    double exe_time;
//
//    QueryPerformanceFrequency(&freq);
//
//    QueryPerformanceCounter(&start);
////	ReadValue("test","yuyzLVm");
//	ReadValue("test","7g4Ey5f");
//    QueryPerformanceCounter(&stop);
//    exe_time = 1e3*(stop.QuadPart - start.QuadPart) / freq.QuadPart;
//    printf("�ķ�ʱ��Ϊ%lf��\n", exe_time);



//    clock_t start, stop;
//    start = clock();
//    for(i = 0; i < 10000; i++) {
//        key1 = genRandomString(8);
//        value1 = genRandomString(10);
//        WriteKeyValue(fileName,key1,value1);
//    }
//    stop = clock();
//    //��������Ϊ��λ������ʱ�䣬������ʱ��-��ʼʱ�䣩/CLK_TCK
//    printf("�ķ�ʱ��Ϊ%f��\n", ((double)(stop - start)) / CLK_TCK);



//    WriteKeyValue(fileName,"7g4Ey4l","helkkko");

//	RemoveKeyValue(fileName,"UR73aXs");

    return 0;
}