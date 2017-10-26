#include <pthread.h>
#include <stdlib.h>
#include <assert.h>
#define INT_MAX (~(1U << 31))
// Set this to 2 or 3, depending what you want the last counter value to be
#define LOOPS 2

int int_max = INT_MAX;
// We are maninly concerned what happens when handling an overflow
int a_cntr = INT_MAX - 4;
int b_cntr = INT_MAX - 4;
int exit;

int check_a, check_a2, check_b, check_b2;

#define tst_atomic_inc(it) (++(*it))
#define tst_atomic_load(it) (*it)
#define tst_atomic_store(val, loc) (*loc = val)

// I am using a macro because Modex/Spin/Promela don't seem to handle C
// function calls very well. Although the docs suggest it should work
#define tst_fzsync_pair_wait(our_cntr, other_cntr)			\
	int o;								\
	o = tst_atomic_inc(other_cntr);					\
	if (o == int_max) {						\
		while (tst_atomic_load(our_cntr) > 0			\
		       && tst_atomic_load(our_cntr) < INT_MAX		\
		       && !tst_atomic_load(&exit))			\
			;						\
									\
		tst_atomic_store(0, other_cntr);			\
		while (tst_atomic_load(our_cntr) > 1			\
		       && !tst_atomic_load(&exit))			\
			;						\
	} else {							\
		while (tst_atomic_load(our_cntr) < tst_atomic_load(other_cntr) \
		       && !tst_atomic_load(&exit))			\
			;						\
	}								\
	cont = !exit || *other_cntr <= *our_cntr

void * thread_a()
{
	int i, cont;

	for (i = 0; i < LOOPS; i++) {
		tst_fzsync_pair_wait(&a_cntr, &b_cntr);
		check_a++;
		tst_fzsync_pair_wait(&a_cntr, &b_cntr);
		check_a2++;
		assert(check_a == check_b);
	}

	exit = 1;
}

void *thread_b()
{
	int cont;

	while(1) {
		tst_fzsync_pair_wait(&b_cntr, &a_cntr);
		if (!cont) break;
		check_b++;
		tst_fzsync_pair_wait(&b_cntr, &a_cntr);
		if (!cont) break;
		check_b2++;
		assert(check_a == check_b);
	}

	assert(check_a2 == check_b2);
}
