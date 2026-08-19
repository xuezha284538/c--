#include <stdio.h>
#include <stdlib.h>

int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
    int i = 0, j = numsSize - 1;
    int k = numsSize;
    while (1) {
        if (j > 0 && nums[j] + nums[i] != target) {
            /*if (i < numsSize && nums[i] > target) {
                i++;
            }
            if (j > 0 && nums[j] > target) {
                j--;
                k = j;
            }*/

            if (i < numsSize && i == j - 1 || i > j) {
                i++;
                j = k;
            }
        }
        if (j < numsSize && nums[j] + nums[i] == target) {
            int* ret = malloc(sizeof(int) * 2);
            ret[0] = i, ret[1] = j;
            *returnSize = 2;
            return ret;
        }
        j--;
    }
    *returnSize = 0;
    return NULL;
}

int main()
{
    int nums[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 }, target = 6;
    int returnSize[2] = { 0 };
    int numsSize = sizeof(nums) / sizeof(nums[0]);
    int* ret = twoSum(nums, numsSize, target, returnSize);

    printf("%d,%d", ret[0], ret[1]);

    return 0;
}
