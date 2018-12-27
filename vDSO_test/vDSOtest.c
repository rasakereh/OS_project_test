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

getcpu_t vdso_getcpu;
getcpu_t vgetcpu;

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

	vdso_getcpu = (getcpu_t)dlsym(vdso, "__vdso_getcpu");
	if (!vdso_getcpu)
		printf("Warning: failed to find getcpu in vDSO\n");

	vgetcpu = (getcpu_t) vsyscall_getcpu();
	
	getcpu_t vgetcpu = (getcpu_t)VSYS(0xffffffffff600800);
}

int main()
{
	printf("vSys output %d\n", 0);
	printf("vSys output %d\n", 0);
}

