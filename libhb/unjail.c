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

#include "unjail.h"
#include "magics.h"

uint16_t target_id[3];
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
void *enableSUDO(struct thread *td) {
	// 
	uint16_t targetid[3];
	uint8_t mmapself[11];

	// Initialize Lib LV2.
	lv2_init();
	kconsole.WriteLine("Liblv2 Initialized.\n");

	// Backup Orig Values first.
#ifdef FW_405
	targetid[0] = *(uint16_t *)(lv2.kern_base + __target_id_0);
	targetid[1] = *(uint16_t *)(lv2.kern_base + __target_id_1);
	targetid[2] = *(uint16_t *)(lv2.kern_base + __target_id_2);
	lv2.copyout(&targetid, &target_id, sizeof(targetid) / sizeof(*targetid));
	kconsole.WriteLine("Target ID backed up.\n");

	// Write out bytes and wait for input.
	kconsole.WriteLine("TragetID Orig:\n%#X\n%#X\n%#X\n\n", targetid[0], targetid[1], targetid[2]);
#elif defined FW_455
	targetid[0] = *(uint16_t *)(lv2.kern_base + __target_id_0);
	targetid[1] = *(uint16_t *)(lv2.kern_base + __target_id_1);
	targetid[2] = *(uint16_t *)(lv2.kern_base + __target_id_2);
	lv2.copyout(&targetid, &target_id, sizeof(targetid) / sizeof(*targetid));
	kconsole.WriteLine("Target ID backed up.\n");

	// Write out bytes and wait for input.
	kconsole.WriteLine("TragetID Orig:\n%#X\n%#X\n%#X\n\n", targetid[0], targetid[1], targetid[2]);
#else
	targetid[0] = *(uint16_t *)(lv2.kern_base + __target_id_0);
	targetid[1] = *(uint16_t *)(lv2.kern_base + __target_id_1);
	lv2.copyout(&targetid, &target_id, sizeof(targetid) / sizeof(*targetid));
	kconsole.WriteLine("Target ID backed up.\n");

	// Write out bytes and wait for input.
	kconsole.WriteLine("TragetID Orig:\n%#X\n%#X\n\n", targetid[0], targetid[1]);
#endif
	
	// Copy out to userland to not get lost.
#if defined FW_405
	mmapself[0]  = *(uint8_t*)(lv2.kern_base + __mmap_self_0);
	mmapself[1]  = *(uint8_t*)(lv2.kern_base + __mmap_self_1);
	mmapself[2]  = *(uint8_t*)(lv2.kern_base + __mmap_self_2);
	mmapself[3]  = *(uint8_t*)(lv2.kern_base + __mmap_self_3);
	lv2.copyout(&mmapself, &mmap_self, sizeof(mmapself) / sizeof(*mmapself));
	kconsole.WriteLine("Mmap Self backed up.\n");

	// Write out bytes and wait for input.
	kconsole.WriteLine("Mmap Self Orig:\n%#X\n%#X\n%#X\n%#X\n\n", mmapself[0], mmapself[1], mmapself[2], mmapself[3]);
#elif defined FW_455
	mmapself[0] = *(uint8_t*)(lv2.kern_base + __mmap_self_0);
	mmapself[1] = *(uint8_t*)(lv2.kern_base + __mmap_self_1);
	mmapself[2] = *(uint8_t*)(lv2.kern_base + __mmap_self_2);
	mmapself[3] = *(uint8_t*)(lv2.kern_base + __mmap_self_3);
	lv2.copyout(&mmapself, &mmap_self, sizeof(mmapself) / sizeof(*mmapself));
	kconsole.WriteLine("Mmap Self backed up.\n");

	// Write out bytes and wait for input.
	kconsole.WriteLine("Mmap Self Orig:\n%#X\n%#X\n%#X\n%#X\n\n", mmapself[0], mmapself[1], mmapself[2], mmapself[3]);
#else
	mmapself[0] = *(uint8_t*)(lv2.kern_base + __mmap_self_0);
	mmapself[1] = *(uint8_t*)(lv2.kern_base + __mmap_self_1);
	mmapself[2] = *(uint8_t*)(lv2.kern_base + __mmap_self_2);

	mmapself[3] = *(uint8_t*)(lv2.kern_base + __mmap_self_3);
	mmapself[4] = *(uint8_t*)(lv2.kern_base + __mmap_self_4);
	mmapself[5] = *(uint8_t*)(lv2.kern_base + __mmap_self_5);

	mmapself[6] = *(uint8_t*)(lv2.kern_base + __mmap_self_6);
	mmapself[7] = *(uint8_t*)(lv2.kern_base + __mmap_self_7);
	mmapself[8] = *(uint8_t*)(lv2.kern_base + __mmap_self_8);
	mmapself[9] = *(uint8_t*)(lv2.kern_base + __mmap_self_9);
	mmapself[10] = *(uint8_t*)(lv2.kern_base + __mmap_self_10);
	lv2.copyout(&mmapself, &mmap_self, sizeof(mmapself) / sizeof(*mmapself));
	kconsole.WriteLine("Mmap Self backed up.\n");

	// Write out bytes and wait for input.
	kconsole.WriteLine("Mmap Self Orig:\n%#X\n%#X\n%#X\n%#X\n%#X\n%#X\n%#X\n%#X\n%#X\n%#X\n%#X\n\n", mmapself[0], mmapself[1], mmapself[2], mmapself[3], mmapself[4], mmapself[5], mmapself[6], mmapself[7], mmapself[8], mmapself[9], mmapself[10]);
#endif
	
	// Disable Write Protection.
	lv2.disable_wp();
	kconsole.WriteLine("CPU Write Protection Disabled.\n");

	// Target ID Patches :)
#if defined FW_405
	if (targetid[0] != __devkit_id) {
		*(uint16_t *)(lv2.kern_base + __target_id_0) = __devkit_id;
		*(uint16_t *)(lv2.kern_base + __target_id_1) = __devkit_id;
		*(uint16_t *)(lv2.kern_base + __target_id_2) = __devkit_id;
	}
#elif defined FW_455
	if (targetid[0] != __devkit_id) {
		*(uint16_t *)(lv2.kern_base + __target_id_0) = __devkit_id;
		*(uint16_t *)(lv2.kern_base + __target_id_1) = __devkit_id;
		*(uint16_t *)(lv2.kern_base + __target_id_2) = __devkit_id;
	}
#else
	if (targetid[0] != __devkit_id) {
		*(uint16_t *)(lv2.kern_base + __target_id_0) = __devkit_id;
		*(uint16_t *)(lv2.kern_base + __target_id_1) = __devkit_id;
	}
#endif
	if (targetid[0] != __devkit_id) kconsole.WriteLine("Console Patched to Devkit.\n");
	else kconsole.WriteLine("Devkit already patched.\n");
	

	// patch allowed to mmap self *thanks to IDC
#if defined FW_405
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(lv2.kern_base + __mmap_self_0) = __mmap_self_patch_0; //0x0F
		*(uint8_t*)(lv2.kern_base + __mmap_self_1) = __mmap_self_patch_1; //0x84
		*(uint8_t*)(lv2.kern_base + __mmap_self_2) = __mmap_self_patch_0; //0x74
		*(uint8_t*)(lv2.kern_base + __mmap_self_3) = __mmap_self_patch_0; //0x0F
	}
#elif defined FW_455
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(lv2.kern_base + __mmap_self_0) = __mmap_self_patch_0; //0x0F
		*(uint8_t*)(lv2.kern_base + __mmap_self_1) = __mmap_self_patch_1; //0x84
		*(uint8_t*)(lv2.kern_base + __mmap_self_2) = __mmap_self_patch_0; //0x74
		*(uint8_t*)(lv2.kern_base + __mmap_self_3) = __mmap_self_patch_0; //0x0F
	}
#else
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(lv2.kern_base + __mmap_self_0) = __mmap_self_patch_0;
		*(uint8_t*)(lv2.kern_base + __mmap_self_1) = __mmap_self_patch_1;
		*(uint8_t*)(lv2.kern_base + __mmap_self_2) = __mmap_self_patch_2;

		*(uint8_t*)(lv2.kern_base + __mmap_self_3) = __mmap_self_patch_0;
		*(uint8_t*)(lv2.kern_base + __mmap_self_4) = __mmap_self_patch_1;
		*(uint8_t*)(lv2.kern_base + __mmap_self_5) = __mmap_self_patch_2;

		*(uint8_t*)(lv2.kern_base + __mmap_self_6) = __mmap_self_patch_3;
		*(uint8_t*)(lv2.kern_base + __mmap_self_7) = __mmap_self_patch_4;
		*(uint8_t*)(lv2.kern_base + __mmap_self_8) = __mmap_self_patch_5;
		*(uint8_t*)(lv2.kern_base + __mmap_self_9) = __mmap_self_patch_5;
		*(uint8_t*)(lv2.kern_base + __mmap_self_10) = __mmap_self_patch_5;
	}
#endif
	if (mmapself[0] != __mmap_self_patch_0) kconsole.WriteLine("Mapping all Selfs patched.\n");
	else kconsole.WriteLine("Mapping all Selfs allready patched.\n");	

	// Enable Write Protection.
	lv2.enable_wp();
	kconsole.WriteLine("Write Protection Enabled.\n");

	return 0;
}

<<<<<<< HEAD
// Disable SUDO.
void *disableSUDO(struct thread *td) {
	// Backup Orig Values first.
	uint16_t targetid[3];
	lv2.copyin(&target_id, &targetid, sizeof(targetid) / sizeof(*targetid));
	kconsole.WriteLine("Target ID loaded.\n");
=======
// Unjail 500
void *unjail500(struct thread *td) {
	struct ucred* cred;
	struct filedesc* fd;
>>>>>>> origin/master

	// Copy out to userland to not get lost.
	uint8_t mmapself[11];
	lv2.copyin(&mmap_self, &mmapself, sizeof(mmapself) / sizeof(*mmapself));
	kconsole.WriteLine("Mmap Self loaded.\n");

<<<<<<< HEAD
	// Disable Write Protection.
	lv2.disable_wp();
	kconsole.WriteLine("CPU Write Protection Disabled.\n");
=======
	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[0x1C0];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 = (void**)&kernel_ptr[0x10986A0];
	void** got_rootvnode = (void**)&kernel_ptr[0x22C19F0];

	cred->cr_uid = 0;
	cred->cr_ruid = 0;
	cred->cr_rgid = 0;
	cred->cr_groups[0] = 0;

	cred->cr_prison = *got_prison0;
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
	*sceProcCap = 0xffffffffffffffff; // Sce Process	*/
	return 0;
}

// Unjail 505
void *unjail505(struct thread *td) {
	struct ucred* cred;
	struct filedesc* fd;

	fd = td->td_proc->p_fd;
	cred = td->td_proc->p_ucred;

	void* kernel_base = &((uint8_t*)__readmsr(0xC0000082))[0X1C0];
	uint8_t* kernel_ptr = (uint8_t*)kernel_base;
	void** got_prison0 =   (void**)&kernel_ptr[0x10986a0];
	void** got_rootvnode = (void**)&kernel_ptr[0x22c1a70];
>>>>>>> origin/master

	// Restore mmap self conditions.
#if defined FW_405
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(lv2.kern_base + __mmap_self_0) = mmapself[0]; //0x0F
		*(uint8_t*)(lv2.kern_base + __mmap_self_1) = mmapself[1]; //0x84
		*(uint8_t*)(lv2.kern_base + __mmap_self_2) = mmapself[2]; //0x74
		*(uint8_t*)(lv2.kern_base + __mmap_self_3) = mmapself[3]; //0x0F
	}
#elif defined FW_455
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(lv2.kern_base + __mmap_self_0) = mmapself[0]; //0x0F
		*(uint8_t*)(lv2.kern_base + __mmap_self_1) = mmapself[1]; //0x84
		*(uint8_t*)(lv2.kern_base + __mmap_self_2) = mmapself[2]; //0x74
		*(uint8_t*)(lv2.kern_base + __mmap_self_3) = mmapself[3]; //0x0F
	}
#else
	if (mmapself[0] != __mmap_self_patch_0) {
		*(uint8_t*)(lv2.kern_base + __mmap_self_0) = mmapself[0];
		*(uint8_t*)(lv2.kern_base + __mmap_self_1) = mmapself[1];
		*(uint8_t*)(lv2.kern_base + __mmap_self_2) = mmapself[2];

		*(uint8_t*)(lv2.kern_base + __mmap_self_3) = mmapself[3];
		*(uint8_t*)(lv2.kern_base + __mmap_self_4) = mmapself[4];
		*(uint8_t*)(lv2.kern_base + __mmap_self_5) = mmapself[5];

		*(uint8_t*)(lv2.kern_base + __mmap_self_6) = mmapself[6];
		*(uint8_t*)(lv2.kern_base + __mmap_self_7) = mmapself[7];
		*(uint8_t*)(lv2.kern_base + __mmap_self_8) = mmapself[8];
		*(uint8_t*)(lv2.kern_base + __mmap_self_9) = mmapself[9];
		*(uint8_t*)(lv2.kern_base + __mmap_self_10) = mmapself[10];
	}
#endif
	if (mmapself[0] != __mmap_self_patch_0) kconsole.WriteLine("Mmap Self condition Restored.\n");
	else kconsole.WriteLine("No need to Restore Mmap Self condition.\n");
	

	// Restore Target ID.
#if defined FW_405
	if (targetid[0] != __devkit_id) {
		*(uint16_t *)(lv2.kern_base + __target_id_0) = targetid[0];
		*(uint16_t *)(lv2.kern_base + __target_id_1) = targetid[1];
		*(uint16_t *)(lv2.kern_base + __target_id_2) = targetid[2];
	}
#elif defined FW_455
	if (targetid[0] != __devkit_id) {
		*(uint16_t *)(lv2.kern_base + __target_id_0) = targetid[0];
		*(uint16_t *)(lv2.kern_base + __target_id_1) = targetid[1];
		*(uint16_t *)(lv2.kern_base + __target_id_2) = targetid[2];
}
#else
	if (targetid[0] != __devkit_id) {
		*(uint16_t *)(lv2.kern_base + __target_id_0) = targetid[0];
		*(uint16_t *)(lv2.kern_base + __target_id_1) = targetid[1];
	}
#endif
	if (targetid[0] != __devkit_id) kconsole.WriteLine("Target Id Restored.\n");
	else kconsole.WriteLine("No need to Restore Target ID.\n");
	

	// Enable Write Protection.
	lv2.enable_wp();
	kconsole.WriteLine("Write Protection Enabled.\n");

	return 0;
}

// Disable ASLR for Big Game Process.
void *disableProcessASLR(struct thread *td) {
	// Get Kernel Base.
	void* kern_base = &((uint8_t*)__readmsr(0xC0000082))[-__Xfast_syscall];
	
	// Disable Write Protection.
	lv2.disable_wp();

	// Disable process ASLR.
	*(uint16_t*)(kern_base + __process_aslr) = 0x9090;

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
