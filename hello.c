// SPDX-License-Identifier: GPL-2.0
/*
 * Lab 4: "Hello, world!" kernel module with a parameter and a list of
 * timestamps.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

MODULE_AUTHOR("Olha Yakubovska <olasha228@gmail.com>");
MODULE_DESCRIPTION("Hello, world module with a list of event timestamps");
MODULE_LICENSE("GPL");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count,
		 "How many times to print \"Hello, world!\" (default 1; 0 or 5..10 gives a warning; >10 is rejected)");

struct hello_event {
	struct list_head node;
	ktime_t time;
};

static LIST_HEAD(hello_list);

static void hello_free_list(bool print)
{
	struct hello_event *ev, *tmp;

	list_for_each_entry_safe(ev, tmp, &hello_list, node) {
		if (print)
			pr_info("hello: event time = %lld ns\n",
				ktime_to_ns(ev->time));
		list_del(&ev->node);
		kfree(ev);
	}
}

static int __init hello_init(void)
{
	struct hello_event *ev;
	unsigned int i;

	if (count > 10) {
		pr_err("hello: invalid count=%u (must be <= 10)\n", count);
		return -EINVAL;
	}

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello: count=%u (0 or 5..10) is unusual, continuing\n",
			count);

	for (i = 0; i < count; i++) {
		ev = kmalloc(sizeof(*ev), GFP_KERNEL);
		if (!ev) {
			hello_free_list(false);
			return -ENOMEM;
		}

		ev->time = ktime_get();
		list_add_tail(&ev->node, &hello_list);

		pr_info("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	hello_free_list(true);
	pr_info("hello: module unloaded\n");
}

module_init(hello_init);
module_exit(hello_exit);
