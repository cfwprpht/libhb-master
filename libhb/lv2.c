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
#include <string.h>

#define X86_CR0_WP (1 << 16)

// Kernel Console Userland Buffer. It's set up here to ensure it's really a userland buffer.
static char kcon_usrld_buffer[1024];
static char kcon_usrld_error_buffer[1024];
static char kcon_usrld_warning_buffer[1024];
static char kcon_input_buffer[10];

static volatile int _global_test = 0;
extern uint8_t _start[], _end[];
KernSymTable lv2;
KConSymTable kconsole;

unsigned int long long __readmsr(unsigned long __register) {
	// Loads the contents of a 64-bit model specific register (MSR) specified in
	// the ECX register into registers EDX:EAX. The EDX register is loaded with
	// the high-order 32 bits of the MSR and the EAX register is loaded with the
	// low-order 32 bits. If less than 64 bits are implemented in the MSR being
	// read, the values returned to EDX:EAX in unimplemented bit locations are
	// undefined.
	unsigned long __edx;
	unsigned long __eax;
	__asm__("rdmsr" : "=d"(__edx), "=a"(__eax) : "c"(__register));
	return (((unsigned int long long)__edx) << 32) | (unsigned int long long)__eax;
}

// Kernel Console Hooked printf.
void hookprintf(const char *format, ...) {
	// Setup a Kernel Buffer.
	char buff[1024];

	// Setup a variadict list for the arguments.
	va_list args;

	// Load Format and arguments.
	va_start(args, format);

	// Copy all together.
	lv2.vsnprintf(buff, sizeof(buff), format, args);

	// Copy Kernel buffer into Userland.
	lv2.copyout(&buff, &kcon_usrld_buffer, lv2.strlen(buff));

	// Free variadict list.
	va_end(args);

	// Sleep a bit.
	lv2.csleep();
}

// Kernel Console WriteLine.
void WriteLine(const char *format, ...) {
	// Setup a Kernel Buffer.
	char buff[1024];

	// Setup a variadict list for the arguments.
	va_list args;

	// Load Format and arguments.
	va_start(args, format);

	// Copy all together.
	lv2.vsnprintf(buff, sizeof(buff), format, args);

	// Copy Kernel buffer into Userland.
	lv2.copyout(&buff, &kcon_usrld_buffer, lv2.strlen(buff));

	// Free variadict list.
	va_end(args);

	// Give the Application loop some time to write our message to the screen.
	lv2.csleep();
}

// Kernel Console WriteError.
void WriteError(const char *format, ...) {
	// Setup a Kernel Buffer.
	char buff[1024];

	// Setup a variadict list for the arguments.
	va_list args;

	// Load Format und arguments.
	va_start(args, format);

	// Copy all together.
	lv2.vsnprintf(buff, sizeof(buff), format, args);

	// Copy Kernel buffer into Userland.
	lv2.copyout(&buff, &kcon_usrld_error_buffer, lv2.strlen(buff));

	// Free variadict list.
	va_end(args);

	// Give the Application loop some time to write our message to the screen.
	lv2.csleep();
}

// Kernel Console WriteWarning.
void WriteWarning(const char *format, ...) {
	// Setup a Kernel Buffer.
	char buff[1024];

	// Setup a variadict list for the arguments.
	va_list args;

	// Load Format und arguments.
	va_start(args, format);

	// Copy all together.
	lv2.vsnprintf(buff, sizeof(buff), format, args);

	// Copy Kernel buffer into Userland.
	lv2.copyout(&buff, &kcon_usrld_warning_buffer, lv2.strlen(buff));

	// Free variadict list.
	va_end(args);

	// Give the Application loop some time to write our message to the screen.
	lv2.csleep();
}

// Kernel Console do Line Break.
void LineBreak(void) {
	// Setup a Kernel Buffer.
	char buff[3] = "\n";

	// Copy Kernel buffer into Userland.
	lv2.copyout(&buff, &kcon_usrld_buffer, lv2.strlen(buff));
}

// Copys the message to the overloaded buffer.
unsigned int getMsg(char *buff) {
	unsigned int len = strlen(kcon_usrld_buffer);
	if (len > 0) {		
		memcpy(buff, kcon_usrld_buffer, len);
		memset(kcon_usrld_buffer, 0, len);		
	}
	return len;
}

// Copys the erro message to the overloaded buffer.
unsigned int  getErrorMsg(char *buff) {
	unsigned int len = strlen(kcon_usrld_error_buffer);
	if (len > 0) {
		memcpy(buff, kcon_usrld_error_buffer, len);
		memset(kcon_usrld_error_buffer, 0, len);
	}
	return len;
}

// Copys the warning message to the overloaded buffer.
unsigned int  getWarningMsg(char *buff) {
	unsigned int len = strlen(kcon_usrld_warning_buffer);
	if (len > 0) {
		memcpy(buff, kcon_usrld_warning_buffer, len);
		memset(kcon_usrld_warning_buffer, 0, len);
	}
	return len;
}

// Kernel Function. Waits till user made a input.
void waitForUsrInput(char *inputToWait) {
	// Setup a Kernel Buffer.
	char input[10];

	// Clear buffer.
	lv2.memset(input, 0, sizeof(input));

	// Ensure that the Input buffer is cleared before we gonna read out the first time.
	lv2.copyout(&input, &kcon_input_buffer, sizeof(input));

	// If the flag to wait starts not with a letter from a Trigger.
	if (!inputToWait) {
		// Just wait for any input.
		// Wait for a valid input, matching the overlaoded trigger.
		while (lv2.strlen(input) == 0) { lv2.copyin(&kcon_input_buffer, &input, sizeof(kcon_input_buffer)); }
	} else if (inputToWait[0] != 'C' && inputToWait[0] != 'T' && inputToWait[0] != 'L' && inputToWait[0] != 'R' && inputToWait[0] != 'S' && inputToWait[0] != 'O' && inputToWait[0] != 'c' && inputToWait[0] != 't' && inputToWait[0] != 'l' && inputToWait[0] != 'r' && inputToWait[0] != 's' && inputToWait[0] != 'o') {
		// Wait for any input and copy to buffer.
		while (lv2.strlen(input) == 0) { lv2.copyin(&kcon_input_buffer, &input, sizeof(kcon_input_buffer)); }

		// Copy input to buffer so the caller can compare against.
		lv2.memcpy(inputToWait, input, lv2.strlen(input));
	} else {
		// Wait for a valid input, matching the overloaded trigger.
		while (lv2.strcmp(input, inputToWait) != 0) {
			lv2.memset(input, 0, sizeof(input));
			lv2.copyin(&kcon_input_buffer, &input, sizeof(kcon_input_buffer));
		}
	}
}

// Set the User Input, copy flag into buffer.
void setMsg(char *buff) {
	memset(kcon_input_buffer, 0, sizeof(kcon_input_buffer));
	memcpy(kcon_input_buffer, buff, sizeof(buff) / sizeof(*buff));
}

/* Custom sleep function. */
void lv2_sleep(void) { for (int i = 0; i < 1000000; i++) { /* Nothing to do here, just wait a bit. */ } }

/* Convert a specific length of byte data to a Hex String. */
char *lv2_toHexString(unsigned char *data, int len) {
	/* Hexmap */
	char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	
	char str[len * 2];
	lv2.memset(str, 0, sizeof(str));
	for (int i = 0; i < len; ++i) {
		str[2 * i] = hexmap[(data[i] & 0xF0) >> 4];
		str[2 * i + 1] = hexmap[data[i] & 0x0F];
	}
	return lv2.strdup(str);
}

/* Hexify a Hex string. */
char *lv2_hexify(char *string) {
	// Work string.
	int len = (lv2.strlen(string) * 2) + (lv2.strlen(string) / 2) + 1;
	char hexifyed[len];

	// The Hex delimeter.
	char hex[3] = "0x";

	// The space to add.
	char space[2] = " ";

	// Array Pointer.
	char *ptr = hexifyed;

	// Now loop over the string and generate a new hexifeyed one.
	for (int i = 0; i < sizeof(hexifyed); i += 2) {
		lv2.memcpy(ptr, hex, 2);
		ptr += 2;
		lv2.memcpy(ptr, &string[i], 1);
		ptr += 1;
		lv2.memcpy(ptr, &string[i + 1], 1);
		ptr += 1;
		if ((i + 2) != sizeof(hexifyed)) {
			lv2.memcpy(ptr, space, 1);
			ptr += 1;
		}
	}

	// Return the result now.
	return lv2.strdup(hexifyed);
}

// Disable cpu write protection.
void lv2_disable_wp(void) {
	// Disable CPU Write Protection on Readonly pages.
	cr0_write(cr0_read() & ~X86_CR0_WP);
}

// Enable cpu write protection.
void lv2_enable_wp(void) {	
	// Enable CPU Write Protection on Readonly pages.
	cr0_write(cr0_read() | X86_CR0_WP);
}

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
void lv2_resolve(void) {
	// Resolve LV2 functions.
#define r(name, offset) name = (void *)(lv2.kern_base + offset);

#ifdef FW_405
	lv2.M_TEMP = (void *)(lv2.kern_base + __M_TEMP);
#endif
	//lv2.icc_query_nowait = (void *)(lv2.kern_base + __icc_query_nowait);
	lv2.Starsha_UcodeInfo = (void *)(lv2.kern_base + __Starsha_UcodeInfo);
	lv2.sysent = (SySent *)(lv2.kern_base + __sysent);

	r(lv2.aslr_get_random_value, __aslr_get_random_value);
	r(lv2.aslr_gen_pseudo_random_number, __aslr_gen_pseudo_random_number);
	r(lv2.bootparam_hwconfig_read, __bootparam_hwconfig_read);
	r(lv2.bootparam_get_mac_address, __bootparam_get_mac_address);
	r(lv2.bootparam_get_config_board_id, __bootparam_get_config_board_id);
	r(lv2.bootparam_get_ddr3_capacity, __bootparam_get_ddr3_capacity);
	r(lv2.bootparam_is_mode_manu, __bootparam_is_mode_manu);
	r(lv2.bootparam_disable_console_output, __bootparam_disable_console_output);
	r(lv2.bootparam_notify_main_on_standby_mode, __bootparam_notify_main_on_standby_mode);
	r(lv2.bootparam_is_main_on_standby, __bootparam_is_main_on_standby);
	r(lv2.binuptime, __binuptime);
	r(lv2.getbinuptime, __getbinuptime);
	r(lv2.bcopy, __bcopy);
	r(lv2.bcmp, __bcmp);
	r(lv2.sbuf_new, __sbuf_new);
	r(lv2.sbuf_vprintf, __sbuf_vprintf);
	r(lv2.sbuf_finish, __sbuf_finish);
	r(lv2.sbuf_len, __sbuf_len);
	r(lv2.sbuf_data, __sbuf_data);
	r(lv2.sbuf_delete, __sbuf_delete);
	r(lv2.sbuf_trim, __sbuf_trim);
	r(lv2.msgbuf_addchar, __msgbuf_addchar);
	r(lv2.msgbuf_addstr_sub, __msgbuf_addstr_sub);
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
	r(lv2.strdup, __strdup);
	r(lv2.strstr, __strstr);
	r(lv2.strncpy, __strncpy);
	r(lv2.strlcpy, __strlcpy);
	r(lv2.strcmp, __strcmp);
	r(lv2.strncmp, __strncmp);
	r(lv2.strcasecmp, __strcasecmp);
	r(lv2.cv_init, __cv_init);
	r(lv2.cv_destroy, __cv_destroy);
	r(lv2._cv_wait, ___cv_wait);
	//r(lv2._cv_wait_unlock, ___cv_wait_unlock);
	r(lv2._cv_wait_sig, ___cv_wait_sig);
	r(lv2._cv_timedwait, ___cv_timedwait);
	r(lv2._cv_timedwait_sig, ___cv_timedwait_sig);
	r(lv2.cv_signal, __cv_signal);
	r(lv2.cv_signalto, __cv_signalto);
	r(lv2.cv_broadcastpri, __cv_broadcastpri);
	r(lv2.sleepq_lock, __sleepq_lock);
	r(lv2.sleepq_lookup, __sleepq_lookup);
	r(lv2.sleepq_release, __sleepq_release);
	r(lv2.sleepq_add, __sleepq_add);
	r(lv2.sleepq_set_timeout, __sleepq_set_timeout);
	r(lv2.sleepq_set_runnable, __sleepq_set_runnable);
	r(lv2.sleepq_sleepcnt, __sleepq_sleepcnt);
	r(lv2.sleepq_wait, __sleepq_wait);
	r(lv2.sleepq_wait_sig, __sleepq_wait_sig);
	r(lv2.sleepq_timedwait, __sleepq_timedwait);
	r(lv2.sleepq_timedwait_sig, __sleepq_timedwait_sig);
	r(lv2.sleepq_signal, __sleepq_signal);
	r(lv2.sleepq_signalto, __sleepq_signalto);
	r(lv2.sleepq_broadcast, __sleepq_broadcast);
	r(lv2.sleepq_remove, __sleepq_remove);
	r(lv2.sleepq_abort, __sleepq_abort);
	r(lv2.sysctl_lock, __sysctl_lock);
	r(lv2.sysctl_unlock, __sysctl_unlock);
	r(lv2.sysctl_register_oid, __sysctl_register_oid);
	r(lv2.sysctl_unregister_oid, __sysctl_unregister_oid);
	r(lv2.sysctl_ctx_init, __sysctl_ctx_init);
	r(lv2.sysctl_ctx_free, __sysctl_ctx_free);
	//r(lv2.sysctl_ctx_entry_add, __sysctl_ctx_entry_add);
	//r(lv2.sysctl_ctx_entry_find, __sysctl_ctx_entry_find);
	//r(lv2.sysctl_ctx_entry_del, __sysctl_ctx_entry_del);
	r(lv2.sysctl_remove_oid, __sysctl_remove_oid);
	//r(lv2.sysctl_remove_name, __sysctl_remove_name);
	r(lv2.sysctl_add_oid, __sysctl_add_oid);
	r(lv2.sysctl_rename_oid, __sysctl_rename_oid);
	//r(lv2.sysctl_move_oid, __sysctl_move_oid);
	r(lv2.sysctl_handle_int, __sysctl_handle_int);
	//r(lv2.sysctl_msec_to_ticks, __sysctl_msec_to_ticks);
	r(lv2.sysctl_handle_long, __sysctl_handle_long);
	r(lv2.sysctl_handle_64, __sysctl_handle_64);
	r(lv2.sysctl_handle_string, __sysctl_handle_string);
	r(lv2.sysctl_handle_opaque, __sysctl_handle_opaque);
	r(lv2.kernel_sysctlbyname, __kernel_sysctlbyname);
	r(lv2.sysctl_wire_old_buffer, __sysctl_wire_old_buffer);
	//r(lv2.sysctl_find_oid, __sysctl_find_oid);
	//r(lv2.sys___sysctl, __sys___sysctl);
	r(lv2.kernel_sysctl, __kernel_sysctl);
	r(lv2.kernel_sysctlbyname, __kernel_sysctlbyname);
	r(lv2.ktrsysctl, __ktrsysctl);
	r(lv2.userland_sysctl, __userland_sysctl);
	r(lv2.sbuf_new_for_sysctl, __sbuf_new_for_sysctl);
	r(lv2.g_obi_create, __g_obi_create);
	r(lv2.g_new_geomf, __g_new_geomf);
	r(lv2.g_new_consumer, __g_new_consumer);
	r(lv2.g_attach, __g_attach);
	r(lv2.g_trace, __g_trace);
	r(lv2.g_access, __g_access);
	r(lv2.g_wither_geom, __g_wither_geom);
	r(lv2.g_do_wither, __g_do_wither);
	r(lv2.g_orphan_provider, __g_orphan_provider);
	r(lv2.g_provider_by_name, __g_provider_by_name);
	r(lv2.g_obi_read, __g_obi_read);
	r(lv2.g_io_request, __g_io_request);
	r(lv2.g_alloc_bio, __g_alloc_bio);
	r(lv2.g_destroy_bio, __g_destroy_bio);
	r(lv2.g_post_notification_for_bio, __g_post_notification_for_bio);
	r(lv2.memcpy, __memcpy);
	r(lv2.memset, __memset);
	r(lv2.memcmp, __memcmp);
	r(lv2.memmove, __memmove);
	r(lv2.icc_nvs_read, __icc_nvs_read);
	r(lv2.icc_nvs_write, __icc_nvs_write);	
	r(lv2.icc_query, __icc_query);
	r(lv2.icc_query_nowait, __icc_query_nowait);
	//r(lv2.icc_sc_config_drvinit, __icc_sc_config_drvinit);
	r(lv2.icc_nvs_init, __icc_nvs_init);
	r(lv2.icc_snvs_read_sector, __icc_snvs_read_sector);
	r(lv2.icc_snvs_write_sector, __icc_snvs_write_sector);
	r(lv2.icc_snvs_get_write_count, __icc_snvs_get_write_count);
	r(lv2.icc_init_snvs_write_lock, __icc_init_snvs_write_lock);
	r(lv2.icc_lock_snvs_write, __icc_lock_snvs_write);
	r(lv2.icc_unlock_snvs_write, __icc_unlock_snvs_write);
	r(lv2.icc_button_init, __icc_button_init);
	r(lv2.icc_button_set_state, __icc_button_set_state);
	//r(lv2.icc_button_get_state, __icc_button_get_state);
	r(lv2.icc_register_notification_handler, __icc_register_notification_handler);
	r(lv2.icc_thermal_set_notify, __icc_thermal_set_notify);
	r(lv2.icc_thermal_get_thermal_state, __icc_thermal_get_thermal_state);
	r(lv2.icc_indicator_set_buzzer, __icc_indicator_set_buzzer);
	r(lv2.icc_indicator_set_led, __icc_indicator_set_led);
	r(lv2.icc_indicator_get_led, __icc_indicator_get_led);
	r(lv2.icc_indicator_set_dynamic_led, __icc_indicator_set_dynamic_led);
	r(lv2.icc_indicator_get_dynamic_led, __icc_indicator_get_dynamic_led);
	r(lv2.icc_indicator_set_dynamic_led_shutdown, __icc_indicator_set_dynamic_led_shutdown);
	r(lv2.icc_indicator_set_dynamic_led_boot, __icc_indicator_set_dynamic_led_boot);
	r(lv2.icc_indicator_set_dynamic_led_off, __icc_indicator_set_dynamic_led_off);
	r(lv2.icc_indicator_set_thermalalert_led_on, __icc_indicator_set_thermalalert_led_on);
	r(lv2.icc_indicator_set_thermalalert_led_off, __icc_indicator_set_thermalalert_led_off);
	r(lv2.icc_indicator_set_dynamic_led_standby, __icc_indicator_set_dynamic_led_standby);
	r(lv2.icc_indicator_set_dynamic_led_standby_boot, __icc_indicator_set_dynamic_led_standby_boot);
	r(lv2.icc_indicator_drvinit, __icc_indicator_drvinit);
	r(lv2.icc_indicator_check_thermal_on_boot, __icc_indicator_check_thermal_on_boot);
	r(lv2.icc_thermal_alert_occurred_when_boot, __icc_thermal_alert_occurred_when_boot);
	r(lv2.icc_thermal_alert_occurred_when_boot_sub, __icc_thermal_alert_occurred_when_boot_sub);
	r(lv2.icc_thermal_alert_status_off, __icc_thermal_alert_status_off);
	r(lv2.icc_thermal_notify_handler, __icc_thermal_notify_handler);
	r(lv2.icc_thermal, __icc_thermal);
	r(lv2.icc_thermal_enable_disable_notify, __icc_thermal_enable_disable_notify);
	r(lv2.icc_thermal_get_thermal_state, __icc_thermal_get_thermal_state);
	r(lv2.icc_thermal_stop_td, __icc_thermal_stop_td);
	r(lv2.icc_thermal_resume_td, __icc_thermal_resume_td);
	r(lv2.icc_thermal_set_notify, __icc_thermal_set_notify);
	//r(lv2.icc_thermal_drvinit, __icc_thermal_drvinit);
	//r(lv2.icc_fan_drvinit, __icc_fan_drvinit);
	//r(lv2.icc_sc_fw_update_handler, __icc_sc_fw_update_handler);
	//r(lv2.icc_sc_fw_update, __icc_sc_fw_update);
	r(lv2.make_dev, __make_dev);
	r(lv2.make_dev_credf, __make_dev_credf);
	r(lv2.destroy_dev, __destroy_dev);
	r(lv2.devstat_new_entry, __devstat_new_entry);
	r(lv2.devstat_start_transaction, __devstat_start_transaction);
	r(lv2.pfs_create_dir, __pfs_create_dir);
	r(lv2.pfs_create_file, __pfs_create_file);
	r(lv2.pfs_create_link, __pfs_create_link);
	r(lv2.pfs_fileno_alloc, __pfs_fileno_alloc);
	r(lv2.dump_pfs, __dump_pfs);
	r(lv2.dump_sign, __dump_sign);
	r(lv2.pfs_savedata, __pfs_savedata);
	r(lv2.pfs_mapped_read, __pfs_mapped_read);
	r(lv2.pfs_check_addr, __pfs_check_addr);
	r(lv2.pfs_checkpath, __pfs_checkpath);
	//r(lv2.pfs_fileno_uninit, __pfs_fileno_uninit);
	r(lv2.pfs_fileno_alloc, __pfs_fileno_alloc);
	r(lv2.pfs_fileno_free, __pfs_fileno_free);
	//r(lv2.pfs_fileno_init, __pfs_fileno_init);
	//r(lv2.pfs_find_node, __pfs_find_node);
	//r(lv2.pfs_destroy, __pfs_destroy);
	r(lv2.pfs_vncache_alloc, __pfs_vncache_alloc);
	r(lv2.pfs_vncache_unload, __pfs_vncache_unload);
	r(lv2.pfs_vncache_load, __pfs_vncache_load);
	r(lv2.pfs_purge, __pfs_purge);
	//r(lv2.pfs_uninit, __pfs_uninit);
	//r(lv2.pfs_init, __pfs_init);
	r(lv2.pfs_mount, __pfs_mount);
	r(lv2.pfs_cmount, __pfs_cmount);
	r(lv2.pfs_unmount, __pfs_unmount);
	r(lv2.pfs_root, __pfs_root);
	//r(lv2.pfs_statfs, __pfs_statfs);
	//r(lv2.p_candebug, __p_candebug);
	//r(lv2.procfs_candebug, __procfs_candebug);
	//r(lv2.procfs_init, __procfs_init);
	r(lv2.procfs_doprocfile, __procfs_doprocfile);
	r(lv2.procfs_docurproc, __procfs_docurproc);
	r(lv2.procfs_attr, __procfs_attr);
	r(lv2.procfs_notsystem, __procfs_notsystem);
	r(lv2.procfs_doosrel, __procfs_doosrel);
	r(lv2.procfs_doprocstatus, __procfs_doprocstatus);
	r(lv2.procfs_doproccmdline, __procfs_doproccmdline);
	r(lv2.procfs_doprocctl, __procfs_doprocctl);
	r(lv2.procfs_doprocdbregs, __procfs_doprocdbregs);
	r(lv2.procfs_doproctype, __procfs_doproctype);
	r(lv2.procfs_doprocfpregs, __procfs_doprocfpregs);
	r(lv2.procfs_doprocmap, __procfs_doprocmap);
	r(lv2.procfs_doprocmem, __procfs_doprocmem);
	r(lv2.procfs_ioctl, __procfs_ioctl);
	//r(lv2.procfs_close, __procfs_close);
	r(lv2.procfs_doprocnote, __procfs_doprocnote);
	//r(lv2.procfs_doprocregs, __procfs_doprocregs);
	r(lv2.procfs_doprocrlimit, __procfs_doprocrlimit);
	r(lv2.vfs_getnewfsid, __vfs_getnewfsid);
	r(lv2.vfs_mountedfrom, __vfs_mountedfrom);
	r(lv2.vfs_donmount, __vfs_donmount);
	r(lv2.vfs_buildopts, __vfs_buildopts);
	r(lv2.g_new_geomf, __g_new_geomf);
	r(lv2.g_cancel_event, __g_cancel_event);
	r(lv2.g_trace, __g_trace);
	r(lv2.g_destroy_geom, __g_destroy_geom);
	r(lv2.g_bdemu_create, __g_bdemu_create);
	r(lv2.bdemu_speed_emu, __bdemu_speed_emu);
	r(lv2.bd_sched_post_done, __bd_sched_post_done);
	r(lv2.bd_sched_update_bitmap_file, __bd_sched_update_bitmap_file);
	r(lv2.bd_sched_copier_handle_suspend_event, __bd_sched_copier_handle_suspend_event);
	r(lv2.bd_sched_copier_handle_resume_event, __bd_sched_copier_handle_resume_event);
	r(lv2.bd_sched_writer_handle_suspend_event, __bd_sched_writer_handle_suspend_event);
	r(lv2.bd_sched_writer_handle_resume_event, __bd_sched_writer_handle_resume_event);
	r(lv2.bd_sched_shutdown_copier, __bd_sched_shutdown_copier);
	r(lv2.bd_sched_shutdown_w_hdd, __bd_sched_shutdown_w_hdd);
	r(lv2.bd_sched_shutdown_r_bd_internal, __bd_sched_shutdown_r_bd_internal);
	r(lv2.bd_sched_reader_handle_suspend_event, __bd_sched_reader_handle_suspend_event);
	r(lv2.bd_sched_reader_handle_resume_event, __bd_sched_reader_handle_resume_event);
	r(lv2.bd_sched_bd_reader_ripping, __bd_sched_bd_reader_ripping);
	r(lv2.bd_sched_bd_reader_bg_copy, __bd_sched_bd_reader_bg_copy);
	r(lv2.bd_sched_lock_bm_file, __bd_sched_lock_bm_file);
	r(lv2.bd_sched_update_bitmap_cache2, __bd_sched_update_bitmap_cache2);
	r(lv2.bd_sched_is_available_on_hdd2, __bd_sched_is_available_on_hdd2);
	r(lv2.bd_sched_unlock_bm_file, __bd_sched_unlock_bm_file);
	r(lv2.get_img_offset, __get_img_offset);
	r(lv2.bd_sched_read_bd_dev, __bd_sched_read_bd_dev);
	r(lv2.get_pkg_offset, __get_pkg_offset);
	r(lv2.bd_sched_get_bdsio_in_cache, __bd_sched_get_bdsio_in_cache);
	r(lv2.bd_sched_get_bdsio_in_writer, __bd_sched_get_bdsio_in_writer);
	r(lv2.bd_sched_private_done, __bd_sched_private_done);
	r(lv2.bd_sched_adjust_bio_completed, __bd_sched_adjust_bio_completed);
	r(lv2.sceSblPfsKeymgrGenEKpfsForGDGPAC, __sceSblPfsKeymgrGenEKpfsForGDGPAC);
	r(lv2.RsaesPkcs1v15Enc2048, __RsaesPkcs1v15Enc2048);
	r(lv2.RsaesPkcs1v15Dec2048CRT, __RsaesPkcs1v15Dec2048CRT);
	r(lv2.AesCbcCfb128Encrypt, __AesCbcCfb128Encrypt);
	r(lv2.AesCbcCfb128Decrypt, __AesCbcCfb128Decrypt);
	r(lv2.AesEncKeySet, __AesEncKeySet);
	r(lv2.AesDecKeySet, __AesDecKeySet);
	r(lv2.AesEncrypt, __AesEncrypt);
	r(lv2.AesDecrypt, __AesDecrypt);
	r(lv2.Sha256HmacFinal, __Sha256HmacFinal);
	r(lv2.Sha256Hash, __Sha256Hash);
	r(lv2.Sha256Hmac, __Sha256Hmac);
	r(lv2.Sha256OneBlk, __Sha256OneBlk);
	r(lv2.Sha256FinalCore, __Sha256FinalCore);
	r(lv2.Sha256Init, __Sha256Init);
	r(lv2.Sha256Transform, __Sha256Transform);
	r(lv2.Sha256Final, __Sha256Final);
	r(lv2.Sha256HmacInit, __Sha256HmacInit);
	r(lv2.index, __index);
	r(lv2.load_prx, __load_prx);
	r(lv2.preload_prx_modules, __preload_prx_modules);
	r(lv2.gs_is_update_bio_ioprio, __gs_is_update_bio_ioprio);
	r(lv2.uma_zalloc_arg, __uma_zalloc_arg);
	//r(lv2.uma_large_free, __uma_large_free);
	//r(lv2.uma_zfree_arg, __uma_zfree_arg);
	r(lv2.sceSblACMgrIsSystemUcred, __sceSblACMgrIsSystemUcred);
	r(lv2.sceSblACMgrIsNongameUcred, __sceSblACMgrIsNongameUcred);
	r(lv2.sceSblACMgrIsDiagProcess, __sceSblACMgrIsDiagProcess);
	r(lv2.sceSblACMgrIsCoredumpProcess, __sceSblACMgrIsCoredumpProcess);
	r(lv2.sceSblACMgrIsDecidProcess, __sceSblACMgrIsDecidProcess);
	r(lv2.sceSblACMgrIsMinisyscoreProcess, __sceSblACMgrIsMinisyscoreProcess);
	r(lv2.sceSblACMgrIsSyscoreProcess, __sceSblACMgrIsSyscoreProcess);
	r(lv2.sceSblACMgrIsShelluiProcess, __sceSblACMgrIsShelluiProcess);
	r(lv2.sceSblACMgrIsShellcoreProcess, __sceSblACMgrIsShellcoreProcess);
	r(lv2.sceSblACMgrIsVideoplayerProcess, __sceSblACMgrIsVideoplayerProcess);
	r(lv2.sceSblACMgrIsDiskplayeruiProcess, __sceSblACMgrIsDiskplayeruiProcess);
	r(lv2.sceSblACMgrIsBdjavaProcess, __sceSblACMgrIsBdjavaProcess);
	r(lv2.sceSblACMgrIsWebcoreProcess, __sceSblACMgrIsWebcoreProcess);
	r(lv2.sceSblACMgrIsVdecProxyProcess, __sceSblACMgrIsVdecProxyProcess);
	r(lv2.sceSblACMgrIsJitCompilerProcess, __sceSblACMgrIsJitCompilerProcess);
	r(lv2.sceSblACMgrIsJitApplicationProcess, __sceSblACMgrIsJitApplicationProcess);
	r(lv2.sceSblACMgrIsAslrSeed2Process, __sceSblACMgrIsAslrSeed2Process);
	r(lv2.sceSblACMgrHasSceProgramAttribute, __sceSblACMgrHasSceProgramAttribute);
	r(lv2.sceSblACMgrHasDrmProcessAttribute, __sceSblACMgrHasDrmProcessAttribute);
	r(lv2.sceSblACMgrHasUseVtrmCapability, __sceSblACMgrHasUseVtrmCapability);
	r(lv2.sceSblACMgrIsDiskplayerdiagProcess, __sceSblACMgrIsDiskplayerdiagProcess);
	r(lv2.sceSblACMgrHasUseHp3dPipeCapability, __sceSblACMgrHasUseHp3dPipeCapability);
	r(lv2.sceSblACMgrHasUseVideoServiceCapability, __sceSblACMgrHasUseVideoServiceCapability);
	r(lv2.sceSblACMgrHasSceProcessCapability, __sceSblACMgrHasSceProcessCapability);
	r(lv2.sceSblACMgrIsAllowedToUsePupUpdate0, __sceSblACMgrIsAllowedToUsePupUpdate0);
	r(lv2.sceSblACMgrIsVtrmadminProcess, __sceSblACMgrIsVtrmadminProcess);
	r(lv2.sceSblACMgrIsSoftwagnerProcess, __sceSblACMgrIsSoftwagnerProcess);
	r(lv2.sceSblACMgrIsDebuggableProcess, __sceSblACMgrIsDebuggableProcess);
	r(lv2.sceSblDiagOSUpdaterProcess, __sceSblDiagOSUpdaterProcess);
	r(lv2.sceSblACMgrIsAllowedSystemLevelDebugging, __sceSblACMgrIsAllowedSystemLevelDebugging);
	r(lv2.sceSblACMgrHasMmapSelfCapability, __sceSblACMgrHasMmapSelfCapability);
	r(lv2.sceSblACMgrIsAllowedToMmapSelf, __sceSblACMgrIsAllowedToMmapSelf);
	r(lv2.sceSblACMgrGetPathId, __sceSblACMgrGetPathId);
	r(lv2.sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall, __sceSblACMgrGetMaskedAuthInfoForGetSelfAuthInfoSyscall);
	r(lv2.sceSblACMgrGetDeviceAccessType, __sceSblACMgrGetDeviceAccessType);
	r(lv2.sceSblACMgrIsAllowedCoredump, __sceSblACMgrIsAllowedCoredump);
	r(lv2.sceSblACMgrIsAllowedDrmProcessDebugging, __sceSblACMgrIsAllowedDrmProcessDebugging);
	r(lv2.sceSblACMgrIsFirstImageWriterProcess, __sceSblACMgrIsFirstImageWriterProcess);
	r(lv2.sceSblACMgrIsMountfusefsProcess, __sceSblACMgrIsMountfusefsProcess);
	r(lv2.sceSblRcMgrIsAllowPsmDebug, __sceSblRcMgrIsAllowPsmDebug);
	r(lv2.sceSblRcMgrIsAllowRegistryAccess, __sceSblRcMgrIsAllowRegistryAccess);
	r(lv2.sceSblRcMgrIsAllowRegistryAccessMini, __sceSblRcMgrIsAllowRegistryAccessMini);
	r(lv2.sceSblRcMgrIsAllowInitVtrm, __sceSblRcMgrIsAllowInitVtrm);
	r(lv2.sceSblRcMgrIsAllowSLDebugger, __sceSblRcMgrIsAllowSLDebugger);
	r(lv2.sceSblRcMgrIsAllowAdClock, __sceSblRcMgrIsAllowAdClock);
	r(lv2.sceSblRcMgrIsStoreMode, __sceSblRcMgrIsStoreMode);
	r(lv2.sceSblRcMgrIsAllowULDebugger, __sceSblRcMgrIsAllowULDebugger);
	r(lv2.sceSblRcMgrIsAllowCreateDebuggingDeviceFile, __sceSblRcMgrIsAllowCreateDebuggingDeviceFile);
	r(lv2.sceSblRcMgrIsSoftwagnerQafForAcmgr, __sceSblRcMgrIsSoftwagnerQafForAcmgr);
	r(lv2.sceSblRcMgrIsAllowDisablingAslr, __sceSblRcMgrIsAllowDisablingAslr);
	r(lv2.sceSblRcMgrIsAllowDebugSoftWagnerClock, __sceSblRcMgrIsAllowDebugSoftWagnerClock);
	r(lv2.sceSblRcMgrIsAllowDataExecutionInFselfProcess, __sceSblRcMgrIsAllowDataExecutionInFselfProcess);
	r(lv2.sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp, __sceSblRcMgrIsAllowUsingGamePortInNonsystemMiniapp);
	r(lv2.sceSblAIMgrIsDevKit, __sceSblAIMgrIsDevKit);
	r(lv2.sceSblAIMgrIsTool, __sceSblAIMgrIsTool);
	r(lv2.sceSblAIMgrIsTestKit, __sceSblAIMgrIsTestKit);
	r(lv2.sceSblAIMgrIsDex, __sceSblAIMgrIsDex);
	r(lv2.sceSblAIMgrIsCex, __sceSblAIMgrIsCex);
	r(lv2.sceSblAIMgrIsDiag, __sceSblAIMgrIsDiag);
	r(lv2.sceSblAIMgrGetProductCode, __sceSblAIMgrGetProductCode);
	r(lv2.sceKernelCheckDipsw, __sceKernelCheckDipsw);
	r(lv2.sceKernelIsDevelopmentMode, __sceKernelIsDevelopmentMode);
	r(lv2.sceKernelIsAssistMode, __sceKernelIsAssistMode);
	r(lv2.sceSblKeymgrClearKey, __sceSblKeymgrClearKey);
	r(lv2.getSealedKeySecret, __getSealedKeySecret);
	r(lv2.sceSblSsDecryptSealedKey, __sceSblSsDecryptSealedKey);
	r(lv2.sceSblSsGenerateSealedKey, __sceSblSsGenerateSealedKey);
	r(lv2.sceSblAuthMgrSmSpawn, __sceSblAuthMgrSmSpawn);
	r(lv2.sceSblServiceSpawn, __sceSblServiceSpawn);
	r(lv2.sceSblSsMemcmpConsttime, __sceSblSsMemcmpConsttime);
	r(lv2.sceSblSsMemset, __sceSblSsMemset);
	r(lv2.sceSblDriverMapPages, __sceSblDriverMapPages);
	//r(lv2.sceSblDriverUnmapPages, __sceSblDriverUnmapPages);
	r(lv2.sceSblServiceMailbox, __sceSblServiceMailbox);
	r(lv2.SceSblSrtcSetTime, __sceSblSrtcSetTime);
	r(lv2.genNewActDat, __genNewActDat);	
	r(lv2.kernel_map, __kernel_map);
	r(lv2.kernel_pmap_store, __kernel_pmap_store);
	r(lv2.kmem_alloc_contig, __kmem_alloc_contig);
	r(lv2.kmem_free, __kmem_free);
	r(lv2.kthread_add, __kthread_add);
	r(lv2.kthread_suspend_check, __kthread_suspend_check);
	//r(lv2.ktrcsw, __ktrcsw);
	r(lv2.kproc_kthread_add, __kproc_kthread_add);
	r(lv2.pmap_extract, __pmap_extract);
	r(lv2.pmap_protect, __pmap_protect);
	r(lv2.kernel_mount, __kernel_mount);		
	//r(lv2.sys_yield, __sys_yield);
	//r(lv2.sys_execve, __sys_execve);
	//r(lv2.exec_copyin_args, __exec_copyin_args);
	r(lv2.kern_execve, __kern_execve);
	r(lv2.kern_ptrace, __kern_ptrace);
	r(lv2.kern_jail_set, __kern_jail_set);
	r(lv2.kern_jail_get, __kern_jail_get);
	//r(lv2.kernacc, __kernacc);
	//r(lv2.useracc, __useracc);
	r(lv2.proc_sstep, __proc_sstep);
	r(lv2.proc_rwmem, __proc_rwmem);
	r(lv2.proc_set_dr, __proc_set_dr);
	r(lv2.proc_get_dr, __proc_get_dr);
	r(lv2.proc_init_dr, __proc_init_dr);
	//r(lv2.kick_proc0, __kick_proc0);
	r(lv2._thread_lock_flags_check_result, ___thread_lock_flags_check_result);
	r(lv2.thread_lock_flags, ___thread_lock_flags);
	r(lv2.critical_enter, __critical_enter);
	r(lv2.critical_exit, __critical_exit);
	r(lv2.securelevel_gt, __securelevel_gt);
	r(lv2.securelevel_ge, __securelevel_ge);
	r(lv2.sx_init_flags, __sx_init_flags);
	r(lv2.sx_xlock, __sx_xlock);
	r(lv2.sx_xunlock, __sx_xunlock);
	r(lv2.sx_destroy, __sx_destroy);
	r(lv2.sx_xlock_hard, __sx_xlock_hard);
	r(lv2._sx_sunlock_hard, ___sx_sunlock_hard);
	r(lv2._sx_sunlock_hard_sub, ___sx_sunlock_hard_sub);
	r(lv2.lock_init, __lock_init);
	r(lv2.unref_dag, __unref_dag);
	r(lv2.id_set, __id_set);
	r(lv2.id_unlock, __id_unlock);
	r(lv2.id_wlock, __id_wlock);
	r(lv2.mtx_destroy, __mtx_destroy);
	r(lv2.fpu_kern_enter, __fpu_kern_enter);
	r(lv2.fpu_kern_leave, __fpu_kern_leave);
	//r(lv2.vm_phys_paddr_to_vm_page, __vm_phys_paddr_to_vm_page);
	r(lv2.vm_page_free_toq, __vm_page_free_toq);
	r(lv2.vm_page_unhold_pages, __vm_page_unhold_pages);
	r(lv2.vflush, __vflush);
	//r(lv2.vslock, __vslock);
	r(lv2.vsunlock, __vsunlock);
	r(lv2.gbase_samu_initialize, __gbase_samu_initialize);
	r(lv2.gbase_samu_finalize, __gbase_samu_finalize);
	r(lv2.gbase_samu_map, __gbase_samu_map);
	r(lv2.gbase_samu_unmap, __gbase_samu_unmap);
	r(lv2.gbase_samu_read_register, __gbase_samu_read_register);
	r(lv2.gbase_samu_write_register, __gbase_samu_write_register);
	r(lv2.gbase_samu_ix_read_register, __gbase_samu_ix_read_register);
	r(lv2.gbase_samu_ix_write_register, __gbase_samu_ix_write_register);
	r(lv2.gbase_samu_pa_to_gpuva, __gbase_samu_pa_to_gpuva);
	r(lv2.gbase_samu_unk_func, __gbase_samu_unk_func);
	r(lv2.gbase_samu_mailbox_writer, __gbase_samu_mailbox_writer);
	r(lv2.gbase_samu_mailbox_reader, __gbase_samu_mailbox_reader);
	r(lv2.gbase_samu_cmd, __gbase_samu_cmd);
	r(lv2.gbase_samu_handler, __gbase_samu_handler);
	r(lv2.gbase_unmap, __gbase_unmap);
	r(lv2.gbase_free_vm_space, __gbase_free_vm_space);
	r(lv2.SceSflash, __SceSflash);
	r(lv2.sched_pin, __sched_pin);
	r(lv2.sched_unpin, __sched_unpin);
	r(lv2.sched_bind, __sched_bind);
	r(lv2.sched_switch, __sched_switch);
	r(lv2.sched_prio, __sched_prio);
	r(lv2.sched_user_prio, __sched_user_prio);
	r(lv2.bioq_init, __bioq_init);
	r(lv2.disk_alloc, __disk_alloc);
	r(lv2.disk_create, __disk_create);
	r(lv2.fork1, __fork1);
	//r(lv2.wakeup, __wakeup);
	r(lv2.sleep, ___sleep);
	//r(lv2.alloc_unr, __alloc_unr);
	r(lv2.alloc_unrl, __alloc_unrl);
	r(lv2.clean_unrhdrl, __clean_unrhdrl);
	r(lv2.delete_unrhdr, __delete_unrhdr);
	r(lv2.mdbg_kproc_hook_create, __mdbg_kproc_hook_create);
	r(lv2.metadbg_log_add, __metadbg_log_add);
	r(lv2.cpu_set_fork_handler, __cpu_set_fork_handler);
	r(lv2.eventhandler_find_list, __eventhandler_find_list);
	r(lv2.eventhandler_prune_list, __eventhandler_prune_list);
	r(lv2.eventhandler_deregister, __eventhandler_deregister);
	r(lv2.eventhandler_register, __eventhandler_register);
	//r(lv2.eventhandler_suspend, __eventhandler_suspend);
	r(lv2.SceSflash, __SceSflash);	
	r(lv2.sflash_pread_for_vtrm, __sflash_pread_for_vtrm);
	r(lv2.sflash_pwrite_for_vtrm, __sflash_pwrite_for_vtrm);
	r(lv2.sflash_erase_for_vtrm, __sflash_erase_for_vtrm);
	r(lv2.sflash_pread_for_vtrm2, __sflash_pread_for_vtrm2);
	r(lv2.sceSblVtrmBackup, __sceSblVtrmBackup);
	r(lv2.sceSblVtrmRestore, __sceSblVtrmRestore);
	r(lv2.VtrmUtilLockInitialize, __VtrmUtilLockInitialize);
	r(lv2.VtrmUtilLockFinialize, __VtrmUtilLockFinialize);
	r(lv2.vtrm_pread, __vtrm_pread);
	r(lv2.vtrm_pwrite, __vtrm_pwrite);
	r(lv2.vtrm_pwrite2, __vtrm_pwrite2);
	r(lv2.vtrm_erase_sector, __vtrm_erase_sector);
	//r(lv2._is_vtrm_access_ok, ___is_vtrm_access_ok);	
	r(lv2.smp_no_rendevous_barrier, __smp_no_rendevous_barrier);
	r(lv2.smp_rendezvous, __smp_rendezvous);
	//r(lv2.sys_sendto, __sys_sendto);
	r(lv2.gpu_devid_is_9924, __gpu_devid_is_9924);
	r(lv2.gc_get_fw_info, __gc_get_fw_info);
	r(lv2.sleep, ___sleep);
	lv2.csleep      = lv2_sleep;
	lv2.toHexString = lv2_toHexString;
	lv2.hexify      = lv2_hexify;
	lv2.disable_wp = lv2_disable_wp;
	lv2.enable_wp = lv2_enable_wp;
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
	cr0_write(cr0_read() & ~X86_CR0_WP);
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
	cr0_write(cr0_read() | X86_CR0_WP);
	lv2.sched_unpin();

	return 1;
}

/* Install a lv2 systemcall. */
void lv2_syscall_install(int num, void *call, int narg) {
	struct sysent_t *sy = &lv2.sysent[num];

	lv2.sched_pin();
	cr0_write(cr0_read() & ~X86_CR0_WP);

	memset(sy, 0, sizeof(*sy));
	sy->sy_narg = narg;
	sy->sy_call = call;
	sy->sy_thrcnt = 1;

	cr0_write(cr0_read() | X86_CR0_WP);
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
	lv2.kern_base = &((uint8_t*)__readmsr(0xC0000082))[-__Xfast_syscall];

	// Resolve Symbols.
	lv2_resolve();

	return 0;
}

/* Initialize Kernel Console Functions. */
void kconsole_init(void) {
	// Resolve internal functions.
	kconsole.WriteLine = WriteLine;
	kconsole.WriteError = WriteError;
	kconsole.WriteWarning = WriteWarning;
	kconsole.LineBreak = LineBreak;
	kconsole.getMsg = getMsg;
	kconsole.getErrorMsg = getErrorMsg;
	kconsole.getWarningMsg = getWarningMsg;
	kconsole.waitForUsrInput = waitForUsrInput;
	kconsole.setMsg = setMsg;
}