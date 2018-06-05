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

#include "lv2.h"
#include "magics.h"
#include "x86-64.h"
#include <string.h>

static volatile int _global_test = 0;
extern uint8_t _start[], _end[];
KernSym lv2;

/* Get Kernel base address. */
/*void lv2_getkernbase() {
	//if (early_printf == NULL) return;
#ifdef FW_405
	lv2.kern_base = ((uint64_t)0x9F34F580) - __printf;
#elif defined FW_455
	lv2.kern_base = ((uint64_t)0x82217F30) - __printf;
#elif defined FW_500 || FW_501 || FW_505
	lv2.kern_base = ((uint64_t)0x82635C70) - __printf;
#endif
	if ((lv2.kern_base & PAGE_MASK) != 0) {
		//eprintf("Kernel base is not aligned\n");
		return;
	}
	//else eprintf("Kernel base = %llx\n", lv2.kern_base);

	uint64_t DMPML4I = *(uint32_t *)(lv2.kern_base + __dmpml4i);
	uint64_t DMPDPI = *(uint32_t *)(lv2.kern_base + __dmpdpi);
	lv2.dmap_base = KVADDR(DMPML4I, DMPDPI, 0, 0);
	//eprintf("Direct map base = %llx\n", lv2.dmap_base);
}*/

/* Resolve some real Kernel functions. */
void lv2_resolve() {
#define r(name, offset) name = (void *)(lv2.kern_base + offset)

#ifdef FW_405
	lv2.M_TEMP = (void *)(lv2.kern_base + __M_TEMP);
#endif
	lv2.icc_query_nowait = (void *)(lv2.kern_base + __icc_query_nowait);
	lv2.Starsha_UcodeInfo = (void *)(lv2.kern_base + __Starsha_UcodeInfo);
	lv2.sysent = (SySent *)(lv2.kern_base + __sysent);

	r(lv2.copyin, __copyin);
	r(lv2.copyout, __copyout);
	r(lv2.copyinstr, __copyinstr);
	r(lv2.copystr, __copystr);
	r(lv2.printf, __printf);
	r(lv2.sprintf, __sprintf);
	r(lv2.snprintf, __snprintf);
	r(lv2.vprintf, __vprintf);
	r(lv2.vsnprintf, __vsnprintf);
	r(lv2.malloc, __malloc);
	r(lv2.free, __free);
	r(lv2.strlen, __strlen);
	r(lv2.memcpy, __memcpy);
	r(lv2.memset, __memset);
	r(lv2.memcmp, __memcmp);
	r(lv2.kernel_map, __kernel_map);
	r(lv2.icc_nvs_read, __icc_nvs_read);
	r(lv2.icc_nvs_write, __icc_nvs_write);
	//r(lv2.sflash_pread_for_vtrm, __sflash_pread_for_vtrm);
	//r(lv2.sflash_pwrite_for_vtrm, __sflash_pwrite_for_vtrm);
	//r(lv2.sflash_erase_for_vtrm, __sflash_erase_for_vtrm);
	r(lv2.sceSblSsDecryptSealedKey, __sceSblSsDecryptSealedKey);
	r(lv2.getSealedKeySecret, __getSealedKeySecret);
	r(lv2.kernel_pmap_store, __kernel_pmap_store);
	r(lv2.kmem_alloc_contig, __kmem_alloc_contig);
	r(lv2.kmem_free, __kmem_free);
	r(lv2.pmap_extract, __pmap_extract);
	r(lv2.pmap_protect, __pmap_protect);
	r(lv2.kthread_add, __kthread_add);
	r(lv2.proc_rwmem, __proc_rwmem);
	r(lv2.sx_init_flags, __sx_init_flags);
	r(lv2.sx_xlock, __sx_xlock);
	r(lv2.sx_xunlock, __sx_xunlock);
	r(lv2.sx_destroy, __sx_destroy);
	r(lv2.fpu_kern_enter, __fpu_kern_enter);
	r(lv2.fpu_kern_leave, __fpu_kern_leave);
	r(lv2.sched_pin, __sched_pin);
	r(lv2.sched_unpin, __sched_unpin);
	r(lv2.sched_bind, __sched_bind);
	r(lv2.sched_switch, __sched_switch);
	r(lv2.sleepq_broadcast, __sleepq_broadcast);
	r(lv2.sleepq_lock, __sleepq_lock);
	r(lv2.sleepq_release, __sleepq_release);
	r(lv2.sleepq_remove, __sleepq_remove);
	r(lv2.smp_no_rendevous_barrier, __smp_no_rendevous_barrier);
	r(lv2.smp_rendezvous, __smp_rendezvous);
	//r(lv2.sys_sendto, __sys_sendto);
	r(lv2.gpu_devid_is_9924, __gpu_devid_is_9924);
	r(lv2.gc_get_fw_info, __gc_get_fw_info);
}

/* Lv2 contiguous Allocator. */
void *lv2_alloc_contig(size_t size) {
	// use kmem_alloc_contig instead of contigalloc to avoid messing with a malloc_type...
	vm_offset_t ret = lv2.kmem_alloc_contig(
		(vm_map_t)*lv2.kernel_map, size, M_ZERO | M_WAITOK, (vm_paddr_t)0,
		~(vm_paddr_t)0, 1, 0, VM_MEMATTR_DEFAULT);

	if (!ret) {
		lv2.printf("Failed to allocate %zud bytes\n", size);
		return NULL;
	}
	return (void *)PA_TO_DM(lv2.pmap_extract(lv2.kernel_pmap_store, ret));
}

/* Lv2 Free contiguous. */
void lv2_free_contig(void *addr, size_t size) {
	if (!addr)
		return;
	lv2.kmem_free((vm_map_t)*lv2.kernel_map, (vm_offset_t)addr, size);
}

/* Install lv2 Hook. */
int lv2_hook_install(void *target, void *hook) {
	uintptr_t t = (uintptr_t)target; // addr to redirect to
	uintptr_t h = (uintptr_t)hook; // place to write the thunk

	if (!hook || !target) { return 0; }

	if (!(t & (1L << 63))) {
		lv2.printf("\n===================== WARNING =====================\n");
		lv2.printf("hook target function address: %p\n", t);
		lv2.printf("It looks like we're running from userland memory.\n");
		lv2.printf("Please run this code from a kernel memory mapping.\n\n");
		return 0;
	}

	sint64_t displacement = t - (h + 5);

	lv2.sched_pin();
	cr0_write(cr0_read() & ~CR0_WP);
	if (displacement < -0x80000000 || displacement > 0x7fffffff) {
		lv2.printf("  Using 64bit absolute jump\n");
		struct __attribute__((packed)) jmp_t {
			uint8_t  op[2];
			sint32_t zero;
			void     *target;
		} jmp = {
			.op = { 0xff, 0x25 },
			.zero = 0,
			.target = target,
		};
		ASSERT_STRSIZE(struct jmp_t, 14);
		memcpy(hook, &jmp, sizeof(jmp));
	}
	else {
		lv2.printf("  Using 32bit relative jump\n");
		struct __attribute__((packed)) jmp_t {
			uint8_t  op[1];
			sint32_t imm;
		} jmp = {
			.op = { 0xe9 },
			.imm = displacement,
		};
		ASSERT_STRSIZE(struct jmp_t, 5);
		memcpy(hook, &jmp, sizeof(jmp));
	}
	wbinvd();
	cr0_write(cr0_read() | CR0_WP);
	lv2.sched_unpin();

	return 1;
}

/* Install a lv2 systemcall. */
void lv2_syscall_install(int num, void *call, int narg) {
	struct sysent_t *sy = &lv2.sysent[num];

	lv2.sched_pin();
	cr0_write(cr0_read() & ~CR0_WP);

	memset(sy, 0, sizeof(*sy));
	sy->sy_narg = narg;
	sy->sy_call = call;
	sy->sy_thrcnt = 1;

	cr0_write(cr0_read() | CR0_WP);
	lv2.sched_unpin();
}

/* Remap Lv2 Memory. */
void lv2_remap(void *start, void *end, int perm) {
	uint64_t s = ((uint64_t)start) & ~(uint64_t)(PAGE_SIZE - 1);
	uint64_t e = ((uint64_t)end + PAGE_SIZE - 1) & ~(uint64_t)(PAGE_SIZE - 1);

	lv2.printf("pmap_protect(pmap, %p, %p, %d)\n", (void*)s, (void*)e, perm);
	lv2.pmap_protect(lv2.kernel_pmap_store, s, e, perm);
}

/* Patch lv2 pmap check. */
int lv2_patch_pmap_check(void) {
	uint8_t *p;

	for (p = (uint8_t*)lv2.pmap_protect;
		p < ((uint8_t*)lv2.pmap_protect + 0x500); p++) {
		if (!memcmp(p, "\x83\xe0\x06\x83\xf8\x06", 6)) {
			p[2] = 0;
			lv2.printf("pmap_protect patch successful (found at %p)\n", p);
			return 1;
		}
	}
	lv2.printf("pmap_protect patch failed!\n");
	return 0;
}

/* Initialize lv2, map functions. */
int lv2_init(void) {
	// Resolve Kernel base.
#ifdef FW_405
	lv2.kern_base = &((uint8_t*)__readmsr(0xC0000082))[-0x30EB30];
#elif defined FW_455
	lv2.kern_base = &((uint8_t*)__readmsr(0xC0000082))[-0x3095D0];
#elif defined FW_500 || FW_501
	lv2.kern_base = &((uint8_t*)__readmsr(0xC0000082))[-0x00001C0];
#elif defined FW_505
	lv2.kern_base = &((uint8_t*)__readmsr(0xC0000082))[-0x00001C0];
#endif

	// Resolve Symbols.
	lv2_resolve();

	return 0;
}
