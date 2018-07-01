/*
*
*        _       _________ ______            _______  _______  _______  ______   _______  _______
*       ( \      \__   __/(  ___ \ |\     /|(  ___  )(       )(  ____ \(  ___ \ (  ____ )(  ____ \|\     /|
*       | (         ) (   | (   ) )| )   ( || (   ) || () () || (    \/| (   ) )| (    )|| (    \/| )   ( |
*       | |         | |   | (__/ / | (___) || |   | || || || || (__    | (__/ / | (____)|| (__    | | _ | |
*       | |         | |   |  __ (  |  ___  || |   | || |(_)| ||  __)   |  __ (  |     __)|  __)   | |( )| |
*       | |         | |   | (  \ \ | (   ) || |   | || |   | || (      | (  \ \ | (\ (   | (      | || || |
*       | (____/\___) (___| )___) )| )   ( || (___) || )   ( || (____/\| )___) )| ) \ \__| (____/\| () () |
*       (_______/\_______/|/ \___/ |/     \|(_______)|/     \|(_______/|/ \___/ |/   \__/(_______/(_______)
*
*
*
*/

#pragma once

#include "fw.h"
#include <_types.h>

#define KERNSIZE   0x2000000
#define PML4SHIFT  39
#define PDPSHIFT   30
#define PDRSHIFT   21

#define KVADDR(l4, l3, l2, l1) ( \
	((unsigned long)-1 << 47) | \
	((unsigned long)(l4) << PML4SHIFT) | \
	((unsigned long)(l3) << PDPSHIFT) | \
	((unsigned long)(l2) << PDRSHIFT) | \
	((unsigned long)(l1) << PAGE_SHIFT))

#define PA_TO_DM(x) (((uintptr_t)x) | lv2.dmap_base)
#define DM_TO_ID(x) (((uintptr_t)x) & (~lv2.dmap_base)) // XXX
#define	M_WAITOK            0x0002
#define	M_ZERO              0x0100
#define	VM_MEMATTR_DEFAULT	0x06
#define	VM_PROT_NONE	    ((vm_prot_t) 0x00)
#define VM_PROT_READ	    ((vm_prot_t) 0x01)	/* read permission */
#define VM_PROT_WRITE	    ((vm_prot_t) 0x02)	/* write permission */
#define VM_PROT_EXECUTE	    ((vm_prot_t) 0x04)	/* execute permission */
#define VM_PROT_DEFAULT	    (VM_PROT_READ|VM_PROT_WRITE)
#define VM_PROT_ALL			(VM_PROT_READ|VM_PROT_WRITE|VM_PROT_EXECUTE)
#define VM_PROT_NO_CHANGE	((vm_prot_t) 0x08)
#define VM_PROT_COPY		((vm_prot_t) 0x10)
#define VM_PROT_WANTS_COPY	((vm_prot_t) 0x10)
#define	TRACEBUF	struct qm_trace trace;
#define JOIN_HELPER(x, y) x##y
#define JOIN(x, y) JOIN_HELPER(x, y)
#define TYPE_PAD(size) char JOIN(_pad_, __COUNTER__)[size]
#define TYPE_VARIADIC_BEGIN(name) name { union {
#define TYPE_BEGIN(name, size) name { union { TYPE_PAD(size)
#define TYPE_END(...) }; } __VA_ARGS__
#define TYPE_FIELD(field, offset) struct { TYPE_PAD(offset); field; }
#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define TYPE_CHECK_SIZE(name, size) \
	_Static_assert(sizeof(name) == (size), "Size of " #name " != " #size)

#define TYPE_CHECK_FIELD_OFFSET(name, member, offset) \
	_Static_assert(offsetof(name, member) == (offset), "Offset of " #name "." #member " != " #offset)

#define TYPE_CHECK_FIELD_SIZE(name, member, size) \
	_Static_assert(sizeof(((name*)0)->member) == (size), "Size of " #name "." #member " != " #size)

#define ASSERT_STRSIZE(struc, size) \
    _Static_assert(sizeof( struc ) == (size), "size of " #struc " != " #size )

unsigned int long long __readmsr(unsigned long __register);

static inline void wbinvd(void) { __asm__ volatile("wbinvd"); }

static inline void disable_interrupts(void) { __asm__ volatile("cli"); }

static inline void enable_interrupts(void) { __asm__ volatile("sti"); }

// Read cr0.
static inline __attribute__((always_inline)) unsigned long cr0_read(void) {
	unsigned long cr0;

	__asm__ volatile ("movq %%cr0, %0\n"
		: "=r" (cr0)
		: : "memory"
		);

	return cr0;
} // AT&T "movq %0, %%cr0"

// Write cr0.
static inline __attribute__((always_inline)) void cr0_write(unsigned long cr0) {
	__asm__ volatile ("movq %0, %%cr0\n"
		: : "r" (cr0)
		: "memory"
		);
} // AT&T "movq %%cr0, %0"

static inline uint64_t read_flags(void) {
	uint64_t flags;
	__asm__ volatile("pushf; pop %0;" : "=r" (flags));
	return flags;
}

static inline uint64_t intr_disable(void) {
	uint64_t flags = read_flags();
	disable_interrupts();
	return flags;
}

static inline void intr_restore(uint64_t flags) {
	// TODO should only IF be or'd in?
	__asm__ volatile("push %0; popf;" : : "rm" (flags) : "memory");
}

typedef int              vm_prot_t;
typedef uint64_t         vm_offset_t;
typedef uint64_t         vm_map_t;
typedef uint64_t         vm_size_t;
typedef char             vm_memattr_t;
typedef void             *pmap_t;
typedef unsigned long    size_t;
typedef signed long      sint64_t;
typedef signed int       sint32_t;
struct                   syscall_args;
typedef unsigned char    byte;

typedef struct {
	time_t        sec;      // Event seconds
	uint32_t  milli;    // millisecond of the timestamp
	unsigned long serial;   // Serial number of the event
	const char    *host;    // Machine's node name
} au_event_t;

typedef struct sendto_args {
	int	   s;
	void   *buf;
	size_t len;
	int	   flags;
	void   *to;
	int	   tolen;
} SendToArgs;

struct qm_trace {
	char * lastfile;
	int lastline;
	char * prevfile;
	int prevline;
};

struct __iovec {
	uint64_t iov_base;
	size_t iov_len;
};

TYPE_BEGIN(struct uio, 0x30);
TYPE_FIELD(uint64_t uio_iov, 0);
TYPE_FIELD(uint32_t uio_iovcnt, 8);
TYPE_FIELD(uint64_t uio_offset, 0x10);
TYPE_FIELD(uint64_t uio_resid, 0x18);
TYPE_FIELD(uint32_t uio_segflg, 0x20);
TYPE_FIELD(uint32_t uio_rw, 0x24);
TYPE_FIELD(struct thread *uio_td, 0x28);
TYPE_END();

struct lock_object {
	const char* lo_name;
	uint32_t lo_flags;
	uint32_t lo_data;
	void* lo_witness;
};

struct mtx {
	struct lock_object lock_object;
	volatile void* mtx_lock;
};

struct sx {
	struct lock_object lock_object;
	volatile uintptr_t sx_lock;
};

// Changed to the Thread struct, used by the payloads.
struct auditinfo_addr {
	/*
	4    ai_auid;
	8    ai_mask;
	24    ai_termid;
	4    ai_asid;
	8    ai_flags;r
	*/
	char useless[184];
};

struct ucred {
	uint32_t cr_ref;					// reference count		0x0000
	uint32_t cr_uid;					// effective user id	0x0004
	uint32_t cr_ruid;					// real user id			0x0008
	uint32_t useless2;					// 						0x000C
	uint32_t useless3;					//
	uint32_t cr_rgid;					// real group id
	uint32_t useless4;					//
	void *useless5;						//
	void *useless6;						//
	void *cr_prison;					// jail(2)				0x0030
	void *useless7;						//
	uint32_t useless8;					//
	void *useless9[2];					//
	void *useless10;					//
	struct auditinfo_addr cr_audit;		//
	uint32_t *cr_groups;				// groups
	uint32_t useless12;					//
};

struct filedesc {
	void *useless1[3];
	void *fd_rdir;
	void *fd_jdir;
};

struct proc {
	char useless[64];
	struct ucred *p_ucred;
	struct filedesc *p_fd;
};

struct thread {
	void *useless;
	struct proc *td_proc;
};

typedef	int	sy_call_t(struct thread *td, void *ptr);
//typedef	void(*systrace_probe_func_t)(struct syscall_args *syscargs, enum systrace_probe_t _enum, int num);
typedef	void(*systrace_args_func_t)(int num, void *ptr, uint64_t *ptr2, int *ptr3);
typedef void(*smp_rendezvous_callback_t)(void *ptr);

typedef struct sysent_t {
	int                  sy_narg;
	sy_call_t            *sy_call;
	au_event_t           sy_auevent;
	systrace_args_func_t *sy_systrace_args_func;
	uint32_t             sy_entry;
	uint32_t             sy_return;
	uint32_t             sy_flags;
	uint32_t             sy_thrcnt;
} SySent;

// LV2 aka Kernel Symbols Structure.
typedef struct ksym_t {
	void        **kernel_map;
	void        *kernel_pmap_store;
	//void        *icc_query_nowait;
	void        *Starsha_UcodeInfo;
	void        *M_TEMP;
	SySent      *sysent;
	void        *kern_base;
	uintptr_t   dmap_base;
		
	int64_t      (*aslr_initialize_process_status)(int64_t a1);
	int64_t      (*aslr_gen_pseudo_random_number)(int64_t a1);
	int64_t      (*aslr_get_random_value)(int64_t a1, int a2);
	int64_t      (*bootparam_is_main_on_standby)(void);
	int64_t      (*bootparam_notify_main_on_standby_mode)(uint32_t a1);
	int64_t      (*bootparam_disable_console_output)(void);
	int64_t      (*bootparam_is_mode_manu)(void);
	sint64_t     (*bootparam_get_ddr3_capacity)(int64_t *a1);
#ifdef FW_405
	sint64_t     (*bootparam_get_config_board_id)(int64_t a1);
	int64_t      (*bootparam_get_mac_address)(int64_t a1);
	sint64_t     (*bootparam_hwconfig_read)(int64_t a1, uint32_t a2, int64_t a3);
#else
	sint64_t     (*bootparam_get_config_board_id)(void *a1);
	int64_t      (*bootparam_get_mac_address)(void *a1);
	sint64_t     (*bootparam_hwconfig_read)(int64_t a1, uint32_t a2, void *a3);
#endif	
	int          (*binuptime)(int64_t a1);
	int          (*getbinuptime)(int64_t a1);
	void         *(*malloc)(unsigned long size, void *type, int flags);
	void         (*free)(void *addr, void *type);
	void         (*memcpy)(void *dst, const void *src, size_t len);
	void         *(*memset)(void * ptr, int value, size_t num);
	int          (*memcmp)(const void * ptr1, const void * ptr2, size_t num);
	int          (*memmove)(int64_t a1, int64_t a2, int64_t a3);
	int          (*bcopy)(char *a1, char *a2, unsigned long a3);
	int          (*bcmp)(const void *a1, const void *a2, unsigned long a3);
	int          (*printf)(const char *fmt, ...);
	int          (*sprintf)(char *str, const	char *format, ...);
	int          (*snprintf)(char *str, size_t size, const char *format, ...);
	int          (*vprintf)(const char *format, va_list ap);
	int          (*vsnprintf)(char *str, size_t size, const char *format, va_list ap);
	int          (*copyin)(const void *uaddr, void *kaddr, size_t len);
	int          (*copyout)(const void *kaddr, void *uaddr, size_t len);
	int          (*copyinstr)(const void *uaddr, void *kaddr, size_t len, size_t *done);
	int          (*copystr)(const void *kfaddr, void *kdaddr, size_t len, size_t *done);
	size_t       (*strlen)(const char *str);
	char         *(*strlcpy)(byte *a1, char *a2, int64_t a3);
	char         *(*strdup)(const char *str);
	char         *(*strstr)(const char *str1, const char *str2);
#ifdef FW_405
	int          (*strncpy)(int64_t a1, char *a2, int64_t a3);
#else
	int64_t      (*strncpy)(int64_t a1, int64_t a2, int64_t a3);
#endif
	int          (*strcmp)(const char *str1, const char *str2);
	int          (*strncmp)(const char *str1, const char *str2, size_t len);
	int          (*strcasecmp)(const char *str1, const char *str2);
	int          (*g_obi_create)(int64_t a1, int a2);
	int          (*g_new_geomf)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, char a7);
	int          (*g_new_consumer)(int64_t a1);
	int          (*g_attach)(int64_t a1, int64_t a2);
	int          (*g_trace)(int a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, char a7);
	int          (*g_access)(int64_t a1, uint32_t a2, uint32_t a3, uint32_t a4);
	int          (*g_wither_geom)(int64_t a1, uint32_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);
	int          (*g_do_wither)(void);
	int          (*g_orphan_provider)(char a1, int64_t a2, uint32_t a3, int64_t a4);
	int          (*g_provider_by_name)(char *a1);
	int          (*g_obi_read)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9, int64_t a10);
	int          (*g_io_request)(int64_t a1, int64_t a2);
	int          (*g_alloc_bio)(void);
	int          (*g_destroy_bio)(int64_t a1);
	int          (*g_post_notification_for_bio)(int64_t a1);
	char         (*g_cancel_event)(int64_t a1);
	int          (*g_destroy_geom)(int64_t *a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6);
	void         (*cv_init)(int64_t a1, int64_t a2);
	void         (*cv_destroy)(void);
	int          (*_cv_wait)(int64_t a1, int64_t a2);
	int64_t      (*_cv_wait_sig)(int64_t a1, int64_t a2);
	int64_t      (*_cv_timedwait)(int64_t a1, int64_t a2, uint32_t a3);
	int64_t      (*_cv_timedwait_sig)(int64_t a1, int64_t a2, uint32_t a3);
	int          (*cv_signal)(int64_t a1);
	int          (*cv_signalto)(int64_t a1, int64_t a2);
	int          (*cv_broadcastpri)(int64_t a1, uint32_t a2);
	char         (*sleepq_lock)(int64_t _RDI);
	int64_t      (*sleepq_lookup)(int64_t _RDI);
	int          (*sleepq_release)(int64_t _RDI);	
#if defined FW_405
	int          (*sleepq_add)(int64_t a1, int64_t a2, int64_t a3, short a4, int a5);
	int64_t      (*sleepq_sleepcnt)(int64_t _RDI, int a2);
	int          (*sleepq_set_runnable)(int64_t a1);
	int          (*sleepq_wait)(int64_t a1, uint32_t a2);
	sint64_t     (*sleepq_signalto)(int64_t _RDI, int64_t a2, uint32_t a3, int a4, sint64_t a5);
	int          (*sleepq_remove)(sint64_t a1, int64_t a2);
#elif defined FW_455
	int          (*sleepq_add)(int64_t a1, int64_t a2, int64_t a3, short a4, int a5);
	int64_t      (*sleepq_sleepcnt)(int64_t _RDI, int a2);
	int          (*sleepq_set_runnable)(int64_t a1);
	int          (*sleepq_wait)(int64_t a1, uint32_t a2);
	sint64_t     (*sleepq_signalto)(int64_t _RDI, int64_t a2, uint32_t a3, int a4, sint64_t a5);
	int          (*sleepq_remove)(sint64_t a1, int64_t a2);
#else
	uint64_t     (*sleepq_add)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int a5);
	int64_t      (*sleepq_sleepcnt)(int64_t _RDI, int a2);
	int          (*sleepq_set_runnable)(int64_t a1);
	uint64_t     (*sleepq_wait)(int64_t a1, uint32_t a2);
	sint64_t     (*sleepq_signalto)(int64_t _RDI, int64_t a2, uint32_t a3, int a4, int64_t a5);
	int          (*sleepq_remove)(int64_t a1, int64_t a2);
#endif
	int64_t      (*sleepq_set_timeout)(int64_t a1, int64_t a2);
	int64_t      (*sleepq_wait_sig)(void);
	int64_t      (*sleepq_timedwait)(int64_t a1, uint32_t a2);
	int64_t      (*sleepq_timedwait_sig)(void);
	int64_t      (*sleepq_signal)(int64_t _RDI, int64_t a2, uint32_t a3, int a4);	
	int64_t      (*sleepq_broadcast)(int64_t _RDI, int64_t a2, uint32_t a3, int a4);	
	int          (*sleepq_abort)(int64_t a1, int a2);
	int          (*sysctl_lock)(void);	
#if defined FW_405
	char         (*sysctl_unlock)(void);
	int          (*sysctl_wire_old_buffer)(int64_t a1, uint64_t a2, int64_t a3);
#elif defined FW_455
	char         (*sysctl_unlock)(void);
	int          (*sysctl_wire_old_buffer)(int64_t a1, uint64_t a2, int64_t a3);
#else
	char         (*sysctl_unlock)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int          (*sysctl_wire_old_buffer)(int64_t a1, uint64_t a2);
#endif
#ifdef FW_405
	int64_t      (*sysctl_register_oid)(char *a1, byte *a2);
	int64_t      (*sysctl_add_oid)(int64_t a1, int64_t *a2, int a3, char *a4, int a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9, char *a10, int a11);
#elif defined FW_455
	int64_t      (*sysctl_register_oid)(byte *a1, byte *a2);
	int64_t      (*sysctl_add_oid)(int64_t a1, int64_t *a2, int a3, byte *a4, int a5, int64_t a6, int64_t a7, int64_t a8, int64_t a9, char *a10, int a11);
#else
	int64_t      **(*sysctl_register_oid)(int64_t **a1, int64_t *a2, int64_t a3);
	uint64_t     *(*sysctl_add_oid)(uint64_t **a1, uint64_t **a2, int a3, int64_t a4, int a5, uint64_t a6, int64_t a7, uint64_t a8, uint64_t a9, char *a10, int a11);
#endif
	int64_t      *(*sysctl_unregister_oid)(int64_t **a1);
	sint64_t     (*sysctl_ctx_init)(int64_t a1);
	int64_t      (*sysctl_ctx_free)(int64_t a1);
	int64_t      (*sysctl_remove_oid)(int64_t a1, uint32_t a2, uint32_t a3);
#ifdef FW_405
	int          (*sysctl_rename_oid)(int64_t a1, char *a2);
#else
	int          (*sysctl_rename_oid)(int64_t a1, int64_t a2);
#endif
	int          (*sysctl_handle_int)(int64_t a1, int *a2, int a3, int64_t a4);
	int          (*sysctl_handle_long)(int64_t a1, int *a2, int64_t a3, int64_t a4);
	int          (*sysctl_handle_64)(int64_t a1, int64_t *a2, int64_t a3, int64_t a4);
	int          (*sysctl_handle_string)(int64_t a1, char *a2, uint64_t a3, int64_t a4);
	int          (*sysctl_handle_opaque)(int64_t a1, int64_t a2, int64_t a3, char *a4);
	sint64_t     (*kern_sysctl_write)(int64_t a1, char *a2, uint64_t a3);
	sint64_t     (*kern_sysctl_read)(int64_t a1, char *a2, uint64_t a3);
	int64_t      (*do_sysctl)(int64_t a1, int64_t _RSI, int64_t *a3);		
	int          (*userland_sysctl_copyout)(int64_t a1, int64_t a2, uint64_t a3);	
#ifdef FW_405
	int64_t      (*userland_sysctl)(int64_t a1, char *a2, uint32_t a3, int64_t a4, int64_t *a5, int a6, int64_t a7, int64_t a8, uint64_t *a9, int a10);
#else
	int64_t      (*userland_sysctl)(int64_t a1, int64_t a2, uint32_t a3, int64_t a4, int64_t *a5, int a6, int64_t a7, int64_t a8, uint64_t *a9, int a10);
#endif
	int          (*userland_sysctl_copyin)(int64_t a1, int64_t a2, uint64_t a3);	
#ifdef FW_405
	int64_t      (*sbuf_new_for_sysctl)(int64_t a1, int64_t a2, sint32_t a3, int64_t a4);
#elif defined FW_455
	void         *(*sbuf_new_for_sysctl)(void *a1, int64_t a2, sint32_t a3, int64_t a4);
#else
	uint64_t     *(*sbuf_new_for_sysctl)(uint64_t *a1, uint64_t a2, sint32_t a3, int64_t a4);
#endif
#ifdef FW_405
	int64_t      (*ktrsysctl)(char *a1, uint32_t a2);
#else
	int64_t      (*ktrsysctl)(int64_t a1, uint32_t a2);
#endif
	int64_t      (*kernel_sysctl)(int64_t a1, int64_t a2, uint32_t a3, int64_t a4, uint64_t *a5, int64_t a6, int64_t a7, uint64_t *a8, int a9);
	//int          (*kernel_sysctlbyname)(struct thread *td, char *name, void *oldVal, size_t *oldlenp, void *newVal, size_t newlen, size_t *retval, int flags);
	int64_t      (*kernel_sysctlbyname)(int64_t a1, int64_t a2, int64_t a3, uint64_t *a4, int64_t a5, int64_t a6, uint64_t *a7, int a8);
	int64_t      (*kernel_mount)(int64_t a1, int64_t a2);
	vm_offset_t  (*kmem_alloc_contig)(vm_map_t map, vm_size_t size, int flags, vm_paddr_t low, vm_paddr_t high, unsigned long alignment, unsigned long boundary, vm_memattr_t memattr);
	void         (*kmem_free)(vm_map_t, vm_offset_t, vm_size_t);	
#if defined FW_405
	int64_t      (*kern_execve)(int64_t a1, int64_t a2);
#elif defined FW_455
	int64_t      (*kern_execve)(int64_t a1, int64_t a2);
#else
	int64_t      (*kern_execve)(int64_t a1, uint64_t a2, int64_t a3, int64_t a4);
#endif
#if defined FW_455
	int64_t      (*kern_ptrace)(int64_t a1, sint32_t a2, sint32_t a3, int64_t a4, int a5);
#else 
	int64_t      (*kern_ptrace)(int64_t a1, uint32_t a2, sint32_t a3, int64_t a4, uint32_t a5);
#endif
	int64_t      (*kern_jail_set)(int64_t a1, int64_t a2, int a3);
	int64_t      (*kern_jail_get)(int64_t a1, int64_t a2, int a3);
	//int          (*kthread_add)(void(*func)(void *), void *arg, struct proc *procp, struct thread **newtdpp, int flags, int pages, const char *fmt, ...);
	int64_t      (*kproc_create)(int64_t a1, int64_t a2, int64_t *a3, int a4, sint32_t a5, int64_t a6, char a7);
	int64_t      (*kproc_kthread_add)(int64_t *a1, int64_t *a2, int64_t a3, int64_t a4, int64_t a5, int64_t a6, char a7);
	int64_t      (*kthread_add)(int64_t a1, int64_t a2, int64_t a3, int64_t *a4, int a5, uint32_t a6, int64_t a7, char a8);
	char         (*kthread_suspend_check)(int64_t a1, int64_t a2);
	int64_t      (*ktr_tracepoint)(int64_t a1, uint16_t a2, uint32_t a3);	
#ifdef FW_405
	int          (*proc_sstep)(int64_t a1);
#else
	int          (*proc_sstep)(int64_t a1, int64_t a2);
#endif
	int          (*proc_rwmem)(struct proc *p, struct uio *uio);
	int64_t      (*proc_set_dr)(int64_t a1, int64_t a2);
	int64_t      (*proc_get_dr)(int64_t a1, int64_t a2);
	int64_t      (*proc_init_dr)(int64_t a1, int64_t a2);
	int          (*sbuf_delete)(int64_t *a1);
	int64_t      (*sbuf_vprintf)(int64_t a1, int64_t a2, int64_t a3);
	sint64_t     (*sbuf_trim)(int64_t a1);
	int64_t      (*sbuf_finish)(int64_t *a1);
	int64_t      (*sbuf_data)(int64_t a1);
#if defined FW_405
	int64_t      (*sbuf_new)(int64_t a1, int64_t a2, sint32_t a3, uint16_t a4);
#elif defined FW_455
	void         *(*sbuf_new)(void *a1, int64_t a2, sint32_t a3, uint16_t a4);
#else
	uint64_t     *(*sbuf_new)(uint64_t *a1, uint64_t a2, uint32_t a3, int64_t a4);
#endif	
#if defined FW_405
	int          (*sbuf_len)(int64_t a1);
	char         (*msgbuf_addchar)(sint64_t a1);
	int          (*msgbuf_addstr_sub)(int64_t a1);
#elif defined FW_455
	int          (*sbuf_len)(int64_t a1);
	char         (*msgbuf_addchar)(sint64_t a1);
	int          (*msgbuf_addstr_sub)(int64_t a1);
#else
	sint64_t     (*sbuf_len)(int64_t a1);
	char         (*msgbuf_addchar)(int64_t a1);
	uint64_t     (*msgbuf_addstr_sub)(int64_t a1);
#endif
	int64_t      (*vfs_getnewfsid)(int64_t a1);
	char         *(*vfs_mountedfrom)(int64_t a1, char *a2);
	int64_t      (*vfs_donmount)(int64_t a1, int a2, int64_t a3);
	int64_t      (*vfs_buildopts)(int64_t a1, int64_t *a2, int64_t a3, int64_t a4);
	vm_paddr_t   (*pmap_extract)(pmap_t pmap, vm_offset_t va);
	void         (*pmap_protect)(pmap_t pmap, uint64_t sva, uint64_t eva, uint8_t pr);
	int          (*sys_sendto)(void *td, SendToArgs sargs);	
	void         (*critical_enter)(void);
	void         (*critical_exit)(void);
	int          (*securelevel_gt)(long a1, int a2);
	int          (*securelevel_ge)(long a1, int a2);
#if defined FW_405
	sint64_t     (*alloc_unrl)(sint64_t a1);
	void         (*clean_unrhdrl)(int64_t a1, int64_t a2);
#elif defined FW_455
	sint64_t     (*alloc_unrl)(sint64_t a1);
	void         (*clean_unrhdrl)(int64_t a1, int64_t a2);
#else
	int64_t      (*alloc_unrl)(int64_t a1);
	int          (*clean_unrhdrl)(int result, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
#endif
	int          (*delete_unrhdr)(int64_t a1);
	char         (*vm_page_unhold_pages)(int64_t a1, int a2);
	char         (*vm_page_free_toq)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);	
	int64_t      (*new_unrhdr)(int64_t a1, int64_t a2, void *a3);
	int          (*DriftUpdate2)(int64_t a1);
	int64_t      (*vflush)(int64_t a1, int a2, char a3, int64_t a4);	
	uint64_t     (*spinlock_exit)(int64_t a1);
	void         (*sx_init_flags)(struct sx *sx, const char *description, int opts);
	void         (*sx_xlock)(struct sx *sx);
	void         (*sx_xunlock)(struct sx *sx);
	void         (*sx_destroy)(struct sx *sx);
	int          (*sx_xlock_hard)(int64_t a1, uint32_t a2);
	int          (*_sx_sunlock_hard)(int64_t a1, int64_t a2);
	int          (*_sx_sunlock_hard_sub)(int64_t a1);
	int          (*lock_init)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int a5);
	void         (*lock_destroy)(int64_t a1);	
	void         (*_thread_lock_flags_check_result)(int64_t a1);	
#if defined FW_405
	int          (*thread_lock_flags)(sint64_t a1);
#elif defined FW_455
	int          (*thread_lock_flags)(sint64_t a1);
#else
	int          (*thread_lock_flags)(int64_t a1);
#endif
	void         (*unref_dag)(int64_t a1, int64_t a2);
	int          (*id_set)(int64_t a1, short a2, int64_t a3, int64_t a4);
	int          (*id_unlock)(int64_t *a1);
	int          (*id_wlock)(int64_t a1, uint32_t a2, byte *a3, int64_t *a4);	
#if defined FW_405
	int          (*mtx_lock_sleep)(int64_t a1, uint32_t a2);
	char         (*mtx_unlock_sleep)(int64_t a1, int64_t a2);
#elif defined FW_455
	int          (*mtx_lock_sleep)(int64_t a1, uint32_t a2);
	char         (*mtx_unlock_sleep)(int64_t a1, int64_t a2);
#else
	int          (*mtx_lock_sleep)(int64_t a1, uint32_t a2, int64_t a3);
	char         (*mtx_unlock_sleep)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
#endif
	int          (*mtx_destroy)(int64_t a1);
	int          (*fpu_kern_enter)(struct thread *td, void *ctx, uint32_t flags);
	int          (*fpu_kern_leave)(struct thread *td, void *ctx);
	int64_t      (*gbase_samu_initialize)(int64_t a1);
	int64_t      (*gbase_samu_finalize)(int64_t a1, int64_t a2);
	int64_t      (*gbase_samu_map)(void);
	int64_t      (*gbase_samu_unmap)(void);
	uint32_t     (*gbase_samu_read_register)(uint32_t a1);
	int64_t      (*gbase_samu_write_register)(uint32_t a1, uint32_t a2);
	int64_t      (*gbase_samu_ix_read_register)(uint32_t a1);
	int          (*gbase_samu_ix_write_register)(uint32_t a1, uint32_t a2);
	uint64_t     (*gbase_samu_pa_to_gpuva)(uint64_t a1);
	int64_t      (*gbase_samu_unk_func)(uint32_t a1);
	int          (*gbase_samu_mailbox_writer)(sint32_t a1, uint32_t a2);
	int          (*gbase_samu_mailbox_reader)();
	int64_t      (*gbase_samu_cmd)(int64_t a1, int64_t a2, int64_t a3);
	int          (*gbase_samu_handler)(int64_t a1);
	sint64_t     (*gbase_unmap)(uint32_t a1, uint64_t a2, int64_t a3, short a4);
	sint64_t     (*gbase_free_vm_space)(uint64_t a1);	
	void         (*SceSflash)(long a);
	void         (*sched_pin)(void);
	void         (*sched_unpin)(void);
	void         (*sched_switch)(struct thread *td, struct thread *newtd, int flags);
	void         (*sched_bind)(struct thread *td, int cpu);
	int          (*sched_prio)(int64_t a1, uint32_t a2);	
	int          (*sched_add)(int64_t a1, uint32_t a2);
	int64_t      (*sched_user_prio)(int64_t a1, uint32_t a2);
	sint32_t     (*sflash_pwrite_for_vtrm)(unsigned long a1, unsigned long a2, long a3);
	int          (*sflash_pread_for_vtrm2)(uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4);
	sint32_t     (*sflash_pread_for_vtrm)(unsigned long a1, unsigned long a2, long a3);
	int          (*sflash_erase_for_vtrm)(unsigned long a1);
	int          (*sceSblVtrmBackup)(char a1, long *a2, long a3);
	int          (*sceSblVtrmRestore)(long a1, long a2);
	int          (*VtrmUtilLockInitialize)(void);
	int          (*VtrmUtilLockFinialize)(void);
	int          (*vtrm_pread)(signed long a1, unsigned long a2, unsigned long a3, unsigned long a4);
	int          (*vtrm_pwrite)(signed long a1, unsigned long a2, long a3);
#ifdef FW_405
	sint32_t     (*vtrm_pwrite2)(sint64_t a1, uint64_t a2, int64_t a3, byte *a4, uint64_t a5);
#else
	int          (*vtrm_pwrite2)(sint64_t a1, uint64_t a2, int64_t a3, int64_t a4, int64_t a5);
#endif
	int          (*vtrm_erase_sector)(uint64_t a1);
	void         (*smp_rendezvous)(smp_rendezvous_callback_t, smp_rendezvous_callback_t, smp_rendezvous_callback_t, void *);
	void         (*smp_no_rendevous_barrier)(void *);  // yes...it is misspelled :)
	int          (*sceSblSsDecryptSealedKey)(void *encryptedSealedKey, void *decryptedSealedKey);
#ifdef FW_405
	int          (*getSealedKeySecret)(byte *saveDataMasterKey, byte *saveDataMasterHashKey);
#elif defined FW_455
	int          (*getSealedKeySecret)(byte *saveDataMasterKey, byte *saveDataMasterHashKey);
#else 
	int          (*getSealedKeySecret)(byte *saveDataMasterKey, byte *saveDataMasterHashKey, int16_t v3);
#endif
	int          (*sceSblSsGenerateSealedKey)(void *decryptedSealedKey);
	int          (*sceSblKeymgrClearKey)(uint32_t a1);
	int          (*sceSblAuthMgrSmSpawn)(long *a1, long a2);
#if defined FW_405
	int          (*sceSblServiceSpawn)(long a1, uint32_t a2, uint32_t a3, int a4, int a5, long *a6);
#elif defined FW_455
	int          (*sceSblServiceSpawn)(long a1, uint32_t a2, uint32_t a3, int a4, int a5, long *a6);
#else
	int64_t      (*sceSblServiceSpawn)(int64_t a1, uint32_t a2, int a3, int a4, int a5, uint64_t *a6);
#endif
	int          (*sceSblSsMemcmpConsttime)(char *a1, byte *a2, int64_t a3);
	byte         *(*sceSblSsMemset)(byte *a1, char a2, int64_t a3);
	int64_t      (*sceSblDriverMapPages)(int64_t *a1, uint64_t a2, uint32_t a3, int64_t a4, uint64_t a5, int64_t *a6);
	int64_t      (*SceSblSrtcSetTime)(int64_t a1, int64_t a2, sint64_t *a3, int64_t a4, int64_t a5, int64_t a6, int64_t a7);
	int64_t      (*sceSblServiceMailbox)(int64_t a1, uint32_t a2);	
#ifdef FW_405
	sint64_t     (*sceSblACMgrGetDeviceAccessType)(int64_t a1, byte *a2, int64_t *a3);
#elif defined FW_455
	sint64_t     (*sceSblACMgrGetDeviceAccessType)(int64_t a1, int64_t a2, int64_t *a3);
#else
	sint64_t     (*sceSblACMgrGetDeviceAccessType)(int64_t a1, int64_t a2, int *a3);
#endif
	sint64_t     (*sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall)(int64_t a1, int64_t a2);
	int64_t      (*sceSblACMgrGetPathId)(int64_t a1);
	sint64_t     (*sceSblACMgrIsAllowedToMmapSelf)(int64_t a1, int64_t a2);
	sint64_t     (*sceSblACMgrHasMmapSelfCapability)(int64_t a1);
	int64_t      (*sceSblACMgrIsAllowedSystemLevelDebugging)(void);
	int64_t      (*sceSblDiagOSUpdaterProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsDebuggableProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsSoftwagnerProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsVtrmadminProcess)(int64_t a1);
	sint64_t     (*sceSblACMgrIsAllowedToUsePupUpdate0)(int64_t a1);
	int64_t      (*sceSblACMgrHasSceProcessCapability)(int64_t a1);
	sint64_t     (*sceSblACMgrHasUseVideoServiceCapability)(int64_t a1);
	sint64_t     (*sceSblACMgrHasUseHp3dPipeCapability)(int64_t a1);
	sint64_t     (*sceSblACMgrIsDiskplayerdiagProcess)(int64_t a1);
	sint64_t     (*sceSblACMgrHasUseVtrmCapability)(int64_t a1);
	int64_t      (*sceSblACMgrHasDrmProcessAttribute)(int64_t _RDI);
	int64_t      (*sceSblACMgrHasSceProgramAttribute)(int64_t a1);
	int64_t      (*sceSblACMgrIsAslrSeed2Process)(int64_t a1);
	int64_t      (*sceSblACMgrIsJitApplicationProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsJitCompilerProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsVdecProxyProcess)(int64_t a1);
	sint64_t     (*sceSblACMgrIsWebcoreProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsBdjavaProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsDiskplayeruiProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsVideoplayerProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsShellcoreProcess)(int64_t a1);
	sint64_t     (*sceSblACMgrIsMinisyscoreProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsShelluiProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsSyscoreProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsDecidProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsCoredumpProcess)(int64_t a1);
	sint64_t     (*sceSblACMgrIsDiagProcess)(int64_t a1);
	sint64_t     (*sceSblACMgrIsNongameUcred)(int64_t a1);
	sint64_t     (*sceSblACMgrIsSystemUcred)(int64_t a1);
	int64_t      (*sceSblACMgrIsAllowedCoredump)(int64_t a1);
	int64_t      (*sceSblACMgrIsAllowedDrmProcessDebugging)(void);
	int64_t      (*sceSblACMgrIsFirstImageWriterProcess)(int64_t a1);
	int64_t      (*sceSblACMgrIsMountfusefsProcess)(int64_t a1);
	int64_t      (*sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp)(void);
	int64_t      (*sceSblRcMgrIsAllowDataExecutionInFselfProcess)(void);
	int64_t      (*sceSblRcMgrIsAllowDebugSoftWagnerClock)(void);
	int64_t      (*sceSblRcMgrIsAllowDisablingAslr)(void);
	int64_t      (*sceSblRcMgrIsSoftwagnerQafForAcmgr)(void);
	int64_t      (*sceSblRcMgrIsAllowCreateDebuggingDeviceFile)(void);
	int64_t      (*sceSblRcMgrIsAllowULDebugger)(void);
	int64_t      (*sceSblRcMgrIsStoreMode)(void);
	char         (*sceSblRcMgrIsAllowAdClock)(void);
	int64_t      (*sceSblRcMgrIsAllowSLDebugger)(void);
	int64_t      (*sceSblRcMgrIsAllowInitVtrm)(void);
	int64_t      (*sceSblRcMgrIsAllowRegistryAccessMini)(void);
	int64_t      (*sceSblRcMgrIsAllowRegistryAccess)(void);
	int64_t      (*sceSblRcMgrIsAllowPsmDebug)(void);
	int          (*sceSblAIMgrIsDevKit)(void);
	int          (*sceSblAIMgrIsTool)(void);
	int          (*sceSblAIMgrIsTestKit)(void);
	int          (*sceSblAIMgrIsDex)(void);
	int          (*sceSblAIMgrIsCex)(void);
	int64_t      (*sceSblAIMgrIsDiag)(void);
	short        (*sceSblAIMgrGetProductCode)(void);
#if defined FW_505
	int64_t      (*sceRegMgrSetInt)(uint32_t regId, int value1, int value2, int value3, int value4);
#else
	int64_t      (*sceRegMgrSetInt)(uint32_t regId, int value);
#endif	
	int          (*sceKernelIsDevelopmentMode)(void);
	int          (*sceKernelIsAssistMode)(void);	
#ifdef FW_405
	int64_t      (*sceKernelCheckDipsw)(uint16_t a1);
#else
	char         (*sceKernelCheckDipsw)(short a1);
#endif
	int          (*genNewActDat)(void);
	int          (*icc_nvs_drvinit)(char a1);
	int          (*icc_nvs_init)(void);
	int          (*icc_lock_snvs_write)(void);
	char         (*icc_unlock_snvs_write)(void);	
	int          (*icc_init_snvs_write_lock)(void);
#ifdef FW_405
	int64_t      (*icc_snvs_read_sector)(int64_t a1, int64_t a2);
	int64_t      (*icc_snvs_write_sector)(int64_t a1, int64_t a2);
	int64_t      (*icc_snvs_get_write_count)(int64_t a1, int64_t a2);
#else
	int64_t      (*icc_snvs_read_sector)(const void *a1, void *a2);
	int64_t      (*icc_snvs_write_sector)(const void *a1, void *a2);
	int64_t      (*icc_snvs_get_write_count)(const void *a1, void *a2);
#endif	
#ifdef FW_405
	int64_t      (*icc_nvs_write)(char a1, uint32_t a2, short a3, uint16_t a4, int64_t a5);
	int64_t      (*icc_nvs_read)(char a1, uint32_t a2, short a3, short a4, int64_t a5);
#elif defined FW_455
	int64_t      (*icc_nvs_write)(char a1, char a2, short a3, uint16_t a4, int64_t a5);
	int64_t      (*icc_nvs_read)(char a1, char a2, short a3, short a4, int64_t a5);
	char         (*icc_sc_fw_update_handler)(int64_t a1, int64_t a2);
#else
	int64_t      (*icc_nvs_write)(char a1, short a2, uint16_t a3, int64_t a4);
	int64_t      (*icc_nvs_read)(char a1, short a2, short a3, int64_t a4);
	int64_t      (*icc_sc_fw_update_handler)(char a1, int64_t a2, int64_t a3);
#endif	
	int          (*icc_nvs_flush)(long a1);
	int          (*icc_query)(long a1, long a2);
	int          (*icc_query_nowait)(long a1);
	int          (*icc_sc_config_drvinit)(void);	
	int          (*icc_thermal_set_notify)(char a1);
	int          (*icc_thermal_get_thermal_state)(int64_t a1);
	int          (*icc_indicator_set_buzzer)(int64_t a1);
	int          (*icc_indicator_set_led)(int64_t a1);
	int          (*icc_indicator_get_led)(int64_t a1);
	int          (*icc_indicator_set_dynamic_led)(int64_t a1);
	int          (*icc_indicator_get_dynamic_led)(int64_t a1);
	int          (*icc_indicator_set_dynamic_led_boot)(void);
	int          (*icc_indicator_set_dynamic_led_shutdown)(int64_t a1);
	int          (*icc_indicator_set_thermalalert_led_off)(int64_t a1);
	int          (*icc_indicator_set_dynamic_led_off)(int64_t a1);
	int          (*icc_indicator_set_thermalalert_led_on)(int64_t a1);
	sint64_t     (*icc_indicator_set_dynamic_led_standby)(int64_t a1);
	int64_t      (*icc_indicator_set_dynamic_led_standby_boot)(int64_t a1);
	int          (*icc_indicator_drvinit)(char a1);
	int64_t      (*icc_indicator_check_thermal_on_boot)(int64_t a1);
	int          (*icc_thermal_alert_occurred_when_boot)(int a1, int a2, int a3);
	int64_t      (*icc_thermal_alert_occurred_when_boot_sub)(void);
	void         (*icc_thermal_alert_status_off)(void);
#ifdef FW_405
	int          (*icc_thermal_notify_handler)(int64_t a1, int64_t a2);
#else
	char         (*icc_thermal_notify_handler)(int64_t a1, int64_t a2);
#endif
	void         (*icc_thermal)(void);
	int64_t      (*icc_thermal_enable_disable_notify)(char a1);
	int64_t      (*icc_thermal_stop_td)(int64_t a1, int64_t a2);
	int64_t      (*icc_thermal_resume_td)(int64_t a1, int64_t a2);
	int64_t      (*icc_register_notification_handler)(int64_t a1, int64_t a2, int64_t a3);
	int64_t      (*icc_button_init)(char a1);
#ifdef FW_405
	int          (*icc_button_set_state)(int64_t a1, int64_t a2);
#else
	char         (*icc_button_set_state)(int64_t a1, int64_t a2);
#endif
	int          (*make_dev)(uint32_t a1, int a2, int a3, char a4);
	int          (*make_dev_credf)(int a1, int64_t a2, int a3, int a4, uint32_t a5, int64_t a6, int64_t a7, char a8);
	int          (*destroy_dev)(int64_t a1);
	int          (*devstat_new_entry)(char *a1, int a2, int a3, int a4, int a5, uint32_t a6);
	int          (*devstat_start_transaction)(int result, int64_t a2, int64_t a3);
	int          (*pfs_create_dir)(long a1, char *a2, long a3, long a4, long a5, int a6);
	int          (*pfs_create_file)(long a1, char *a2, long a3, long a4, long a5, long a6, int a7);
	int          (*pfs_create_link)(long a1, char *a2, long a3, long a4, long a5, long a6, int a7);
	int          (*dump_pfs)(char a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5, int64_t a);
	int          (*dump_sign)(int64_t a1);
	int64_t      (*pfs_savedata)(int64_t a1, char *a2, void *a3);
	int64_t      (*pfs_mapped_read)(const char *a1, const char *a2, int64_t a3, int64_t a4, int64_t a5);
	int64_t      (*pfs_check_addr)(int64_t a1, int64_t a2, int64_t a3);
	int64_t      (*pfs_checkpath)(const char *a1, const char *a2, int64_t a3, const char *a4, uint32_t *a5);
	sint64_t     (*pfs_mount)(int64_t a1, sint64_t a2);
	int          (*pfs_cmount)(int64_t a1, int64_t a2, uint32_t a3);
	int          (*pfs_unmount)(int64_t a1, uint32_t a2);
	int          (*pfs_root)(int64_t a1, int64_t a2, int64_t a3);
	int          (*pfs_fileno_alloc)(int64_t a1);
	uint64_t     (*pfs_fileno_free)(int64_t a1);
	int          (*pfs_vncache_load)();
	int64_t      (*pfs_vncache_alloc)(int64_t a1, int64_t *a2, int64_t a3, int a4);
	int64_t      (*pfs_fileno_init)(int64_t a1);
	char         (*pfs_purge)(int64_t a1);
	int          (*pfs_vncache_unload)(int64_t a1, int64_t a2);
	int          (*sceSblPfsKeymgrGenEKpfsForGDGPAC)(long a1);
	void         (*RsaesPkcs1v15Enc2048)(void);
	void         (*RsaesPkcs1v15Dec2048CRT)(void);
	sint32_t     (*AesCbcCfb128Encrypt)(unsigned long a1, unsigned long a2, int a3, long a4, uint32_t a5, long a6);
	sint32_t     (*AesCbcCfb128Decrypt)(unsigned long a1, unsigned long a2, int a3, long a4, uint32_t a5, long a6);
	sint32_t     (*AesEncKeySet)(long _RDI, long _RSI, int a3);
	int          (*AesDecKeySet)(long a1, long a2, int a3);
	int          (*AesEncrypt)(long _RDI, long _RSI, long _RDX, sint32_t a4);
	int          (*AesDecrypt)(long _RDI, long _RSI, long _RDX, sint32_t a4);
	sint32_t     (*Sha256HmacFinal)(long a1, long a2);
	sint32_t     (*Sha256Hash)(long a1, long a2, long a3);
	sint32_t     (*Sha256Hmac)(long a1, long a2, uint32_t a3, char *a4, uint32_t a5);
	int          (*Sha256OneBlk)(long a1, long a2);
	sint32_t     (*Sha256FinalCore)(long a1, long a2);
	int          (*Sha256Init)(long a1);
	sint32_t     (*Sha256Transform)(long a1, char *a2, long a3);
	sint32_t     (*Sha256Final)(long a1, long a2);
	sint32_t     (*Sha256HmacInit)(long a1, char *a2, uint32_t a3);
	int          (*index)(byte *a1, int a2);
	int          (*load_prx)(int64_t a1, byte *a2, char a3, int64_t *a4);
	int          (*preload_prx_modules)(int64_t a1, int64_t a2);
	int          (*gpu_devid_is_9924)();
	void         *(*gc_get_fw_info)();
	int          (*gs_is_update_bio_ioprio)(int64_t a1, int64_t a2);
#ifdef FW_405	
	int          (*uma_zalloc_arg)(int64_t seconds, int64_t a2, uint32_t a3);
#elif defined FW_455
	void         *(*uma_zalloc_arg)(int64_t a1, int64_t a2, uint32_t a3);
#else
	uint64_t     *(*uma_zalloc_arg)(uint64_t *seconds, uint64_t a2, uint32_t a3, int64_t a4);
#endif		
	uint32_t     (*sleep)(uint32_t seconds);
	void         (*csleep)(void);
	char         *(*toHexString)(byte *buffer, int len);
	char         *(*hexify)(char *string);
	void         (*disable_wp)(void);
	void         (*enable_wp)(void);
	int          (*_sceSblKeyMgrSmStart)(void);
#ifdef FW_405	
	int64_t      (*_sceSblAuthMgrSmStart)(int64_t *a1);	
#else
	int          (*_sceSblAuthMgrSmStart)(void);
#endif
	int64_t      (*fork1)(int64_t a1, uint32_t a2, sint32_t a3, int64_t *a4, int64_t a5, uint32_t a6);
	int          (*preLoad)(void);	
	int64_t      (*mdbg_kproc_hook_create)(int64_t a1);
	int          (*metadbg_log_add)(int64_t a1, int a2);
	int64_t      (*cpu_set_fork_handler)(int64_t a1, int64_t a2, int64_t a3);
	int          (*turnstile_adjust)(int64_t a1, uint8_t a2);
	void         (*bioq_init)(int64_t a1);
	int          (*disk_alloc)(void);
	int64_t      (*disk_create)(int64_t a1, int a2);
	int64_t      (*eventhandler_find_list)(char *a1);	
#if defined FW_405
	int          (*eventhandler_prune_list)(int result, int64_t a2, void *a3);
#elif defined FW_455
	int          (*eventhandler_prune_list)(int result, int64_t a2, void *a3);
#else
	int          (*eventhandler_prune_list)(int result, int64_t a2, int64_t a3, void *a4);
#endif
	int64_t      (*eventhandler_register)(int64_t a1, char *a2, int64_t a3, int64_t a4, int a5);
	char         (*eventhandler_deregister)(int64_t a1, int64_t *a2);
	void         (*sce_cam_error_log_init)(int a1);
	int          (*sce_cam_error_log_read)(int result);
	int64_t      (*sce_cam_error_log_write)(void);
	int64_t      (*sce_cam_error_put)(char a1, char a2, char a3, char a4, char a5, int a6, int64_t a7, int a8);
	//int          (*sce_exfatfs_vop_readdir)(int64_t a1, int64_t a2, int64_t a3);
	//int64_t      (*sce_exfatfs_vop_readdir)(int64_t a1);
	void         (*vsunlock)(void);
	int64_t      (*get_td_locks)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int          (*gen_mem_keys)(int64_t a1, int64_t a2);
	int64_t      (*procfs_docurproc)(int64_t a1, int64_t a2, int64_t a3, char a4);
	int64_t      (*procfs_attr)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int          (*procfs_notsystem)(int64_t a1, int64_t a2);
	int64_t      (*procfs_doprocfile)(int64_t a1, int64_t a2, int64_t a3, char a4);
	int64_t      (*procfs_doproccmdline)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int64_t      (*procfs_doprocstatus)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int64_t      (*procfs_doosrel)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
	int64_t      (*procfs_doprocctl)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
	int64_t      (*procfs_doprocdbregs)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
	int64_t      (*procfs_doproctype)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int64_t      (*procfs_doprocfpregs)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
	int64_t      (*procfs_doprocmap)(int64_t a1, int64_t a2, int64_t a3, char a4, int64_t a5);
	int          (*procfs_doprocmem)(int64_t a1, int64_t a2, int64_t a3, int64_t a4, int64_t a5);
	int          (*procfs_ioctl)(int64_t a1, sint64_t a2, int64_t a3, sint64_t a4, int64_t a5);
	sint64_t     (*procfs_doprocnote)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int64_t      (*procfs_doprocrlimit)(int64_t a1, int64_t a2, int64_t a3, char a4);
	void         (*SceBdemu)(int64_t seconds);
	int64_t      (*bd_sched_copier_handle_resume_event)(int64_t a1);
	int64_t      (*bd_sched_copier_handle_suspend_event)(int64_t a1);
	int64_t      (*bd_sched_update_bitmap_file)(const char *a1, int64_t a2);
	int64_t      (*bd_sched_writer_handle_resume_event)(int64_t a1);
	int64_t      (*bd_sched_writer_handle_suspend_event)(int64_t a1);
	int64_t      (*bd_sched_get_bdsio_in_writer)(int64_t a1, int64_t a2);
	int64_t      (*get_pkg_offset)(int64_t a1, int64_t a2);
	int64_t      (*bd_sched_read_bd_dev)(int64_t a1, int64_t a2);
	const char   *(*get_img_offset)(const char *a1, const char *a2);
	char         (*bd_sched_unlock_bm_file)(int64_t a1);
	int64_t      (*bd_sched_is_available_on_hdd2)(int64_t a1, int64_t a2, int64_t a3, int a4);
	int64_t      (*bd_sched_update_bitmap_cache2)(int64_t a1, int64_t a2, uint64_t a3, int a4);
	char         (*bd_sched_lock_bm_file)(int64_t seconds);
	int64_t      (*bd_sched_bd_reader_bg_copy)(int64_t a1, int64_t a2, int64_t a3, int64_t a4);
	int64_t      (*bd_sched_bd_reader_ripping)(int64_t a1);
	int64_t      (*bd_sched_reader_handle_resume_event)(int64_t a1);
	int64_t      (*bd_sched_reader_handle_suspend_event)(int64_t a1);
	char         (*bd_sched_shutdown_r_bd_internal)(int64_t a1, int a2);
	char         (*bd_sched_shutdown_w_hdd)(int64_t a1);
	char         (*bd_sched_shutdown_copier)(int64_t a1);
	int64_t      (*bd_sched_get_bdsio_in_cache)(int64_t a1, int64_t a2);
	char         (*bd_sched_private_done)(int64_t a1);
	int          (*bd_sched_post_done)(const char *a1, const char *a2);
	int          (*bd_sched_adjust_bio_completed)(int64_t a1);
	int64_t      (*bdemu_speed_emu)(int64_t a1);
	int64_t      (*g_bdemu_create)(int a1);	
} KernSymTable;

// Kernel Console Symbols structure.
typedef struct kcon_sym_t {
	void     (*WriteLine)(const char *format, ...);
	void     (*WriteError)(const char *format, ...);
	void     (*WriteWarning)(const char *format, ...);
	void     (*LineBreak)(void);
	uint32_t (*getMsg)(char *buff);
	uint32_t (*getErrorMsg)(char *buff);
	uint32_t (*getWarningMsg)(char *buff);
	void     (*waitForUsrInput)(char *inputToWait);
	void     (*setMsg)(char *buff);
} KConSymTable;

/* Global Accessor. */
extern KernSymTable lv2;

/* Global Acessor. */
extern KConSymTable kconsole;

/* Get current CPU ID. */
static inline int curcpu(void) {
	int cpuid;
	// TODO ensure offsetof(struct pcpu, pc_cpuid) == 0x34 on all fw
	__asm__ volatile ("mov %0, gs:0x34;" : "=r" (cpuid));
	return cpuid;
}

/* Get Kernel Base Offset. */
//void lv2_getkernbase();

/* Resolve Kernel Functions. */
void lv2_resolve(void);

/* Lv2 contiguous Allocator. */
void *lv2_alloc_contig(size_t size);

/* Lv2 Free contiguous. */
void lv2_free_contig(void *addr, size_t size);

/* Remap lv2. */
void lv2_remap(void *start, void *end, int perm);

/* Patch lv2 pmap check. */
int lv2_patch_pmap_check(void);

/* Install a lv2 Hook. */
int lv2_hook_install(void *target, void *hook);

/* Install a new Syscall. */
void lv2_syscall_install(int num, void *call, int narg);

/* Initialize lv2, map functions. */
int lv2_init(void);

/* Initialize lib kernel console, map functions. */
void kconsole_init(void);