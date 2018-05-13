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
#define CR0_WP (1 << 16) // write protect

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

static inline __attribute__((always_inline)) uint64_t cr0_read(void) {
	uint64_t cr0;

	asm volatile (
		"movq %0, %%cr0"
		: "=r" (cr0)
		: : "memory"
		);

	return cr0;
}

static inline __attribute__((always_inline)) void cr0_write(uint64_t cr0) {
	asm volatile (
		"movq %%cr0, %0"
		: : "r" (cr0)
		: "memory"
		);
}

static inline void wbinvd(void) { asm volatile("wbinvd"); }

static inline void disable_interrupts(void) { asm volatile("cli"); }

static inline void enable_interrupts(void) { asm volatile("sti"); }

static inline uint64_t read_flags(void) {
	uint64_t flags;
	asm volatile("pushf; pop %0;" : "=r" (flags));
	return flags;
}

static inline uint64_t intr_disable(void) {
	uint64_t flags = read_flags();
	disable_interrupts();
	return flags;
}

static inline void intr_restore(uint64_t flags) {
	// TODO should only IF be or'd in?
	asm volatile("push %0; popf;" : : "rm" (flags) : "memory");
}

typedef int              vm_prot_t;
typedef uint64_t         vm_offset_t;
typedef uint64_t         vm_map_t;
typedef uint64_t         vm_size_t;
typedef char             vm_memattr_t;
typedef void             *pmap_t;
typedef unsigned long    size_t;
typedef signed long long sint64_t;
typedef int              sint32_t;
struct                   syscall_args;

typedef struct {
	time_t        sec;      // Event seconds
	unsigned int  milli;    // millisecond of the timestamp
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

typedef struct ksym_t {
	void        **kernel_map;
	void        *kernel_pmap_store;
	void        *icc_query_nowait;
	void        *Starsha_UcodeInfo;
	void        *M_TEMP;
	SySent      *sysent;
	void        *kern_base;
	uintptr_t   dmap_base;

	void *(*malloc)(unsigned long size, void *type, int flags);
	void (*free)(void *addr, void *type);
	void (*memcpy)(void *dst, const void *src, size_t len);
	void *(*memset)(void * ptr, int value, size_t num);
	int (*memcmp)(const void * ptr1, const void * ptr2, size_t num);
	int (*printf)(const char *fmt, ...);
	int (*sprintf)(char *str, const	char *format, ...);
	int (*snprintf)(char *str, size_t size, const char *format, ...);
	int (*vprintf)(const char *format, va_list ap);
	int (*vsnprintf)(char *str, size_t size, const char *format, va_list ap);
	int (*copyin)(const void *uaddr, void *kaddr, size_t len);
	int (*copyout)(const void *kaddr, void *uaddr, size_t len);
	int (*copyinstr)(const void *uaddr, void *kaddr, size_t len, size_t *done);
	int (*copystr)(const void *kfaddr, void *kdaddr, size_t len, size_t *done);
	size_t (*strlen)(const char *str);
	int (*kthread_add)(void(*func)(void *), void *arg, struct proc *procp, struct thread **newtdpp, int flags, int pages, const char *fmt, ...);
	vm_offset_t (*kmem_alloc_contig)(vm_map_t map, vm_size_t size, int flags, vm_paddr_t low, vm_paddr_t high, unsigned long alignment, unsigned long boundary, vm_memattr_t memattr);
	void (*kmem_free)(vm_map_t, vm_offset_t, vm_size_t);
	vm_paddr_t (*pmap_extract)(pmap_t pmap, vm_offset_t va);
	void (*pmap_protect)(pmap_t pmap, uint64_t sva, uint64_t eva, uint8_t pr);
	int (*sys_sendto)(void *td, SendToArgs sargs);
	int (*proc_rwmem)(struct proc *p, struct uio *uio);
	void (*sx_init_flags)(struct sx *sx, const char *description, int opts);
	void (*sx_xlock)(struct sx *sx);
	void (*sx_xunlock)(struct sx *sx);
	void (*sx_destroy)(struct sx *sx);
	int (*fpu_kern_enter)(struct thread *td, void *ctx, unsigned int flags);
	int (*fpu_kern_leave)(struct thread *td, void *ctx);
	void (*sched_pin)(void);
	void (*sched_unpin)(void);
	void (*sched_switch)(struct thread *td, struct thread *newtd, int flags);
	void (*sched_bind)(struct thread *td, int cpu);
	int (*sflash_pread_for_vtrm)(void);                        // Incomplete
	int (*sflash_pwrite_for_vtrm)(void);                       // Incomplete
	int (*sflash_erase_for_vtrm)(uint64_t off);                // Incomplete
	int (*sceSblVtrmRestore)();                                // Incomplete
	void (*smp_rendezvous)(smp_rendezvous_callback_t, smp_rendezvous_callback_t, smp_rendezvous_callback_t, void *);
	void (*smp_no_rendevous_barrier)(void *);  // yes...it is misspelled :)
	int (*sceSblSsDecryptSealedKey)(void *encryptedSealedKey, void *decryptedSealedKey);
#ifdef FW_405
	int (*getSealedKeySecret)(unsigned char *saveDataMasterKey, unsigned char *saveDataMasterHashKey);
#elif defined FW_455
	int (*getSealedKeySecret)(unsigned char *saveDataMasterKey, unsigned char *saveDataMasterHashKey);
#elif defined FW_500 || FW_501 || FW_505
	int (*getSealedKeySecret)(unsigned char *saveDataMasterKey, unsigned char *saveDataMasterHashKey, unsigned char *v3);
#endif
	int (*sceSblSsGenerateSealedKey)(void *encryptedSealedKey);  // ?
	int (*kernel_sysctl)(struct thread *td, int *name, u_int namelen, void *oldVal, size_t *oldlenp, void *newVal, size_t newlen, size_t *retval, int flags);
	int (*kernel_sysctlbyname)(struct thread *td, char *name, void *oldVal, size_t *oldlenp, void *newVal, size_t newlen, size_t *retval, int flags);
	int64_t (*icc_nvs_read)(size_t bank_id, size_t block_id, size_t offset, size_t size, uint8_t *data_ptr);
	int (*icc_nvs_write)(size_t bank_id, size_t block_id, size_t offset, size_t size, uint8_t *data_ptr);
	int (*gpu_devid_is_9924)();
	void *(*gc_get_fw_info)();
	int (*sleepq_broadcast)(void *wchan, int flags, int pri, int queue);
	void (*sleepq_lock)(void *wchan);
	void (*sleepq_release)(void *wchan);
	void (*sleepq_remove)(struct thread *td, void *wchan);
} KernSym;

/* Global Accessor. */
extern KernSym lv2;

/* Get current CPU ID. */
static inline int curcpu(void) {
	int cpuid;
	// TODO ensure offsetof(struct pcpu, pc_cpuid) == 0x34 on all fw
	asm volatile ("mov %0, gs:0x34;" : "=r" (cpuid));
	return cpuid;
}

/* Get Kernel Base Offset. */
void lv2_getkernbase();

/* Resolve Kernel Functions. */
void lv2_resolve();

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