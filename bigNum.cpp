#define _CRT_SECURE_NO_WARNINGS

#include "ecc.h"
#include<iostream>
#include <fstream>

void print_mem(bigint* x,const char* arr) {
    printf("%s  :   %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n",arr, x->bigInt[7], x->bigInt[6], x->bigInt[5], x->bigInt[4], x->bigInt[3], x->bigInt[2], x->bigInt[1], x->bigInt[0]);

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
    if (src->carry == 1) {
        
        //printf("reduction\n");
        uint32 carry = 0;
        // printf("sum : %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", x->bigInt[0], x->bigInt[1], x->bigInt[2], x->bigInt[3], x->bigInt[4], x->bigInt[5], x->bigInt[6], x->bigInt[7]);

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
        // printf("prime : %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", ~(prime->bigInt[0]), ~(prime->bigInt[1]), ~(prime->bigInt[2]), ~(prime->bigInt[3]), ~(prime->bigInt[4]), ~(prime->bigInt[5]), ~(prime->bigInt[6]), ~(prime->bigInt[7]));

       // printf("result : %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", result->bigInt[0], result->bigInt[1], result->bigInt[2], result->bigInt[3], result->bigInt[4], result->bigInt[5], result->bigInt[6], result->bigInt[7]);
    }
    else {
        memcpy(dest, src, sizeof(bigint));
    }
   // print_mem(dest, "add_reduction _result");
   // printf("******\n");
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
    //printf("*%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", sum->bigInt[0], sum->bigInt[1], sum->bigInt[2], sum->bigInt[3], sum->bigInt[4], sum->bigInt[5], sum->bigInt[6], sum->bigInt[7]);

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
                //printf(" %08x - %08x -%d = %08x\n", x1->bigInt[i] , x2->bigInt[i] , borrow,result->bigInt[i]);
                borrow = 0;
            }
            else {
                result.bigInt[i] = (0xffffffff - src2->bigInt[i]) + 1 + src1->bigInt[i] - borrow;
                //result->bigInt[i] = (x1->bigInt[i]) + 1 + ~(x2->bigInt[i]-borrow);
                //cout << borrow << endl;;
                //printf(" %08x  +1 + %08x -borrow(%d)= %08x\n", ~(x1->bigInt[i])  , x2->bigInt[i],borrow,result->bigInt[i]);
                //printf("0xffffffff - %08x +1 + %08x = %08x\n", x2->bigInt[i] , x1->bigInt[i],result->bigInt[i]);
                borrow = 1;
            }
        }
    }
    else if (Compare(src1, src2) == 1) {//x1<x2
        int carry = 0;
        for (int i = 0; i < 8; i++) {
            if (src1->bigInt[i] - borrow >= src2->bigInt[i]) {
                //          cout<<"if"<<endl;
                result.bigInt[i] = src1->bigInt[i] - src2->bigInt[i] - borrow;
                result.bigInt[i] = result.bigInt[i] + prime->bigInt[i] + carry;
                //printf("*%08x  -  %08x-%dborrow + %dcarry +%08x = %08x\n", x1->bigInt[i], x2->bigInt[i], borrow,carry,prime->bigInt[i], result->bigInt[i]);

                if (prime->bigInt[i] > result.bigInt[i])carry = 1;
                else carry = 0;
                borrow = 0;
            }

            else {
                result.bigInt[i] = (0xffffffff - src2->bigInt[i]) + 1 + src1->bigInt[i] - borrow;
                result.bigInt[i] += prime->bigInt[i] + carry;
                //printf("*%08x  -  %08x-%dborrow + %dcarry +%08x = %08x\n", x1->bigInt[i], x2->bigInt[i], borrow, carry, prime->bigInt[i], result->bigInt[i]);
                borrow = 1;
                if (prime->bigInt[i] > result.bigInt[i]) { carry = 0; borrow = 0; }
                else carry = 0;

            }
        }
    }
    memcpy(dest, &result, sizeof(bigint));
    //printf("minus__%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", result->bigInt[0], result->bigInt[1], result->bigInt[2], result->bigInt[3], result->bigInt[4], result->bigInt[5], result ->bigInt[6], result->bigInt[7]);
    //print_mem(dest, "minus result");

}

void p256_mul_reduction(uint32 x[16], bigint* dest, bigint* prime)
{
    
    //printf("reduction 배열 \n***\n%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7], x[8], x[9], x[10], x[11], x[12], x[13], x[14], x[15]);

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
    //printf("mul_prime값\%08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n***\n", prime->bigInt[7], prime->bigInt[6], prime->bigInt[5], prime->bigInt[4], prime->bigInt[3], prime->bigInt[2], prime->bigInt[1], prime->bigInt[0]);

    p256_mul_reduction(result, dest, prime);
    //printf("answer : %08x,%08x,%08x,%08x,%08x,%08x,%08x,%08x\n\n", answer->bigInt[7], answer->bigInt[6], answer->bigInt[5], answer->bigInt[4], answer->bigInt[3], answer->bigInt[2], answer->bigInt[1], answer->bigInt[0]);
    
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
   // print_mem(&result, "result");
    //getchar();

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
    print_mem(src1_x, "src1_***x");
    print_mem(src1_y, "src1_***y");
    if (isEmpty(src1_x) && isEmpty(src1_y)) {
        init(src2_x, dest_x);
        init(src2_y, dest_y);
        print_mem(dest_x, "src1 is empty   x");
        print_mem(dest_y, "src1 is empty   y");
    }
    else if (isEmpty(src2_x) && isEmpty(src2_y)) {
        init(src1_x, dest_x);
        init(src1_y, dest_y);
        print_mem(dest_x, "src2 is empty   x");
        print_mem(dest_y, "src2 is empty   y");
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
    num = src->bigInt[arrNum] >> 1;
    src->bigInt[arrNum] = src->bigInt[arrNum] >> 1;
    if (num == 1)return true;
    else return false;
}
void Ecc_ScalaMul(bigint* scala, bigint* a, bigint* src_x, bigint* src_y, bigint* dest_x, bigint* dest_y, bigint* prime) {
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };
    bigint temp_x = { {0x00,},0 };
    bigint temp_y = { {0x00,},0 };
    bigint temp_DBL_x1 = { {0x00,},0 };
    bigint temp_DBL_y1 = { {0x00,},0 };
    bigint temp_DBL_x2 = { {0x00,},0 };
    bigint temp_DBL_y2 = { {0x00,},0 };
    ecDBL(src_x, src_y,a,&temp_DBL_x1,&temp_DBL_y1,prime);
    print_mem(&temp_DBL_x1, "scala_DBL_x");
    print_mem(&temp_DBL_y1, "scala_DBL_y");
    if (scala->bigInt[0] & 0x01 == 1) {
        ecADD(src_x, src_y, &temp_x, &temp_y, &result_x, &result_y, prime);
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

int main() {
    bigint result_x = { {0x00,},0 };
    bigint result_y = { {0x00,},0 };

    bigint x ={{0x00,},0};
    x.bigInt[7] = 0x6b17d1f2;
    x.bigInt[6] = 0xe12c4247;
    x.bigInt[5] = 0xf8bce6e5;
    x.bigInt[4] = 0x63a440f2;
    x.bigInt[3] = 0x77037d81;
    x.bigInt[2] = 0x2deb33a0;
    x.bigInt[1] = 0xf4a13945;
    x.bigInt[0] = 0xd898c296;
    /*
    for (int i = 0; i < 32; i++) {
        shift(&x, 0);
        print_mem(&x, "x");
    }
    getchar();
    */

    bigint y = { {0x00,},0 };
    y.bigInt[7] = 0x4fe342e2;
    y.bigInt[6] = 0xfe1a7f9b;
    y.bigInt[5] = 0x8ee7eb4a;
    y.bigInt[4] = 0x7c0f9e16;
    y.bigInt[3] = 0x2bce3357;
    y.bigInt[2] = 0x6b315ece;
    y.bigInt[1] = 0xcbb64068;
    y.bigInt[0] = 0x37bf51f5; 

    bigint prime = { {0x00,},0 };
    prime.bigInt[0] = 0xFFFFFFFF;
    prime.bigInt[1] = 0xFFFFFFFF;
    prime.bigInt[2] = 0xFFFFFFFF;
    prime.bigInt[3] = 0x00000000;
    prime.bigInt[4] = 0x00000000;
    prime.bigInt[5] = 0x00000000;
    prime.bigInt[6] = 0x00000001;
    prime.bigInt[7] = 0xFFFFFFFF;

    bigint a = { {0x00,},0 };

    a.bigInt[0] = 0xFFFFFFFC;
    a.bigInt[1] = 0xFFFFFFFF;
    a.bigInt[2] = 0xFFFFFFFF;
    a.bigInt[3] = 0x00000000;
    a.bigInt[4] = 0x00000000;
    a.bigInt[5] = 0x00000000;
    a.bigInt[6] = 0x00000001;
    a.bigInt[7] = 0xFFFFFFFF;
    
    bigint test = { {0x00,},0 };
    test.bigInt[0] = 0xFFFFFFFF;
    test.bigInt[1] = 0xFFFFFFFF;
    test.bigInt[2] = 0xFFFFFFFF;
    test.bigInt[3] = 0xFFFFFFFF;
    test.bigInt[4] = 0xFFFFFFFF;
    test.bigInt[5] = 0xFFFFFFFF;
    test.bigInt[7] = 0x00000000;
    test.bigInt[6] = 0xFFFFFFFF;

    bigint test_x = { {0x00,},0 };
    test_x.bigInt[7] = 0xcae428e8; 
    test_x.bigInt[6] = 0x7d3cd816;
    test_x.bigInt[5] = 0x8484cf1a; 
    test_x.bigInt[4] = 0x2aa3b288; 
    test_x.bigInt[3] = 0x9b23cbdb; 
    test_x.bigInt[2] = 0x183be973; 
    test_x.bigInt[1] = 0x0080b044; 
    test_x.bigInt[0] = 0xc741bea5;
    
    printf("ecc DBL result\n");
    ecDBL(&x, &y, &a, &result_x, &result_y, &prime);
    print_mem(&result_x, "x");
    print_mem(&result_y, "y");

    bigint result_add_x = { {0x00,},0 };
    bigint result_add_y = { {0x00,},0 };

    printf("ecc Add result\n");
    ecADD(&x, &y,  &result_x, &result_y, &result_add_x, &result_add_y, &prime);
    print_mem(&result_add_x, "x");
    print_mem(&result_add_y, "y");

    printf("ecc scala MUL result\n");
    bigint result_mul_x = { {0x00,},0 };
    bigint result_mul_y = { {0x00,},0 };
    bigint scala = { {0x00,},0 };
    scala.bigInt[7] = 0xDDB7F114;
    scala.bigInt[6] = 0x71AFC9F6;
    scala.bigInt[5] = 0xB6D14865;
    scala.bigInt[4] = 0xB568A7A2;
    scala.bigInt[3] = 0xBA08EE99;
    scala.bigInt[2] = 0x5E4D9E0A;
    scala.bigInt[1] = 0x18671BCA;
    scala.bigInt[0] = 0x3933224B;
    Ecc_ScalaMul(&scala, &a, &x, &y, &result_mul_x, &result_mul_y, &prime);
    print_mem(&result_mul_x, "scala mul x");
    print_mem(&result_mul_y, "scala mul y");
    //add(&a,&test_x, &result_x, &prime);
    //print_mem(&result_x, "add");
    
    //file_check();
    
}