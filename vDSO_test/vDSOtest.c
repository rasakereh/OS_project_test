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
	if (!vdso_getcpu)
		printf("Warning: failed to find oscpuid in vDSO\n");

	voscpuid = (getcpu_t)(0xffffffffff600c00);
}

void exec_time(getcpu_t f)
{
	clock_t begin = clock();

	long res = f();

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	
	printf("call returns %ld, done in %f\n", res, time_spent);
}

int main()
{
	fill_function_pointers();
	printf("vSys\n");
	exec_time(voscpuid);
	printf("vDSO\n");
	exec_time(vdso_getcpu);
	
	return 0;
}

