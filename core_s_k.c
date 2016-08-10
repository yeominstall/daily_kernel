/* kernel/sched/core.c */
/* linux kernel 3.10 */
/* schedule() -> context_switch(rq, prev, next) */

asmlinkage void __sched schedule(void)
{
	/*
	* if we come here after a random call to set_need_resched(),
	* or we have been woken up remotely but the IPI has not yet arrived,
	* we haven't yet exited the RCU idle mode. Do it here manually until
	* we find a better solution.
	*/
	user_exit();
	schedule();
	user_enter();
}


