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

#define LIBRARY_IMPL  (1)

#include "unjail.h"
#include "magics.h"

uint8_t mmap_self[11];

// Unjail 4.05
void *unjail(struct thread *td) {
	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[-__Xfast_syscall];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[__prison0];
	void** got_rootvnode = (void**)&kernel_ptr[__rootvnode];

	cred->cr_uid              = 0;
	cred->cr_ruid             = 0;
	cred->cr_rgid             = 0;
	cred->cr_groups[0]        = 0;
	cred->cr_prison           = *got_prison0;
	fd->fd_rdir = fd->fd_jdir = *got_rootvnode;

	// escalate ucred privs, needed for access to the filesystem ie* mounting & decrypting files
	void *td_ucred = *(void **)(((char *)td) + 304); // p_ucred == td_ucred

	// sceSblACMgrIsSystemUcred
	uint64_t *sonyCred = (uint64_t *)(((char *)td_ucred) + 96);
	*sonyCred = 0xffffffffffffffff;

	// sceSblACMgrGetDeviceAccessType
	uint64_t *sceProcType = (uint64_t *)(((char *)td_ucred) + 88);
	*sceProcType = 0x3801000000000013; // Max access

	// sceSblACMgrHasSceProcessCapability
	uint64_t *sceProcCap = (uint64_t *)(((char *)td_ucred) + 104);
	*sceProcCap = 0xffffffffffffffff; // Sce Process
	return 0;
}

// Enable SUDO (DevKit ID & Allow Mapping of SELF Patch).
void *enableMMAPSelf(struct thread *td) {
	// Internal buffer to read into orig values.
	uint8_t mmapself[11];

	// Initialize Lib LV2.
	lv2_init();
	
	// Copy out to userland to not get lost.
#if defined FW_405
	mmapself[0]  = *(uint8_t*)(&lv2.kern_base + __mmap_self_0);
	mmapself[1]  = *(uint8_t*)(&lv2.kern_base + __mmap_self_1);
	mmapself[2]  = *(uint8_t*)(&lv2.kern_base + __mmap_self_2);
	mmapself[3]  = *(uint8_t*)(&lv2.kern_base + __mmap_self_3);
	lv2.copyout(&mmapself, &mmap_self, sizeof(mmapself) / sizeof(*mmapself));
#elif defined FW_455
	mmapself[0] = *(uint8_t*)(&lv2.kern_base + __mmap_self_0);
	mmapself[1] = *(uint8_t*)(&lv2.kern_base + __mmap_self_1);
	mmapself[2] = *(uint8_t*)(&lv2.kern_base + __mmap_self_2);
	mmapself[3] = *(uint8_t*)(&lv2.kern_base + __mmap_self_3);
	lv2.copyout(&mmapself, &mmap_self, sizeof(mmapself) / sizeof(*mmapself));
#else
	mmapself[0] = *(uint8_t*)(&lv2.kern_base + __mmap_self_0);
	mmapself[1] = *(uint8_t*)(&lv2.kern_base + __mmap_self_1);
	mmapself[2] = *(uint8_t*)(&lv2.kern_base + __mmap_self_2);

	mmapself[3] = *(uint8_t*)(&lv2.kern_base + __mmap_self_3);
	mmapself[4] = *(uint8_t*)(&lv2.kern_base + __mmap_self_4);
	mmapself[5] = *(uint8_t*)(&lv2.kern_base + __mmap_self_5);

	mmapself[6] = *(uint8_t*)(&lv2.kern_base + __mmap_self_6);
	mmapself[7] = *(uint8_t*)(&lv2.kern_base + __mmap_self_7);
	mmapself[8] = *(uint8_t*)(&lv2.kern_base + __mmap_self_8);
	mmapself[9] = *(uint8_t*)(&lv2.kern_base + __mmap_self_9);
	mmapself[10] = *(uint8_t*)(&lv2.kern_base + __mmap_self_10);

	// Copy out to userland to not get lost.
	lv2.copyout(&mmapself, &mmap_self, sizeof(mmapself) / sizeof(*mmapself));
#endif
	
	// Disable Write Protection.
	lv2.disable_wp();

	// patch allowe to mmap self *thanks to IDC
#if defined FW_405
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(&lv2.kern_base + __mmap_self_0) = __mmap_self_patch_0; //0x0F
		*(uint8_t*)(&lv2.kern_base + __mmap_self_1) = __mmap_self_patch_1; //0x84
		*(uint8_t*)(&lv2.kern_base + __mmap_self_2) = __mmap_self_patch_0; //0x74
		*(uint8_t*)(&lv2.kern_base + __mmap_self_3) = __mmap_self_patch_0; //0x0F
	}
#elif defined FW_455
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(&lv2.kern_base + __mmap_self_0) = __mmap_self_patch_0; //0x0F
		*(uint8_t*)(&lv2.kern_base + __mmap_self_1) = __mmap_self_patch_1; //0x84
		*(uint8_t*)(&lv2.kern_base + __mmap_self_2) = __mmap_self_patch_0; //0x74
		*(uint8_t*)(&lv2.kern_base + __mmap_self_3) = __mmap_self_patch_0; //0x0F
	}
#else
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(&lv2.kern_base + __mmap_self_0) = __mmap_self_patch_0;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_1) = __mmap_self_patch_1;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_2) = __mmap_self_patch_2;

		*(uint8_t*)(&lv2.kern_base + __mmap_self_3) = __mmap_self_patch_0;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_4) = __mmap_self_patch_1;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_5) = __mmap_self_patch_2;

		*(uint8_t*)(&lv2.kern_base + __mmap_self_6) = __mmap_self_patch_3;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_7) = __mmap_self_patch_4;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_8) = __mmap_self_patch_5;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_9) = __mmap_self_patch_5;
		*(uint8_t*)(&lv2.kern_base + __mmap_self_10) = __mmap_self_patch_5;
	}
#endif

	// Enable Write Protection.
	lv2.enable_wp();
	return 0;
}

// Disable SUDO.
void *disableMMAPSelf(struct thread *td) {
	// Copy in backed up values.
	uint8_t mmapself[11];
	lv2.copyin(&mmap_self, &mmapself, sizeof(mmapself) / sizeof(*mmapself));

	// Disable Write Protection.
	lv2.disable_wp();

	// Restore mmap self conditions.
#if defined FW_405
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(&lv2.kern_base + __mmap_self_0) = mmapself[0]; //0x0F
		*(uint8_t*)(&lv2.kern_base + __mmap_self_1) = mmapself[1]; //0x84
		*(uint8_t*)(&lv2.kern_base + __mmap_self_2) = mmapself[2]; //0x74
		*(uint8_t*)(&lv2.kern_base + __mmap_self_3) = mmapself[3]; //0x0F
	}
#elif defined FW_455
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(&lv2.kern_base + __mmap_self_0) = mmapself[0]; //0x0F
		*(uint8_t*)(&lv2.kern_base + __mmap_self_1) = mmapself[1]; //0x84
		*(uint8_t*)(&lv2.kern_base + __mmap_self_2) = mmapself[2]; //0x74
		*(uint8_t*)(&lv2.kern_base + __mmap_self_3) = mmapself[3]; //0x0F
	}
#else
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(&lv2.kern_base + __mmap_self_0) = mmapself[0];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_1) = mmapself[1];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_2) = mmapself[2];

		*(uint8_t*)(&lv2.kern_base + __mmap_self_3) = mmapself[3];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_4) = mmapself[4];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_5) = mmapself[5];

		*(uint8_t*)(&lv2.kern_base + __mmap_self_6) = mmapself[6];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_7) = mmapself[7];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_8) = mmapself[8];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_9) = mmapself[9];
		*(uint8_t*)(&lv2.kern_base + __mmap_self_10) = mmapself[10];
	}
#endif

	// Enable Write Protection.
	lv2.enable_wp();
	return 0;
}

// Disable ASLR for Big Game Process.
void *disableProcessASLR(struct thread *td) {
	// Get Kernel Base.
	void* kern_base = &((uint8_t*)__readmsr(0xC0000082))[-__Xfast_syscall];
	
	// Disable Write Protection.
	lv2.disable_wp();

	// Disable process ASLR.
	*(uint16_t*)(&kern_base + __process_aslr) = 0x9090;

	// Enable Write Protection.
	lv2.enable_wp();

	return 0;
}

// Disable ASLR for Big Game Process.
void *enableBrowser(struct thread *td) {
	// Init liblv2.
	lv2_init();

#if defined FW_405
	lv2.sceRegMgrSetInt(0x3C040000, 0);
#elif defined FW_455
	lv2.sceRegMgrSetInt(0x3C040000, 0);
#else
	lv2.sceRegMgrSetInt(0x3C040000, 0, 0, 0, 0);
#endif
	return 0;
}