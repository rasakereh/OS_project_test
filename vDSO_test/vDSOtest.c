#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <sched.h>
#include <stdbool.h>

typedef long (*getcpu_t)(void);

getcpu_t vdso_oscpuid;
getcpu_t voscpuid;

static void fill_function_pointers()
{
	void *vdso = dlopen("linux-vdso.so.1",
			    RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD);
	if (!vdso)
		vdso = dlopen("linux-gate.so.1",
			      RTLD_LAZY | RTLD_LOCAL | RTLD_NOLOAD);
	if (!vdso) {
		printf("[WARN]\tfailed to find vDSO\n");
		return;
	}

	vdso_oscpuid = (getcpu_t)dlsym(vdso, "__vdso_oscpuid");
	if (!vdso_oscpuid)
		printf("Warning: failed to find oscpuid in vDSO\n");

	voscpuid = (getcpu_t)(0xffffffffff600c00);
}

void exec_time(getcpu_t f)
{
	double sum = 0;
	int i = 0;
	long res = 0;
	for(i = 0; i < 100; i++)
	{
		clock_t begin = clock();

		res = f();

		clock_t end = clock();
		double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		
		sum += time_spent;
	}
	sum /= 100;
	
	printf("call returns %ld, done in %f\n", res, sum);
}

int main()
{
	fill_function_pointers();
	printf("vSys\n");
	exec_time(voscpuid);
	printf("vDSO\n");
	exec_time(vdso_oscpuid);
	
	return 0;
}

