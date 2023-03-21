#define _CRT_SECURE_NO_WARNINGS

#include "ecc.h"
#include<iostream>
#include <fstream>
#include <Windows.h>

void print_mem(bigint* x,const char* arr) {
    printf("%s  :   %08X %08X %08X %08X %08X %08X %08X %08X\n\n",arr, x->bigInt[7], x->bigInt[6], x->bigInt[5], x->bigInt[4], x->bigInt[3], x->bigInt[2], x->bigInt[1], x->bigInt[0]);

}

void init(bigint* src, bigint* dest) {
    for (int i = 0; i < 8; i++) {
        dest->bigInt[i] = src->bigInt[i];
    }
}

bool isEmpty(bigint* src) {
    if (src->bigInt[7] == 0x00 && src->bigInt[6] == 0x00 && src->bigInt[5] == 0x00 && src->bigInt[4] == 0x00 && src->bigInt[3] == 0x00 && src->bigInt[2] == 0x00 && src->bigInt[1] == 0x00 && src->bigInt[0] == 0x00)return 1;
    else return 0;
}

void getMem(bigint* x, uint32 res[8])
{
    for (int i = 0; i < 8; i++) {
        x->bigInt[i] = res[i];

    }
    //printf("%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", x->bigInt[0], x->bigInt[1], x->bigInt[2], x->bigInt[3], x->bigInt[4], x->bigInt[5], x->bigInt[6], x->bigInt[7]);
}

void getFileMem(FILE* fin, bigint* x)
{
    uint32 read_buf[100] = { 0x00, };
    uint32 read_tmp[100] = { 0x00, };
    uint32 temp = 0;
    for (int cnt_j = 7; cnt_j >= 0; cnt_j--)
    {
        fscanf(fin, "%08X", &read_tmp[0]);
        x->bigInt[cnt_j] = read_tmp[0];
        //printf("*/*/*/*/*/%X\n", read_tmp[0]);
        //printf("*/xxxxxxxxxx*/*/*/%X\n", (*x).bigInt[cnt_j]);
    }
    fscanf(fin, "\n\n", read_buf);
    //print_mem(x, "x");
}

void getFile_xy_Mem(FILE* fin, bigint* x,bigint* y)
{
    //printf("fffff\n");
    uint32 read_buf[100] = { 0x00, };
    uint32 read_tmp[100] = { 0x00, };
    uint32 temp = 0;
    for (int cnt_j = 7; cnt_j >= 0; cnt_j--)
    {
        fscanf(fin, "%08X", &read_tmp[0]);
        x->bigInt[cnt_j] = read_tmp[0];
        //printf("*/*/*/*/*/%X\n", read_tmp[0]);
        //printf("*/xxxxxxxxxx*/*/*/%X\n", (*x).bigInt[cnt_j]);
    }
    fscanf(fin, "\n", read_buf);
    //print_mem(x, "x");
    for (int cnt_j = 7; cnt_j >= 0; cnt_j--)
    {
        fscanf(fin, "%08X", &read_tmp[0]);
        y->bigInt[cnt_j] = read_tmp[0];
        //printf("*/*/*/*/*/%X\n", read_tmp[0]);
        //printf("*/xxxxxxxxxx*/*/*/%X\n", (*x).bigInt[cnt_j]);
    }
    fscanf(fin, "\n\n", read_buf);
    //print_mem(x, "x");
}

int Compare(bigint* x, bigint* y)
{
    for (int i = 7; i >= 0; i--)
    {
        if (x->bigInt[i] > y->bigInt[i]) {
            return 0;
            break;
        }
        else if (x->bigInt[i] < y->bigInt[i]) {
            return 1;
            break;
        }
    }
    //루프가 돌고도 끝까지 같았다면 같은거임
    return 2;
}

void reduction(bigint* src, bigint* dest ,bigint* prime)
{
    bigint result = { {0x00,},0 };
   // print_mem(src, "add_reduction src");
   // printf("carry : %d\n", src->carry);
    if (src->carry == 1 || Compare(src,prime)==0) {
        
        //printf("reduction\n");
        uint32 carry = 0;

        for (int i = 0; i < 8; i++) {
            result.bigInt[i] = src->bigInt[i] + ~(prime->bigInt[i]) + carry;
           // printf("*%08x  +  %08x + %d = %08x\n", src->bigInt[i], ~(prime->bigInt[i]), carry, result.bigInt[i]);

            if (src->bigInt[i] > result.bigInt[i]) {
                carry = 1;
                if (result.bigInt[i] + carry < carry)carry++;
            }
            else if ((result.bigInt[i] == src->bigInt[i] + (prime->bigInt[i])) && carry == 1) {
                carry = 1;
                //cout << "carryyyyyyyyyyyyyyyy" << endl;
            }
            else carry = 0;
            //printf("carry of reduction %d", carry);

        }
        result.bigInt[0] += 1;
        memcpy(dest, &result, sizeof(bigint));
    }
    else {
        memcpy(dest, src, sizeof(bigint));
    }
   // print_mem(dest, "add_reduction _result");
   // printf("******\n");
}

void noReduct_add(bigint* src1, bigint* src2, bigint* dest, bigint* prime)
{
    bigint result = { {0x00,}, };
    
    int carry = 0;
    for (int i = 0; i < 8; i++) {

        result.bigInt[i] = src1->bigInt[i] + src2->bigInt[i] + carry;
        //printf("*%08x  +  %08x + %d = %08x\n", src1->bigInt[i], src2->bigInt[i], carry,result.bigInt[i]);

        if (src1->bigInt[i] > result.bigInt[i])carry = 1;
        else if (src2->bigInt[i] > result.bigInt[i])carry = 1;
        else carry = 0;
        //cout << "carry" << carry << endl;
    }

    result.carry = carry;
    memcpy(dest, &result, sizeof(bigint));
}

void add(bigint* src1, bigint* src2, bigint* dest ,bigint* prime)
{
    bigint result = { {0x00,}, };
    /*
    int carry = 0;
    for (int i = 0; i < 8; i++) {

        sum->bigInt[i] = x1->bigInt[i] + x2->bigInt[i] + carry;
        //   printf("*%08x  +  %08x + %d = %08x\n", x1->bigInt[i], x2->bigInt[i], carry,sum->bigInt[i]);

        if (x1->bigInt[i] > sum->bigInt[i])carry = 1;
        else carry = 0;
        //cout << "carry" << carry << endl;
    }
    //printf("*%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", sum->bigInt[0], sum->bigInt[1], sum->bigInt[2], sum->bigInt[3], sum->bigInt[4], sum->bigInt[5], sum->bigInt[6], sum->bigInt[7]);
    
    sum->carry = carry;
    // printf("***x3***%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", x3->bigInt[0], x3->bigInt[1], x3->bigInt[2], x3->bigInt[3], x3->bigInt[4], x3->bigInt[5], x3->bigInt[6], x3->bigInt[7]);

    sum = reduction(sum, x3);
    */
    int carry = 0;
    for (int i = 0; i < 8; i++) {

        result.bigInt[i] = src1->bigInt[i] + src2->bigInt[i] + carry;
        //printf("*%08x  +  %08x + %d = %08x\n", src1->bigInt[i], src2->bigInt[i], carry,result.bigInt[i]);

        if (src1->bigInt[i] > result.bigInt[i])carry = 1;
        else if(src2->bigInt[i] > result.bigInt[i])carry = 1;
        else carry = 0;
        //cout << "carry" << carry << endl;
    }

    result.carry = carry;
   //printf("add before reduection %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n", result.bigInt[0], result.bigInt[1], result.bigInt[2], result.bigInt[3], result.bigInt[4], result.bigInt[5], result.bigInt[6], result.bigInt[7]);
   //printf("large carry : %d\n", result.carry);
    reduction(&result, dest, prime);
   //print_mem(dest, "add_result");
   // printf("add after reduection %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n", result.bigInt[0], result.bigInt[1], result.bigInt[2], result.bigInt[3], result.bigInt[4], result.bigInt[5], result.bigInt[6], result.bigInt[7]);

}

void minus_(bigint* src1, bigint* src2, bigint* dest ,bigint* prime)
{
    bigint result = { {0x00,},0 };
    int borrow = 0;
    //cout << "compare값 0일때 x1이 큼" << Compare(x1, x2) << endl;
    if (Compare(src1, src2) == 0 || Compare(src1, src2) == 2) {//x1>x2
        for (int i = 0; i < 8; i++) {

            if (src1->bigInt[i] - borrow >= src2->bigInt[i]) {
                result.bigInt[i] = src1->bigInt[i] - src2->bigInt[i] - borrow;
                borrow = 0;
            }
            else {
                result.bigInt[i] = (0xffffffff - src2->bigInt[i]) + 1 + src1->bigInt[i] - borrow;
                borrow = 1;
            }
        }
    }
    else if (Compare(src1, src2) == 1) {//x1<x2
        int carry = 0;
        for (int i = 0; i < 8; i++) {
            if (src1->bigInt[i] - borrow >= src2->bigInt[i]) {
                result.bigInt[i] = src1->bigInt[i] - src2->bigInt[i] - borrow;
                result.bigInt[i] = result.bigInt[i] + prime->bigInt[i] + carry;
                if (prime->bigInt[i] > result.bigInt[i])carry = 1;
                else carry = 0;
                borrow = 0;
            }

            else {
                result.bigInt[i] = (0xffffffff - src2->bigInt[i]) + 1 + src1->bigInt[i] - borrow;
                result.bigInt[i] += prime->bigInt[i] + carry;
                borrow = 1;
                if (prime->bigInt[i] > result.bigInt[i]) { carry = 0; borrow = 0; }
                else carry = 0;

            }
        }
    }
    memcpy(dest, &result, sizeof(bigint));
}

void p256_mul_reduction(uint32 x[16], bigint* dest, bigint* prime)
{
    bigint temp1 = { {0x00,}, 0 };
    bigint temp2 = { {0x00,}, 0 };
    bigint s1 = { {0x00,}, 0 };
    bigint s2 = { {0x00,}, 0 };
    bigint s3 = { {0x00,}, 0 };
    bigint s4 = { {0x00,}, 0 };
    bigint s5 = { {0x00,}, 0 };
    bigint s6 = { {0x00,}, 0 };
    bigint s7 = { {0x00,}, 0 };
    bigint s8 = { {0x00,}, 0 };
    bigint s9 = { {0x00,}, 0 };
    uint32 add_mem[8] = { 0x00, };
    
    uint32 s1_mem[8] = { x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7] };
    uint32 s2_mem[8] = { 0x00,0x00,0x00,x[11],x[12],x[13],x[14],x[15] };
    uint32 s3_mem[8] = { 0x00,0x00,0x00,x[12],x[13],x[14],x[15],0x00 };
    uint32 s4_mem[8] = { x[8],x[9],x[10],0x00,0x00,0x00,x[14],x[15] };
    uint32 s5_mem[8] = { x[9],x[10],x[11],x[13],x[14],x[15],x[13],x[8] };
    uint32 s6_mem[8] = { x[11],x[12],x[13],0x00,0x00,0x00,x[8],x[10] };
    uint32 s7_mem[8] = { x[12],x[13],x[14],x[15],0x00,0x00,x[9],x[11] };
    uint32 s8_mem[8] = { x[13],x[14],x[15],x[8],x[9],x[10],0x00,x[12] };
    uint32 s9_mem[8] = { x[14],x[15],0x00,x[9],x[10],x[11],0x00,x[13] };
   // getMem(dest, add_mem);
    //print_mem(prime, "prime_redu");
    //getchar();
    getMem(&s1, s1_mem);
    getMem(&s2, s2_mem);
    getMem(&s3, s3_mem);
    getMem(&s4, s4_mem);
    getMem(&s5, s5_mem);
    getMem(&s6, s6_mem);
    getMem(&s7, s7_mem);
    getMem(&s8, s8_mem);
    getMem(&s9, s9_mem);
    
    add(&s1, &s2, &temp1, prime);
    minus_(&temp1, &s6, &temp2, prime);
    add(&temp2, &s2, &temp1, prime);
    minus_(&temp1, &s7, &temp2, prime);
    add(&temp2, &s3, &temp1, prime);
    minus_(&temp1, &s8, &temp2, prime);
    add(&temp2, &s3, &temp1, prime);
    minus_(&temp1, &s9, &temp2, prime);
    add(&temp2, &s4, &temp1, prime);
    add(&temp1, &s5, &temp2, prime);
   // print_mem(&temp2, "mul_reduction");
    memcpy(dest, &temp2, sizeof(bigint));
   // print_mem(dest, "mul_result");
}

void multi(bigint* src1, bigint* src2, bigint* dest, bigint* prime)
{
    unsigned long long res = 0x00;
    unsigned long long upper_tmp = 0x00;//tmp>>32
    unsigned long long lower_tmp = 0x00;//tmp&0xffffffff
    uint32 result[16] = { 0x00, };
    for (int i = 0; i < 8; i++) {
        //dest->bigInt[i] = 0x00;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            res = (unsigned long long)src1->bigInt[i] * (unsigned long long)src2->bigInt[j];
            upper_tmp = res >> 32;
            //printf("\nres :  %16x", res);

            lower_tmp = res & 0xffffffff;
            // cout << "\ni+j : " << i + j  << endl;
             //printf("\nbefore result[%d] %08x",i+j, result[i + j]);
            result[i + j] += lower_tmp;
            //printf("\nafter  result[%d] %08x",i+j, result[i + j]);
            if (lower_tmp > result[i + j])result[i + j + 1] += 1;
            //cout <<"\ni+j+1 : " << i + j + 1 << endl;
            //printf("\nbefore  result[%d]  %08x", i+j+1,result[i + j+1]);
            result[i + j + 1] += upper_tmp;
            //printf("\nafter result[%d] %08x",i+j+1, result[i + j+1]);

            if (upper_tmp > result[i + j + 1])result[i + j + 2] += 1;
        }
    }
    //printf("mul_prime값\%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", prime->bigInt[7], prime->bigInt[6], prime->bigInt[5], prime->bigInt[4], prime->bigInt[3], prime->bigInt[2], prime->bigInt[1], prime->bigInt[0]);
    //printf("answer\n***\n%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X\n***\n", result[15], result[14], result[13], result[12], result[11], result[10], result[9], result[8], result[7], result[6], result[5], result[4], result[3], result[2], result[1], result[0]);

    p256_mul_reduction(result, dest, prime);
    //print_mem(dest, "mul_result");
    //printf("answer : %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", answer->bigInt[7], answer->bigInt[6], answer->bigInt[5], answer->bigInt[4], answer->bigInt[3], answer->bigInt[2], answer->bigInt[1], answer->bigInt[0]);
   // printf("answer\n***\n%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X%08X\n***\n", result[15], result[14], result[13], result[12], result[11], result[10], result[9], result[8], result[7], result[6], result[5], result[4], result[3], result[2], result[1], result[0]);
     //return result;

}

void squ(bigint* src1, bigint* dest, bigint* prime)
{
    unsigned long long res = 0x00;
    unsigned long long upper_tmp = 0x00;//tmp>>32
    unsigned long long lower_tmp = 0x00;//tmp&0xffffffff
    uint32 result[16] = { 0x00, };
    for (int i = 0; i < 8; i++) {
        dest->bigInt[i] = 0x00;
    }
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            res = (unsigned long long)src1->bigInt[i] * (unsigned long long)src1->bigInt[j];
            upper_tmp = res >> 32;
            //printf("\nres :  %16x", res);

            lower_tmp = res & 0xffffffff;
            // cout << "\ni+j : " << i + j  << endl;
             //printf("\nbefore result[%d] %08x",i+j, result[i + j]);
            result[i + j] += lower_tmp;
            //printf("\nafter  result[%d] %08x",i+j, result[i + j]);
            if (lower_tmp > result[i + j])result[i + j + 1] += 1;
            //cout <<"\ni+j+1 : " << i + j + 1 << endl;
            //printf("\nbefore  result[%d]  %08x", i+j+1,result[i + j+1]);
            result[i + j + 1] += upper_tmp;
            //printf("\nafter result[%d] %08x",i+j+1, result[i + j+1]);

            if (upper_tmp > result[i + j + 1])result[i + j + 2] += 1;
        }
    }

    p256_mul_reduction(result, dest, prime);    
    //printf("answer\n***\n%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", result[0], result[1], result[2], result[3], result[4], result[5], result[6], result[7], result[8], result[9], result[10], result[11], result[12], result[13], result[14], result[15]);
     //return result;
}

void squ_Mul_times(int num, bigint* src, bigint* dest, bigint* prime)
{
    bigint result = { {0x00,}, };
    bigint squ_src = { {0x00,},0 };
    bigint temp = { {0x00,},0 };
    uint32 mem[8] = { 0x00000001, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    getMem(&temp, mem);
    squ(src, &squ_src, prime);//squ_src에 src의 제곱값 넣음
    //print_mem(&squ_src,"squ_src");
    //int count = 1;
    for (int i = 1; i < num; i++) {
        squ(&squ_src, &temp, prime);
        memcpy(&squ_src, &temp, sizeof(bigint));
        //print_mem(&temp,"dest");
        //count++;
    }
    memcpy(dest, &temp, sizeof(bigint));
}

void flt_inverse(bigint* z, bigint* dest, bigint* prime)
{
   
    bigint temp1 = { {0x00,}, 0 };
    bigint temp2 = { {0x00,}, 0 };
    bigint temp3 = { {0x00,}, 0 };
    bigint z_3 = { {0x00,}, 0};
    bigint z_15 = { {0x00,}, 0 };
    bigint t_0 = { {0x00,}, 0 };
    bigint t_1 = { {0x00,}, 0 };
    bigint t_2 = { {0x00,}, 0 };
    bigint t_3 = { {0x00,}, 0 };
    bigint t_4 = { {0x00,}, 0 };
    bigint t_5 = { {0x00,}, 0 };
    bigint result = { {0x00,}, 0 };
    /*
    uint32 mem[8] = { 0x00000001, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    getMem(&temp1, mem);
    getMem(&temp2, mem);
    getMem(&temp3, mem);
    getMem(&z_3, mem);
    getMem(&z_15, mem);
    getMem(&t_0, mem);
    getMem(&t_0, mem);
    getMem(&t_1, mem);
    getMem(&t_2, mem);
    getMem(&t_3, mem);
    getMem(&t_4, mem);
    getMem(&t_5, mem);
    getMem(&result, mem);
    //squ(bigint * x1, bigint * answer, bigint * prime)
    //multi(bigint* x1, bigint* x2, bigint* answer,bigint* prime)
    */
    //z_3
    squ(z, &temp1, prime);
    multi(&temp1,z,&z_3,prime);
    //print_mem(&z_3, "z_3");
    
    //z_15
    squ_Mul_times(2, &z_3, &temp1, prime);
    multi(&temp1, &z_3, &z_15, prime);
    //print_mem(&z_15, "z_15");
    
    //t_0
    squ_Mul_times(2, &z_15, &temp1, prime);
    multi(&temp1, &z_3, &t_0, prime);
    //print_mem(&t_0, "t_0");
    
    //t_1
    squ_Mul_times(6, &t_0, &temp1, prime);
   //print_mem(&temp1, "temp1");
    multi(&temp1, &t_0, &t_1, prime);
    //print_mem(&t_1, "t_1");
    
    
    //t_2
    squ_Mul_times(12, &t_1, &temp1, prime);
    multi(&temp1, &t_1, &temp2, prime);
    squ_Mul_times(6, &temp2, &temp3, prime);
    multi(&temp3, &t_0, &t_2, prime);
    //print_mem(&t_2, "t_2");
    
    
    //t_3
    squ_Mul_times(2, &t_2, &temp1, prime);
    multi(&temp1, &z_3, &t_3, prime);
    //print_mem(&t_3, "t_3");
    
    //t_4
    squ_Mul_times(32, &t_3, &temp1, prime);
    //print_mem(&temp1, "temp1");
    multi(&temp1, z, &temp2, prime);
    //print_mem(&temp2, "temp2");
    squ_Mul_times(96, &temp2, &t_4, prime);
    //print_mem(&t_4, "t_4");
    
    //t_5
    squ_Mul_times(32, &t_4, &temp1, prime);
    //print_mem(&temp1, "temp1");
    multi(&temp1, &t_3, &temp2, prime);
    //print_mem(&temp2, "temp2");
    squ_Mul_times(32, &temp2, &temp3, prime);
    //print_mem(&temp3, "temp3");
    multi(&temp3, &t_3, &t_5, prime);
    //print_mem(&t_5, "t_5");
    
   
    
    //result
    squ_Mul_times(30, &t_5, &temp1, prime);
    //print_mem(&temp1, "temp1");
    multi(&temp1, &t_2, &temp2, prime);
    //print_mem(&temp2, "temp2");
    squ_Mul_times(2, &temp2, &temp3, prime);
    //print_mem(&temp3, "temp3");
    multi(&temp3, z, &result, prime);
    //print_mem(&result, "result");
    

    memcpy(dest, &result, sizeof(bigint));
  // print_mem(dest, "inv_result ");
   
}

void file_check()
{
    int count = 0;
    FILE* fin = NULL;
    FILE* ffin = NULL;
    FILE* fffin = NULL;
    FILE* ffffin = NULL;
    fin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\TV_opA (8).txt", "r");
    if (fin == NULL) {
        fprintf(stderr, "fopen failed\n");
        return;
    }
    ffin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\TV_opB.txt", "r");
    if (ffin == NULL) {
        fprintf(stderr, "fopen failed\n");
        return;
    }
    fffin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\prime.txt", "r");
    if (fffin == NULL) {
        fprintf(stderr, "fopen failed\n");
        return;
    }

    ffffin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\inverse_flt_result.txt", "w");
    if (ffffin == NULL) {
        fprintf(stderr, "fopen failed\n");
        return;
    }
    bigint src1 = {{0x00,},};
    bigint src2 = { {0x00,}, };
    bigint prime = { {0x00,}, };
    getFileMem(fffin, &prime);
    //printf("filecheck_prime값\%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", prime.bigInt[7], prime.bigInt[6], prime.bigInt[5], prime.bigInt[4], prime.bigInt[3], prime.bigInt[2], prime.bigInt[1], prime.bigInt[0]);

    printf("***\n");
    bigint result = { {0x00,}, };
    //uint32 result_mul[17]={0x00,};

    while (!feof(fin)) {
        //cout << count << endl;
        count++;
        printf("%d\n", count);
        getFileMem(fin, &src1);
        //print_mem(&src1, "src1");
        getFileMem(ffin, &src2);
        //print_mem(&src2, "src2");
        /*
        for (int u = 0; u < 8; u++) {
            result->bigInt[u] = 0;
        }*/
        //printf("filecheck_prime값\%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", prime->bigInt[7], prime->bigInt[6], prime->bigInt[5], prime->bigInt[4], prime->bigInt[3], prime->bigInt[2], prime->bigInt[1], prime->bigInt[0]);

        /////////////////////////////////////////////////////////
        //**************파일 내에서 돌릴 함수 넣으면 됨****************
       // cout << "************" << count << "**********************" << endl;
       // squ(&src1,  &result, &prime);
       //multi(&src1, &src2, &result,&prime);
       // add(&src1, &src2, &result, &prime);
        //result = squ(x1, result,prime);
       flt_inverse(&src1, &result, &prime);
       if (count == 248)getchar();
       // squ(z, &temp1, prime);
        /*
        bigint temp1 = { {0x00,},0 };
        bigint temp2 = { {0x00,},0 };
        
        squ(&src1, &temp1, &prime);//2
        multi(&temp1, &src1, &temp2, &prime);//3
        multi(&temp2, &src1, &temp1, &prime);//4
        multi(&temp1, &src1, &temp2, &prime);//5
        multi(&temp2, &src1, &temp1, &prime);//6
        multi(&temp1, &src1, &temp2, &prime);//7
        multi(&temp2, &src1, &temp1, &prime);//8
        multi(&temp1, &src1, &temp2, &prime);//9
        multi(&temp2, &src1, &temp1, &prime);//10
        multi(&temp1, &src1, &temp2, &prime);//11
        multi(&temp2, &src1, &temp1, &prime);//12
        multi(&temp1, &src1, &temp2, &prime);//13
        multi(&temp2, &src1, &temp1, &prime);//14
        multi(&temp1, &src1, &temp2, &prime);//15
       
        print_mem(&temp2, "z_15");
        getchar();
        */
        //cout << "************" << count << "**********************" << endl;

        //////////////////////////////////////////////////////////

        for (int i = 7; i >= 0; i--) {
            fprintf(ffffin, "%08X", result.bigInt[i]);
        }fprintf(ffffin, "\n\n");

        //printf("answer\n***\n%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", result_mul[0], result_mul[1], result_mul[2], result_mul[3], result_mul[4], result_mul[5], result_mul[6], result_mul[7], result_mul[8], result_mul[9], result_mul[10], result_mul[11], result_mul[12], result_mul[13], result_mul[14], result_mul[15]);

    }
}

void ecADD(bigint* src1_x, bigint* src1_y, bigint* src2_x, bigint* src2_y,bigint* dest_x, bigint* dest_y, bigint* prime) {
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint temp1 = { {0x00,},0 };
    bigint temp2 = { {0x00,},0 };
    bigint temp3 = { {0x00,},0 };
    //print_mem(src1_x, "src1_***x");
    //print_mem(src1_y, "src1_***y");
    if (isEmpty(src1_x) && isEmpty(src1_y)) {
        init(src2_x, dest_x);
        init(src2_y, dest_y);
       // print_mem(dest_x, "src1 is empty   x");
        //print_mem(dest_y, "src1 is empty   y");
    }
    else if (isEmpty(src2_x) && isEmpty(src2_y)) {
        init(src1_x, dest_x);
        init(src1_y, dest_y);
        //print_mem(dest_x, "src2 is empty   x");
        //print_mem(dest_y, "src2 is empty   y");
    }
    else {
        minus_(src2_y, src1_y, &temp1, prime);
        minus_(src2_x, src1_x, &temp2, prime);
        flt_inverse(&temp2, &result_x, prime);
        multi(&temp1, &result_x, &temp2, prime);
        squ(&temp2, &temp1, prime);
        minus_(&temp1, src2_x, &temp2, prime);
        minus_(&temp2, src1_x, &result_x, prime);
        memcpy(dest_x, &result_x, sizeof(bigint));

        minus_(src2_y, src1_y, &temp1, prime);
        minus_(src2_x, src1_x, &temp2, prime);
        flt_inverse(&temp2, &result_y, prime);
        multi(&temp1, &result_y, &temp2, prime);
        minus_(src1_x, &result_x, &temp3, prime);
        multi(&temp2, &temp3, &temp1, prime);
        minus_(&temp1, src1_y, &result_y, prime);
        memcpy(dest_y, &result_y, sizeof(bigint));
    }
}

void ecDBL(bigint* src_x, bigint* src_y, bigint* a, bigint* dest_x, bigint* dest_y, bigint* prime) {
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint temp1 = { {0x00,},0 };
    bigint temp2 = { {0x00,},0 };
    bigint temp3 = { {0x00,},0 };

    squ(src_x, &temp1, prime);          // t = (x1)^2
    add(&temp1, &temp1, &temp2, prime); //2t
    add(&temp2, &temp1, &temp3, prime); //3t
   // print_mem(&temp3, "3x^2");
    add(&temp3,a, &temp1, prime);//분자 완료//3t + a
    //print_mem(&temp1, "3x^2 + a");

    add(src_y, src_y, &temp2, prime);   //2y
    flt_inverse(&temp2, &temp3, prime); //2y-1 = k
    //print_mem(&temp3, "2y^-1");
    multi(&temp1, &temp3, &temp2, prime); //tk
    //getchar();
    //print_mem(&temp2, "(3x^2 + a)*(2y^-1)");
    squ(&temp2, &temp1, prime);
    //print_mem(&temp1, "((3x^2 + a)*(2y^-1))^2 ");

    add(src_x, src_x, &temp3, prime);
    minus_(&temp1, &temp3, &result_x, prime);
    //print_mem(&result_x, "result");
    memcpy(dest_x , &result_x, sizeof(bigint));

    squ(src_x, &temp1, prime);
    add(&temp1, &temp1, &temp2, prime);
    add(&temp2, &temp1, &temp3, prime);
    add(a, &temp3, &temp1, prime);//분자 완료
    add(src_y, src_y, &temp2, prime);
    flt_inverse(&temp2, &temp3, prime);
    multi(&temp1, &temp3, &temp2, prime);
    minus_(src_x, &result_x, &temp1,prime);
    multi(&temp2, &temp1, &temp3, prime);
    minus_(&temp3, src_y, &result_y, prime);
    memcpy(dest_y, &result_y, sizeof(bigint));

}
bool shift(bigint* src, int arrNum) {
    int num = 0;
    num = (src->bigInt[arrNum] >> 1)&0x01;
    src->bigInt[arrNum] = src->bigInt[arrNum] >> 1;
    if (num == 1)return true;
    else return false;
}
/*
void Ecc_ScalaMul(bigint* scala, bigint* a, bigint* src_x, bigint* src_y, bigint* dest_x, bigint* dest_y, bigint* prime) {
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint temp_x = { {0x00,},0 };
    bigint temp_y = { {0x00,},0 };
    bigint temp_DBL_x1 = { {0x00,},0 };
    bigint temp_DBL_y1 = { {0x00,},0 };
    bigint temp_DBL_x2 = { {0x00,},0 };
    bigint temp_DBL_y2 = { {0x00,},0 };
    ecDBL(src_x, src_y,a,&temp_DBL_x1,&temp_DBL_y1,prime);//2p
    print_mem(&temp_DBL_x1, "scala_DBL_x");
    print_mem(&temp_DBL_y1, "scala_DBL_y");
    if (shift(scala,0)) {
        ecADD(src_x, src_y, &temp_x, &temp_y, &result_x, &result_y, prime);//p
        init(&result_x, &temp_x);
        init(&result_y, &temp_y);
        print_mem(&temp_x, "scala_x");
        print_mem(&temp_y, "scala_y");
    }
    for (int i = 1; i < 256; i++) {
        if (i % 32 == 0) {
            if ((shift(scala,i/32))) {
               // printf("%d, %d", i, (scala->bigInt[i / 32]) & 0x01) );
                printf("%d은 1이다\n", i);
                ecADD(&temp_x, &temp_y, &temp_DBL_x1, &temp_DBL_y1, &result_x, &result_y, prime);
                init(&result_x, &temp_x);
                init(&result_y, &temp_y);
            }
        }
        else{

     if ((shift(scala, i / 32))) {
        printf("%d은 1이다\n", i);
        ecADD(&temp_x, &temp_y, &temp_DBL_x1, &temp_DBL_y1, &result_x, &result_y, prime);
        init(&result_x, &temp_x);
        init(&result_y, &temp_y);

    }
    else {
        init(&temp_DBL_x1, &result_x);
        init(&temp_DBL_y1, &result_y);
    }
        }
        
        printf("%d\n", i);
       ecDBL(&temp_DBL_x1, &temp_DBL_y1, a, &temp_DBL_x2, &temp_DBL_y2, prime);
       init(&temp_DBL_x2, &temp_DBL_x1);
       init(&temp_DBL_y2, &temp_DBL_y1);
       print_mem(&temp_DBL_x2, "DBL_x");
       print_mem(&temp_DBL_y2, "DBL_y");
    }
    memcpy(dest_x, &result_x, sizeof(bigint));
    memcpy(dest_y, &result_y, sizeof(bigint));
    print_mem(dest_x, "x_dest");
    print_mem(dest_y, "y_dest");
}
*/
void Ecc_ScalaMul_R2L(bigint* scala, bigint* a, bigint* src_x, bigint* src_y, bigint* dest_x, bigint* dest_y, bigint* prime) {
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint temp_x1 = { {0x00,},0 };
    bigint temp_y1 = { {0x00,},0 };
    bigint temp_x2 = { {0x00,},0 };
    bigint temp_y2 = { {0x00,},0 };
    bigint temp_DBL_x = { {0x00,},0 };
    bigint temp_DBL_y = { {0x00,},0 };
   
    
    if ((scala->bigInt[0] & 0x01) == 1) {
       // printf("1번째 비트는 1입니다\n");
        ecADD(src_x, src_y, &result_x, &result_y, &temp_x1, &temp_y1, prime);  //temp1 = src
    }
    ecDBL(src_x, src_y, a, &temp_DBL_x, &temp_DBL_y, prime); //temp_DBL = src^2

    for (int i = 1; i < 256; i++) {
        if (i % 32 ==0 ) {
            if (scala->bigInt[i / 32] & 0x01 == 1) {
                //printf("다음 32비트\n");
                //printf("%d번째 비트는 1입니다\n", i);
                ecADD(&temp_DBL_x, &temp_DBL_y, &temp_x1, &temp_y1, &result_x, &result_y, prime);  //result = temp_DBL + temp
                //print_mem(&result_x, "result_x");
               // print_mem(&result_y, "result_y***");
                init(&result_x, &temp_x1);
                init(&result_y, &temp_y1);
                init(&temp_DBL_x, &temp_x2);
                init(&temp_DBL_y, &temp_y2);  //temp2 = temp_DBL
                ecDBL(&temp_x2, &temp_y2, a, &temp_DBL_x, &temp_DBL_y, prime); //temp_DBL = temp2^2
            }
            else {
               // printf("다음 32비트\n");
               // printf("%d번째 비트는 0입니다\n", i);
                init(&temp_DBL_x, &temp_x2);
                init(&temp_DBL_y, &temp_y2);
                ecDBL(&temp_x2, &temp_y2, a, &temp_DBL_x, &temp_DBL_y, prime);
            }
        }
        else if (shift(scala, i / 32)) {
           // printf("%d번째 비트는 1입니다\n", i);


            ecADD(&temp_DBL_x, &temp_DBL_y, &temp_x1, &temp_y1, &result_x, &result_y,  prime);  //result = temp_DBL + temp
          // print_mem(&result_x, "result_x");
           // print_mem(&result_y, "result_y***");
            init(&result_x, &temp_x1);
            init(&result_y, &temp_y1);
            init(&temp_DBL_x, &temp_x2);
            init(&temp_DBL_y, &temp_y2);  //temp2 = temp_DBL
            ecDBL(&temp_x2, &temp_y2, a, &temp_DBL_x, &temp_DBL_y, prime); //temp_DBL = temp2^2
        }
        else {
          //  printf("%d번째 비트는 0입니다\n", i);
            init(&temp_DBL_x, &temp_x2);
            init(&temp_DBL_y, &temp_y2);
            ecDBL(&temp_x2, &temp_y2, a, &temp_DBL_x, &temp_DBL_y, prime);
        }
    }
    //print_mem(&result_x, "result_x");
   // print_mem(&result_y, "result_y");
    init(&result_x, dest_x);
    init(&result_y, dest_y);
    
    //memcpy( &dest_x, &result_x, sizeof(bigint));
    //memcpy( &dest_y, &result_y, sizeof(bigint));
}

void Ecc_ScalaMul_L2R(bigint* scala, bigint* a, bigint* src_x, bigint* src_y, bigint* dest_x, bigint* dest_y, bigint* prime) {
    int flag = 0;
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint temp_x1 = { {0x00,},0 };
    bigint temp_y1 = { {0x00,},0 };
    bigint temp_x2 = { {0x00,},0 };
    bigint temp_y2 = { {0x00,},0 };
    bigint temp_DBL_x = { {0x00,},0 };
    bigint temp_DBL_y = { {0x00,},0 };

   

    for (int i = 255; i >= 0; i--) {
        if ((scala->bigInt[i / 32] >> (i % 32) & 0x01) == 1)flag = 1;
        if (flag == 1) {
            if ((scala->bigInt[i / 32] >> (i % 32) & 0x01) == 1) {
               // printf("%d번째 ,1입니다\n", i);

               if(isEmpty(&temp_x1)&& isEmpty(&temp_y1)){
                   //printf("empty\n");
                    init(src_x, &temp_x1);
                    init(src_y, &temp_y1);
               }
               else {
                   ecDBL(&temp_x1, &temp_y1, a, &temp_x2, &temp_y2, prime);
                   ecADD(&temp_x2, &temp_y2, src_x, src_y, &result_x, &result_y, prime);
                   init(&result_x, &temp_x1);
                   init(&result_y, &temp_y1);
               }
            }
            else {
                //printf("%d번째 ,0입니다\n", i);
                ecDBL(&temp_x1, &temp_y1, a, &temp_x2, &temp_y2, prime);
                init(&temp_x2 ,&result_x);
                init(&temp_y2 ,&result_y);
                init(&result_x, &temp_x1);
                init(&result_y, &temp_y1);
            }

        }
        
    }
    init(&result_x, dest_x);
    init(&result_y, dest_y);
}

void affine2jaco(bigint* src_aff_x,bigint* src_aff_y,bigint* dest_jaco_x,bigint* dest_jaco_y,bigint* dest_jaco_z) {
    init(src_aff_x, dest_jaco_x);
    init(src_aff_y, dest_jaco_y);
    uint32 mem[8] = { 0x00000001, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
    getMem(dest_jaco_z, mem);
}

void jaco2affine(bigint* src_jaco_x, bigint* src_jaco_y, bigint* src_jaco_z, bigint* dest_aff_x, bigint* dest_aff_y, bigint* prime) {
    //c=2,d=3
    bigint temp1 = { {0x00,},0 };
    bigint temp2 = { {0x00,},0 };
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };

    squ(src_jaco_z, &temp1, prime);//z^2
    flt_inverse(&temp1, &temp2, prime);
    multi(src_jaco_x,&temp2,&result_x,prime);

    multi(src_jaco_z,&temp1,&temp2,prime);//z^3
    flt_inverse(&temp2, &temp1, prime);
    multi(src_jaco_y, &temp1, &result_y, prime);

    init(&result_x, dest_aff_x);
    init(&result_y, dest_aff_y);
}
void bitShift_Once(bigint* src, bigint* dest) {
    /*
    dest->bigInt[0] = src->bigInt[0] >> 1;
    dest->bigInt[1] = src->bigInt[1] >> 1;
    dest->bigInt[2] = src->bigInt[2] >> 1;
    dest->bigInt[3] = src->bigInt[3] >> 1;
    dest->bigInt[4] = src->bigInt[4] >> 1;
    dest->bigInt[5] = src->bigInt[5] >> 1;
    dest->bigInt[6] = src->bigInt[6] >> 1;
    dest->bigInt[7] = src->bigInt[7] >> 1;
    print_mem(dest, "src_shift");

    dest->bigInt[0] |= (src->bigInt[1] & 0x01);
    dest->bigInt[1] |= (src->bigInt[2] & 0x01);
    dest->bigInt[2] |= (src->bigInt[3] & 0x01);
    dest->bigInt[3] |= (src->bigInt[4] & 0x01);
    dest->bigInt[4] |= (src->bigInt[5] & 0x01);
    dest->bigInt[5] |= (src->bigInt[6] & 0x01);
    dest->bigInt[6] |= (src->bigInt[7] & 0x01);   
    */
    for (int cnt_i = 0; cnt_i < 8; cnt_i++) {
        src->bigInt[cnt_i] = src->bigInt[cnt_i] >> 1;
        if (cnt_i < 7) {
            uint32 tmp = src->bigInt[cnt_i + 1] & 0x01;
            src->bigInt[cnt_i] = src->bigInt[cnt_i] | (tmp << 31);
        }
    }
    memcpy(dest, src, sizeof(bigint));
}




void ec_Div_2(bigint* src, bigint* dest, bigint* prime) {
    bigint temp = { {0x00,},0 };
    bigint two = { { 0x00000002, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 },0 };

    if ((src->bigInt[0] & 0x01) == 1) {// 홀
        noReduct_add(prime,src,  &temp, prime);
        //print_mem(&temp, "add");
        bitShift_Once(&temp, dest);
        if (temp.carry == 1) {
            //print_mem(dest, "dest");
            dest->bigInt[7] ^= 0x80000000;
            temp.carry = 0;
        }
    }
    else {//짝
        bitShift_Once(src, dest);
    }
}

void ecDBL_Jaco(bigint* src_x, bigint* src_y, bigint* src_z, bigint* dest_x, bigint* dest_y, bigint* dest_z, bigint* prime) {
    bigint t1 = { { 0x00, }, 0 };
    bigint t2 = { { 0x00, }, 0 };
    bigint t3 = { { 0x00, }, 0 };
    bigint temp = { { 0x00, }, 0 };
    bigint result_x = { { 0x00, }, 0 };
    bigint result_y = { { 0x00, }, 0 };
    bigint result_z = { { 0x00, }, 0 };
    bigint three = { { 0x00000003, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 },0 };
    bigint two = { { 0x00000002, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 },0 };

    squ(src_z, &t1, prime);//t1<-z1^2                          2
    minus_(src_x, &t1, &t2, prime);//t2<-x1-t1                 3
    init(&t1, &temp);
    add(src_x, &temp, &t1, prime);//t1<-x1+t1(temp)            4    
    init(&t2, &temp);
    multi(&temp, &t1, &t2, prime);//t2<-t2(temp)*t1            5
    init(&t2, &temp);
    multi(&three, &temp, &t2, prime);//                        6 
    //print_mem(&t2, "A"); 
    multi(&two, src_y, &result_y, prime);//                    7
    //print_mem(&result_y, "B");
    multi(&result_y, src_z, &result_z, prime);//               8
    init(&result_y, &temp);
    squ(&temp, &result_y, prime);//                            9 
    //print_mem(&result_y, "C");
    multi(&result_y, src_x, &t3, prime);//                     10
    //print_mem(&t3, "D");
    init(&result_y, &temp);
    squ(&temp, &result_y, prime);//                            11
    //print_mem(&result_y, "11");
    init(&result_y, &temp);
    ec_Div_2(&temp, &result_y, prime);//                       12
    //print_mem(&result_y, "12");
    squ(&t2, &result_x, prime);//                              13
    //print_mem(&result_x, "13");
    multi(&two, &t3, &t1, prime);//                            14
    //print_mem(&t1, "14");
    init(&result_x, &temp);
    minus_(&temp, &t1, &result_x, prime);//                    15
    //print_mem(&result_x, "15");
    minus_(&t3 , &result_x, &t1, prime);//                     16
    //print_mem(&t1, "16");
    init(&t1, &temp);
    multi(&temp, &t2, &t1, prime);//                           17
    //print_mem(&t1, "17");
    init(&result_y, &temp);
    minus_(&t1, &temp, &result_y,prime);//                     18
    //print_mem(&result_y, "18");
    //print_mem(&t2, "A");
   // print_mem(&t3, "D");
    init(&result_x, dest_x);
    init(&result_y, dest_y);
    init(&result_z, dest_z);
   // getchar();
}
bool isZero(bigint* src) {
    for (int i = 0; i < 8; i++) {
        if (src->bigInt[i] == 0x00)return 0;
    }
    return 1;
}
void ecADD_Jaco(bigint* src1_x, bigint* src1_y, bigint* src1_z, bigint* src2_x, bigint* src2_y, bigint* dest_x, bigint* dest_y, bigint* dest_z, bigint* prime) {
    bigint t1 = { { 0x00, }, 0 };
    bigint t2 = { { 0x00, }, 0 };
    bigint t3 = { { 0x00, }, 0 };
    bigint t4 = { { 0x00, }, 0 };
    bigint temp = { { 0x00, }, 0 };
    
    bigint x3 = { { 0x00, }, 0 };
    bigint y3 = { { 0x00, }, 0 };
    bigint z3 = { { 0x00, }, 0 };
    bigint one = { { 0x00000001, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 },0 };
    bigint two = { { 0x00000002, 0x00,0x00,0x00,0x00,0x00,0x00,0x00 },0 };

    //무한원점
    if (Compare(src1_x, src2_x) == 2) {
        printf("its infinite");
    }

    squ(src1_z,&t1,prime);//3
    //print_mem(&t1, "3");
    multi(&t1, src1_z, &t2, prime);//4
    //print_mem(&t2, "4");
    init(&t1, &temp);
    multi(&temp, src2_x, &t1,prime);//5
    //print_mem(&t1, "5");
    init(&t2, &temp);
    multi(&temp, src2_y, &t2, prime);//6
    //print_mem(&t2, "6");
    init(&t1, &temp);
    minus_(&temp, src1_x, &t1, prime);//7
    //print_mem(&t1, "7");
    init(&t2, &temp);
    minus_(&temp, src1_y, &t2, prime);//8
    //print_mem(&t2, "8");
    if (isZero(&t1) == 1) {                  //9
        if (isZero(&t2) == 1) {
            ecDBL_Jaco(src2_x, src2_y, &one, &x3, &y3, &z3, prime);
            //print_mem(&t1, "9");
        }
        else {
            printf("its infinite");
        }
    }
    multi(src1_z, &t1, &z3, prime);//10
    //print_mem(&z3, "10");
    squ(&t1, &t3, prime);//11
    //print_mem(&t3, "11");
    multi(&t3, &t1, &t4, prime);//12
    //print_mem(&t4, "12");
    init(&t3, &temp);
    multi(&temp, src1_x, &t3, prime);//13
    //print_mem(&t3, "13");
    multi( &t3,&two, &t1, prime);//14
    //print_mem(&t1, "14");
    squ(&t2, &x3, prime);//15
    //print_mem(&x3, "15");
    init(&x3, &temp);
    minus_(&temp, &t1, &x3, prime);//16
    //print_mem(&x3, "16");
    init(&x3, &temp);
    minus_(&temp, &t4, &x3, prime);//17
    //print_mem(&x3, "17");
    init(&t3, &temp);
    minus_(&temp, &x3, &t3, prime);//18
    //print_mem(&t3, "18");
    init(&t3, &temp);
    multi(&temp, &t2, &t3, prime);//19
    //print_mem(&t3, "19");
    init(&t4, &temp);
    multi(&temp, src1_y, &t4, prime);//20
    //print_mem(&t4, "20");
    minus_(&t3, &t4, &y3, prime);//21
    //print_mem(&y3, "3");
    //getchar();
    init(&x3, dest_x);
    init(&y3, dest_y);
    init(&z3, dest_z);
}

void Ecc_ScalaMul_L2R_jaco(bigint* scala, bigint* a, bigint* src_x, bigint* src_y, bigint* dest_x, bigint* dest_y, bigint* prime) {
    int flag = 0;
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint result_z = { {0x00,},0 };
    bigint temp_x1 = { {0x00,},0 };
    bigint temp_y1 = { {0x00,},0 };
    bigint temp_z1 = { {0x00,},0 };
    bigint temp_x2 = { {0x00,},0 };
    bigint temp_y2 = { {0x00,},0 };
    bigint temp_z2 = { {0x00,},0 };
    bigint src_jaco_x = { {0x00,},0 };
    bigint src_jaco_y = { {0x00,},0 };
    bigint src_jaco_z = { {0x00,},0 };
    bigint temp_DBL_x = { {0x00,},0 };
    bigint temp_DBL_y = { {0x00,},0 };
    bigint temp_DBL_z = { {0x00,},0 };
    bigint jaco_x = { {0x00,},0 };
    bigint jaco_y = { {0x00,},0 };
    bigint jaco_z = { {0x00,},0 };
    bigint one = { {0x00000001,0x00,0x00,0x00,0x00,0x00,0x00,0x00},0 };
    /*
    affine2jaco(src_x, src_y, &src_jaco_x, &src_jaco_y, &src_jaco_z);
    print_mem(&src_jaco_x, "src_jaco_x");
    print_mem(&src_jaco_y, "src_jaco_y");
    print_mem(&src_jaco_z, "src_jaco_z");
    */
    for (int i = 255; i >= 0; i--) {
        if ((scala->bigInt[i / 32] >> (i % 32) & 0x01) == 1)flag = 1;
        if (flag == 1) {
            if ((scala->bigInt[i / 32] >> (i % 32) & 0x01) == 1) {
                //printf("%d번째 ,1입니다\n", i);

                if (isEmpty(&temp_x1) && isEmpty(&temp_y1)) {
                    //printf("empty\n");
                    init(src_x, &temp_x1);
                    init(src_y, &temp_y1);
                    init(&one, &temp_z1);
                }
                else {
                    ecDBL_Jaco(&temp_x1, &temp_y1, &temp_z1, &temp_x2, &temp_y2, &temp_z2, prime);
                    //print_mem(&temp_x2, "temp_x");
                    //print_mem(&temp_y2, "temp_y");
                    //print_mem(&temp_z2, "temp_z");
                    ecADD_Jaco(&temp_x2, &temp_y2, &temp_z2, src_x,src_y ,&result_x, &result_y, &result_z, prime);
                    //print_mem(&result_x, "result_x");
                    //print_mem(&result_y, "result_y");
                    //print_mem(&result_z, "result_z");
                    init(&result_x, &temp_x1);
                    init(&result_y, &temp_y1);
                    init(&result_z, &temp_z1);
                }
            }
            else {
                //printf("%d번째 ,0입니다\n", i);
                ecDBL_Jaco(&temp_x1, &temp_y1, &temp_z1, &temp_x2, &temp_y2, &temp_z2, prime);
                init(&temp_x2, &result_x);
                init(&temp_y2, &result_y);
                init(&temp_z2, &result_z);
                init(&result_x, &temp_x1);
                init(&result_y, &temp_y1);
                init(&result_z, &temp_z1);
            }

        }

    }
    jaco2affine(&result_x, &result_y, &result_z, dest_x, dest_y, prime);
}

void scala_Mul_file_check() {
    int count = 0;
    FILE* fin = NULL;
    FILE* ffin = NULL;
    FILE* fffin = NULL;
    FILE* ffffin = NULL;
    
    
    fin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\TV_SM.txt", "r");
    if (fin == NULL) {
        fprintf(stderr, "fin fopen failed\n");
        return;
    }
    ffin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\TV_Scalar.txt", "r");
    if (ffin == NULL) {
        fprintf(stderr, "ffin fopen failed\n");
        return;
    }
    fffin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\prime.txt", "r");
    if (fffin == NULL) {
        fprintf(stderr, "fffin fopen failed\n");
        return;
    }

    ffffin = fopen("C:\\Users\\82103\\source\\repos\\ecc_2023_01\\ecc_2023_01\\sm_Mul_result.txt", "w");
    if (ffffin == NULL) {
        fprintf(stderr, "ffffin fopen failed\n");
        return;
    }

    bigint a = { {0x00,},0 };

    a.bigInt[0] = 0xFFFFFFFC;
    a.bigInt[1] = 0xFFFFFFFF;
    a.bigInt[2] = 0xFFFFFFFF;
    a.bigInt[3] = 0x00000000;
    a.bigInt[4] = 0x00000000;
    a.bigInt[5] = 0x00000000;
    a.bigInt[6] = 0x00000001;
    a.bigInt[7] = 0xFFFFFFFF;

    bigint src1 = { {0x00,}, };
    bigint src2 = { {0x00,}, };
    bigint prime = { {0x00,}, };
    bigint scala = { {0x00,},0 };

    getFileMem(fffin, &prime);
    //printf("filecheck_prime값\%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", prime.bigInt[7], prime.bigInt[6], prime.bigInt[5], prime.bigInt[4], prime.bigInt[3], prime.bigInt[2], prime.bigInt[1], prime.bigInt[0]);

    //printf("***\n");
    bigint result_x = { {0x00,}, };
    bigint result_y = { {0x00,}, };
    //uint32 result_mul[17]={0x00,};

    while (!feof(ffin)) {
        //cout << count << endl;
        count++;
        getFile_xy_Mem(fin, &src1,&src2);
        //print_mem(&src1, "src1");
        //print_mem(&src2, "src2");
        getFileMem(ffin, &scala);
        //print_mem(&scala, "scala");
        //print_mem(&src1, "src1");
        Ecc_ScalaMul_L2R_jaco(&scala,&a,&src1,&src2,&result_x,&result_y,&prime);
      //  print_mem(&result_x, "result_x");
       // print_mem(&result_y, "result_y");
        for (int i = 7; i >= 0; i--) {
            fprintf(ffffin, "%08X", result_x.bigInt[i]);
        }fprintf(ffffin, "\n");
        for (int i = 7; i >= 0; i--) {
            fprintf(ffffin, "%08X", result_y.bigInt[i]);
        }fprintf(ffffin, "\n\n");
    }
}


int main() {
   // scala_Mul_file_check();

    
    bigint prime = { {0x00,},0 };
    prime.bigInt[0] = 0xFFFFFFFF;
    prime.bigInt[1] = 0xFFFFFFFF;
    prime.bigInt[2] = 0xFFFFFFFF;
    prime.bigInt[3] = 0x00000000;
    prime.bigInt[4] = 0x00000000;
    prime.bigInt[5] = 0x00000000;
    prime.bigInt[6] = 0x00000001;
    prime.bigInt[7] = 0xFFFFFFFF;


    bigint x ={{0x00,},0};
    x.bigInt[7] = 0x6b17d1f2;
    x.bigInt[6] = 0xe12c4247;
    x.bigInt[5] = 0xf8bce6e5;
    x.bigInt[4] = 0x63a440f2;
    x.bigInt[3] = 0x77037d81;
    x.bigInt[2] = 0x2deb33a0;
    x.bigInt[1] = 0xf4a13945;
    x.bigInt[0] = 0xd898c296;

    bigint y = { {0x00,},0 };
    y.bigInt[7] = 0x4fe342e2;
    y.bigInt[6] = 0xfe1a7f9b;
    y.bigInt[5] = 0x8ee7eb4a;
    y.bigInt[4] = 0x7c0f9e16;
    y.bigInt[3] = 0x2bce3357;
    y.bigInt[2] = 0x6b315ece;
    y.bigInt[1] = 0xcbb64068;
    y.bigInt[0] = 0x37bf51f5; 
    
    

    bigint a = { {0x00,},0 };

    a.bigInt[0] = 0xFFFFFFFC;
    a.bigInt[1] = 0xFFFFFFFF;
    a.bigInt[2] = 0xFFFFFFFF;
    a.bigInt[3] = 0x00000000;
    a.bigInt[4] = 0x00000000;
    a.bigInt[5] = 0x00000000;
    a.bigInt[6] = 0x00000001;
    a.bigInt[7] = 0xFFFFFFFF;

    bigint scala = { {0x00,},0 };
    scala.bigInt[7] = 0xDDB7F114;
    scala.bigInt[6] = 0x71AFC9F6;
    scala.bigInt[5] = 0xB6D14865;
    scala.bigInt[4] = 0xB568A7A2;
    scala.bigInt[3] = 0xBA08EE99;
    scala.bigInt[2] = 0x5E4D9E0A;
    scala.bigInt[1] = 0x18671BCA;
    scala.bigInt[0] = 0x3933224B;

    bigint dbl_x = { {0x00,},0 };
    x.bigInt[7] = 0x7cf27b18;
    x.bigInt[6] = 0x8d034f7e;
    x.bigInt[5] = 0x8a523803;
    x.bigInt[4] = 0x04b51ac3;
    x.bigInt[3] = 0xc08969e2;
    x.bigInt[2] = 0x77f21b35;
    x.bigInt[1] = 0xa60b48fc;
    x.bigInt[0] = 0x47669978;

    bigint dbl_y = { {0x00,},0 };
    y.bigInt[7] =  07775510;
    y.bigInt[6] = 0xdb8ed040;
    y.bigInt[5] = 0x293d9ac6;
    y.bigInt[4] = 0x9f7430db;
    y.bigInt[3] = 0xba7dade6;
    y.bigInt[2] = 0x3ce98229;
    y.bigInt[1] = 0x9e04b79d;
    y.bigInt[0] = 0x227873d1;

    

    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint result_z = { {0x00,},0 };


    bigint result_dbl_x = { {0x00,},0 };
    bigint result_dbl_y = { {0x00,},0 };
    bigint temp_x = { {0x00,},0 };
    bigint temp_y = { {0x00,},0 };
    bigint temp_z = { {0x00,},0 };

    bigint temp1 = { {0x00,},0 };
    bigint temp2 = { {0x00,},0 };
    bigint temp3 = { {0x00,},0 };
    affine2jaco(&x, &y, &temp1, &temp2, &temp3);
    jaco2affine(&temp1, &temp2, &temp3 ,&x, &y,&prime);
    //print_mem(&x, "x****");
    //print_mem(&y, "y****");



    //printf("ecc dbl result\n");
    bigint resultx = { {0x00,},0 };
    bigint resulty = { {0x00,},0 };
    bigint resultz = { {0x00,},0 };
    bigint result_add_x = { {0x00,},0 };
    bigint result_add_y = { {0x00,},0 };
    bigint result_add_z = { {0x00,},0 };
    affine2jaco(&x, &y, &temp_x, &temp_y, &temp_z);
    ecDBL_Jaco(&temp_x, &temp_y, &temp_z,&resultx, &resulty, &resultz,&prime);
    //print_mem(&resultx, "dbl_result_x");
    //print_mem(&resulty, "dbl_result_y");
    //print_mem(&resultz, "dbl_result_z");
    jaco2affine(&resultx, &resulty, &resultz, &temp_x, &temp_y, &prime);
    //print_mem(&temp_x, "a**ff_dbl_result_x");
    //print_mem(&temp_y, "a**rr_dbl_result_y");
    ecADD_Jaco(&resultx, &resulty, &resultz, &x, &y, &result_add_x, &result_add_y, &result_add_z, &prime);
    //print_mem(&result_add_x, "add_result_x");
    //print_mem(&result_add_y, "add_result_x");
    //print_mem(&result_add_z, "add_result_x");
   
    jaco2affine(&result_add_x, &result_add_y, &result_add_z, &result_x, &result_y,&prime);
    //print_mem(&result_x, "3_x");
    //print_mem(&result_y, "3_y");

    //printf("ecc scala MUL result\n");
    bigint result_mul_x = { {0x00,},0 };
    bigint result_mul_y = { {0x00,},0 };
   
    bigint src_x = { {0x00,},0 };
    bigint src_y = { {0x00,},0 };
    bigint src_z = { {0x00,},0 };
    
    uint64_t start = 0;
    uint64_t end = 0;
    printf("성능 \n");
    start = __rdtsc();

    printf("memcpy 성능 : ");
    for (int i = 0; i < 10000; i++) {
         //ecDBL(&x, &y, &a, &result_x, &result_y, &prime);
         //ecADD( &x, &y, &dbl_x, &dbl_y, &result_x, &result_y, &prime);
         //Ecc_ScalaMul_R2L(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
         //Ecc_ScalaMul_L2R(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
         //affine2jaco(&x, &y, &src_x, &src_y, &src_z);
         //ecDBL_Jaco(&src_x, &src_y, &src_z, &result_x, &result_y, &result_z, &prime);
         //ecADD_Jaco(&src_x, &src_y,&src_z, &x, &y, &result_x, &result_y, &result_z, &prime);  
        // Ecc_ScalaMul_L2R_jaco(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
        // jaco2affine(&src_x, &src_y, &src_z, &x, &y);
        init(&x, &result_y);
        //memcpy(&result_x, &x, sizeof(bigint));
    }
    end = __rdtsc();
    printf("%llu\n", (end - start)/10000);
    printf("init 성능 : ");
    for (int i = 0; i < 100; i++) {
        //ecDBL(&x, &y, &a, &result_x, &result_y, &prime);
        //ecADD( &x, &y, &dbl_x, &dbl_y, &result_x, &result_y, &prime);
        //Ecc_ScalaMul_R2L(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
        //Ecc_ScalaMul_L2R(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
        //affine2jaco(&x, &y, &src_x, &src_y, &src_z);
        //ecDBL_Jaco(&src_x, &src_y, &src_z, &result_x, &result_y, &result_z, &prime);
        //ecADD_Jaco(&src_x, &src_y,&src_z, &x, &y, &result_x, &result_y, &result_z, &prime);  
        Ecc_ScalaMul_L2R_jaco(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
       // jaco2affine(&src_x, &src_y, &src_z, &x, &y);
        //init(&x, &result_y);
       // memcpy(&result_x, &x, sizeof(bigint));
    }
    end = __rdtsc();
    printf("%llu5", (end - start) / 100);
}