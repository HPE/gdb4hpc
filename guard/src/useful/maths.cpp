/*********************************************************************************\
 * maths.c
 *
 * Copyright 2010-2013, 2020 Hewlett Packard Enterprise Development LP.
 *
 * Unpublished Proprietary Information.
 * This unpublished work is protected to trade secret, copyright and other laws.
 * Except as permitted by contract or express written permission of Hewlett Packard Enterprise Development LP.,
 * no part of this work or its content may be used, reproduced or disclosed
 * in any form.
 *
 * $HeadURL$
 * $Date$
 * $Rev$
 * $Author$
 *
 *********************************************************************************/

#include "maths.h"

int min(int a, int b) { return a < b ? a : b; }

int max(int a, int b) { return a > b ? a : b; }

int lcm(int a, int b) {
    int maximum = max(a, b);
    int n = maximum;
    int i = 1;
    while (((n % a) != 0) || ((n % b) != 0)) {
        n = maximum * (++i);
    }
    return n;
}

int gcd(int a, int b) {
    int n = min(a, b);
    while (((a % n) != 0) || ((b % n) != 0)) n--;
    return n;
}

/* this function works out the lowest common factor which is not 1
 * for example: lcf(4,6)=2
 */
int lcf(int a, int b, int c) {
    int result = c;
    int found = 0;
    while (result <= a && result <= b) {
        if (a % result == 0 && b % result == 0) {
            found = 1;
            break;
        }
        result++;
    }
    if (found) return result;
    return -1;
}

long int product(int *nums, int no) {
    long int result = 1;
    int i;
    for (i = 0; i < no; i++) result *= nums[i];
    return result;
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void sort(int arr[], int beg, int end) {
    if (end > beg + 1) {
        int piv = arr[beg], l = beg + 1, r = end;
        while (l < r) {
            if (arr[l] <= piv)
                l++;
            else
                swap(&arr[l], &arr[--r]);
        }
        swap(&arr[--l], &arr[beg]);
        sort(arr, beg, l);
        sort(arr, r, end);
    }
}

int quickSort(int *arr, int elements) {
#define MAX_LEVELS 1000

    int piv, beg[MAX_LEVELS], end[MAX_LEVELS], i = 0, L, R;

    beg[0] = 0;
    end[0] = elements;
    while (i >= 0) {
        L = beg[i];
        R = end[i] - 1;
        if (L < R) {
            piv = arr[L];
            if (i == MAX_LEVELS - 1) return 0;
            while (L < R) {
                while (arr[R] >= piv && L < R) R--;
                if (L < R) arr[L++] = arr[R];
                while (arr[L] <= piv && L < R) L++;
                if (L < R) arr[R--] = arr[L];
            }
            arr[L] = piv;
            beg[i + 1] = L + 1;
            end[i + 1] = end[i];
            end[i++] = L;
        } else
            i--;
    }
    return 1;
}
