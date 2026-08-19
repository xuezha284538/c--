#include <stdio.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
    int i, j;
    for (i = 0; i < numsSize; i++) {
        // int temp=nums[i];
        for (j = numsSize; j > i; j--) {
            if (i != j) {
                if (nums[i] + nums[j] == target) {
                    int* ret = malloc(sizeof(int) * 2);
                    ret[0] = i, ret[1] = j;
                    *returnSize = 2;
                    return ret;
                }
            }
        }
    }
    *returnSize = 0;
    return NULL;
}

int main()
{
    int nums[] = { 3, 3 }, target = 6;

    int i, j;
    int numsSize = sizeof(nums) / sizeof(int);
    for (i = 0; i < numsSize; i++) {
        // int temp=nums[i];
        for (j = numsSize; j > i; j--) {
            if (i != j) {
                if (nums[i] + nums[j] == target) {
                    printf("[%d,%d]", i, j);
                }
            }
        }
    }
}
