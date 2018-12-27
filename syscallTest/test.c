#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main()
{  
    //long int ret_status = syscall(333); // 333 is the syscall number
    long int ret_status = getCpuId();   
    printf("salam googoolam! Syscall returned %ld\n", ret_status);
          
     return 0;
}
