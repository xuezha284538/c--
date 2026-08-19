/** 
 * type.c
 * 
 * @author xuezha@qq.com
 * @date 2025/11/22
 */
# include <stdio.h>


int main()
{
     // printf("")参数一:字符串模板
     // sizeof 获得类型或变量在内存中占用的空间大小,单位字节
     // 复制:shift +alt+ 下
     //撤销 ctrl+z
     //编辑: ctrl alt+下
     //% 占位符,%d格式化输出整数,%ld 格式输出longint
    printf("大小:%ld \n",sizeof(char));       
    printf("大小:%ld \n",sizeof(int));        
    printf("大小:%ld \n",sizeof(long));  
    printf("大小:%ld \n",sizeof(long long));  
    printf("大小:%ld \n",sizeof(float));  
    printf("大小:%ld \n",sizeof(double));  
    printf("大小:%ld \n",sizeof(_Bool));  
    printf("大小:%ld \n",sizeof(long double));  
    
    
    

   
    return 0;


}








