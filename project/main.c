#include <stdio.h>

#define sort_debug(fmt...) printf(fmt)
enum
{
	    FALSE = 0,
	        TRUE = 1
};

typedef int BOOL;

void swap(int *src, int *dst)
{
	    int k;
	        k = *src;
		    *src = *dst;
		        *dst = k;
}

/**
 *  * @brief bubble1 基本排序，复杂度O(n2)
 *   * @param arr 待排序数组地址
 *    * @param n 数组长度
 *     * @return
 *      */
int bubble1(int *arr, int n)
{
	    int i,j;

	        for(i=0; i<n; i++)
			    {
				            for(j=1; j<n-i; j++)
						            {
								                if(arr[i] > arr[j])
											            {
													                    swap(&arr[i], &arr[j]);
															                }
										        }
					        }

		    return 0;
}

/**
 *  * @brief bubble2 基本排序优化1，复杂度
 *   * @param arr 待排序数组地址
 *    * @param n 数组长度
 *     * @return
 *      */
int bubble2(int *arr, int n)
{
	    int i,j;
	        BOOL flag = TRUE;
		    j = n;

		        while(flag)
				    {
					            flag = FALSE;

						            for(i=1; i<j; i++)
								            {
										                if(arr[i-1] > arr[i])
													            {
															                    swap(&arr[i-1], &arr[i]);
																	                    flag = TRUE;
																			                }
												        }

							            j--;
								        }

			    return 0;
}

/**
 *  * @brief bubble3 基本排序优化2，复杂度
 *   * @param arr 待排序数组地址
 *    * @param n 数组长度
 *     * @return
 *      */
int bubble3(int *arr, int n)
{
	    int i,j;
	        int flag = n;

		    while(flag > 0)
			        {
					        j = flag;
						        flag = 0;
							        for(i=1; i<j; i++)
									        {
											            if(arr[i-1] > arr[i])
													                {
																                swap(&arr[i-1], &arr[i]);
																		                flag = i;
																				            }
												            }
								    }

		        return 0;
}

int print(int *arr, int n)
{
	    int i = 0;
	        for(; i<n; i++)
			    {
				            sort_debug("arr[%d] : %d\n", i, arr[i]);
					        }

		    return 0;
}

int main(void)
{
	    int array[] = {101, 45, 64, 12, 31};

	        if(bubble3(array, sizeof(array)/sizeof(int)) != 0)
			    {
				            printf("sort failed!\n");
					            return -1;
						        }

		    print(array, sizeof(array)/sizeof(int));

		        return 0;
}


