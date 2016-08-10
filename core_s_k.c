/* kernel/sched/core.c */
/* linux kernel 3.10 */
/* schedule() -> context_switch(rq, prev, next) */

asmlinkage void __sched schedule(void)
{
	struct task_struct *tsk = current;

	sched_submit_work(tsk);
	__schedule();
}
EXPORT_SYMBOL(schedule);

#ifdef CONFIG_CONTEXT_TRACKING
asmlinkage void __sched schedule(void) {
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
#endif

/*
* __schedule() is the main scheduler function.
* ...
*/

static void __sched __schedule(void) {
	struct task_struct *prev, *next;
	unsigned long *switch_count;
	struct rq *rq;
	int cpu;

need_resched:
	preempt_disable();
	cpu = smp_processor_id();
	rq = cpu_rq(cpu);
	rcu_note_context_switch(cpu);
	prev = rq->curr;

	schedule_debug(prev);

	if(sched_feat(HRTICK))
		hrtick_clear(rq);

	smp_mb__before_spinlock();
	raw_spin_lock_irq(&rq->lock);

	switch_count = &prev->nivcsw;
	if(prev->state && !(preempt_count() & PREEMPT_ACTIVE)) {
		if(unlikely(signal_pending_state(prev->state, prev))) {
			prev->state = TASK_RUNNING;
		} else {
			deactivate_task(rq, prev, DEQUEUE_SLEEP);
			prev->on_rq = 0;

			if(prev->flags & PF_WQ_WORKER) {
				struct task_struct *to_wakeup;

				to_wakeup = wq_worker_sleeping(prev, cpu);
				if(to_wakeup)
					try_to_wake_up_local(to_wakeup);
			}
		}
		switch_count = &prev->nvcsw;
	}

	pre_schedule(rq, prev);

	if(unlikely(!rq->nr_running))
		idle_balance(cpu_rq);

	put_prev_task(rq, prev);
	next = pick_next_task(rq);
	clear_tsk_need_resched(prev);
	rq->skip_clock_update = 0;

	if(likely(prev != next)) {
		rq->nr_switches++;
		rq->curr = next;
		++*switch_count;

		context_switch(rq, prev, next);

		cpu = smp_processor_id();
		rq = cpu_rq(cpu);
	} else {
		raw_spin_unlock_irq(&rq->lock);
	}
	post_schedule(rq);

	sched_preempt_enable_no_resched();
	if(need_resched())
		goto need_resched;
}
