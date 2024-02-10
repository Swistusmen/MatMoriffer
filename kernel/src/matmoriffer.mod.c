#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x5da629, "__netlink_kernel_create" },
	{ 0xfe1a7e0b, "nf_register_net_hook" },
	{ 0x37a0cba, "kfree" },
	{ 0xd5b6677d, "proc_remove" },
	{ 0x8323e2e4, "misc_deregister" },
	{ 0xf757d01f, "netlink_kernel_release" },
	{ 0xaee25155, "nf_unregister_net_hook" },
	{ 0xe2d5255a, "strcmp" },
	{ 0xa916b694, "strnlen" },
	{ 0xaa22ea2f, "__alloc_skb" },
	{ 0xab309351, "__nlmsg_put" },
	{ 0x9166fada, "strncpy" },
	{ 0x9a431584, "netlink_unicast" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0x33ed7f44, "kmalloc_caches" },
	{ 0xb14c46ef, "kmalloc_trace" },
	{ 0x656e4a6e, "snprintf" },
	{ 0x8ae5be25, "seq_read" },
	{ 0xd5cf23fc, "seq_lseek" },
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0x754d539c, "strlen" },
	{ 0x69acdf38, "memcpy" },
	{ 0xcbd4898c, "fortify_panic" },
	{ 0x3f0ef343, "seq_release" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0xc6f399f0, "seq_open" },
	{ 0x899fb2bc, "seq_printf" },
	{ 0x122c3a7e, "_printk" },
	{ 0x2e0e2d28, "proc_create" },
	{ 0xae692520, "misc_register" },
	{ 0x5ada5852, "init_net" },
	{ 0xaac58f6c, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "FC0F6A866597391AB8DD858");
