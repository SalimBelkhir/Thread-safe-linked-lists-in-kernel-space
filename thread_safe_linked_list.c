#include <linux/slab.h> 
#include <linux/list.h> 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/spinlock.h>

MODULE_DESCRIPTION("Linux Kernel API training");
MODULE_AUTHOR("salim");
MODULE_LICENSE("GPL");

static int ret ; 

struct pid_list{
	pid_t pid;
	struct list_head list ;
};

LIST_HEAD(my_list);
DEFINE_RWLOCK(lock);

static int have_pid(struct list_head *lh,int pid)
{
	struct list_head *i;
	void *elem ; 
	
	read_lock(&lock);
	list_for_each(i, lh) {
		struct pid_list *pl = list_entry(i,struct pid_list,list);
		if(pl->pid == pid) {
			read_unlock(&lock);
			return 1 ;
		}
	}
	read_unlock(&lock);
	return 0 ; 
} 

static int add_pid(pid_t pid) 
{
	struct pid_list *ple = kmalloc(sizeof *ple,GFP_KERNEL);
	if(!ple)
		return -ENOMEM ;
	ple->pid=pid;
	list_add(&ple->list,&my_list);
	pr_info("added pid: %d \n",ple->pid);
	return 0 ;
}

static void add_pid_lock(struct list_head *lh,struct pid_list *pl)
{
	write_lock(&lock);
	list_add(&pl->list,lh);
	pr_info("added pid : %d using spinlock \n",pl->pid);
	write_unlock(&lock); 
}

static int del_pid(pid_t pid)
{
	struct list_head *i, *tmp ;
	struct pid_list *ple ; 

  write_lock(&lock);
	list_for_each_safe(i,tmp , &my_list){
		ple = list_entry(i,struct pid_list , list);
		if(ple->pid == pid){
			list_del(i);
			kfree(i);
      write_unlock(&lock); 
			pr_info("deleted pid: %d \n",ple->pid);
			return 0 ;
		}
    write_unlock(&lock); 
	}

	return -EINVAL ;
}

static void show_list(void) 
{
	struct pid_list *ple ; 
  read_lock(&lock);
	list_for_each_entry(ple,&my_list,list){
		pr_info("PID :%d \n",ple->pid);
    read_unlock(&lock);
	}
}

static void destroy_list(void) 
{
	struct list_head *i, *n ;
	struct pid_list *ple ; 

  write_lock(&lock);
	list_for_each_safe(i,n,&my_list){
		ple = list_entry(i,struct pid_list ,list) ; 
		list_del(i);
		kfree(ple);
    write_unlock(&lock);
	}
}

static int __init api_init(void){
	pr_info("module init \n");
	
	struct pid_list *p  = kmalloc(sizeof* p, GFP_KERNEL);
	if(!p)
		return -ENOMEM ;
	p->pid = 1111 ;
	add_pid_lock(&my_list,p);

	ret = add_pid(2222);
	if(ret)
		return ret ;
	ret = add_pid(3333);
	if(ret)
		return ret ; 

	pr_info("Current list: \n");
	show_list();
	return 0 ;
}

static void __exit api_exit(void){
	destroy_list();
	pr_info("Module exit \n");
}

module_init(api_init);
module_exit(api_exit);
