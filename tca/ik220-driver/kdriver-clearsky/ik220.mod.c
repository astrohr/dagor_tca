#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xae141548, "module_layout" },
	{ 0xe8b8056d, "__register_chrdev" },
	{ 0x5cc0b72, "__pci_register_driver" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xab0f1ce3, "pci_unregister_driver" },
	{ 0x3af98f9e, "ioremap_nocache" },
	{ 0x49e75ea9, "pci_request_regions" },
	{ 0x2b3acc9, "pci_enable_device" },
	{ 0x2cb6407f, "dev_get_drvdata" },
	{ 0x67668174, "pci_release_regions" },
	{ 0x5f8bdac9, "dev_set_drvdata" },
	{ 0xedc03953, "iounmap" },
	{ 0xf0fdf6cb, "__stack_chk_fail" },
	{ 0xb72397d5, "printk" },
	{ 0x37a0cba, "kfree" },
	{ 0x12da5bb2, "__kmalloc" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0xa1c76e0a, "_cond_resched" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0xf2a644fb, "copy_from_user" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("pci:v000010B5d00009050sv000010B5sd00001172bc*sc*i*");
