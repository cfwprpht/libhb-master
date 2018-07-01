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

#ifdef FW_355

#define __printf 0x1df550
#define __copyin 0x3b96e0
#define __copyout 0x3b9660
#define __copyinstr 0x3b9a50
#define __kmem_alloc_contig 0x337ea0
#define __kmem_free 0x33bca0
#define __pmap_extract 0x3afd70
#define __pmap_protect 0x3b1f50
#define __sched_pin 0x1ced60
#define __sched_unpin 0x1cedc0
#define __smp_rendezvous 0x1e7810
#define __smp_no_rendevous_barrier 0x1e75d0
#define __icc_query_nowait 0x3ed450
#define __kernel_map 0x196acc8
#define __sysent 0xeed880
#define __kernel_pmap_store 0x19bd628
#define __Starsha_UcodeInfo 0x1869fa0

#define __pml4pml4i 0x19bd618
#define __dmpml4i 0x19bd61c
#define __dmpdpi 0x19bd620

#elif defined FW_370

#define __printf 0x1df620
#define __copyin 0x3b97d0
#define __copyout 0x3b9750
#define __copyinstr 0x3b9b40
#define __kmem_alloc_contig 0x337f70
#define __kmem_free 0x33bd70
#define __pmap_extract 0x3afe60
#define __pmap_protect 0x3b2040
#define __sched_pin 0x1cee30
#define __sched_unpin 0x1cee90
#define __smp_rendezvous 0x1e78e0
#define __smp_no_rendevous_barrier 0x1e76a0
#define __icc_query_nowait 0x3ed7f0
#define __kernel_map 0x1976cc8
#define __sysent 0xef6d90
#define __kernel_pmap_store 0x19c9628
#define __Starsha_UcodeInfo 0
#define __gpu_devid_is_9924 0x443a20
#define __gc_get_fw_info 0x44b5a0

#define __pml4pml4i 0x19c9618
#define __dmpml4i 0x19c961c
#define __dmpdpi 0x19c9620

#elif defined FW_400 || FW_401

#define __printf 0x347450
#define __copyin 0x286cc0
#define __copyout 0x286c40
#define __copyinstr 0x287030
#define __kmem_alloc_contig 0x275da0
#define __kmem_free 0x369580
#define __pmap_extract 0x3eeed0
#define __pmap_protect 0x3f1120
#define __sched_pin 0x1d1120
#define __sched_unpin 0x1d1180
#define __smp_rendezvous 0x34a020
#define __smp_no_rendevous_barrier 0x349de0
#define __icc_query_nowait 0x46c5a0
#define __kernel_map 0x1fe71b8
#define __sysent 0xf17790
#define __kernel_pmap_store 0x200c310
#define __Starsha_UcodeInfo 0x18dafb0

#define __pml4pml4i 0x200c300
#define __dmpml4i 0x200c304
#define __dmpdpi 0x200c308

#elif defined FW_405
#define __Xfast_syscall				     		        0x30EB30
#define __aslr_initialize_process_status                0x212660
#define __aslr_gen_pseudo_random_number                 0x212750
#define __aslr_get_random_value                         0x212800
#define __bootparam_hwconfig_read                       0x2EA160
#define __bootparam_get_mac_address                     0x2EA1A0
#define __bootparam_get_config_board_id                 0x2EA1C0
#define __bootparam_get_ddr3_capacity                   0x2EA1F0
#define __bootparam_is_mode_manu                        0x2EA210
#define __bootparam_disable_console_output              0x2EA230
#define __bootparam_notify_main_on_standby_mode         0x2EA240
#define __bootparam_is_main_on_standby                  0x2EA250
#define __bootparam_get_eap_part_key                    0x6249D0
#define __eap_internal_key                              0x23DC000
#define __copyin					     			    0x286DF0
#define __copyout					     			    0x286D70
#define __copyinstr                                     0x287160
#define __copystr                                       0x2871F0
#define __printf			     					    0x347580
#define __device_printf                                 0x3FADB0
#define __vprintf                                       0x3475F0
#define __sprintf                                       0x347790
#define __snprintf                                      0x347860
#define __vsnprintf                                     0x347900
#define __vsnrprintf                                    0x347990
#define __kvprintf                                      0x347A00
#define __cnputc                                        0x14D20
#define __cnputs                                        0x15190
#define __strlen     								    0x1D3640
#define __strlcpy                                       0x1934E0
#define __strdup                                        0x21640
#define __strncpy                                       0x113C10
#define __strcmp                                        0x274970
#define __strncmp                                       0x251250
#define __strstr                                        0x32DAB0
#define __strcasecmp                                    0x44C1D0
#define __g_obi_create                                  0x469810
#define __g_new_geomf                                   0xEE1D0
#define __g_new_consumer                                0xEEC70
#define __g_attach                                      0xEF0E0
#define __g_trace                                       0x20980
#define __g_access                                      0xEE640
#define __g_wither_geom                                 0xEE4E0
#define __g_do_wither                                   0x14000
#define __g_orphan_provider                             0x134E0
#define __g_provider_by_name                            0xEEF40
#define __g_obi_read                                    0x469970
#define __g_io_request                                  0x52530
#define __g_alloc_bio                                   0x52130
#define __g_destroy_bio                                 0x52170
#define __g_post_notification_for_bio                   0x6B5880
#define __gbase_samu_initialize                         0x495330
#define __gbase_samu_finalize                           0x4955E0
#define __gbase_samu_map                                0x4957C0
#define __gbase_samu_unmap                              0x4957D0
#define __gbase_samu_read_register                      0x495800
#define __gbase_samu_write_register                     0x495820
#define __gbase_samu_ix_read_register                   0x495840
#define __gbase_samu_ix_write_register                  0x4958D0
#define __gbase_samu_pa_to_gpuva                        0x495960
#define __gbase_samu_cmd                                0x71CAF0
#define __gbase_samu_unk_func                           0x495990
#define __gbase_unmap                                   0x4AAE60
#define __gbase_free_vm_space                           0x4AA520
#define __cv_init                                       0x2DA3F0
#define __cv_destroy                                    0x2DA400
#define ___cv_wait                                      0x2DA410
#define ___cv_wait_unlock                               0x2DA5F0
#define ___cv_wait_sig                                  0x2DA7A0
#define ___cv_timedwait                                 0x2DA970
#define ___cv_timedwait_sig                             0x2DAB60
#define __cv_signal                                     0x2DAD50
#define __cv_signalto                                   0x2DADB0
#define __cv_broadcastpri                               0x2DAE10
#define __malloc     								    0x1D1700
#define __free	     								    0x1D18D0
#define __firmware_get                                  0x2E6280
#define __memcpy	     						        0x286CF0
#define __memset	     							    0x1ECB10
#define __memcmp	     						        0x29CD10
#define __memmove                                       0x339440
#define __kmem_alloc     						        0x369500
#define __kmem_free                                     0x3696b0
#define __kmem_alloc_contig                             0x275ed0
#define __pmap_extract                                  0x3ef000
#define __pmap_protect                                  0x3f1250
#define __pmap_kextract                                 0x3EF390
#define __sched_pin                                     0x1d1250
#define __sched_unpin                                   0x1d12B0
#define __sched_switch                                  0x1D0040
#define __sched_bind                                    0x1D1120
#define ___sleep                                        0x8A2E0
#define __smp_rendezvous                                0x34a150
#define __smp_no_rendevous_barrier                      0x349f10
#define __kthread_add					     		    0x1C9890
#define __sx_init_flags					     		    0x27AAF0
#define __sx_xlock						     		    0x27AC20
#define __sx_xunlock					     		    0x27ADB0
#define __sx_destroy                                    0x27AB60
#define __sx_xlock_hard                                 0x27AC20
#define ___sx_sunlock_hard                              0x27ACF0
#define ___sx_sunlock_hard_sub                          0x27B880
#define __lock_init                                     0x327730
#define __id_set                                        0x465360
#define __id_unlock                                     0x465430
#define __id_wlock                                      0x4659A0
#define __fpu_kern_enter		     				    0x39A120
#define __fpu_kern_leave		     				    0x39A220
#define __vm_map_lock_read		     				    0x43DF10
#define __vm_map_lookup_entry	     				    0x43E470
#define __vm_map_unlock_read	     				    0x43DF60
#define __vmspace_free			     			        0x43DB70
#define __vmspace_acquire_ref     				        0x43DD40
#define __bzero			     						    0x286C30
#define __pagezero 		     						    0x286C50
#define __bcmp			     						    0x286C80
#define __bcopy			     						    0x286CA0
#define __binuptime                                     0x259960
#define __getbinuptime                                  0x259D20
#define __casuword32	     				            0x286E70
#define __kick_proc0      					            0x2168E0
#define __wakeup 	     							    0x8A7C0
#define __mi_switch 						     	    0x8A840
#define __eventlog_write_crash			     		    0x259180
#define __metadbg_perform_dump_on_panic                 0x71BAE0
#define __savectx                                       0x90B40
#define __savectx2                                      0x90BD0
#define __sleepq_lock                                   0x15ECD0
#define __sleepq_lookup                                 0x15ED00
#define __sleepq_release                                0x15ED50
#define __sleepq_add                                    0x15ED80
#define __sleepq_set_timeout                            0x15EF20
#define __sleepq_set_runnable                           0x15EF80
#define __sleepq_sleepcnt                               0x15F0F0
#define __sleepq_wait                                   0x15F130
#define __sleepq_wait_sig                               0x15F2A0
#define __sleepq_timedwait                              0x15F5C0
#define __sleepq_timedwait_sig                          0x15F690
#define __sleepq_signal                                 0x15F820
#define __sleepq_signalto                               0x15FAB0
#define __sleepq_broadcast                              0x15FB70
#define __sleepq_remove                                 0x15FC30
#define __sleepq_abort                                  0x15FD50
#define __hexdump                                       0x348DF0
#define __kern_reboot                                   0x25FC10
#define __kern_execve                                   0x212B80
#define __kern_jail_set                                 0xAEB60
#define __kern_jail_get                                 0xB2C70
#define __kernacc                                       0x215A30
#define __useracc                                       0x215AD0
#define __sys_yield                                     0x8ABD0
#define __sys_execve                                    0x2128C0
#define __exec_copyin_args                              0x212930
#define __sysctl_lock                                   0x1CD100
#define __sysctl_unlock                                 0x1CD120
#define __sysctl_register_oid                           0x1CD140
#define __sysctl_unregister_oid                         0x1CD240
#define __sysctl_ctx_init                               0x1CD2B0
#define __sysctl_ctx_free                               0x1CD2D0
#define __sysctl_ctx_entry_add                          0x1CD610
#define __sysctl_ctx_entry_find                         0x1CD670
#define __sysctl_ctx_entry_del                          0x1CD6B0
#define __sysctl_remove_oid                             0x1CD7F0
#define __sysctl_remove_name                            0x1CD860
#define __sysctl_add_oid                                0x1CD900
#define __sysctl_rename_oid                             0x1CDBB0
#define __sysctl_move_oid                               0x1CDC60
#define __sysctl_handle_int                             0x1CDDD0
#define __sysctl_msec_to_ticks                          0x1CDE50
#define __sysctl_handle_long                            0x1CDF30
#define __sysctl_handle_64                              0x1CDFE0
#define __sysctl_handle_string                          0x1CE060
#define __sysctl_handle_opaque                          0x1CE170
#define __sysctl_wire_old_buffer                        0x1CE960
#define __sysctl_find_oid                               0x1CEA40
#define __sys___sysctl                                  0x1CEB20
#define __userland_sysctl                               0x1CEC60
#define __sbuf_new_for_sysctl                           0x1CEF30
#define __kernel_sysctl                                 0x1CE260
#define __kernel_sysctlbyname                           0x1CE850
#define __ktrsysctl                                     0x2CED00
#define __kern_ptrace                                   0xAC230
#define __kern_ptrace_check_1                           0xAC2F1
#define __kern_ptrace_check_2                           0xAC6A2
#define __kernel_pmap_store                             0x200c310
#define ____stack_chk_fail                              0x68F9B0
#define ___thread_lock_flags                            0x36E940
#define ___thread_lock_flags_check_result               0x36E8F0
#define __vm_phys_paddr_to_vm_page                      0x3705C0
#define __critical_enter                                0x1E2660
#define __critical_exit                                 0x1E2670
#define __mutex_init                                    0x36EC70
#define __msgbuf_addchar                                0x36E640
#define __msgbufinit                                    0x348D90
#define __msgbuf_copy                                   0x73DA0
#define __msgbuf_getchar                                0x73AE0
#define __msgbuf_reinit                                 0x736D0
#define __msgbuf_addchar                                0x36E640
#define __msgbuf_addstr                                 0x738D0
#define __msgbuf_addstr_sub                             0x36E800
#define __sbuf_new                                      0x17CBA0
#define __sbuf_vprintf                                  0x17D2A0
#define __sbuf_trim                                     0x17D4C0
#define __sbuf_finish                                   0x17D530
#define __sbuf_len                                      0x17D5F0
#define __sbuf_data                                     0x17D5E0
#define __sbuf_delete                                   0x17CDF0
#define __log                                           0x347360
#define __priv_check                                    0x1E4BF0
#define __priv_check_cred                               0x1E4A40
#define __prison_priv_check                             0xB4A90
#define __spinlock_enter                                0x38AC00
#define __spinlock_exit                                 0x38AC40
#define __icc_query                                     0x46BD80
#define __icc_query_nowait                              0x46c6d0
#define __icc_enable_crashmode                          0x46CBB0
#define __icc_nvs_write                                 0x3C83A0
#define __icc_nvs_read                                  0x3C8490
#define __icc_nvs_drvinit                               0x3C85A0
#define __icc_nvs_init                                  0x3C85E0
#define ___is_vtrm_access_ok                            0x61AF40
#define __icc_snvs_read_sector                          0x1A21B0
#define __icc_snvs_write_sector                         0x1A2270
#define __icc_snvs_get_write_count                      0x1A2330
#define __icc_init_snvs_write_lock                      0x1A2400
#define __icc_lock_snvs_write                           0x1A2410
#define __icc_unlock_snvs_write                         0x1A2430
#define __icc_button_init                               0x323020
#define __icc_button_set_state                          0x3231D0
#define __icc_button_get_state                          0x3232D0
#define __icc_register_notification_handler             0x46CAB0
#define __icc_sc_config_drvinit                         0x33216E
#define __icc_thermal_get_thermal_state                 0x1DA750
#define __icc_indicator_set_buzzer                      0x3C22A0
#define __icc_indicator_set_led                         0x3C2920
#define __icc_indicator_get_led                         0x3C2A90
#define __icc_indicator_set_dynamic_led                 0x3C2C10
#define __icc_indicator_get_dynamic_led                 0x3C2D90
#define __icc_indicator_set_dynamic_led_boot            0x3C2350
#define __icc_indicator_set_dynamic_led_off             0x3C2420
#define __icc_indicator_set_dynamic_led_shutdown        0x3C3020
#define __icc_indicator_set_thermalalert_led_on         0x3C24B0
#define __icc_indicator_set_thermalalert_led_off        0x3C2540
#define __icc_indicator_set_dynamic_led_standby         0x3C2600
#define __icc_indicator_set_dynamic_led_standby_boot    0x3C3110
#define __icc_indicator_drvinit                         0x3C2690
#define __icc_indicator_check_thermal_on_boot           0x3C2F70
#define __icc_thermal_alert_occurred_when_boot          0x1DA290
#define __icc_thermal_alert_occurred_when_boot_sub      0x1DA2B0
#define __icc_thermal_alert_status_off                  0x1DA380
#define __icc_thermal_notify_handler                    0x1DA480
#define __icc_thermal                                   0x1DA540
#define __icc_thermal_enable_disable_notify             0x1DA690
#define __icc_thermal_get_thermal_state                 0x1DA750
#define __icc_thermal_stop_td                           0x1DA7D0
#define __icc_thermal_resume_td                         0x1DA900
#define __icc_thermal_set_notify                        0x1DA3D0
#define __icc_thermal_drvinit                           0x1DA790
#define __icc_sc_fw_update_handler                      0x339890
#define __icc_sc_fw_update                              0x3398F0
#define __get_icc_max                                   0x21E010
#define __icc_wdt_start                                 0x71680
#define __icc_wdt_stop                                  0x71710
#define __devstat_new_entry                             0x34C6B0
#define __devstat_start_transaction                     0x34CB40
#define __make_dev                                      0x1F51E0
#define __make_dev_credf                                0x1F5610
#define __destroy_dev                                   0x1F5BF0
#define __panic0                                        0x101E40
#define __panic                                         0x2603B0
#define __kick_proc0                                    0x2168E0
#define __ssb_rtc_init_exclock                          0xE5C10
#define __ktrcsw                                        0x2CF000
#define ___mtx_lock_sleep                               0x36E260
#define ___mtx_unlock_sleep                             0x36E510
#define __mtx_destroy                                   0x36ECE0
#define ___sleep                                        0x8A2E0
#define __uma_zfree_arg                                 0xB190
#define __uma_large_free                                0xC5A0
#define __uma_large_free_error                          0xC600
#define __uma_zalloc_arg                                0xA950
#define __lgdt                                          0x287220
#define __longjmp                                       0x287270
#define __pfs_create_dir                                0x1D97C0
#define __pfs_create_file                               0x1D9A50
#define __pfs_create_link                               0x1D9B60
#define __pfs_find_node                                 0x1D9D00
#define __pfs_fileno_alloc                              0x548E0
#define __dump_pfs                                      0x65FFC2
#define __dump_sign                                     0x660120
#define __index                                         0x8E100
#define __load_prx                                      0xDF030
#define __preload_prx_modules                           0xDE3C0
#define __unref_dag                                     0xDFE30
#define __alloc_unr                                     0x41FE0
#define __alloc_unrl                                    0x41A00
#define __clean_unrhdrl                                 0x41550
#define ___sceSblKeyMgrSmStart                          0x5FFD70
#define ___sceSblAuthMgrSmStart                         0x6153F0
#define __sceSblPfsKeymgrGenEKpfsForGDGPAC     		    0x5FFE80
#define __sceSblPfsSetKey			     			    0x600640
#define __sceSblPfsClearKey		     				    0x6007C0
#define __sceSblKeymgrClearKey                          0x5FEE10
#define __sceSblACMgrIsAllowedSystemLevelDebugging      0x360570
#define __sceSblACMgrIsSystemUcred                      0x35FE10
#define __sceSblACMgrIsSyscoreProcess                   0x35FF80
#define __sceSblACMgrHasUseVideoServiceCapability       0x360380
#define __sceSblACMgrIsJitApplicationProcess            0x360260
#define __sceSblRcMgrIsAllowDataExecutionInFselfProcess 0x1E3C00
#define __sceSblRcMgrIsAllowSLDebugger                  0x1E31F0
#define __sceSblAuthMgrIsLoadable                       0x611820
#define __sceSblAuthMgrAuthHeader                       0x611C10
#define __sceSblAuthMgrFinalize                         0x612330
#define __sceSblAuthMgrLoadBlock                        0x612490
#define ___sceSblAuthMgrGetElfSegmentInformation        0x6136F0
#define ___sceSblAuthMgrLoadSelfBlock                   0x614BB0
#define ___sceSblAuthMgrGetSelfSegmentInformation       0x615110
#define ___sceSblAuthMgrCheckSelfHeader                 0x615270
#define ___sceSblAuthMgrGetSelfInfo                     0x615360
#define ___sceSblAuthMgrSmDriveClearKey                 0x6154E0
#define ___sceSblAuthMgrSmDriveClearSessionKey          0x6155C0
#define ___sceSblAuthMgrSmDriveData                     0x6156A0
#define ___sceSblAuthMgrSmVerifyActCodeCommon           0x6158E0
#define ___sceSblAuthMgrSmGenActRequest                 0x615EF0
#define ___sceSblAuthMgrSmGenActHeader                  0x615AC0
#define ___sceSblAuthMgrSmGenPassCodeData               0x615CA0
#define ___sceSblAuthMgrSmCheckPassCodeData             0x615DC0
#define __sceSblAuthMgrSmSpawn                          0x606E00
#define __sceSblServiceSpawn                            0x607140
#define ___sceSblAuthMgrSmVerifyPupExpiration           0x606F40
#define __sceSblAuthMgrIsLoadable2	     			    0x614A80
#define __sceSblAuthMgrVerifyHeader	     		        0x614AE0
#define __sceSblAuthMgrGetSelfInfo     			 	    0x615360
#define __sceSblAuthMgrSmStart     				  	    0x6153F0
#define __sceSblKeymgrSmCallfunc	     			    0x5FF500
#define __sceSblServiceMailbox	     				    0x606F40
#define __sceSblServiceCrypt     					    0x5F7820
#define __sceSblServiceCryptAsync				        0x5F79B0
#define __sceSblSsGenerateSealedKey                     0x5FB4D0
#define __getSealedKeySecret                            0x5FB630
#define __sceSblSsDecryptSealedKey                      0x5FB800
#define __sceSblSsMemcmpConsttime                       0x600EB0
#define __sceSblSsMemset                                0x600F10
#define __sceSblVtrmBackup 				                0x61AD9E
#define __sceSblVtrmRestore 				            0x61ADE0
#define __SceDaAtcev                                    0x37BDC0
#define __SceHidAuth                                    0x5D3330
#define __sceSblACMgrIsSystemUcred                      0x35FE10
#define __sceSblACMgrIsNongameUcred                     0x35FE30
#define __sceSblACMgrIsDiagProcess                      0x35FE40
#define __sceSblACMgrIsCoredumpProcess                  0x35FEA0
#define __sceSblACMgrIsDecidProcess                     0x35FF00
#define __sceSblACMgrIsMinisyscoreProcess               0x35FF50
#define __sceSblACMgrIsSyscoreProcess                   0x35FF80
#define __sceSblACMgrIsShelluiProcess                   0x35FFA0
#define __sceSblACMgrIsShellcoreProcess                 0x35FFC0
#define __sceSblACMgrIsVideoplayerProcess               0x360040
#define __sceSblACMgrIsDiskplayeruiProcess              0x360060
#define __sceSblACMgrIsBdjavaProcess                    0x360080
#define __sceSblACMgrIsWebcoreProcess                   0x3600A0
#define __sceSblACMgrIsVdecProxyProcess                 0x360170
#define __sceSblACMgrIsJitCompilerProcess               0x360230
#define __sceSblACMgrIsJitApplicationProcess            0x360260
#define __sceSblACMgrIsAslrSeed2Process                 0x3602B0
#define __sceSblACMgrHasSceProgramAttribute             0x3602D0
#define __sceSblACMgrHasDrmProcessAttribute             0x3602E0
#define __sceSblACMgrHasUseVtrmCapability               0x3602F0
#define __sceSblACMgrIsDiskplayerdiagProcess            0x360320
#define __sceSblACMgrHasUseHp3dPipeCapability           0x360350
#define __sceSblACMgrHasUseVideoServiceCapability       0x360380
#define __sceSblACMgrHasSceProcessCapability            0x3603A0
#define __sceSblACMgrIsAllowedToUsePupUpdate0           0x3603B0
#define __sceSblACMgrIsVtrmadminProcess                 0x360440
#define __sceSblACMgrIsSoftwagnerProcess                0x360460
#define __sceSblACMgrIsDebuggableProcess                0x360480
#define __sceSblDiagOSUpdaterProcess                    0x360520
#define __sceSblACMgrIsAllowedSystemLevelDebugging      0x360570
#define __sceSblACMgrHasMmapSelfCapability              0x3605F0
#define __sceSblACMgrIsAllowedToMmapSelf                0x360600
#define __sceSblACMgrGetPathId                          0x360620
#define __sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall 0x360830
#define __sceSblACMgrGetDeviceAccessType                0x360930
#define __sceSblACMgrIsAllowedCoredump                  0x360590
#define __sceSblACMgrIsAllowedDrmProcessDebugging       0x3604E0
#define __sceSblACMgrIsFirstImageWriterProcess          0x360130
#define __sceSblACMgrIsMountfusefsProcess               0x35FEE0
#define __sceSblRcMgrIsAllowPsmDebug                    0x1E2FC0
#define __sceSblRcMgrIsAllowRegistryAccess              0x1E3030
#define __sceSblRcMgrIsAllowRegistryAccessMini          0x1E30A0
#define __sceSblRcMgrIsAllowInitVtrm                    0x1E3180
#define __sceSblRcMgrIsAllowSLDebugger                  0x1E31F0
#define __sceSblRcMgrIsAllowAdClock                     0x1E3260
#define __sceSblRcMgrIsStoreMode                        0x1E3310
#define __sceSblRcMgrIsAllowULDebugger                  0x1E3480
#define __sceSblRcMgrIsAllowCreateDebuggingDeviceFile   0x1E34F0
#define __sceSblRcMgrIsSoftwagnerQafForAcmgr            0x1E3560
#define __sceSblRcMgrIsAllowDisablingAslr               0x1E35D0
#define __sceSblRcMgrIsAllowDebugSoftWagnerClock        0x1E3640
#define __sceSblRcMgrIsAllowDataExecutionInFselfProcess 0x1E3C00
#define __sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp 0x1E3C70
#define __sceSblAIMgrIsDevKit                           0x1A7810
#define __sceSblAIMgrIsTool                             0x1A7880
#define __sceSblAIMgrIsTestKit                          0x1A7960
#define __sceSblAIMgrIsDex                              0x1A79D0
#define __sceSblAIMgrIsCex                              0x1A7A40
#define __sceSblAIMgrIsDiag                             0x1A7C60
#define __sceSblAIMgrGetProductCode                     0x1A7D90
#define __sceRegMgrSetInt                               0x4CEAB0
#define __regMgrComApiPrologue                          0x4CEBA0
#define __regMgrComCheckParam                           0x4CEC60
#define __regMgrComSetReg                               0x4CF0E0
#define ___regMgrComSetErrInfo                          0x4CF960
#define __regMgrComApiEpilogue                          0x4CF9A0
#define __sceRegMgrGetInt                               0x4CF9C0
#define __regMgrComCheckAddr                            0x4CFAD0
#define __regMgrComGetReg                               0x4CFB10
#define __sceKernelCheckDipsw                           0x626640
#define __sceKernelIsDevelopmentMode                    0x626CC0
#define __sceKernelIsAssistMode                         0x626CD0
#define __proc_sstep                                    0xABB90
#define __proc_rwmem                                    0xABBB0
#define __proc_set_dr                                   0xABE30
#define __proc_get_dr                                   0xABEC0
#define __proc_init_dr                                  0xABF80
#define __process_aslr                                  0x2862D6
#define __devkit_id                                     0x8101
#define __target_id_0                                   0x1FE59E4
#define __target_id_1                                   0X1FE5A2C
#define __target_id_2                                   0x200151C
#define __mmap_self_0                                   0x31EE40
#define __mmap_self_1                                   0x31EE41
#define __mmap_self_2                                   0x31EF98
#define __mmap_self_3                                   0x31EF99
#define __mmap_self_patch_0                             0x90
#define __mmap_self_patch_1                             0xE9
#define __preLoad                                       0x607460
#define __gen_mem_keys                                  0x6075C0
#define __fork1                                         0x17BF0
#define __kproc_create                                  0x1C92F0
#define __mdbg_kproc_hook_create                        0x102C00
#define __metadbg_log_add                               0x101EC0
#define __cpu_set_fork_handler                          0x3427F0
#define __sched_prio                                    0x1CFEF0
#define __turnstile_adjust                              0x140940
#define __sched_user_prio                               0x1D0450
#define __sched_add                                     0x1D0240
#define __bioq_init                                     0x12D860
#define __disk_alloc                                    0x126340
#define __disk_create                                   0x126360
#define __eventhandler_find_list                        0x459710
#define __eventhandler_prune_list                       0x4597C0
#define __eventhandler_register                         0x4591B0
#define __eventhandler_suspend                          0x4594A0
#define __eventhandler_deregister                       0x459520
#define __sce_cam_error_log_init                        0xF57A0
#define __sce_cam_error_log_read                        0xF5850
#define __sce_cam_error_log_write                       0xF5A20
#define __sce_cam_error_put                             0xF5A50
#define __vslock                                        0x215B70
#define __vsunlock                                      0x215CA0
#define __SceSflash           		     		        0x4B090
#define __securelevel_gt                                0x87270
#define __securelevel_ge                                0x87290
#define __sflash_pwrite_for_vtrm 	     			    0x4B380
#define __sflash_pread_for_vtrm  		     		    0x4B3F0
#define __sflash_erase_for_vtrm                         0x4B520
#define __sflash_pread_for_vtrm2                        0x4B470
#define __taskqueue_enqueue                             0x14DCE0
#define __VtrmUtilLockInitialize 	     			    0x6182E0
#define __VtrmUtilLockFinialize 	     			    0x618300
#define __vtrm_pread				                    0x6181E0
#define __vtrm_pwrite 				                    0x618209
#define __vtrm_erase_sector                             0x6181C0
#define __vtrm_pwrite2                                  0x618240
#define __genNewActDat			     				    0x61E9F0
#define __AesCbcCfb128Encrypt	     		 		    0x19D650
#define __AesCbcCfb128Decrypt	     		 	 	    0x19D880
#define __AesEncKeySet 		     			            0x463950
#define __AesDecKeySet 		     			            0x464050
#define __AesEncrypt  		     			            0x4640A0
#define __AesDecrypt  		     			            0x4640E0
#define __Sha256HmacFinal	     			            0x3367B0
#define __Sha256Hash     	     			            0x3368E0
#define __Sha256Hmac      	     			            0x3369B0
#define __Sha256OneBlk   	     			            0x333810
#define __Sha256FinalCore 	     			            0x336300
#define __Sha256Init     	     			            0x3364C0
#define __Sha256Transform	     			            0x336510
#define __Sha256Final          				            0x336670
#define __Sha256HmacInit       				            0x3366C0
#define __sceSblRngGetRandomNumber				        0x61B690
#define __Fips186Prng 				                    0x183F40
#define __RsaesPkcs1v15Enc2048		     			    0x44C1C0
#define __RsaesPkcs1v15Dec2048CRT     				    0x1E82A0
#define __Starsha_UcodeInfo                             0
#define __gpu_devid_is_9924                             0x4b9030
#define __gc_get_fw_info                                0x4a19a0
#define __pml4pml4i                                     0x200c300
#define __dmpml4i                                       0x200c304
#define __dmpdpi                                        0x200c308
#define __sysent		     				            0xF17790
#define __allproc	     					            0x1FE3498
#define __prison0						                0xF26010
#define __kick_proc0                                    0x2168E0
#define __kernel_map					                0x1FE71B8
#define __rootvnode					     	            0x206D250
#define __M_TEMP				     		            0x134B730
#define __fpu_kern_ctx					                0x235C6C0
#define __mini_syscore_self_binary 		                0x136B3E8
#define __sbl_driver_mapped_pages  		                0x234ED68
#define __sbl_keymgr_key_rbtree	                        0x2370058
#define __gs_is_update_bio_ioprio                       0x6A8090

/* Need fix */
#define __pfs_savedata                                  0x0
#define __pfs_mapped_read                               0x0
#define __pfs_check_addr                                0x0
#define __pfs_checkpath                                 0x0
#define __pfs_fileno_uninit                             0x0
#define __pfs_fileno_free                               0x0
#define __pfs_fileno_init                               0x0
#define __pfs_destroy                                   0x0
#define __pfs_vncache_alloc                             0x0
#define __pfs_vncache_unload                            0x0
#define __pfs_vncache_load                              0x0
#define __pfs_purge                                     0x0
#define __pfs_uninit                                    0x0
#define __pfs_init                                      0x0
#define __pfs_mount                                     0x0
#define __pfs_cmount                                    0x0
#define __pfs_unmount                                   0x0
#define __pfs_root                                      0x0
#define __pfs_statfs                                    0x0
#define __p_candebug                                    0x0
#define __procfs_candebug                               0x0
#define __procfs_init                                   0x0
#define __procfs_doprocfile                             0x0
#define __procfs_docurproc                              0x0
#define __procfs_attr                                   0x0
#define __procfs_notsystem                              0x0
#define __procfs_doosrel                                0x0
#define __procfs_doprocstatus                           0x0
#define __procfs_doproccmdline                          0x0
#define __procfs_doprocctl                              0x0
#define __procfs_doprocdbregs                           0x0
#define __procfs_doproctype                             0x0
#define __procfs_doprocfpregs                           0x0
#define __procfs_doprocmap                              0x0
#define __procfs_doprocmem                              0x0
#define __procfs_ioctl                                  0x0
#define __procfs_close                                  0x0
#define __procfs_doprocnote                             0x0
#define __procfs_doprocregs                             0x0
#define __procfs_doprocrlimit                           0x0
#define __vfs_getnewfsid                                0x0
#define __vfs_mountedfrom                               0x0
#define __vfs_donmount                                  0x0
#define __vfs_buildopts                                 0x0
#define __g_cancel_event                                0x0
#define __g_destroy_geom                                0x0
#define __g_bdemu_create                                0x0
#define __bdemu_speed_emu                               0x0
#define __bd_sched_post_done                            0x0
#define __bd_sched_update_bitmap_file                   0x0
#define __bd_sched_copier_handle_suspend_event          0x0
#define __bd_sched_copier_handle_resume_event           0x0
#define __bd_sched_writer_handle_suspend_event          0x0
#define __bd_sched_writer_handle_resume_event           0x0
#define __bd_sched_shutdown_copier                      0x0
#define __bd_sched_shutdown_w_hdd                       0x0
#define __bd_sched_shutdown_r_bd_internal               0x0
#define __bd_sched_reader_handle_suspend_event          0x0
#define __bd_sched_reader_handle_resume_event           0x0
#define __bd_sched_bd_reader_ripping                    0x0
#define __bd_sched_bd_reader_bg_copy                    0x0
#define __bd_sched_lock_bm_file                         0x0
#define __bd_sched_update_bitmap_cache2                 0x0
#define __bd_sched_is_available_on_hdd2                 0x0
#define __bd_sched_unlock_bm_file                       0x0
#define __get_img_offset                                0x0
#define __bd_sched_read_bd_dev                          0x0
#define __get_pkg_offset                                0x0
#define __bd_sched_get_bdsio_in_cache                   0x0
#define __bd_sched_get_bdsio_in_writer                  0x0
#define __bd_sched_private_done                         0x0
#define __bd_sched_adjust_bio_completed                 0x0
#define __sceSblDriverMapPages                          0x0
#define __sceSblDriverUnmapPages                        0x0
#define __kthread_suspend_check                         0x0
#define __kernel_mount                                  0x0
#define __kproc_kthread_add                             0x0
#define __vm_page_free_toq                              0x0
#define __vm_page_unhold_pages                          0x0
#define __vflush                                        0x0
#define __gbase_samu_mailbox_writer                     0x0
#define __gbase_samu_mailbox_reader                     0x0
#define __gbase_samu_handler                            0x0
#define __delete_unrhdr                                 0x0
#define __sceSblSrtcSetTime                             0x0

#elif defined FW_455
#define __Xfast_syscall				     		        0x3095D0
#define __aslr_initialize_process_status                0x2E67A0
#define __aslr_get_random_value                         0x2E6940
#define __aslr_gen_pseudo_random_number                 0x2E6890
#define __bootparam_hwconfig_read                       0x430490
#define __bootparam_get_mac_address                     0x4304D0
#define __bootparam_get_config_board_id                 0x4304F0
#define __bootparam_get_ddr3_capacity                   0x430520
#define __bootparam_is_mode_manu                        0x430540
#define __bootparam_disable_console_output              0x430560
#define __bootparam_notify_main_on_standby_mode         0x430570
#define __bootparam_is_main_on_standby                  0x430590
#define __bootparam_get_eap_part_key                    0x6337D0
#define __eap_internal_key                              0x258CCD0
#define __printf                                        0x17F30
#define __copyin                                        0x14A890
#define __copyout                                       0x14A7B0
#define __copyinstr                                     0x14AD00
#define __kmem_alloc_contig                             0x250320
#define __kmem_free                                     0x16EEA0
#define __pmap_kextract                                 0x41DF40
#define __pmap_extract                                  0x41DBC0
#define __pmap_protect                                  0x420310
#define __process_aslr                                  0x1BA559
#define __devkit_id                                     0x8101
#define __target_id_0                                   0x1AF82C4
#define __target_id_1                                   0X1AF85A4
#define __target_id_2                                   0x1B6D08C
#define __mmap_self_0                                   0x143BF2
#define __mmap_self_1                                   0x143BF3
#define __mmap_self_2                                   0x143E0E
#define __mmap_self_3                                   0x143E0F
#define __mmap_self_patch_0                             0x90
#define __mmap_self_patch_1                             0xE9
#define __sched_pin                                     0x73770
#define __sched_unpinv                                  0x73780
#define __smp_rendezvous                                0xB2BB0
#define __smp_no_rendevous_barrier                      0xB2970
#define __kernel_map                                    0x1B31218
#define __sysent                                        0x102B690
#define __kernel_pmap_store                             0x21BCC38
#define __Starsha_UcodeInfo                             0
#define __gpu_devid_is_9924                             0x496720
#define __gc_get_fw_info                                0x4A12D0
#define __disable_console_output                        0x1997BC8
#define __prison0                                       0x10399B0
#define __rootvnode                                     0x21AFA30
#define __pml4pml4i                                     0x21BCC28
#define __dmpml4i                                       0x21BCC2C
#define __dmpdpi                                        0x21BCC30
#define __preload_prx_modules                           0x645D0
#define __cv_init                                       0x29340
#define __cv_destroy                                    0x29350
#define ___cv_wait                                      0x29360
#define ___cv_wait_unlock                               0x29550
#define ___cv_wait_sig                                  0x29710
#define ___cv_timedwait                                 0x298F0
#define ___cv_timedwait_sig                             0x29AF0
#define __cv_signal                                     0x29CF0
#define __cv_signalto                                   0x29D70
#define __cv_broadcastpri                               0x29E00
#define __sleepq_lock                                   0x1492C0
#define __sleepq_lookup                                 0x1492F0
#define __sleepq_release                                0x149340
#define __sleepq_add                                    0x149370
#define __sleepq_set_timeout                            0x149510
#define __sleepq_set_runnable                           0x149570
#define __sleepq_sleepcnt                               0x1496C0
#define __sleepq_wait                                   0x149700
#define __sleepq_wait_sig                               0x149870
#define __sleepq_timedwait                              0x149B80
#define __sleepq_timedwait_sig                          0x149C50
#define __sleepq_signal                                 0x149DE0
#define __sleepq_signalto                               0x14A080
#define __sleepq_broadcast                              0x14A140
#define __sleepq_remove                                 0x14A200
#define __sleepq_abort                                  0x14A320
#define __sysctl_lock                                   0x52750
#define __sysctl_unlock                                 0x52770
#define __sysctl_register_oid                           0x52790
#define __sysctl_unregister_oid                         0x52890
#define __sysctl_ctx_init                               0x52900
#define __sysctl_ctx_free                               0x52920
#define __sysctl_ctx_entry_add                          0x52C60
#define __sysctl_ctx_entry_find                         0x52CC0
#define __sysctl_ctx_entry_del                          0x52D00
#define __sysctl_remove_oid                             0x52E40
#define __sysctl_remove_name                            0x52EB0
#define __sysctl_add_oid                                0x52F50
#define __sysctl_rename_oid                             0x53200
#define __sysctl_move_oid                               0x532B0
#define __sysctl_handle_int                             0x53420
#define __sysctl_msec_to_ticks                          0x534A0
#define __sysctl_handle_long                            0x53580
#define __sysctl_handle_64                              0x53630
#define __sysctl_handle_string                          0x536B0
#define __sysctl_handle_opaque                          0x537C0
#define __sysctl_wire_old_buffer                        0x53EA0
#define __sysctl_find_oid                               0x53F80
#define __sys___sysctl                                  0x54060
#define __kernel_sysctl                                 0x538B0
#define __kernel_sysctlbyname                           0x53D90
#define __ktrsysctl                                     0x1F5D40
#define __userland_sysctl                               0x54190
#define __sbuf_new_for_sysctl                           0x54460
#define __gbase_samu_initialize                         0x498A50
#define __gbase_samu_finalize                           0x498D00
#define __gbase_samu_map                                0x498EE0
#define __gbase_samu_unmap                              0x498EF0
#define __gbase_samu_read_register                      0x498F20
#define __gbase_samu_write_register                     0x498F40
#define __gbase_samu_ix_read_register                   0x498F60
#define __gbase_samu_ix_write_register                  0x498FF0
#define __gbase_samu_pa_to_gpuva                        0x499080
#define __gbase_samu_unk_func                           0x4990B0
#define __gbase_samu_mailbox_writer                     0x49E710
#define __gbase_samu_mailbox_reader                     0x49E7E0
#define __gbase_samu_cmd                                0x736EA0
#define __gbase_samu_handler                            0x600D40
#define __gbase_unmap                                   0x4B78B0
#define __gbase_free_vm_space                           0x4B6F60
#define __vprintf                                       0x17FA0
#define __sprintf                                       0x18160
#define __snprintf                                      0x18230
#define __vsnprintf                                     0x182D0
#define __kvprintf_putc                                 0x18340
#define __vsnrprintf                                    0x18360
#define __kvprintf                                      0x183D0
#define __strlcpy                                       0x24F5E0
#define __strcmp                                        0x1DAEF0
#define __strncpy                                       0x2DEA20
#define __strlen                                        0x3514F0
#define __strdup                                        0x452A30
#define __strstr                                        0x261710
#define __strncmp                                       0x3DB670
#define __strcasecmp                                    0x28EA80
#define __malloc                                        0x3F7750
#define __bcopy                                         0x14A630
#define __sbuf_new                                      0x1BE610
#define __sbuf_delete                                   0x1BE860
#define __sbuf_vprintf                                  0x1BED10
#define __sbuf_trim                                     0x1BEF30
#define __sbuf_finish                                   0x1BEFA0
#define __sbuf_data                                     0x1BF050
#define __sbuf_len                                      0x1BF060
#define __msgbuf_addchar                                0x30DA70
#define __msgbuf_addstr_sub                             0x30DC30
#define __alloc_unr                                     0x837B0
#define __alloc_unrl                                    0x831E0
#define __clean_unrhdrl                                 0x82D30
#define __delete_unrhdr                                 0x83130
#define __genNewActDat                                  0x62F720
#define __memset                                        0xFAA20
#define __memcpy                                        0x14A6B0
#define __memcmp                                        0x62D810
#define __free                                          0x3F7930
#define __fork1                                         0x73AE0
#define __wakeup                                        0x261140
#define ___sleep                                        0x260C80
#define __bzero                                         0x14A570
#define ___mtx_lock_sleep                               0x30D6A0
#define ___mtx_unlock_sleep                             0x30D940
#define __mtx_destroy                                   0x30E130
#define __lock_destroy                                  0x37F140
#define __spinlock_exit                                 0x2828D0
#define __sx_xlock_hard                                 0x38FA30
#define __sx_destroy                                    0x38F970
#define __sx_xunlock                                    0x736900
#define __sx_init_flags                                 0x38F900
#define __lock_init                                     0x37F070
#define __get_td_locks                                  0x736F50
#define ____stack_chk_fail                              0x6A3390
#define __critical_enter                                0x23D560
#define __critical_exit                                 0x23D570
#define __securelevel_gt                                0x115B60
#define __securelevel_ge                                0x115B70
#define ___thread_lock_flags_check_result               0x30DD20
#define ___thread_lock_flags                            0x30DD70
#define __sceSblACMgrIsSystemUcred                      0x169DD0
#define __sceSblACMgrIsNongameUcred                     0x169DF0
#define __sceSblACMgrIsDiagProcess                      0x169E00
#define __sceSblACMgrIsCoredumpProcess                  0x169E60
#define __sceSblACMgrIsDecidProcess                     0x169EC0
#define __sceSblACMgrIsMinisyscoreProcess               0x169F10
#define __sceSblACMgrIsSyscoreProcess                   0x169F40
#define __sceSblACMgrIsShelluiProcess                   0x169F60
#define __sceSblACMgrIsShellcoreProcess                 0x169F80
#define __sceSblACMgrIsVideoplayerProcess               0x16A000
#define __sceSblACMgrIsDiskplayeruiProcess              0x16A020
#define __sceSblACMgrIsBdjavaProcess                    0x16A040
#define __sceSblACMgrIsWebcoreProcess                   0x16A060
#define __sceSblACMgrIsVdecProxyProcess                 0x16A130
#define __sceSblACMgrIsJitCompilerProcess               0x16A1F0
#define __sceSblACMgrIsJitApplicationProcess            0x16A220
#define __sceSblACMgrIsAslrSeed2Process                 0x16A270
#define __sceSblACMgrHasSceProgramAttribute             0x16A290
#define __sceSblACMgrHasDrmProcessAttribute             0x16A2A0
#define __sceSblACMgrHasUseVtrmCapability               0x16A2B0
#define __sceSblACMgrIsDiskplayerdiagProcess            0x16A2E0
#define __sceSblACMgrHasUseHp3dPipeCapability           0x16A310
#define __sceSblACMgrHasUseVideoServiceCapability       0x16A340
#define __sceSblACMgrHasSceProcessCapability            0x16A360
#define __sceSblACMgrIsAllowedToUsePupUpdate0           0x16A370
#define __sceSblACMgrIsVtrmadminProcess                 0x16A400
#define __sceSblACMgrIsSoftwagnerProcess                0x16A420
#define __sceSblACMgrIsDebuggableProcess                0x16A440
#define __sceSblDiagOSUpdaterProcess                    0x16A4E0
#define __sceSblACMgrIsAllowedSystemLevelDebugging      0x16A530
#define __sceSblACMgrHasMmapSelfCapability              0x16A5B0
#define __sceSblACMgrIsAllowedToMmapSelf                0x16A5C0
#define __sceSblACMgrGetPathId                          0x16A5E0
#define __sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall 0x16A7E0
#define __sceSblACMgrGetDeviceAccessType                0x16A8E0
#define __sceSblACMgrIsAllowedCoredump                  0x16A550
#define __sceSblACMgrIsAllowedDrmProcessDebugging       0x16A4A0
#define __sceSblACMgrIsFirstImageWriterProcess          0x16A0F0
#define __sceSblACMgrIsMountfusefsProcess               0x169EA0
#define __sceSblRcMgrIsAllowPsmDebug                    0x29DC10
#define __sceSblRcMgrIsAllowRegistryAccess              0x29DC80
#define __sceSblRcMgrIsAllowRegistryAccessMini          0x29DCF0
#define __sceSblRcMgrIsAllowInitVtrm                    0x29DE40
#define __sceSblRcMgrIsAllowSLDebugger                  0x29DEB0
#define __sceSblRcMgrIsAllowAdClock                     0x29DF20
#define __sceSblRcMgrIsStoreMode                        0x29DFD0
#define __sceSblRcMgrIsAllowULDebugger                  0x29E140
#define __sceSblRcMgrIsAllowCreateDebuggingDeviceFile   0x29E1B0
#define __sceSblRcMgrIsSoftwagnerQafForAcmgr            0x29E220
#define __sceSblRcMgrIsAllowDisablingAslr               0x29E290
#define __sceSblRcMgrIsAllowDebugSoftWagnerClock        0x29E300
#define __sceSblRcMgrIsAllowDataExecutionInFselfProcess 0x29E8C0
#define __sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp 0x29E930
#define __sceSblAIMgrIsDevKit                           0x20D740
#define __sceSblAIMgrIsTool                             0x20D7B0
#define __sceSblAIMgrIsTestKit                          0x20D890
#define __sceSblAIMgrIsDex                              0x20D900
#define __sceSblAIMgrIsCex                              0x20D970
#define __sceSblAIMgrIsDiag                             0x20DB90
#define __sceSblAIMgrGetProductCode                     0x20DCC0
#define __sceRegMgrSetInt                               0x4D6F00
#define __regMgrComApiPrologue                          0x4D6FF0
#define __regMgrComCheckParam                           0x4D70A0
#define __regMgrComSetReg                               0x4D7510
#define ___regMgrComSetErrInfo                          0x4D7D50
#define __regMgrComApiEpilogue                          0x4D7D90
#define __sceRegMgrGetInt                               0x4D7DB0
#define __regMgrComCheckAddr                            0x4D7EC0
#define __regMgrComGetReg                               0x4D7F00
#define __sceKernelCheckDipsw                           0x6351F0
#define __sceKernelIsDevelopmentMode                    0x635950
#define __sceKernelIsAssistMode                         0x635980
#define __sceSblSsDecryptSealedKey                      0x603580
#define __getSealedKeySecret                            0x603700
#define ___sceSblAuthMgrSmIsLoadable2                   0x38FBC0
#define __sceSblAuthMgrSmVerifyPupExpiration            0x6146C0
#define __sceSblServiceMailbox                          0x614010
#define __sceSblSsMemcmpConsttime                       0x607130
#define __sceSblSsMemset                                0x607190
#define __sceSblDriverMapPages                          0x5FFD90
#define __sceSblDriverUnmapPages                        0x600450
#define __sceSblAuthMgrSmSpawn                          0x614580
#define ___sceSblAuthMgrSmStart                         0x622020
#define __sceSblACMgrIsSystemUcred                      0x169DD0
#define __sceSblServiceSpawn                            0x6148C0
#define __SceBdemu                                      0x5FFAC0
#define __sceSblSrtcSetTime                             0x6173E0
#define __sceSblVtrmRestore                             0x628F50
#define __sceSblVtrmBackup                              0x628EE0
#define __preLoad                                       0x614B60
#define __gen_mem_keys                                  0x614CC0
#define __s_npdrmCdev                                   0x62EE00
#define __DriftUpdate2                                  0x622ED0
#define __VtrmUtilLockInitialize                        0x626520
#define __VtrmUtilLockFinialize                         0x626540
#define __sflash_pread_for_vtrm                         0x56EE0
#define __sflash_erase_for_vtrm                         0x57010
#define __sflash_pwrite_for_vtrm                        0x56E70
#define __sflash_pread_for_vtrm2                        0x56F60
#define ___is_vtrm_access_ok                            0x6290B0
#define __vtrm_erase_sector                             0x6297A0
#define __vtrm_pread                                    0x6297C0
#define __vtrm_pwrite                                   0x6297F0
#define __vtrm_pwrite2                                  0x629820
#define __icc_query                                     0x7FE10
#define __icc_query_nowait                              0x808C0
#define __icc_nvs_write                                 0x1B770
#define __icc_nvs_read                                  0x1B850
#define __icc_nvs_drvinit                               0x1B960
#define __icc_nvs_init                                  0x1B9A0
#define __icc_snvs_read_sector                          0x451670
#define __icc_snvs_write_sector                         0x451730
#define __icc_snvs_get_write_count                      0x4517F0
#define __icc_init_snvs_write_lock                      0x4518B0
#define __icc_lock_snvs_write                           0x4518D0
#define __icc_unlock_snvs_write                         0x4518F0
#define __icc_button_init                               0xBEE0
#define __icc_button_set_state                          0xC090
#define __icc_button_get_state                          0xC190
#define __icc_register_notification_handler             0x80CA0
#define __icc_indicator_set_buzzer                      0x470CE0
#define __icc_indicator_set_led                         0x471350
#define __icc_indicator_get_led                         0x4714C0
#define __icc_indicator_set_dynamic_led                 0x471640
#define __icc_indicator_get_dynamic_led                 0x4717D0
#define __icc_indicator_set_dynamic_led_shutdown        0x471A50
#define __icc_indicator_set_dynamic_led_boot            0x470D90
#define __icc_indicator_set_dynamic_led_off             0x470E60
#define __icc_indicator_set_thermalalert_led_on         0x470EF0
#define __icc_indicator_set_thermalalert_led_off        0x470F80
#define __icc_indicator_set_dynamic_led_standby         0x471040
#define __icc_indicator_set_dynamic_led_standby_boot    0x471B40
#define __icc_indicator_drvinit                         0x4710D0
#define __icc_indicator_check_thermal_on_boot           0x4719A0
#define __icc_thermal_alert_occurred_when_boot          0x3F690
#define __icc_thermal_alert_occurred_when_boot_sub      0x3F6B0
#define __icc_thermal_alert_status_off                  0x3F780
#define __icc_thermal_notify_handler                    0x3F880
#define __icc_thermal                                   0x3F930
#define __icc_thermal_enable_disable_notify             0x3FA80
#define __icc_thermal_get_thermal_state                 0x3FB40
#define __icc_thermal_stop_td                           0x3FBC0
#define __icc_thermal_resume_td                         0x3FCF0
#define __icc_thermal_set_notify                        0x3F7D0
#define __icc_thermal_drvinit                           0x3FB80
#define __icc_fan_drvinit                               0x45B410
#define __icc_sc_fw_update_handler                      0x9240
#define __icc_sc_fw_update                              0x92A0
#define __make_dev                                      0x36E0E0
#define __make_dev_credf                                0x36E510
#define __destroy_dev                                   0x36EAD0
#define __sce_cam_error_log_init                        0x3A550
#define __sce_cam_error_log_read                        0x3A600
#define __sce_cam_error_log_write                       0x3A790
#define __sce_exfatfs_vop_readdir                       0x0FA5F0
#define __sce_cam_error_put                             0x3A7C0
#define __sys_yield                                     0x261550
#define __sys_execve                                    0x2E6960
#define __exec_copyin_args                              0x2E69C0
#define __kern_execve                                   0x2E6C10
#define __kern_ptrace                                   0x17D200
#define __kern_ptrace_check                             0x17D2C1
#define __kern_jail_set                                 0x1B0D60
#define __kern_jail_get                                 0x1B4F50
#define __kernacc                                       0x30E320
#define __useracc                                       0x30E3C0
#define __kernel_mount                                  0xDD1C0
#define __proc_sstep                                    0x17CB50
#define __proc_rwmem                                    0x17CB70
#define __proc_set_dr                                   0x17CE10
#define __proc_get_dr                                   0x17CEA0
#define __proc_init_dr                                  0x17CF60
#define __kick_proc0                                    0x30F240
#define __kproc_kthread_add                             0x465490
#define __kthread_suspend_check                         0x465380
#define __kthread_add                                   0x464C90
#define __ktrcsw                                        0x1F6030
#define __vm_phys_paddr_to_vm_page                      0x06CA40
#define __vm_page_free_toq                              0x456AD0
#define __vm_page_unhold_pages                          0x456D70
#define __vflush                                        0x2A7D60
#define __vslock                                        0x30E460
#define __vsunlock                                      0x30E590
#define __uma_zalloc_arg                                0x469990
#define __uma_zfree_arg                                 0x46A1C0
#define __uma_large_free                                0x46B5E0
#define __sleepq_remove                                 0x14A200
#define __sleepq_lock                                   0x1492C0
#define __mdbg_kproc_hook_create                        0xE8AD0
#define __metadbg_log_add                               0x3A3BB0
#define __cpu_set_fork_handler                          0x6E520
#define __sched_prio                                    0x72410
#define __sched_user_prio                               0x72960
#define __bioq_init                                     0xF3010
#define __disk_alloc                                    0x15A3C0
#define __disk_create                                   0x15A3E0
#define __eventhandler_find_list                        0x3C9D50
#define __eventhandler_prune_list                       0x3C9E00
#define __eventhandler_deregister                       0x3C9B60
#define __eventhandler_register                         0x3C97F0
#define __eventhandler_suspend                          0x3C9AE0
#define __dump_pfs                                      0x688770
#define __dump_sign                                     0x6888C0
#define __pfs_savedata                                  0x610D60
#define __pfs_mapped_read                               0x684300
#define __pfs_check_addr                                0x689490
#define __pfs_checkpath                                 0x68B960
#define __pfs_fileno_uninit                             0x4548B0
#define __pfs_fileno_alloc                              0x4548E0
#define __pfs_fileno_free                               0x454960
#define __pfs_fileno_init                               0x454870
#define __pfs_create_dir                                0x329B50
#define __pfs_create_file                               0x329DE0
#define __pfs_create_link                               0x329EF0
#define __pfs_find_node                                 0x32A000
#define __pfs_destroy                                   0x32A090
#define __pfs_vncache_alloc                             0x6BD20
#define __pfs_vncache_unload                            0x6BCE0
#define __pfs_vncache_load                              0x6BC00
#define __pfs_purge                                     0x6C190
#define __pfs_uninit                                    0x32A590
#define __pfs_init                                      0x32A350
#define __pfs_mount                                     0x32A220
#define __pfs_cmount                                    0x32A2F0
#define __pfs_unmount                                   0x32A300
#define __pfs_root                                      0x32A320
#define __pfs_statfs                                    0x32A340
#define __p_candebug                                    0x116050
#define __procfs_candebug                               0x1FD2E0
#define __procfs_init                                   0x1FD310
#define __procfs_doprocfile                             0x1FD0B0
#define __procfs_docurproc                              0x1FD1A0
#define __procfs_attr                                   0x1FD1D0
#define __procfs_notsystem                              0x1FD2C0
#define __procfs_doosrel                                0x1FB6F0
#define __procfs_doprocstatus                           0x1FB7E0
#define __procfs_doproccmdline                          0x1FBC00
#define __procfs_doprocctl                              0x390760
#define __procfs_doprocdbregs                           0xC240
#define __procfs_doproctype                             0x113B60
#define __procfs_doprocfpregs                           0x4031D0
#define __procfs_doprocmap                              0x2004E0
#define __procfs_doprocmem                              0x5E900
#define __procfs_ioctl                                  0x372560
#define __procfs_close                                  0x372770
#define __procfs_doprocnote                             0xD6900
#define __procfs_doprocregs                             0x9B6A0
#define __procfs_doprocrlimit                           0xA5470
#define __vfs_getnewfsid                                0x2A52F0
#define __vfs_mountedfrom                               0xDCF00
#define __vfs_donmount                                  0xD9C00
#define __vfs_buildopts                                 0xD9630
#define __g_cancel_event                                0x383430
#define __g_trace                                       0xD6390
#define __g_new_geomf                                   0x2DC950
#define __g_destroy_geom                                0x2DCB10
#define __g_bdemu_create                                0x5FE850
#define __bdemu_speed_emu                               0x5FE090
#define __bd_sched_post_done                            0x5FCD30
#define __bd_sched_update_bitmap_file                   0x5FC660
#define __bd_sched_copier_handle_suspend_event          0x5FCA80
#define __bd_sched_copier_handle_resume_event           0x5FCC30
#define __bd_sched_writer_handle_suspend_event          0x5FBE60
#define __bd_sched_writer_handle_resume_event           0x5FC020
#define __bd_sched_shutdown_copier                      0x5F9B20
#define __bd_sched_shutdown_w_hdd                       0x5F9CA0
#define __bd_sched_shutdown_r_bd_internal               0x5F9E30
#define __bd_sched_reader_handle_suspend_event          0x5F9FC0
#define __bd_sched_reader_handle_resume_event           0x5FA130
#define __bd_sched_bd_reader_ripping                    0x5FA230
#define __bd_sched_bd_reader_bg_copy                    0x5FA890
#define __bd_sched_lock_bm_file                         0x5FAEC0
#define __bd_sched_update_bitmap_cache2                 0x5FAFD0
#define __bd_sched_is_available_on_hdd2                 0x5FB3C0
#define __bd_sched_unlock_bm_file                       0x5FB5B0
#define __get_img_offset                                0x5FB660
#define __bd_sched_read_bd_dev                          0x5FB760
#define __get_pkg_offset                                0x5FB860
#define __bd_sched_get_bdsio_in_cache                   0x5FB900
#define __bd_sched_get_bdsio_in_writer                  0x5FBA30
#define __bd_sched_private_done                         0x5FD8A0
#define __bd_sched_adjust_bio_completed                 0x5FDB60

/*--------------------------------------------------------------------
                          Need fix
--------------------------------------------------------------------*/
#define __load_prx                                      0x0
#define __copystr                                       0x0
#define __device_printf                                 0x0
#define __cnputc                                        0x0
#define __cnputs                                        0x0
#define __g_obi_create                                  0x0
#define __g_new_consumer                                0x0
#define __g_attach                                      0x0
#define __g_access                                      0x0
#define __g_wither_geom                                 0x0
#define __g_do_wither                                   0x0
#define __g_orphan_provider                             0x0
#define __g_provider_by_name                            0x0
#define __g_obi_read                                    0x0
#define __g_io_request                                  0x0
#define __g_alloc_bio                                   0x0
#define __g_destroy_bio                                 0x0
#define __g_post_notification_for_bio                   0x0
#define __gs_is_update_bio_ioprio                       0x0
#define __firmware_get                                  0x0
#define __memmove                                       0x0
#define __sched_unpin                                   0x0
#define __sched_switch                                  0x0
#define __sched_bind                                    0x0
#define __sx_xlock						     		    0x0
#define ___sx_sunlock_hard                              0x0
#define ___sx_sunlock_hard_sub                          0x0
#define __id_set                                        0x0
#define __id_unlock                                     0x0
#define __id_wlock                                      0x0
#define __fpu_kern_enter		     				    0x0
#define __fpu_kern_leave		     				    0x0
#define __vm_map_lock_read		     				    0x0
#define __vm_map_lookup_entry	     				    0x0
#define __vm_map_unlock_read	     				    0x0
#define __vmspace_free			     			        0x0
#define __vmspace_acquire_ref     				        0x0
#define __pagezero 		     						    0x0
#define __bcmp			     						    0x0
#define __binuptime                                     0x0
#define __getbinuptime                                  0x0
#define __casuword32	     				            0x0
#define __mi_switch 						     	    0x0
#define __eventlog_write_crash			     		    0x0
#define __metadbg_perform_dump_on_panic                 0x0
#define __savectx                                       0x0
#define __savectx2                                      0x0
#define __hexdump                                       0x0
#define __kern_reboot                                   0x0
#define __mutex_init                                    0x0
#define __msgbufinit                                    0x0
#define __msgbuf_copy                                   0x0
#define __msgbuf_getchar                                0x0
#define __msgbuf_reinit                                 0x0
#define __msgbuf_addstr                                 0x0
#define __log                                           0x0
#define __priv_check                                    0x0
#define __priv_check_cred                               0x0
#define __prison_priv_check                             0x0
#define __spinlock_enter                                0x0
#define __icc_enable_crashmode                          0x0
#define __icc_sc_config_drvinit                         0x0
#define __get_icc_max                                   0x0
#define __icc_wdt_start                                 0x0
#define __icc_wdt_stop                                  0x0
#define __devstat_new_entry                             0x0
#define __devstat_start_transaction                     0x0
#define __panic0                                        0x0
#define __panic                                         0x0
#define __ssb_rtc_init_exclock                          0x0
#define __uma_large_free_error                          0x0
#define __lgdt                                          0x0
#define __longjmp                                       0x0
#define __index                                         0x0
#define __load_prx                                      0x0
#define __unref_dag                                     0x0
#define __sceSblPfsKeymgrGenEKpfsForGDGPAC     		    0x0
#define __sceSblPfsSetKey			     			    0x0
#define __sceSblPfsClearKey		     				    0x0
#define __sceSblKeymgrClearKey                          0x0
#define __sceSblAuthMgrIsLoadable                       0x0
#define __sceSblAuthMgrAuthHeader                       0x0
#define __sceSblAuthMgrFinalize                         0x0
#define __sceSblAuthMgrLoadBlock                        0x0
#define ___sceSblAuthMgrGetElfSegmentInformation        0x0
#define ___sceSblAuthMgrLoadSelfBlock                   0x0
#define ___sceSblAuthMgrGetSelfSegmentInformation       0x0
#define ___sceSblAuthMgrCheckSelfHeader                 0x0
#define ___sceSblAuthMgrGetSelfInfo                     0x0
#define ___sceSblAuthMgrSmDriveClearKey                 0x0
#define ___sceSblAuthMgrSmDriveClearSessionKey          0x0
#define ___sceSblAuthMgrSmDriveData                     0x0
#define ___sceSblAuthMgrSmVerifyActCodeCommon           0x0
#define ___sceSblAuthMgrSmGenActRequest                 0x0
#define ___sceSblAuthMgrSmGenActHeader                  0x0
#define ___sceSblAuthMgrSmGenPassCodeData               0x0
#define ___sceSblAuthMgrSmCheckPassCodeData             0x0
#define ___sceSblAuthMgrSmVerifyPupExpiration           0x0
#define __sceSblAuthMgrIsLoadable2	     			    0x0
#define __sceSblAuthMgrVerifyHeader	     		        0x0
#define __sceSblAuthMgrGetSelfInfo     			 	    0x0
#define __sceSblAuthMgrSmStart     				  	    0x0
#define __sceSblKeymgrSmCallfunc	     			    0x0
#define __sceSblServiceCrypt     					    0x0
#define __sceSblServiceCryptAsync				        0x0
#define __sceSblSsGenerateSealedKey                     0x0
#define __SceDaAtcev                                    0x0
#define __SceHidAuth                                    0x0
#define __SceSflash           		     		        0x0
#define __AesCbcCfb128Encrypt	     		 		    0x0
#define __AesCbcCfb128Decrypt	     		 	 	    0x0
#define __AesEncKeySet 		     			            0x0
#define __AesDecKeySet 		     			            0x0
#define __AesEncrypt  		     			            0x0
#define __AesDecrypt  		     			            0x0
#define __Sha256HmacFinal	     			            0x0
#define __Sha256Hash     	     			            0x0
#define __Sha256Hmac      	     			            0x0
#define __Sha256OneBlk   	     			            0x0
#define __Sha256FinalCore 	     			            0x0
#define __Sha256Init     	     			            0x0
#define __Sha256Transform	     			            0x0
#define __Sha256Final          				            0x0
#define __Sha256HmacInit       				            0x0
#define __sceSblRngGetRandomNumber				        0x0
#define __Fips186Prng 				                    0x0
#define __RsaesPkcs1v15Enc2048		     			    0x0
#define __RsaesPkcs1v15Dec2048CRT     				    0x0
#define __allproc	     					            0x0
#define __M_TEMP				     		            0x0
#define __fpu_kern_ctx					                0x0

#elif defined FW_500 || FW_501
#define __Xfast_syscall				     		        0x1C0
#define __aslr_initialize_process_status                0x3884E0
#define __aslr_gen_pseudo_random_number                 0x3885E0
#define __aslr_get_random_value                         0x388690
#define __bootparam_hwconfig_read                       0x3F8740
#define __bootparam_get_mac_address                     0x3F8780
#define __bootparam_get_config_board_id                 0x3F87A0
#define __bootparam_get_ddr3_capacity                   0x3F87D0
#define __bootparam_is_mode_manu                        0x3F87F0
#define __bootparam_disable_console_output              0x3F8810
#define __bootparam_notify_main_on_standby_mode         0x3F8820
#define __bootparam_is_main_on_standby                  0x3F8840
#define __bootparam_get_eap_part_key                    0x651F70
#define __eap_internal_key                              0x2790C90
#define DT_HASH_SEGMENT                                 0xB5EE20
#define __printf                                        0x435C70
#define __vprintf                                       0x435CE0
#define __snprintf                                      0x435F80
#define __sprintf                                       0x435EB0
#define __vsprintf                                      0x435F37
#define __vsnrprintf                                    0x4360B0
#define __vsnprintf                                     0x436020
#define __copyin                                        0x1EA600
#define __copyout                                       0x1EA520
#define __copyinstr                                     0x1EAA30
#define __kmem_alloc_contig                             0xF1B80
#define __kmem_free                                     0xFCD40
#define __pmap_extract                                  0x2E02A0
#define __pmap_protect                                  0x2E2D00
#define __pmap_kextract                                 0x2E0620
#define __pmap_protect_p                                0x2E2D44
#define __kernel_pmap_store                             0x22CB4F0
#define __process_aslr                                  0x194765 // Thanks to J00ni3 - Need Verification
#define __devkit_id                                     0x8101
#define __target_id_0                                   0x1CD068C
#define __target_id_1                                   0x236B7FC
#define __mmap_self_0                                   0x117B0
#define __mmap_self_1                                   0x117B1
#define __mmap_self_2                                   0x117B2
#define __mmap_self_3                                   0x117C0
#define __mmap_self_4                                   0x117C1
#define __mmap_self_5                                   0x117C2
#define __mmap_self_6                                   0x13EF2F
#define __mmap_self_7                                   0x13EF30
#define __mmap_self_8                                   0x13EF31
#define __mmap_self_9                                   0x13EF32
#define __mmap_self_10                                  0x13EF33
#define __mmap_self_patch_0                             0xB0
#define __mmap_self_patch_1                             0x01
#define __mmap_self_patch_2                             0xC3
#define __mmap_self_patch_4                             0x31
#define __mmap_self_patch_5                             0xC0
#define __mmap_self_patch_6                             0x90
#define __sched_pin                                     0x31FB70
#define __sched_unpin                                   0x31FB80
#define __smp_rendezvous                                0x1B84A0
#define __smp_no_rendevous_barrier                      0x1B8260
#define __sys_yield                                     0x3FB990
#define __cv_init                                       0x45D390
#define __cv_destroy                                    0x45D3A0
#define ___cv_wait                                      0x45D3B0
#define __cv_wait_unlock                                0x45D5A0
#define ___cv_wait_sig                                  0x45D760
#define ___cv_timedwait                                 0x45D940
#define ___cv_timedwait_sig                             0x45DB40
#define __cv_signal                                     0x45DD40
#define __cv_signalto                                   0x45DDC0
#define __cv_broadcastpri                               0x45DE50
#define __sleepq_lock                                   0x412600
#define __sleepq_lookup                                 0x412630
#define __sleepq_release                                0x412670
#define __sleepq_add                                    0x4126A0
#define __call_samu                                     0x412860
#define __sleepq_set_timeout                            0x412920
#define __sleepq_set_runnable                           0x412980
#define __sleepq_sleepcnt                               0x412AC0
#define __sleepq_wait                                   0x412B10
#define __sleepq_wait_sig                               0x412C70
#define __sleepq_timedwait                              0x412F90
#define __sleepq_timedwait_sig                          0x413060
#define __sleepq_signal                                 0x4131F0
#define __sleepq_signalto                               0x4134A0
#define __sleepq_broadcast                              0x413570
#define __sleepq_remove                                 0x413640
#define __sleepq_abort                                  0x413750
#define __sysctl_lock                                   0x260E30
#define __sysctl_unlock                                 0x260E50
#define __sysctl_register_oid                           0x260E70
#define __sysctl_unregister_oid                         0x260F70
#define __sysctl_ctx_init                               0x260FE0
#define __sysctl_ctx_free                               0x261000
#define __sysctl_ctx_entry_add                          0x261350
#define __sysctl_ctx_entry_find                         0x2613B0
#define __sysctl_ctx_entry_del                          0x2613F0
#define __sysctl_remove_oid                             0x261530
#define __sysctl_remove_name                            0x2615A0
#define __sysctl_add_oid                                0x261640
#define __sysctl_rename_oid                             0x261900
#define __sysctl_move_oid                               0x2619B0
#define __sysctl_handle_int                             0x261B20
#define __sysctl_msec_to_ticks                          0x261BA0
#define __sysctl_handle_long                            0x261C80
#define __sysctl_handle_64                              0x261D40
#define __sysctl_handle_string                          0x261DC0
#define __sysctl_handle_opaque                          0x261ED0
#define __sysctl_wire_old_buffer                        0x2625B0
#define __sysctl_find_oid                               0x2626C0
#define __sys___sysctl                                  0x262790
#define __userland_sysctl                               0x262900
#define __sbuf_new_for_sysctl                           0x262C00
#define __ktrsysctl                                     0x349F10
#define __kernel_sysctl                                 0x261FC0
#define __kernel_sysctlbyname                           0x2624A0
#define __sys_execve                                    0x388750
#define __exec_copyin_args                              0x3887B0
#define __kern_execve                                   0x388A50
#define __kern_ptrace                                   0x30D550
#define __kern_ptrace_check                             0x30D633 // Thanks to J00ni3 - Need Verification
#define __kern_jail_set                                 0x3ABC30
#define __kern_jail_get                                 0x3AFEF0
#define __kern_ioctl                                    0x153880
#define __kernacc                                       0x62E0
#define __useracc                                       0x6380
#define __kern_unk                                      0x3FBA00
#define __kern_close                                    0xC0F40
#define __kernel_mount                                  0x1E1810
#define __kernel_map                                    0x1AC60E0
#define __ktr_tracepoint                                0x138BB0
#define __proc_sstep                                    0x30CDA0
#define __proc_rwmem                                    0x30CDC0
#define __proc_set_dr                                   0x30D060
#define __proc_get_dr                                   0x30D0F0
#define __proc_init_dr                                  0x30D1B0
#define __kick_proc0                                    0x7210
#define __call_samu                                     0x412860
#define __sysent                                        0x107C610
#define __Starsha_UcodeInfo                             0
#define __gpu_devid_is_9924                             0x4DDC40
#define __gc_get_fw_info                                0x4D33D0
#define __cnputc                                        0x3DFAC0
#define ___thread_lock_flags_check_result               0x402000
#define __spinlock_enter                                0x234960
#define __spinlock_exit                                 0x2349A0
#define __critical_enter                                0x28E4D0
#define __critical_exit                                 0x28E4E0
#define ___thread_lock_flags                            0x402050
#define __sched_bind                                    0x31FA50
#define __alloc_unr                                     0x156C60
#define __alloc_unrl                                    0x156670
#define __clean_unrhdrl                                 0x1561C0
#define __delete_unrhdr                                 0x1565C0
#define __msgbuf_addstr                                 0x26C230
#define __sbuf_new                                      0x212390
#define __sbuf_delete                                   0x2125E0
#define __sbuf_vprintf                                  0x212B10
#define __sbuf_trim                                     0x212D40
#define __sbuf_finish                                   0x212DB0
#define __sbuf_data                                     0x212E60
#define __sbuf_len                                      0x212E70
#define __msgbuf_addchar                                0x401D30
#define __msgbuf_addstr_sub                             0x401EF0
#define __log                                           0x435300
#define __kick_proc0                                    0x7210
#define __wakeup                                        0x3FB570
#define ___sx_sunlock_hard                              0xF69A0
#define ___sx_sunlock                                   0xF5E00
#define __sx_xunlock                                    0x767690
#define __sx_xlock_hard                                 0xF5D00
#define __sx_init_flags                                 0xF5AA0
#define __sx_xlock                                      0xF5D00
#define ___sx_xlock_hard                                0xF60A0
#define __sx_destroy                                    0xF5B10
#define ___thread_lock_flags_check_result               0x402000
#define ___thread_lock_flags                            0x402050
#define ___mtx_lock_sleep                               0x401900
#define ___mtx_lock_sleep                               0x401900
#define ___mtx_unlock_sleep                             0x401BD0
#define __mtx_destroy                                   0x402420
#define __lock_init                                     0x3C2550
#define __jump_lock_destroy                             0xA6480
#define __get_td_locks                                  0x767CE0
#define __kern_jail                                     0x3AB4C5
#define __priv_check_cred                               0x2FF820
#define __priv_check                                    0x2FF940
#define __prison_priv_check                             0x3B1D90
#define __load_prx                                      0x2AE5C0
#define __preload_prx_modules                           0x2AD260
#define __sceSblACMgrIsSystemUcred                      0x10F90
#define __sceSblACMgrIsNongameUcred                     0x10FB0
#define __sceSblACMgrIsDiagProcess                      0x10FC0
#define __sceSblACMgrIsCoredumpProcess                  0x11020
#define __sceSblACMgrIsDecidProcess                     0x11080
#define __sceSblACMgrIsMinisyscoreProcess               0x110D0
#define __sceSblACMgrIsSyscoreProcess                   0x11100
#define __sceSblACMgrIsShelluiProcess                   0x11120
#define __sceSblACMgrIsShellcoreProcess                 0x11140
#define __sceSblACMgrIsVideoplayerProcess               0x111C0
#define __sceSblACMgrIsDiskplayeruiProcess              0x111E0
#define __sceSblACMgrIsBdjavaProcess                    0x11200
#define __sceSblACMgrIsWebcoreProcess                   0x11220
#define __sceSblACMgrIsVdecProxyProcess                 0x112F0
#define __sceSblACMgrIsJitCompilerProcess               0x113B0
#define __sceSblACMgrIsJitApplicationProcess            0x11410
#define __sceSblACMgrIsAslrSeed2Process                 0x11480
#define __sceSblACMgrHasSceProgramAttribute             0x114A0
#define __sceSblACMgrHasDrmProcessAttribute             0x114B0
#define __sceSblACMgrHasUseVtrmCapability               0x114C0
#define __sceSblACMgrIsDiskplayerdiagProcess            0x114F0
#define __sceSblACMgrHasUseHp3dPipeCapability           0x11520
#define __sceSblACMgrHasUseVideoServiceCapability       0x11550
#define __sceSblACMgrHasSceProcessCapability            0x11560
#define __sceSblACMgrIsAllowedToUsePupUpdate0           0x11570
#define __sceSblACMgrIsVtrmadminProcess                 0x11600
#define __sceSblACMgrIsSoftwagnerProcess                0x11620
#define __sceSblACMgrIsDebuggableProcess                0x11640
#define __sceSblDiagOSUpdaterProcess                    0x116E0
#define __sceSblACMgrIsAllowedSystemLevelDebugging      0x11730
#define __sceSblACMgrHasMmapSelfCapability              0x117B0
#define __sceSblACMgrIsAllowedToMmapSelf                0x117C0
#define __sceSblACMgrGetPathId                          0x117E0
#define __sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall 0x11A00
#define __sceSblACMgrGetDeviceAccessType                0x11B00
#define __sceSblACMgrIsAllowedCoredump                  0x11750
#define __sceSblACMgrIsAllowedDrmProcessDebugging       0x116A0
#define __sceSblACMgrIsFirstImageWriterProcess          0x112B0
#define __sceSblACMgrIsMountfusefsProcess               0x11060
#define __sceSblRcMgrIsAllowPsmDebug                    0x1CFC80
#define __sceSblRcMgrIsAllowRegistryAccess              0x1CFCF0
#define __sceSblRcMgrIsAllowRegistryAccessMini          0x1CFD60
#define __sceSblRcMgrIsAllowInitVtrm                    0x1CFEB0
#define __sceSblRcMgrIsAllowSLDebugger                  0x1CFF20
#define __sceSblRcMgrIsAllowAdClock                     0x1CFF90
#define __sceSblRcMgrIsStoreMode                        0x1D0040
#define __sceSblRcMgrIsAllowULDebugger                  0x1D01B0
#define __sceSblRcMgrIsAllowCreateDebuggingDeviceFile   0x1D0220
#define __sceSblRcMgrIsSoftwagnerQafForAcmgr            0x1D0290
#define __sceSblRcMgrIsAllowDisablingAslr               0x1D0300
#define __sceSblRcMgrIsAllowDebugSoftWagnerClock        0x1D03E0
#define __sceSblRcMgrIsAllowDataExecutionInFselfProcess 0x1D09A0
#define __sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp 0x1D0A10
#define __sceSblAIMgrIsDevKit                           0x428930
#define __sceSblAIMgrIsTool                             0x4289A0
#define __sceSblAIMgrIsTestKit                          0x428A80
#define __sceSblAIMgrIsDex                              0x428AF0
#define __sceSblAIMgrIsCex                              0x428B60
#define __sceSblAIMgrIsDiag                             0x428D90
#define __sceSblAIMgrGetProductCode                     0x428ED0
#define __sceRegMgrSetInt                               0x4F8940
#define __regMgrComApiPrologue                          0x4F8B10
#define __regMgrComCheckParam                           0x4F8B50
#define __regMgrComSetReg                               0x4F8F70
#define __regMgrComCheckAddr                            0x4F9B50
#define __sceKernelCheckDipsw                           0x653870
#define __sceKernelIsDevelopmentMode                    0x654250
#define __sceKernelIsAssistMode                         0x654280
#define __sceSblRcMgrIsAllowDataExecutionInFselfProcess 0x1D09A0
#define __sceSblSsDecryptSealedKey                      0x61F2A0
#define __getSealedKeySecret                            0x61F420
#define __sceSblACMgrIsSystemUcred                      0x10F90
#define __sceSblServiceMailbox                          0x6308C0
#define ___sceSblKeyMgrSmStart                          0x62E7F0
#define ___sceSblAuthMgrSmStart                         0x641500
#define ___sceSblAuthMgrSmIsLoadable2                   0xF5EC0
#define ___sceSblAuthMgrSmVerifyPupExpiration           0x643EE0 
#define __sceSblVtrmRestore                             0x6466B0
#define __sceSblVtrmBackup                              0x646640
#define __sceSblServiceMailbox                          0x6308C0
#define __sceSblSsMemcmpConsttime                       0x629AA0
#define __sceSblSsMemset                                0x629AF0
#define __sceSblDriverMapPages                          0x61B9B0
#define __sceSblDriverUnmapPages                        0x61C080
#define __sceSblSrtcSetTime                             0x633B20
#define __sceSblServiceSpawn                            0x632010
#define __SceBdemu                                      0x61B240
#define __s_npdrmCdev                                   0x64D230
#define __DriftUpdate2                                  0x6423E0
#define __vm_phys_paddr_to_vm_page                      0x298980
#define __vm_page_unhold_pages                          0x264520
#define __vm_page_free_toq                              0x264290
#define __vfs_getnewfsid                                0x278FB0
#define __vfs_mountedfrom                               0x1E1550
#define __vfs_donmount                                  0x1DE2A0
#define __vfs_buildopts                                 0x1DDC90
#define __vflush                                        0x27BE30
#define __vslock                                        0x6420
#define __vsunlock                                      0x6550
#define __uma_large_free                                0x22B670
#define __uma_zfree_arg                                 0x22A290
#define __uma_zalloc_arg                                0x2299C0
#define __audit_arg_fd                                  0x19D090
#define __currecord                                     0x3F94B0
#define __audit_arg_cmd                                 0x19D890
#define __malloc                                        0x10E140
#define __free                                          0x10E350
#define __memcpy                                        0x1EA420
#define __memset                                        0x3201F0
#define __free                                          0x10E350
#define __memcmp                                        0x64AFC0
#define __bcopy                                         0xAB0
#define __strtol                                        0x218760
#define __prison0                                       0x10986A0                         
#define __rootvnode                                     0x22C19F0
#define __pml4pml4i                                     0x22CB4E0
#define __dmpml4i                                       0x22CB4E4
#define __dmpdpi                                        0x22CB4E8
#define __gbase_samu_initialize                         0x4BAD30
#define __gbase_samu_finalize                           0x4BAFE0
#define __gbase_samu_map                                0x4BB1C0
#define __gbase_samu_unmap                              0x4BB1D0
#define __gbase_samu_read_register                      0x4BB200
#define __gbase_samu_write_register                     0x4BB220
#define __gbase_samu_ix_read_register                   0x4BB240
#define __gbase_samu_ix_write_register                  0x4BB2D0
#define __gbase_samu_pa_to_gpuva                        0x4BB360
#define __gbase_samu_unk_func                           0x4BB390
#define __gbase_samu_handler                            0x61CAE0
#define __gbase_samu_cmd                                0x767C30
#define __gbase_unmap                                   0x4C1BD0
#define __gbase_free_vm_space                           0x4C1280
#define ____stack_chk_fail                              0x6C53B0
#define __kvprintf                                      0x436120
#define __strlcpy                                       0x20630
#define __strcmp                                        0x1D0EC0
#define __strncpy                                       0x3C0B0
#define __strlen                                        0x3B6DD0
#define __strdup                                        0x1C1B20
#define __strstr                                        0x17DEA0
#define __strncmp                                       0x1B8ED0
#define __strcasecmp                                    0xA6250
#define __vstrcmp                                       0x1D0EC0
#define __genNewActDat                                  0x64B470
#define __bzero                                         0x1EA360
#define __preLoad                                       0x6323C0
#define __gen_mem_keys                                  0x6324E0                  
#define __VtrmUtilLockInitialize                        0x6440B0
#define __VtrmUtilLockFinialize                         0x6440D0
#define __sflash_pread_for_vtrm                         0x34DF90
#define __sflash_erase_for_vtrm                         0x34E0C0
#define __sflash_pwrite_for_vtrm                        0x34DF20
#define __sflash_pread_for_vtrm2                        0x34E010
#define ___is_vtrm_access_ok                            0x646810
#define __vtrm_erase_sector                             0x644370
#define __vtrm_pread                                    0x644390
#define __vtrm_pwrite2                                  0x6443F0
#define __vtrm_pwrite                                   0x6443C0
#define __icc_query                                     0x43540
#define __icc_query_nowait                              0x44020
#define __icc_nvs_write                                 0x395380
#define __icc_nvs_read                                  0x395460
#define __icc_nvs_drvinit                               0x395550
#define __icc_nvs_init                                  0x395590
#define __icc_snvs_read_sector                          0x26C770
#define __icc_snvs_write_sector                         0x26C830
#define __icc_snvs_get_write_count                      0x26C8F0
#define __icc_init_snvs_write_lock                      0x26C9A1
#define __icc_lock_snvs_write                           0x26C9D0
#define __icc_unlock_snvs_write                         0x26C9F0
#define __icc_button_init                               0x41FAA0
#define __icc_button_set_state                          0x41FD10
#define __icc_button_get_state                          0x41FC40
#define __icc_thermal_get_thermal_state                 0xA88E0
#define __icc_thermal_set_notify                        0xA8820
#define __icc_indicator_set_buzzer                      0x1041D0
#define __icc_indicator_set_led                         0x1048A0
#define __icc_indicator_get_led                         0x104A10
#define __icc_indicator_set_dynamic_led                 0x104B90
#define __icc_indicator_get_dynamic_led                 0x104D10
#define __icc_indicator_set_dynamic_led_boot            0x104280
#define __icc_indicator_set_dynamic_led_shutdown        0x105010
#define __icc_indicator_set_thermalalert_led_off        0x104450
#define __icc_indicator_set_dynamic_led_off             0x104330
#define __icc_indicator_set_thermalalert_led_on         0x1043C0
#define __icc_indicator_set_dynamic_led_standby         0x104510
#define __icc_indicator_set_dynamic_led_standby_boot    0x105150
#define __icc_indicator_drvinit                         0x104620
#define __icc_indicator_check_thermal_on_boot           0x104EE0
#define __icc_thermal_alert_occurred_when_boot          0xA8430
#define __icc_thermal_alert_occurred_when_boot_sub      0xA8450
#define __icc_thermal_alert_status_off                  0xA8520
#define __icc_thermal_notify_handler                    0xA8620
#define __icc_thermal                                   0xA86D0
#define __icc_thermal_enable_disable_notify             0xA8820
#define __icc_thermal_get_thermal_state                 0xA88E0
#define __icc_thermal_stop_td                           0xA8960
#define __icc_thermal_resume_td                         0xA8A90
#define __icc_thermal_set_notify                        0xA8570
#define __icc_thermal_drvinit                           0xA8920
#define __icc_fan_drvinit                               0x15C790
#define __icc_fan_get_fan_manual_duty                   0x15C810
#define __icc_sc_fw_update_handler                      0x401090
#define __icc_sc_fw_update                              0x4011D0
#define __make_dev                                      0x1B9250
#define __destroy_dev                                   0x1B9C40
#define __make_dev_credf                                0x1B9670
#define __sce_cam_error_log_init                        0x5C020
#define __sce_cam_error_log_read                        0x5C0D0
#define __sce_cam_error_log_write                       0x5C260
#define __sce_cam_error_put                             0x5C290
#define __sce_exfatfs_vop_readdir                       0x117EF0
#define __fork1                                         0x207D0
#define __mdbg_kproc_hook_create                        0x3A48D0
#define __metadbg_log_add                               0xE7DF0
#define __cpu_set_fork_handler                          0x2740B0
#define __sched_user_prio                               0x31EFA0
#define __bioq_init                                     0x3203C0
#define __disk_alloc                                    0x2DC9B0
#define __disk_create                                   0x2DC9F0
#define __eventhandler_find_list                        0x1EC870
#define __eventhandler_prune_list                       0x1EC910
#define __eventhandler_deregister                       0x1EC680
#define __eventhandler_register                         0x1EC2F0
#define __eventhandler_suspend                          0x1EC600
#define __dump_pfs                                      0x69F260
#define __dump_sign                                     0x69F4A0
#define __pfs_savedata                                  0x62F860
#define __pfs_mapped_read                               0x69AC80
#define __pfs_check_addr                                0x6A0070
#define __pfs_checkpath                                 0x6B2C70
#define __pfs_fileno_uninit                             0x3A3930
#define __pfs_fileno_alloc                              0x3A3960
#define __pfs_fileno_free                               0x3A39D0
#define __pfs_fileno_init                               0x454870
#define __pfs_create_dir                                0x23C490
#define __pfs_create_dir_sub                            0x23C5C0
#define __pfs_create_file                               0x23C760
#define __pfs_create_link                               0x23C870
#define __pfs_find_node                                 0x23C980
#define __pfs_destroy                                   0x23CA10
#define __pfs_purge                                     0x464E30
#define __pfs_mount                                     0x23CBA0
#define __pfs_cmount                                    0x23CC70
#define __pfs_unmount                                   0x23CC80
#define __pfs_root                                      0x23CCA0
#define __pfs_statfs                                    0x23CCC0
#define __pfs_vncache_unload                            0x464980
#define __pfs_vncache_load                              0x4648A0
#define __pfs_vncache_alloc                             0x4649C0
#define __p_candebug                                    0x56B70
#define __procfs_candebug                               0x1620B0
#define __procfs_init                                   0x1620E0
#define __procfs_doprocfile                             0x161E80
#define __procfs_docurproc                              0x161F70
#define __procfs_attr                                   0x161FA0
#define __procfs_notsystem                              0x162090
#define __procfs_doosrel                                0x309220
#define __procfs_doprocstatus                           0xBE960
#define __procfs_doproccmdline                          0xBED90
#define __procfs_doprocctl                              0x461220
#define __procfs_doprocdbregs                           0x3D1950
#define __procfs_doproctype                             0x31E4B0
#define __procfs_doprocfpregs                           0x163C0
#define __procfs_doprocmap                              0x53B40
#define __procfs_doprocmem                              0x4890D0
#define __procfs_ioctl                                  0x46A3D0
#define __procfs_close                                  0x46A5E0
#define __procfs_doprocnote                             0x3A2140
#define __procfs_doprocregs                             0xF6A60
#define __procfs_doprocrlimit                           0x197620
#define __bd_sched_private_done                         0x619900
#define __bd_sched_post_done                            0x618D40
#define __bd_sched_copier_handle_resume_event           0x618C40
#define __bd_sched_copier_handle_suspend_event          0x618A90
#define __bd_sched_update_bitmap_file                   0x618670
#define __bd_sched_writer_handle_resume_event           0x618000
#define __bd_sched_writer_handle_suspend_event          0x617E40
#define __bd_sched_get_bdsio_in_cache                   0x6178E0
#define __bd_sched_get_bdsio_in_writer                  0x617A20
#define __get_pkg_offset                                0x617850
#define __bd_sched_read_bd_dev                          0x617750
#define __get_img_offset                                0x617640
#define __bd_sched_unlock_bm_file                       0x617590
#define __bd_sched_is_available_on_hdd2                 0x6173A0
#define __bd_sched_update_bitmap_cache2                 0x616FA0
#define __bd_sched_lock_bm_file                         0x616EA0
#define __bd_sched_bd_reader_bg_copy                    0x616880
#define __bd_sched_bd_reader_ripping                    0x616230
#define __bd_sched_shutdown_copier                      0x615AE0
#define __bd_sched_shutdown_w_hdd                       0x615C80
#define __bd_sched_shutdown_r_bd_internal               0x615E20
#define __bd_sched_reader_handle_suspend_event          0x615FC0
#define __bd_sched_reader_handle_resume_event           0x616130
#define __bd_sched_adjust_bio_completed                 0x619BC0
#define __bdemu_speed_emu                               0x61B4D0
#define __g_bdemu_create                                0x61A020
#define __g_new_geomf                                   0xB60C0

/*--------------------------------------------------------------------
                               Need fix
--------------------------------------------------------------------*/
#define __copystr                                       0x0
#define __device_printf                                 0x0
#define __cnputs                                        0x0
#define __g_obi_create                                  0x0
#define __g_new_geomf                                   0x0
#define __g_new_consumer                                0x0
#define __g_attach                                      0x0
#define __g_trace                                       0x0
#define __g_access                                      0x0
#define __g_wither_geom                                 0x0
#define __g_do_wither                                   0x0
#define __g_orphan_provider                             0x0
#define __g_provider_by_name                            0x0
#define __g_obi_read                                    0x0
#define __g_io_request                                  0x0
#define __g_alloc_bio                                   0x0
#define __g_destroy_bio                                 0x0
#define __g_post_notification_for_bio                   0x0
#define __firmware_get                                  0x0
#define __memmove                                       0x0
#define __kthread_add					     		    0x0
#define ___sx_sunlock_hard                              0x0
#define ___sx_sunlock_hard_sub                          0x0
#define __id_set                                        0x0
#define __id_unlock                                     0x0
#define __id_wlock                                      0x0
#define __fpu_kern_enter		     				    0x0
#define __fpu_kern_leave		     				    0x0
#define __vm_map_lock_read		     				    0x0
#define __vm_map_lookup_entry	     				    0x0
#define __vm_map_unlock_read	     				    0x0
#define __vmspace_free			     			        0x0
#define __vmspace_acquire_ref     				        0x0
#define __bzero			     						    0x0
#define __pagezero 		     						    0x0
#define __bcmp			     						    0x0
#define __binuptime                                     0x0
#define __getbinuptime                                  0x0
#define __casuword32	     				            0x0
#define __mi_switch 						     	    0x0
#define __eventlog_write_crash			     		    0x0
#define __metadbg_perform_dump_on_panic                 0x0
#define __savectx                                       0x0
#define __savectx2                                      0x0
#define __ktrcsw                                        0x0
#define __hexdump                                       0x0
#define __kern_reboot                                   0x0
#define __mutex_init                                    0x0
#define __msgbuf_addchar                                0x0
#define __msgbufinit                                    0x0
#define __msgbuf_copy                                   0x0
#define __msgbuf_getchar                                0x0
#define __msgbuf_reinit                                 0x0
#define __msgbuf_addchar                                0x0
#define __msgbuf_addstr                                 0x0
#define __icc_register_notification_handler             0x0
#define __icc_sc_config_drvinit                         0x0
#define __get_icc_max                                   0x0
#define __icc_wdt_start                                 0x0
#define __icc_wdt_stop                                  0x0
#define __devstat_new_entry                             0x0
#define __devstat_start_transaction                     0x0
#define __ssb_rtc_init_exclock                          0x0
#define __uma_large_free_error                          0x0
#define __lgdt                                          0x0
#define __longjmp                                       0x0
#define __index                                         0x0
#define __load_prx                                      0x0
#define __preload_prx_modules                           0x0
#define __unref_dag                                     0x0
#define __sceSblPfsKeymgrGenEKpfsForGDGPAC     		    0x0
#define __sceSblPfsSetKey			     			    0x0
#define __sceSblPfsClearKey		     				    0x0
#define __sceSblKeymgrClearKey                          0x0
#define __sceSblAuthMgrIsLoadable                       0x0
#define __sceSblAuthMgrAuthHeader                       0x0
#define __sceSblAuthMgrFinalize                         0x0
#define __sceSblAuthMgrLoadBlock                        0x0
#define ___sceSblAuthMgrGetElfSegmentInformation        0x0
#define ___sceSblAuthMgrLoadSelfBlock                   0x0
#define ___sceSblAuthMgrGetSelfSegmentInformation       0x0
#define ___sceSblAuthMgrCheckSelfHeader                 0x0
#define ___sceSblAuthMgrGetSelfInfo                     0x0
#define ___sceSblAuthMgrSmDriveClearKey                 0x0
#define ___sceSblAuthMgrSmDriveClearSessionKey          0x0
#define ___sceSblAuthMgrSmDriveData                     0x0
#define ___sceSblAuthMgrSmVerifyActCodeCommon           0x0
#define ___sceSblAuthMgrSmGenActRequest                 0x0
#define ___sceSblAuthMgrSmGenActHeader                  0x0
#define ___sceSblAuthMgrSmGenPassCodeData               0x0
#define ___sceSblAuthMgrSmCheckPassCodeData             0x0
#define __sceSblAuthMgrSmSpawn                          0x0
#define __sceSblAuthMgrIsLoadable2	     			    0x0
#define __sceSblAuthMgrVerifyHeader	     		        0x0
#define __sceSblAuthMgrGetSelfInfo     			 	    0x0
#define __sceSblAuthMgrSmStart     				  	    0x0
#define __sceSblKeymgrSmCallfunc	     			    0x0
#define __sceSblServiceMailbox	     				    0x0
#define __sceSblServiceCrypt     					    0x0
#define __sceSblServiceCryptAsync				        0x0
#define __SceDaAtcev                                    0x0
#define __SceHidAuth                                    0x0
#define __SceSflash           		     		        0x0
#define __AesCbcCfb128Encrypt	     		 		    0x0
#define __AesCbcCfb128Decrypt	     		 	 	    0x0
#define __AesEncKeySet 		     			            0x0
#define __AesDecKeySet 		     			            0x0
#define __AesEncrypt  		     			            0x0
#define __AesDecrypt  		     			            0x0
#define __Sha256HmacFinal	     			            0x0
#define __Sha256Hash     	     			            0x0
#define __Sha256Hmac      	     			            0x0
#define __Sha256OneBlk   	     			            0x0
#define __Sha256FinalCore 	     			            0x0
#define __Sha256Init     	     			            0x0
#define __Sha256Transform	     			            0x0
#define __Sha256Final          				            0x0
#define __Sha256HmacInit       				            0x0
#define __sceSblRngGetRandomNumber				        0x0
#define __Fips186Prng 				                    0x0
#define __RsaesPkcs1v15Enc2048		     			    0x0
#define __RsaesPkcs1v15Dec2048CRT     				    0x0
#define __sysent		     				            0x0
#define __allproc	     					            0x0
#define __M_TEMP				     		            0x0
#define __fpu_kern_ctx					                0x0

#elif defined FW_505
#define __Xfast_syscall				     		        0x1C0
#define __aslr_initialize_process_status                0x3884E0
#define __aslr_gen_pseudo_random_number                 0x3885E0
#define __aslr_get_random_value                         0x388690
#define __bootparam_hwconfig_read                       0x3F8B10 // fixed.
#define __bootparam_get_mac_address                     0x3F8B50 // fixed.
#define __bootparam_get_config_board_id                 0x3F8B70 // fixed.
#define __bootparam_get_ddr3_capacity                   0x3F8BA0 // fixed.
#define __bootparam_is_mode_manu                        0x3F8BC0 // fixed.
#define __bootparam_disable_console_output              0x3F8BE0 // fixed.
#define __bootparam_notify_main_on_standby_mode         0x3F8BF0 // fixed.
#define __bootparam_is_main_on_standby                  0x3F8C10 // fixed.
#define __bootparam_get_eap_part_key                    0x652350 // fixed.
#define __eap_internal_key                              0x2790C90 // fixed.
#define DT_HASH_SEGMENT                                 0x0B5EF30
#define __printf                                        0x436040  // Thanks to #J0nni3
#define __vprintf                                       0x4360B0 // fixed.
#define __snprintf                                      0x436350 // fixed.
#define __sprintf                                       0x436280 // fixed.
#define __vsprintf                                      0x4363F0 // fixed.
#define __vsnrprintf                                    0x436480 // fixed.
#define __vsnprintf                                     0x4363F0 // fixed. 
#define __copyin                                        0x1EA710  // Thanks to #J0nni3
#define __copyout                                       0x1EA630  // Thanks to #J0nni3
#define __copyinstr                                     0x1EAB40  // Thanks to #J0nni3
#define __kmem_alloc_contig                             0xF1C90   // Thanks to #J0nni3
#define __kmem_free                                     0xFCE50   // Thanks to #J0nni3
#define __pmap_extract                                  0x2E0570  // Thanks to #J0nni3
#define __pmap_protect                                  0x2E3090  // Thanks to #J0nni3
#define __sched_pin                                     0x31FF40  // Thanks to #J0nni3
#define __sched_unpin                                   0x31FF50  // Thanks to #J0nni3
#define __smp_rendezvous                                0x1B85B0  // Thanks to #J0nni3
#define __smp_no_rendevous_barrier                      0x1B8366  // Thanks to #J0nni3
#define __sys_yield                                     0x3FB990
#define __cv_init                                       0x45D760 // fixed.
#define __cv_destroy                                    0x45D3A0
#define ___cv_wait                                      0x45D780 // fixed.
#define __cv_wait_unlock                                0x45D5A0
#define ___cv_wait_sig                                  0x45D760
#define ___cv_timedwait                                 0x45D940
#define ___cv_timedwait_sig                             0x45DB40
#define __cv_signal                                     0x45E110 // fixed.
#define __cv_signalto                                   0x45DDC0
#define __cv_broadcastpri                               0x45E220 // fixed.
#define __sleepq_lock                                   0x4129D0 // fixed.
#define __sleepq_lookup                                 0x412A00 // fixed.
#define __sleepq_release                                0x412A40 // fixed.
#define __sleepq_add                                    0x412A70 // fixed.
#define __sleepq_set_timeout                            0x412CF0 // fixed.
#define __sleepq_set_runnable                           0x412D50 // fixed.
#define __sleepq_sleepcnt                               0x412E90 // fixed.
#define __sleepq_wait                                   0x412EE0 // fixed.
#define __sleepq_wait_sig                               0x413040 // fixed.
#define __sleepq_timedwait                              0x413360 // fixed.
#define __sleepq_timedwait_sig                          0x413430 // fixed.
#define __sleepq_signal                                 0x4135C0 // fixed.
#define __sleepq_signalto                               0x413870 // fixed.
#define __sleepq_broadcast                              0x413940 // fixed.
#define __sleepq_remove                                 0x413A10 // fixed.
#define __sleepq_abort                                  0x413B20 // fixed.
#define __sysctl_lock                                   0x261100 // fixed.
#define __sysctl_unlock                                 0x261120 // fixed.
#define __sysctl_register_oid                           0x261140 // fixed.
#define __sysctl_unregister_oid                         0x261240 // fixed.
#define __sysctl_ctx_init                               0x2612B0 // fixed.
#define __sysctl_ctx_free                               0x2612D0 // fixed.
#define __sysctl_ctx_entry_add                          0x261350
#define __sysctl_ctx_entry_find                         0x2613B0
#define __sysctl_ctx_entry_del                          0x2613F0
#define __sysctl_remove_oid_locked                      0x261430 // fixed.
#define __sysctl_remove_oid                             0x261800 // fixed.
#define __sysctl_remove_name                            0x2615A0
#define __sysctl_add_oid                                0x261910 // fixed.
#define __sysctl_rename_oid                             0x261BD0 // fixed.
#define __sysctl_move_oid                               0x2619B0
#define __sysctl_handle_int                             0x261DF0 // fixed.
#define __sysctl_msec_to_ticks                          0x261BA0
#define __sysctl_handle_long                            0x261F50 // fixed.
#define __sysctl_handle_64                              0x262010 // fixed.
#define __sysctl_handle_string                          0x262090 // fixed.
#define __sysctl_handle_opaque                          0x2621A0 // fixed.
#define __sysctl_wire_old_buffer                        0x262880 // fixed.
#define __sysctl_find_oid                               0x2626C0
#define __sys___sysctl                                  0x262790
#define __do_sysctl                                     0x2624C0 // fixed.
#define __userland_sysctl                               0x262BD0 // fixed.
#define __userland_sysctl_copyout                       0x2628F0 // fixed.
#define __userland_sysctl_copyin                        0x262E60 // fixed.
#define __sbuf_new_for_sysctl                           0x262ED0 // fixed.
#define __ktrsysctl                                     0x349F10
#define __kernel_sysctl                                 0x262290 // fixed.
#define __kernel_sysctlbyname                           0x262770 // fixed.
#define __kern_sysctl_write                             0x2623F0 // fixed.
#define __kern_sysctl_read                              0x262470 // fixed.
#define __sys_execve                                    0x388750
#define __exec_copyin_args                              0x3887B0
#define __kern_execve                                   0x388E20 // fixed.
#define __kern_recvit                                   0x31AFD0 // fixed.
#define __kern_setsockopt                               0x31B780 // fixed.
#define __kern_getsockopt                               0x31B970 // fixed.
#define __kern_linkat                                   0x33C9E0 // fixed.
#define __kern_wait                                     0x35590 // fixed.
#define __kern_uuidgen                                  0x38900 // fixed.
#define __kern_psignal                                  0xD35F0 // fixed.
#define __kern_reboot                                   0x10D390 // fixed.
#define __kern_ioctl                                    0x153990 // fixed.
#define __kern_ptrace                                   0x30D8E0 // fixed.
#define __kern_timer_delete                             0x467FA0 // fixed.
#define __kern_ptrace_check                             0x30D9C3 //Corrected Offset for 5.05
#define __kern_jail_set                                 0x3AC000 // fixed.
#define __kern_jail_get                                 0x3AFEF0
#define __kernacc                                       0x62E0
#define __useracc                                       0x6380
#define __kern_unk                                      0x3FBA00
#define __kern_close                                    0xC0F40
#define __kernel_mount                                  0x1E1920 // fixed.
#define __mount_arg                                     0x1E16A0 // fixed.
#define __mount_init                                    0x1E1BF0 // fixed.
#define __mount_fini                                    0x1E1C40 // fixed.
#define __kernel_map                                    0x1AC60E0
#define __ktr_tracepoint                                0x138BB0
#define __fpu_kern_enter		     				    0x1BFF90 // fixed.
#define __fpu_kern_leave		     				    0x1C0090 // fixed.
#define __proc_sstep                                    0x30CDA0
#define __proc_rwmem                                    0x30D150 // fixed.
#define __proc_set_dr                                   0x30D060
#define __proc_get_dr                                   0x30D0F0
#define __proc_init_dr                                  0x30D1B0
#define __kick_proc0                                    0x7210   // fixed.
#define __process_aslr                                  0x194765 // Thanks to J00ni3 - Need Verification
#define __devkit_id                                     0x8101
#define __target_id_0                                   0x1CD068C
#define __target_id_1                                   0x236B7FC
#define __mmap_self_0                                   0x117B0
#define __mmap_self_1                                   0x117B1
#define __mmap_self_2                                   0x117B2
#define __mmap_self_3                                   0x117C0
#define __mmap_self_4                                   0x117C1
#define __mmap_self_5                                   0x117C2
#define __mmap_self_6                                   0x13F03F
#define __mmap_self_7                                   0x13F040
#define __mmap_self_8                                   0x13F041
#define __mmap_self_9                                   0x13F042
#define __mmap_self_10                                  0x13F043
#define __mmap_self_patch_0                             0xB0
#define __mmap_self_patch_1                             0x01
#define __mmap_self_patch_2                             0xC3
#define __mmap_self_patch_3                             0x31
#define __mmap_self_patch_4                             0xC0
#define __mmap_self_patch_5                             0x90
#define __call_samu                                     0x412860
#define __sysent                                        0x22CB570
#define __kernel_pmap_store                             0x22CB4F0  // Thanks to #J0nni3
#define __Starsha_UcodeInfo                             0
#define __gpu_devid_is_9924                             0x4DE010   // Thanks to #J0nni3
#define __gc_get_fw_info                                0x4D37A0   // Thanks to #J0nni3
#define __cnputc                                        0x3DFE90 // fixed.
#define __cnputs                                        0x3E0300 // fixed.
#define ___thread_lock_flags_check_result               0x402000
#define __spinlock_enter                                0x234A70 // fixed.
#define __spinlock_exit                                 0x234AB0 // fixed.
#define __critical_enter                                0x28E7A0 // fixed.
#define __critical_exit                                 0x28E7B0
#define __sched_bind                                    0x31FA50
#define __alloc_unr                                     0x156C60
#define __alloc_unrl                                    0x156670
#define __clean_unrhdrl                                 0x1561C0
#define __delete_unrhdr                                 0x1565C0
#define __sbuf_new                                      0x2124A0 // fixed.
#define __sbuf_delete                                   0x2125E0
#define __sbuf_vprintf                                  0x212B10
#define __sbuf_clear                                    0x212750 // fixed.
#define __sbuf_set_drain                                0x212790 // fixed.
#define __sbuf_trim                                     0x212E50 // fixed.
#define __sbuf_finish                                   0x212EC0 // fixed.
#define __sbuf_done                                     0x212FA0 // fixed.
#define __sbuf_data                                     0x212E60
#define __sbuf_len                                      0x212E70
#define __msgbuf_addchar                                0x402100 // fixed.
#define __msgbuf_addstr                                 0x26C500 // fixed.
#define __msgbuf_addstr_sub                             0x4022C0 // fixed
#define __log                                           0x435300
#define __kick_proc0                                    0x7210
#define __wakeup                                        0x3FB570
#define ___sx_sunlock_hard                              0xF6AB0 // fixed.
#define ___sx_sunlock                                   0xF5E00
#define __sx_xunlock                                    0x767690
#define __sx_xlock_hard                                 0xF5E10 // fixed.
#define __sx_init_flags                                 0xF5BB0 // fixed.
#define __sx_xlock                                      0x278BC0 // fixed.
#define ___sx_xlock_hard                                0xF60A0
#define __sx_destroy                                    0xF5C20 // fixed.
#define ___sleep                                        0x3FB0B0
#define ___thread_lock_flags_check_result               0x402000
#define ___thread_lock_flags                            0x402420 // fixed.
#define ___mtx_lock_sleep                               0x401CD0 // fixed.
#define ___mtx_unlock_sleep                             0x401FA0 // fixed.
#define __mutex_init                                    0x402780 // fixed.
#define __mtx_destroy                                   0x4027F0 // fixed.
#define __mtx_pool_find                                 0x3D1320 // fixed.
#define __mtx_pool_create                               0x3D1350 // fixed.
#define __umtx_thread_alloc                             0x47E050 // fixed.
#define __dce_flip_mtx_lock_again                       0x530B80 // fixed.
#define __lock_init                                     0x3C2920 // fixed.
#define __jump_lock_destroy                             0xA6480
#define __get_td_locks                                  0x767CE0
#define __kern_jail                                     0x3AB4C5
#define __priv_check_cred                               0x2FFBB0 // fixed.
#define __priv_check                                    0x2FFCD0 // fixed.
#define __prison_priv_check                             0x3B1D90
#define __panic0                                        0x3A3F00 // fixed.
#define __binuptime                                     0x1053C0 // fixed.
#define __nanouptime                                    0x105440 // fixed.
#define __microuptime                                   0x1054E0 // fixed.
#define __nanotime                                      0x105620 // fixed.
#define __getnanouptime                                 0x1057B0 // fixed.
#define __getmicrouptime                                0x1057E0 // fixed.
#define __initialize_dmem                               0x23DD50 // fixed.
#define __add_gpu_dmem                                  0x23FCB0 // fixed.
#define __add_gpu_private_dmem                          0x23FCE0 // fixed.
#define __load_prx                                      0x2AE5C0
#define __preload_prx_modules                           0x2AD260
#define __sceSblACMgrIsSystemUcred                      0x10F90 // fixed.
#define __sceSblACMgrIsNongameUcred                     0x10FB0 // fixed.
#define __sceSblACMgrIsDiagProcess                      0x10FC0
#define __sceSblACMgrIsCoredumpProcess                  0x11020 // fixed.
#define __sceSblACMgrIsDecidProcess                     0x11080
#define __sceSblACMgrIsMinisyscoreProcess               0x110D0
#define __sceSblACMgrIsSyscoreProcess                   0x11100 // fixed.
#define __sceSblACMgrIsShelluiProcess                   0x11120 // fixed.
#define __sceSblACMgrIsShellcoreProcess                 0x11140 // fixed.
#define __sceSblACMgrIsVideoplayerProcess               0x111C0 // fixed.
#define __sceSblACMgrIsDiskplayeruiProcess              0x111E0 // fixed.
#define __sceSblACMgrIsBdjavaProcess                    0x11200
#define __sceSblACMgrIsWebcoreProcess                   0x11220 // fixed.
#define __sceSblACMgrIsVdecProxyProcess                 0x112F0
#define __sceSblACMgrIsJitCompilerProcess               0x113B0
#define __sceSblACMgrIsJitApplicationProcess            0x11410
#define __sceSblACMgrIsAslrSeed2Process                 0x11480
#define __sceSblACMgrHasSceProgramAttribute             0x114A0 // fixed.
#define __sceSblACMgrHasDrmProcessAttribute             0x114B0
#define __sceSblACMgrHasUseVtrmCapability               0x114C0 // fixed.
#define __sceSblACMgrIsDiskplayerdiagProcess            0x114F0 // fixed.
#define __sceSblACMgrHasUseHp3dPipeCapability           0x11520 // fixed.
#define __sceSblACMgrHasUseVideoServiceCapability       0x11550 // fixed.
#define __sceSblACMgrHasSceProcessCapability            0x11560
#define __sceSblACMgrIsAllowedToUsePupUpdate0           0x11570 // fixed.
#define __sceSblACMgrIsVtrmadminProcess                 0x11600 // fixed.
#define __sceSblACMgrIsSoftwagnerProcess                0x11620
#define __sceSblACMgrIsDebuggableProcess                0x11640 // fixed.
#define __sceSblDiagOSUpdaterProcess                    0x116E0
#define __sceSblACMgrIsAllowedSystemLevelDebugging      0x11730 // fixed.
#define __sceSblACMgrHasMmapSelfCapability              0x117B0
#define __sceSblACMgrIsAllowedToMmapSelf                0x117C0
#define __sceSblACMgrGetPathId                          0x117E0 // fixed.
#define __sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall 0x11A00
#define __sceSblACMgrGetDeviceAccessType                0x11B00 // fixed.
#define __sceSblACMgrIsAllowedCoredump                  0x11750
#define __sceSblACMgrIsAllowedDrmProcessDebugging       0x116A0
#define __sceSblACMgrIsFirstImageWriterProcess          0x112B0
#define __sceSblACMgrIsMountfusefsProcess               0x11060
#define __sceSblRcMgrIsAllowPsmDebug                    0x1CFC80
#define __sceSblRcMgrIsAllowRegistryAccess              0x1CFE00 // fixed.
#define __sceSblRcMgrIsAllowRegistryAccessMini          0x1CFD60
#define __sceSblRcMgrIsAllowInitVtrm                    0x1CFEB0
#define __sceSblRcMgrIsAllowSLDebugger                  0x1CFF20
#define __sceSblRcMgrIsAllowAdClock                     0x1CFF90
#define __sceSblRcMgrIsStoreMode                        0x1D0040
#define __sceSblRcMgrIsAllowULDebugger                  0x1D01B0
#define __sceSblRcMgrIsAllowCreateDebuggingDeviceFile   0x1D0220
#define __sceSblRcMgrIsSoftwagnerQafForAcmgr            0x1D0290
#define __sceSblRcMgrIsAllowDisablingAslr               0x1D0300
#define __sceSblRcMgrIsAllowDebugSoftWagnerClock        0x1D03E0
#define __sceSblRcMgrIsAllowDataExecutionInFselfProcess 0x1D09A0
#define __sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp 0x1D0A10
#define __sceSblAIMgrIsDevKit                           0x4DE010 // fixed.
#define __sceSblAIMgrIsTool                             0x4289A0
#define __sceSblAIMgrIsTestKit                          0x428A80
#define __sceSblAIMgrIsDex                              0x428AF0
#define __sceSblAIMgrIsCex                              0x428F30 // fixed.
#define __sceSblAIMgrIsDiag                             0x428D90
#define __sceSblAIMgrGetProductCode                     0x428ED0
#define __sceRegMgrSetInt                               0x4F8D10 // fixed.
#define __regMgrComApiPrologue                          0x4F8EE0 // fixed.
#define __regMgrComCheckParam                           0x4F8F20 // fixed.
#define __regMgrComSetReg                               0x4F9340 // fixed.
#define __regMgrComCheckAddr                            0x4F9F20 // fixed.
#define __regMgrInitRegistry                            0x4FB6D0 // fixed.
#define __regMgrCheckKey                                0x4FC230 // fixed.
#define __regMgrCnvRegId                                0x506960 // fixed.
#define __regMgrSetInformationType                      0x507280 // fixed.
#define __regMgrSetInformation                          0x507680 // fixed.
#define __regMgrEvtInit                                 0x50D510 // fixed.
#define __sceKernelCheckDipsw                           0x653D70 // fixed.
#define __getSelfContentId                              0x63CBE0 // fixed.
#define __sceKernelIsDevelopmentMode                    0x654250
#define __sceKernelIsAssistMode                         0x654280
#define __sceSblGetKernelCrashDumpEncKey                0x62F430 // fixed.
#define ___sceSblAuthMgrSmStart                         0x641500
#define ___sceSblAuthMgrSmIsLoadable2                   0xF5FD0 // fixed.
#define ___sceSblAuthMgrSmVerifyPupExpiration           0x643EE0
#define ___sceSblAuthMgrCheckSelfHeader                 0x63CC50 // fixed.
#define __sceSblAuthMgrAuthHeader                       0x63E600 // fixed.
#define ___sceSblKeyMgrSmStart                          0x62E7F0
#define __sceSblKeymgrDropKey                           0x62E210 // fixed.
#define ___sceSblKeyMgrSmLoad                           0x62EBD0 // fixed.
#define ___sceSblKeyMgrInitialize                       0x62ED30 // fixed.
#define __sceSblVtrmRestore                             0x6466B0
#define __sceSblVtrmBackup                              0x646640
#define ___sceSblVtrmInitialize                         0x644A80 // fixed.
#define ___sceSblVtrmInitializeCommon                   0x644AA0 // fixed.
#define __sceSblDriverMapPages                          0x61B9B0
#define ___sceSblDriverGvmInitialize                    0x61BD30 // fixed.
#define __sceSblDriverUnmapPages                        0x61C530 // fixed.
#define __sceSblDriverGetMappedPageListsWithCpuAddr     0x61C780 // fixed.
#define __sceSblDriverRegisterMsgHandler                0x61CAC0 // fixed.
#define __sceSblDriverInitialize                        0x61CBB0 // fixed.
#define __sceSblSrtcSetTime                             0x633D60 // fixed.
#define __sceSblSrtcGetTime                             0x633F00 // fixed.
#define __sceSblSrtcGetDebugTime                        0x634440 // fixed.
#define __sceSblSrtcSetDebugTime                        0x634450 // fixed.
#define __sceSblSrtcClearDebugTime                      0x634460 // fixed.
#define ___sceSblSrtcSetTimeDifference                  0x62AA80 // fixed.
#define __sceSblServiceSpawn                            0x6323F0 // fixed.
#define __sceSblServiceCrypt                            0x6242F0 // fixed.
#define __sceSblServiceCryptAsync                       0x624480 // fixed.
#define ___sceSblServiceOsPartGetOsPartStartSector      0x6309E0 // fixed.
#define ___sceSblServiceOsPartGetEntry                  0x630AF0 // fixed.
#define ___sceSblServiceRequest                         0x630CA0 // fixed.
#define ___sceSblServiceRequestInitialize               0x630F90 // fixed.
#define ___sceSblServiceIoInitialize                    0x631250 // fixed.
#define __sceSblServiceMailbox                          0x6308C0
#define __sceSblSsGenerateSealedKey                     0x61F4D0 // fixed.
#define __sceSblSsDecryptSealedKey                      0x61F680 // fixed.
#define __getSealedKeySecret                            0x61F800 // fixed.
#define ___sceSblSsSealedkeyIoctl                       0x61FD00 // fixed.
#define ___sceSblSsRootparamIoctl                       0x6270D0 // fixed.
#define __sceSblSsDecryptWithPortability                0x627E70 // fixed.
#define ___sceSblSsEncDecServIoctl                      0x628430 // fixed.
#define ___sceSblSsEncDecServInitialize                 0x6285C0 // fixed.
#define ___sceSblSsKeystoneIoctl                        0x628AA0 // fixed.
#define __sceSblSsMemcmpConsttime                       0x629E80 // fixed.
#define __sceSblSsMemset                                0x629ED0 // fixed.
#define ___sceSblSsUtilsIoctl                           0x62B4C0 // fixed.
#define ___sceSblSsUtilsInitialize                      0x62B7F0 // fixed.
#define __sceSblContentIdUtilB32ToStr                   0x313F00 // fixed.
#define ___sceSblCrepoInitialize                        0x624C00 // fixed.
#define __sceSblMbrLoadMbrFromBankHeader                0x630860 // fixed.
#define __SceBdemu                                      0x61B240
#define __s_npdrmCdev                                   0x64D230
#define __DriftUpdate2                                  0x6423E0
#define __vm_phys_paddr_to_vm_page                      0x298980
#define __vm_page_unhold_pages                          0x264520
#define __vm_page_free_toq                              0x264290
#define __vfs_getnewfsid                                0x278FB0
#define __vfs_mountedfrom                               0x1E1550
#define __vfs_donmount                                  0x1DE2A0
#define __vfs_buildopts                                 0x1DDC90
#define __vflush                                        0x27BE30
#define __vslock                                        0x6420 // fixed.
#define __vsunlock                                      0x6550
#define __pmap_kextract                                 0x2E0620
#define __uma_large_free                                0x22B670
#define __uma_zfree_arg                                 0x22A290
#define __uma_zalloc_arg                                0x2299C0
#define __audit_arg_fd                                  0x19D090
#define __currecord                                     0x3F94B0
#define __audit_arg_cmd                                 0x19D890
#define __malloc                                        0x10E250 // fixed.
#define __free                                          0x10E460 // fixed.
#define __memcpy                                        0x1EA530 // fixed.
#define __memset                                        0x3205C0 // fixed.
#define __memcmp                                        0x50AC0 // fixed.
#define __bcopy                                         0xAB0 // fixed.
#define __strtol                                        0x218760
#define __prison0                                       0x10986A0                         
#define __rootvnode                                     0x22C1A70
#define __pml4pml4i                                     0x22CB4E0
#define __dmpml4i                                       0x22CB564  // Thanks to #J0nni3
#define __dmpdpi                                        0x22CB568  // Thanks to #J0nni3
#define __gbase_samu_initialize                         0x4BB100 // fixed.
#define __gbase_samu_finalize                           0x4BB3B0 // fixed.
#define __gbase_samu_map                                0x4BB590 // fixed.
#define __gbase_samu_unmap                              0x4BB5A0 // fixed.
#define __gbase_samu_read_register                      0x4BB5D0 // fixed.
#define __gbase_samu_write_register                     0x4BB5F0 // fixed.
#define __gbase_samu_ix_read_register                   0x4BB610 // fixed.
#define __gbase_samu_ix_write_register                  0x4BB6A0 // fixed.
#define __gbase_samu_pa_to_gpuva                        0x4BB730 // fixed.
#define __gbase_samu_unk_func                           0x4BB760 // fixed.
#define __gbase_samu_handler                            0x61CAE0
#define __gbase_samu_cmd                                0x767C30
#define __gbase_get_cu_redundant                        0x2DCBC0 // fixed.
#define __gbase_initialize                              0x4C05D0 // fixed.
#define __gbase_finalize                                0x4C0970 // fixed.
#define __gbase_resume_vm_space                         0x4C09B0 // fixed.
#define __gbase_write_register                          0x4C0F40 // fixed.
#define __gbase_alloc_vm_space                          0x4C1060 // fixed.
#define __gbase_free_vm_space                           0x4C1650 // fixed.
#define __gbase_unmap                                   0x4C1FA0 // fixed.
#define __gbase_read_register                           0x4C2CF0 // fixed.
#define __gbase_map                                     0x4C3580 // fixed.
#define __gbase_vtophys                                 0x4C3DF0 // fixed.
#define __gbase_get_pid                                 0x4C42E0 // fixed.
#define __gbase_map_to_vmid0                            0x4C5630 // fixed.
#define __gbase_dump_page_tables                        0x4C6100 // fixed.
#define __gbase_reset_vm_hw                             0x4C6A70 // fixed.
#define __gbase_cail_init                               0x4CF470 // fixed.
#define __gbase_init_vbios_call                         0x4D2180 // fixed.
#define __gbase_set_cu_power_gate                       0x4D2C40 // fixed.
#define __gbase_dump_dbg_regs_for_user                  0x4D4C30 // fixed.
#define __gbase_dump_dbg_common_regs                    0x4D6300 // fixed.
#define __gbase_dump_dbg_me0_indexed_regs               0x4D6430 // fixed.
#define __gbase_initialize_vmid0_va_allocator           0x4DF1F0 // fixed.
#define __gbase_reset_ih_hw                             0x4EBF40 // fixed.
#define __gbase_initialize_ih                           0x4EC070 // fixed.
#define __gbase_unregister_interrupt_handler            0x4EC500 // fixed. 
#define ____stack_chk_fail                              0x6C53B0
#define __kvprintf                                      0x4364F0 // fixed.
#define __strlcpy                                       0x20630  // fixed.
#define __strcmp                                        0x1D0FD0 // fixed.
#define __strncpy                                       0x3C0B0  // fixed.
#define __strlen                                        0x3B71A0 // fixed.
#define __strnlen                                       0xE2620  // fixed.
#define __strdup                                        0x1C1C30 // fixed.
#define __strstr                                        0x17DEA0
#define __strncmp                                       0x1B8FE0 // fixed.
#define __strcasecmp                                    0xA6250
#define __vstrcmp                                       0x1D0EC0
#define __genNewActDat                                  0x64B470
#define __preLoad                                       0x6323C0
#define __gen_mem_keys                                  0x6324E0                  
#define __VtrmUtilLockInitialize                        0x644490 // fixed.
#define __VtrmUtilLockFinialize                         0x6444B0 // fixed.
#define __VtrmUtilLock                                  0x6444D0 // fixed.
#define __VtrmUtilUnlock                                0x644500 // fixed.
#define __VTRM_CONTIGMALLOC                             0x644550 // fixed.
#define __VtrmUtilFillVtrmHeaderDefault                 0x6445E0 // fixed.
#define __SceSflash                                     0x34DFF0 // fixed.
#define __sflash_pread_for_vtrm                         0x34E360 // fixed.
#define __sflash_erase_for_vtrm                         0x34E490 // fixed.
#define __sflash_pwrite_for_vtrm                        0x34E2F0 // fixed.
#define __sflash_pread_for_vtrm2                        0x34E3E0 // fixed.
#define ___is_vtrm_access_ok                            0x646810
#define __vtrm_erase_sector                             0x101781C0 // fixed.
#define __vtrm_pread                                    0x101781E0 // fixed.
#define __vtrm_pwrite2                                  0x10178240 // fixed.
#define __vtrm_pwrite                                   0x6443C0
#define __icc_query                                     0x43540 // fixed.
#define __icc_query_nowait                              0x44020 // fixed.
#define __icc_send                                      0x44160 // fixed.
#define __icc_nvs_write                                 0x395380
#define __icc_nvs_read                                  0x395460
#define __icc_nvs_drvinit                               0x395550
#define __icc_nvs_init                                  0x395590
#define __icc_snvs_read_sector                          0x26CA40 // fixed.
#define __icc_snvs_write_sector                         0x26CB00 // fixed.
#define __icc_snvs_get_write_count                      0x26CBC0 // fixed.
#define __icc_init_snvs_write_lock                      0x26C9A1
#define __icc_lock_snvs_write                           0x26CCA0 // fixed.
#define __icc_unlock_snvs_write                         0x26CCC0 // fixed.
#define __icc_button_init                               0x41FE70 // fixed.
#define __icc_button_set_state                          0x4200E0 // fixed.
#define __icc_button_get_state                          0x41FC40
#define __icc_indicator_set_dynamic_led_boot            0x104390 // fixed.
#define __icc_indicator_set_dynamic_led_off             0x104440 // fixed.
#define __icc_indicator_set_thermalalert_led_on         0x1044D0 // fixed.
#define __icc_indicator_set_thermalalert_led_off        0x104560 // fixed.
#define __icc_indicator_set_dynamic_led_standby_boot    0x105260 // fixed.
#define __icc_indicator_set_dynamic_led_shutdown        0x105120 // fixed.
#define __icc_indicator_check_thermal_on_boot           0x104FF0 // fixed.
#define __icc_indicator_get_dynamic_led                 0x104E20 // fixed.
#define __icc_indicator_set_dynamic_led                 0x104CA0 // fixed.
#define __icc_indicator_get_led                         0x104B20 // fixed.
#define __icc_indicator_set_led                         0x1049B0 // fixed.
#define __icc_indicator_set_buzzer                      0x1042E0 // fixed.
#define __icc_indicator_set_dynamic_led_standby         0x104510
#define __icc_indicator_drvinit                         0x104620
#define __icc_thermal_alert_occurred_when_boot          0xA83B0 // fixed.
#define __icc_thermal_alert_occurred_when_boot_sub      0xA83D0 // fixed.
#define __icc_thermal_alert_status_off                  0xA84A0 // fixed.
#define __icc_thermal_notify_handler                    0xA85A0 // fixed.
#define __icc_thermal                                   0xA8650 // fixed.
#define __icc_thermal_enable_disable_notify             0xA87A0 // fixed.
#define __icc_thermal_get_thermal_state                 0xA8860 // fixed.
#define __icc_thermal_stop_td                           0xA88E0 // fixed.
#define __icc_thermal_resume_td                         0xA8A10 // fixed.
#define __icc_thermal_set_notify                        0xA8570
#define __icc_thermal_drvinit                           0xA8920
#define __icc_fan_drvinit                               0x15C790
#define __icc_fan_get_fan_manual_duty                   0x15C810
#define __icc_sc_fw_update_handler                      0x401460 // fixed.
#define __icc_sc_configuration_set_code_flash_security  0x4018F0 // fixed.
#define __icc_sc_configuration_get_code_flash_security  0x401A20 // fixed.
#define __icc_sc_fw_update                              0x4015A0 // fixed.
#define __icc_configuration_get_firmware_version        0x434260 // fixed.
#define __icc_configuration_get_bd_address              0x434280 // fixed.
#define __icc_configuration_set_bd_address              0x434320 // fixed.
#define __icc_configuration_clear_bd_address            0x4343E0 // fixed.
#define __icc_configuration_get_ddr_capacity            0x434450 // fixed.
#define __icc_configuration_save_context                0x434500 // fixed.
#define __icc_configuration_load_context                0x434630 // fixed.
#define __icc_configuration_get_devlan_setting          0x434730 // fixed.
#define __icc_configuration_set_devlan_setting          0x4347D0 // fixed.
#define __icc_configuration_clear_cpu_info_bit          0x434860 // fixed.
#define __icc_register_notification_handler             0x0
#define __make_dev                                      0x1B9360 // fixed.
#define __destroy_dev                                   0x1B9C40
#define __make_dev_credf                                0x1B93F0 // fixed.
#define __dev_lock                                      0x1B9020 // fixed.
#define __dev_unlock                                    0x1B9040 // fixed.
#define __dev_refl                                      0x1B90B0 // fixed.
#define __sce_cam_error_log_init                        0x5C020
#define __sce_cam_error_log_read                        0x5C0D0
#define __sce_cam_error_log_write                       0x5C260
#define __sce_cam_error_put                             0x5C290
#define __sce_exfatfs_vop_readdir                       0x117EF0
#define __fork1                                         0x207D0
#define __mdbg_kproc_hook_create                        0x3A48D0
#define __metadbg_log_add                               0xE7DF0
#define __cpu_set_fork_handler                          0x2740B0
#define __sched_user_prio                               0x31EFA0
#define __bioq_init                                     0x3203C0
#define __disk_alloc                                    0x2DC9B0
#define __disk_create                                   0x2DC9F0
#define __eventhandler_find_list                        0x1EC870
#define __eventhandler_prune_list                       0x1EC910
#define __eventhandler_deregister                       0x1EC680
#define __eventhandler_register                         0x1EC2F0
#define __eventhandler_suspend                          0x1EC600
#define __dump_pfs                                      0x69F260
#define __dump_sign                                     0x69F4A0
#define __pfs_savedata                                  0x62FC40 // fixed.
#define __pfslowtimo                                    0x1AB7F0 // fixed.
#define __pfs_fileno_init                               0x3A3CC0 // fixed.
#define __pfs_fileno_uninit                             0x3A3D00 // fixed.
#define __pfs_fileno_alloc                              0x3A3D30 // fixed.
#define __pfs_fileno_free                               0x3A3DA0 // fixed.
#define __pfs_vncache_load                              0x464C70 // fixed.
#define __pfs_vncache_unload                            0x464D50 // fixed.
#define __pfs_vncache_alloc                             0x464D90 // fixed.
#define __pfs_purge                                     0x465200 // fixed.
#define __tmpfs_alloc_node                              0x4A6A50 // fixed.
#define __tmpfs_free_node                               0x4A6CB0 // fixed.
#define __tmpfs_alloc_dirent                            0x4A6E50 // fixed.
#define __tmpfs_free_dirent                             0x4A6ED0 // fixed.
#define __tmpfs_alloc_vp                                0x4A6F10 // fixed.
#define __tmpfs_free_vp                                 0x4A7280 // fixed.
#define __tmpfs_dir_attach                              0x4A7530 // fixed.
#define __tmpfs_dir_getdents                            0x4A78D0 // fixed.
#define __tmpfs_chflags                                 0x4A7D40 // fixed.
#define __tmpfs_nocacheread                             0x4AA540 // fixed.
#define __pfs_sbl_sig_verify                            0x629220 // fixed.
#define __pfs_raw_write                                 0x689040 // fixed.
#define __pfs_fill_idblk_cache                          0x68EFB0 // fixed.
#define __pfs_alloc_idblk_cache                         0x699C60 // fixed.
#define __pfs_refer_to_idblk_table                      0x699EC0 // fixed.
#define __pfs_sbl_fini                                  0x6A0E20 // fixed.
#define __pfs_generate_icv                              0x6A12D0 // fixed.
#define __pfs_sbl_inflate_init                          0x6A2CD0 // fixed.
#define ___pfs_format                                   0x6A42D0 // fixed.
#define ___pfs_format_backup_superblock                 0x6A4CE0 // fixed.
#define ___pfs_format_init                              0x6A4F30 // fixed.
#define __pfs_format                                    0x6A5330 // fixed.
#define ___pfs_compaction                               0x6AD530 // fixed.
#define __pfs_sbram_write_metadata                      0x6B0BD0 // fixed.
#define __pfs_lookup_ino                                0x6B1B20 // fixed.
#define __pfs_dirrewrite                                0x6BC290 // fixed.
#define __pfs_create_dir                                0x23C5A0 // fixed.
#define __pfs_create_dir_sub                            0x23C6D0 // fixed.
#define __pfs_create_file                               0x23C870 // fixed.
#define __pfs_create_link                               0x23C980 // fixed.
#define __pfs_destroy                                   0x23CB20 // fixed.
#define __pfs_mapped_read                               0x69B060 // fixed.
#define __pfs_mapped_write                              0x69B520 // fixed.
#define __pfs_checkpath                                 0x6B3050 // fixed.
#define __pfs_mount                                     0x23CCB0 // fixed.
#define __pfs_cmount                                    0x23CD80 // fixed.
#define __pfs_unmount                                   0x23CD90 // fixed.
#define __pfs_root                                      0x23CDB0 // fixed.
#define __pfs_statfs                                    0x23CDD0 // fixed.
#define __pfs_init                                      0x23CDE0 // fixed.
#define __pfs_uninit                                    0x23CEC0 // fixed.
#define __pfs_check_addr                                0x6A0070
#define __pfs_find_node                                 0x23C980
#define __p_candebug                                    0x56B70
#define __procfs_candebug                               0x1620B0
#define __procfs_init                                   0x1620E0
#define __procfs_doprocfile                             0x161E80
#define __procfs_docurproc                              0x161F70
#define __procfs_attr                                   0x161FA0
#define __procfs_notsystem                              0x162090
#define __procfs_doosrel                                0x309220
#define __procfs_doprocstatus                           0xBE8E0 // fixed.
#define __procfs_doproccmdline                          0xBED90
#define __procfs_doprocctl                              0x4615F0 // fixed.
#define __procfs_doprocdbregs                           0x3D1950
#define __procfs_doproctype                             0x31E880 // fixed.
#define __procfs_doprocfpregs                           0x163C0
#define __procfs_doprocmap                              0x53B40 // fixed.
#define __procfs_doprocmem                              0x4890D0
#define __procfs_ioctl                                  0x46A7A0 // fixed.
#define __procfs_close                                  0x46A5E0
#define __procfs_doprocnote                             0x3A2510 // fixed.
#define __procfs_doprocregs                             0xF6A60
#define __procfs_doprocrlimit                           0x197620
#define __bd_sched_private_done                         0x619900
#define __bd_sched_post_done                            0x618D40
#define __bd_sched_copier_handle_resume_event           0x618C40
#define __bd_sched_copier_handle_suspend_event          0x618A90
#define __bd_sched_writer_handle_resume_event           0x618000
#define __bd_sched_writer_handle_suspend_event          0x617E40
#define __bd_sched_get_bdsio_in_cache                   0x6178E0
#define __bd_sched_get_bdsio_in_writer                  0x617A20
#define __get_pkg_offset                                0x617850
#define __get_img_offset                                0x617640
#define __bd_sched_is_available_on_hdd2                 0x617780 // fixed.
#define __bd_sched_update_bitmap_cache2                 0x617380 // fixed.
#define __bd_sched_lock_bm_file                         0x616EA0
#define __bd_sched_unlock_bm_file                       0x617970 // fixed.
#define __bd_sched_read_bd_dev                          0x617B30 // fixed.
#define __bd_sched_update_bitmap_file                   0x618A50 // fixed.
#define __bd_sched_init_bg_copy_req_pool                0x60DBC0 // fixed.
#define __bd_sched_pop_bg_copy_req                      0x60DCB0 // fixed.
#define __bd_sched_push_bg_copy_req                     0x60DDF2 // fixed.
#define __bd_sched_init_bdsio_pool                      0x60DE40 // fixed.
#define __bd_sched_hdd_writer                           0x614D00 // fixed.
#define __bd_sched_bd_reader                            0x616610 // fixed.
#define __bd_sched_bd_reader_bg_copy                    0x616880
#define __bd_sched_bd_reader_ripping                    0x616230
#define __bd_sched_shutdown_copier                      0x615AE0
#define __bd_sched_shutdown_w_hdd                       0x615C80
#define __bd_sched_shutdown_r_bd_internal               0x615E20
#define __bd_sched_reader_handle_suspend_event          0x615FC0
#define __bd_sched_reader_handle_resume_event           0x616130
#define __bd_sched_adjust_bio_completed                 0x619BC0
#define __g_bdemu_create                                0x61A400 // fixed.
#define __g_bdemu_destroy                               0x61AD30 // fixed.
#define __bdemu_speed_emu                               0x61B4D0
#define __g_new_geomf                                   0xB60C0
#define __g_part_check_integrity                        0x72380 // fixed.
#define __g_part_ctl_commit                             0x72C80 // fixed.
#define __g_part_ctl_setunset                           0x735C0 // fixed.
#define __g_load_class                                  0xB58C0 // fixed.
#define __g_modevent                                    0xB5AF0 // fixed.
#define __g_retaste_event                               0xB5E80 // fixed.
#define __g_destroy_geom                                0xB6210 // fixed.
#define __g_wither_geom                                 0xB6370 // fixed.
#define __g_wither_geom_close                           0xB6410 // fixed.
#define __g_access                                      0xB64F0 // fixed.
#define __g_detach                                      0xB69B0 // fixed.
#define __g_destroy_consumer                            0xB6A90 // fixed.
#define __g_new_consumer                                0xB6B70 // fixed.
#define __g_attach                                      0xB7090 // fixed.
#define __g_handleattr                                  0xB7480 // fixed.
#define __g_std_done                                    0xB7660 // fixed.
#define __g_conftxt_geom                                0x16A030 // fixed.
#define __g_slice_dumpconf                              0x196990 // fixed.
#define __g_slice_config                                0x196AE0 // fixed.
#define __g_slice_spoiled                               0x196E50 // fixed.
#define __g_slice_destroy_geom                          0x196EF0 // fixed.
#define __g_slice_orphan                                0x197190 // fixed.
#define __g_disk_create                                 0x2DCE90 // fixed.
#define __g_new_bio                                     0x32E830 // fixed.
#define __g_destroy_bio                                 0x32E8B0 // fixed.
#define __g_clone_bio                                   0x32E920 // fixed.
#define __g_io_getattr                                  0x32EB60 // fixed.
#define __g_io_request                                  0x32EC70 // fixed.
#define __g_io_flush                                    0x32EEB0 // fixed.
#define __g_io_deliver                                  0x32F120 // fixed.
#define __g_io_schedule_down                            0x32F320 // fixed.
#define __g_print_bio                                   0x32FEF0 // fixed.
#define __g_init                                        0x380170 // fixed.
#define __g_ctl_init                                    0x391440 // fixed.
#define __g_ctl_req                                     0x3921F0 // fixed.
#define __g_waitidle                                    0x3944B0 // fixed.
#define __g_orphan_provider                             0x394700 // fixed.
#define __g_run_events                                  0x394950 // fixed.
#define __g_post_event_x                                0x395110 // fixed.
#define __g_do_wither                                   0x3952B0 // fixed.
#define __g_waitfor_event                               0x395300 // fixed.
#define __g_event_init                                  0x395430 // fixed.
#define __g_vfs_done                                    0x3B3930 // fixed.
#define __g_label_create                                0x3EC540 // fixed.
#define __g_obi_destroy                                 0x461FA0 // fixed.
#define __g_obi_read                                    0x462000 // fixed.
#define __g_obi_write                                   0x462160 // fixed.
#define __g_crypt_dumpconf                              0x652CE0 // fixed.
#define __g_limiter_start                               0x6E48E0 // fixed.
#define __sched_prio                                    0x31EA30
#define __securelevel_gt                                0x56650
#define __securelevel_ge                                0x56660
#define __AesEncKeySet 		     			            0x410940 // fixed.
#define __AesDecKeySet 		     			            0x411040 // fixed.
#define __AesEncrypt  		     			            0x411090 // fixed.
#define __AesDecrypt  		     			            0x4110D0 // fixed.
#define __AesCbcCfb128Decrypt	     		 	 	    0x3A2E00 // fixed.

/*--------------------------------------------------------------------
Need fix
--------------------------------------------------------------------*/
#define __copystr                                       0x0
#define __device_printf                                 0x0
#define __g_obi_create                                  0x0
#define __g_trace                                       0x0
#define __g_provider_by_name                            0x0
#define __g_alloc_bio                                   0x0
#define __g_cancel_event                                0x0
#define __g_post_notification_for_bio                   0x0
#define __firmware_get                                  0x0
#define __memmove                                       0x0
#define __kthread_add					     		    0x0
#define __id_set                                        0x0
#define __id_unlock                                     0x0
#define __id_wlock                                      0x0
#define __vm_map_lock_read		     				    0x0
#define __vm_map_lookup_entry	     				    0x0
#define __vm_map_unlock_read	     				    0x0
#define __vmspace_free			     			        0x0
#define __vmspace_acquire_ref     				        0x0
#define __bzero			     						    0x0
#define __pagezero 		     						    0x0
#define __bcmp			     						    0x0
#define __getbinuptime                                  0x0
#define __casuword32	     				            0x0
#define __mi_switch 						     	    0x0
#define __eventlog_write_crash			     		    0x0
#define __metadbg_perform_dump_on_panic                 0x0
#define __savectx                                       0x0
#define __savectx2                                      0x0
#define __ktrcsw                                        0x0
#define ___sx_sunlock_hard_sub                          0x0
#define __hexdump                                       0x0
#define __msgbufinit                                    0x0
#define __msgbuf_copy                                   0x0
#define __msgbuf_reinit                                 0x0
#define __icc_sc_config_drvinit                         0x0
#define __get_icc_max                                   0x0
#define __icc_wdt_stop                                  0x0
#define __devstat_new_entry                             0x0
#define __devstat_start_transaction                     0x0
#define __ssb_rtc_init_exclock                          0x0
#define __uma_large_free_error                          0x0
#define __lgdt                                          0x0
#define __longjmp                                       0x0
#define __index                                         0x0
#define __unref_dag                                     0x0
#define __sceSblPfsKeymgrGenEKpfsForGDGPAC     		    0x0
#define __sceSblPfsSetKey			     			    0x0
#define __sceSblPfsClearKey		     				    0x0
#define __sceSblKeymgrClearKey                          0x0
#define __gs_is_update_bio_ioprio                       0x0
#define __sceSblAuthMgrIsLoadable                       0x0
#define __sceSblAuthMgrFinalize                         0x0
#define __sceSblAuthMgrLoadBlock                        0x0
#define ___sceSblAuthMgrGetElfSegmentInformation        0x0
#define ___sceSblAuthMgrLoadSelfBlock                   0x0
#define ___sceSblAuthMgrGetSelfSegmentInformation       0x0
#define ___sceSblAuthMgrGetSelfInfo                     0x0
#define ___sceSblAuthMgrSmDriveClearKey                 0x0
#define ___sceSblAuthMgrSmDriveClearSessionKey          0x0
#define ___sceSblAuthMgrSmDriveData                     0x0
#define ___sceSblAuthMgrSmVerifyActCodeCommon           0x0
#define ___sceSblAuthMgrSmGenActRequest                 0x0
#define ___sceSblAuthMgrSmGenActHeader                  0x0
#define ___sceSblAuthMgrSmGenPassCodeData               0x0
#define ___sceSblAuthMgrSmCheckPassCodeData             0x0
#define __sceSblAuthMgrSmSpawn                          0x0
#define __sceSblAuthMgrIsLoadable2	     			    0x0
#define __sceSblAuthMgrVerifyHeader	     		        0x0
#define __sceSblAuthMgrGetSelfInfo     			 	    0x0
#define __sceSblAuthMgrSmStart     				  	    0x0
#define __sceSblKeymgrSmCallfunc	     			    0x0
#define __SceDaAtcev                                    0x0
#define __SceHidAuth                                    0x0
#define __AesCbcCfb128Encrypt	     		 		    0x0
#define __Sha256HmacFinal	     			            0x0
#define __Sha256Hash     	     			            0x0
#define __Sha256Hmac      	     			            0x0
#define __Sha256OneBlk   	     			            0x0
#define __Sha256FinalCore 	     			            0x0
#define __Sha256Init     	     			            0x0
#define __Sha256Transform	     			            0x0
#define __Sha256Final          				            0x0
#define __Sha256HmacInit       				            0x0
#define __sceSblRngGetRandomNumber				        0x0
#define __Fips186Prng 				                    0x0
#define __RsaesPkcs1v15Enc2048		     			    0x0
#define __RsaesPkcs1v15Dec2048CRT     				    0x0
#define __allproc	     					            0x0
#define __M_TEMP				     		            0x0
#define __fpu_kern_ctx					                0x0
#define __sched_switch                                  0x0
#define __kthread_suspend_check                         0x0
#define __kproc_kthread_add                             0x0
#define __gbase_samu_mailbox_writer                     0x0
#define __gbase_samu_mailbox_reader                     0x0

#endif