cmd_/root/ik220-driver/kdriver-clearsky/ik220.o :=  gcc-4.3 -Wp,-MD,/root/ik220-driver/kdriver-clearsky/.ik220.o.d  -nostdinc -isystem /usr/lib/gcc/i486-linux-gnu/4.3.5/include -Iinclude -Iinclude2 -I/usr/src/linux-headers-2.6.32-5-common/include -I/usr/src/linux-headers-2.6.32-5-common/arch/x86/include -include include/linux/autoconf.h   -I/root/ik220-driver/kdriver-clearsky -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Os -m32 -msoft-float -mregparm=3 -freg-struct-return -mpreferred-stack-boundary=2 -march=i686 -mtune=generic -Wa,-mtune=generic32 -ffreestanding -fstack-protector -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow  -DMODULE -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(ik220)"  -D"KBUILD_MODNAME=KBUILD_STR(ik220)"  -c -o /root/ik220-driver/kdriver-clearsky/.tmp_ik220.o /root/ik220-driver/kdriver-clearsky/ik220.c

deps_/root/ik220-driver/kdriver-clearsky/ik220.o := \
  /root/ik220-driver/kdriver-clearsky/ik220.c \
  include/linux/version.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/module/unload.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/sysfs.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/stddef.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/compiler-gcc4.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/prefetch.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/types.h \
    $(wildcard include/config/x86/64.h) \
    $(wildcard include/config/highmem64g.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/types.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/int-ll64.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/bitsperlong.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitsperlong.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/posix_types.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/posix_types_32.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/processor.h \
    $(wildcard include/config/x86/vsmp.h) \
    $(wildcard include/config/cc/stackprotector.h) \
    $(wildcard include/config/paravirt.h) \
    $(wildcard include/config/m386.h) \
    $(wildcard include/config/m486.h) \
    $(wildcard include/config/x86/debugctlmsr.h) \
    $(wildcard include/config/cpu/sup/amd.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/processor-flags.h \
    $(wildcard include/config/vm86.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/vm86.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ptrace.h \
    $(wildcard include/config/x86/ptrace/bts.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ptrace-abi.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/segment.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/math_emu.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/sigcontext.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/current.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/percpu.h \
    $(wildcard include/config/x86/64/smp.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/numa.h) \
  /usr/lib/gcc/i486-linux-gnu/4.3.5/include/stdarg.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/linkage.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/linkage.h \
    $(wildcard include/config/x86/alignment/16.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/stringify.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/bitops.h \
    $(wildcard include/config/x86/cmov.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/alternative.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/asm.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/cpufeature.h \
    $(wildcard include/config/x86/invlpg.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/required-features.h \
    $(wildcard include/config/x86/minimum/cpu/family.h) \
    $(wildcard include/config/math/emulation.h) \
    $(wildcard include/config/x86/pae.h) \
    $(wildcard include/config/x86/cmpxchg64.h) \
    $(wildcard include/config/x86/use/3dnow.h) \
    $(wildcard include/config/x86/p6/nop.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitops/sched.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitops/hweight.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitops/fls64.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitops/ext2-non-atomic.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitops/le.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/byteorder.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/byteorder/little_endian.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/swab.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/swab.h \
    $(wildcard include/config/x86/bswap.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/byteorder/generic.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bitops/minix.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/typecheck.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/ratelimit.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/param.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/param.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/param.h \
    $(wildcard include/config/hz.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dynamic_debug.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/div64.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/percpu.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/system.h \
    $(wildcard include/config/ia32/emulation.h) \
    $(wildcard include/config/x86/32/lazy/gs.h) \
    $(wildcard include/config/x86/ppro/fence.h) \
    $(wildcard include/config/x86/oostore.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/cmpxchg.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/cmpxchg_32.h \
    $(wildcard include/config/x86/cmpxchg.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/nops.h \
    $(wildcard include/config/mk7.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/irqflags.h \
    $(wildcard include/config/debug/lock/alloc.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/paravirt.h \
    $(wildcard include/config/highpte.h) \
    $(wildcard include/config/paravirt/spinlocks.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable_types.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/compat/vdso.h) \
    $(wildcard include/config/proc/fs.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/const.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/page_types.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/page_32_types.h \
    $(wildcard include/config/highmem4g.h) \
    $(wildcard include/config/page/offset.h) \
    $(wildcard include/config/4kstacks.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable_32_types.h \
    $(wildcard include/config/highmem.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable-2level_types.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/pgtable-nopud.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/pgtable-nopmd.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/paravirt_types.h \
    $(wildcard include/config/x86/local/apic.h) \
    $(wildcard include/config/paravirt/debug.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/desc_defs.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/kmap_types.h \
    $(wildcard include/config/debug/highmem.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/kmap_types.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/bitmap.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/string.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/string_32.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/page.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/page_32.h \
    $(wildcard include/config/hugetlb/page.h) \
    $(wildcard include/config/debug/virtual.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/x86/3dnow.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/memory_model.h \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/sparsemem/vmemmap.h) \
    $(wildcard include/config/sparsemem.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/getorder.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/msr.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/msr-index.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/ioctl.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ioctl.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/ioctl.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/errno.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/errno.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/errno-base.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/cpumask.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ds.h \
    $(wildcard include/config/x86/ds.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/err.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/personality.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/cache.h \
    $(wildcard include/config/x86/l1/cache/shift.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/math64.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/stat.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/stat.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/seqlock.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/preempt.h \
    $(wildcard include/config/preempt/notifiers.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ftrace.h \
    $(wildcard include/config/function/tracer.h) \
    $(wildcard include/config/dynamic/ftrace.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/atomic.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/atomic_32.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/atomic-long.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/bottom_half.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/spinlock_types.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/spinlock_types.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/spinlock.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/rwlock.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/spinlock_api_smp.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kmod.h \
    $(wildcard include/config/pm/sleep.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/gfp.h \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/wait.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/nodemask.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/notifier.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/errno.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/rwsem.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/srcu.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/smp.h \
    $(wildcard include/config/x86/io/apic.h) \
    $(wildcard include/config/x86/32/smp.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/mpspec.h \
    $(wildcard include/config/x86/numaq.h) \
    $(wildcard include/config/mca.h) \
    $(wildcard include/config/eisa.h) \
    $(wildcard include/config/x86/mpparse.h) \
    $(wildcard include/config/acpi.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/mpspec_def.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/x86_init.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/bootparam.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/screen_info.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/apm_bios.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/edd.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/e820.h \
    $(wildcard include/config/efi.h) \
    $(wildcard include/config/hibernation.h) \
    $(wildcard include/config/memtest.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/ioport.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ist.h \
  /usr/src/linux-headers-2.6.32-5-common/include/video/edid.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/apic.h \
    $(wildcard include/config/x86/x2apic.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/delay.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/delay.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pm.h \
    $(wildcard include/config/pm/runtime.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/workqueue.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/timer.h \
    $(wildcard include/config/timer/stats.h) \
    $(wildcard include/config/debug/objects/timers.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/ktime.h \
    $(wildcard include/config/ktime/scalar.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/jiffies.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/timex.h \
    $(wildcard include/config/no/hz.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/timex.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/tsc.h \
    $(wildcard include/config/x86/tsc.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/debugobjects.h \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/debug/objects/free.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/apicdef.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/fixmap.h \
    $(wildcard include/config/provide/ohci1394/dma/init.h) \
    $(wildcard include/config/x86/visws/apic.h) \
    $(wildcard include/config/x86/f00f/bug.h) \
    $(wildcard include/config/x86/cyclone/timer.h) \
    $(wildcard include/config/pci/mmconfig.h) \
    $(wildcard include/config/intel/txt.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/acpi.h \
    $(wildcard include/config/acpi/numa.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/acpi/pdc_intel.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/numa.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/numa_32.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/mmu.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/io_apic.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/irq_vectors.h \
    $(wildcard include/config/sparse/irq.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/topology.h \
    $(wildcard include/config/x86/ht.h) \
    $(wildcard include/config/x86/64/acpi/numa.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/topology.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/mmdebug.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/elf.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/elf-em.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/elf.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/user.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/user_32.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/auxvec.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/vdso.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/desc.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ldt.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kobject.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/sysfs.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kref.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/tracepoint.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/rcupdate.h \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/completion.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/rcutree.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/local.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/percpu.h \
    $(wildcard include/config/have/legacy/per/cpu/area.h) \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/debug/kmemleak.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/slub_def.h \
    $(wildcard include/config/slub/stats.h) \
    $(wildcard include/config/slub/debug.h) \
    $(wildcard include/config/kmemtrace.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kmemtrace.h \
  /usr/src/linux-headers-2.6.32-5-common/include/trace/events/kmem.h \
  /usr/src/linux-headers-2.6.32-5-common/include/trace/define_trace.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kmemleak.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pfn.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/module.h \
    $(wildcard include/config/m586.h) \
    $(wildcard include/config/m586tsc.h) \
    $(wildcard include/config/m586mmx.h) \
    $(wildcard include/config/mcore2.h) \
    $(wildcard include/config/matom.h) \
    $(wildcard include/config/m686.h) \
    $(wildcard include/config/mpentiumii.h) \
    $(wildcard include/config/mpentiumiii.h) \
    $(wildcard include/config/mpentiumm.h) \
    $(wildcard include/config/mpentium4.h) \
    $(wildcard include/config/mk6.h) \
    $(wildcard include/config/mk8.h) \
    $(wildcard include/config/x86/elan.h) \
    $(wildcard include/config/mcrusoe.h) \
    $(wildcard include/config/mefficeon.h) \
    $(wildcard include/config/mwinchipc6.h) \
    $(wildcard include/config/mwinchip3d.h) \
    $(wildcard include/config/mcyrixiii.h) \
    $(wildcard include/config/mviac3/2.h) \
    $(wildcard include/config/mviac7.h) \
    $(wildcard include/config/mgeodegx1.h) \
    $(wildcard include/config/mgeode/lx.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/module.h \
  /usr/src/linux-headers-2.6.32-5-common/include/trace/events/module.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/sched.h \
    $(wildcard include/config/sched/debug.h) \
    $(wildcard include/config/detect/softlockup.h) \
    $(wildcard include/config/detect/hung/task.h) \
    $(wildcard include/config/core/dump/default/elf/headers.h) \
    $(wildcard include/config/bsd/process/acct.h) \
    $(wildcard include/config/taskstats.h) \
    $(wildcard include/config/audit.h) \
    $(wildcard include/config/virt/cpu/accounting.h) \
    $(wildcard include/config/inotify/user.h) \
    $(wildcard include/config/epoll.h) \
    $(wildcard include/config/posix/mqueue.h) \
    $(wildcard include/config/keys.h) \
    $(wildcard include/config/perf/events.h) \
    $(wildcard include/config/schedstats.h) \
    $(wildcard include/config/task/delay/acct.h) \
    $(wildcard include/config/fair/group/sched.h) \
    $(wildcard include/config/rt/group/sched.h) \
    $(wildcard include/config/blk/dev/io/trace.h) \
    $(wildcard include/config/sysvipc.h) \
    $(wildcard include/config/auditsyscall.h) \
    $(wildcard include/config/rt/mutexes.h) \
    $(wildcard include/config/task/xacct.h) \
    $(wildcard include/config/cpusets.h) \
    $(wildcard include/config/cgroups.h) \
    $(wildcard include/config/futex.h) \
    $(wildcard include/config/fault/injection.h) \
    $(wildcard include/config/latencytop.h) \
    $(wildcard include/config/function/graph/tracer.h) \
    $(wildcard include/config/have/unstable/sched/clock.h) \
    $(wildcard include/config/irq/time/accounting.h) \
    $(wildcard include/config/stack/growsup.h) \
    $(wildcard include/config/cgroup/sched.h) \
    $(wildcard include/config/mm/owner.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/capability.h \
    $(wildcard include/config/security/file/capabilities.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/rbtree.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/mm_types.h \
    $(wildcard include/config/split/ptlock/cpus.h) \
    $(wildcard include/config/want/page/debug/flags.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/aio.h) \
    $(wildcard include/config/mmu/notifier.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/auxvec.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/prio_tree.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/page-debug-flags.h \
    $(wildcard include/config/page/poisoning.h) \
    $(wildcard include/config/page/debug/something/else.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/cputime.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/cputime.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/sem.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/ipc.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/ipcbuf.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/ipcbuf.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/sembuf.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/signal.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/signal.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/signal-defs.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/siginfo.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/siginfo.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/path.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pid.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/proportions.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/percpu_counter.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/seccomp.h \
    $(wildcard include/config/seccomp.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/seccomp.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/seccomp_32.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/unistd.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/unistd.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/unistd_32.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/rculist.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/rtmutex.h \
    $(wildcard include/config/debug/rt/mutexes.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/plist.h \
    $(wildcard include/config/debug/pi/list.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/resource.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/resource.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/resource.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/hrtimer.h \
    $(wildcard include/config/high/res/timers.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/task_io_accounting.h \
    $(wildcard include/config/task/io/accounting.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/latencytop.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/cred.h \
    $(wildcard include/config/debug/credentials.h) \
    $(wildcard include/config/security.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/key.h \
    $(wildcard include/config/sysctl.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/sysctl.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/selinux.h \
    $(wildcard include/config/security/selinux.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/aio.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/aio_abi.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/uio.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/io.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/io_32.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/iomap.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/vmalloc.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/uaccess.h \
    $(wildcard include/config/x86/wp/works/ok.h) \
    $(wildcard include/config/x86/intel/usercopy.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/uaccess_32.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pci.h \
    $(wildcard include/config/pci/iov.h) \
    $(wildcard include/config/pcieaspm.h) \
    $(wildcard include/config/pci/msi.h) \
    $(wildcard include/config/pci.h) \
    $(wildcard include/config/pci/legacy.h) \
    $(wildcard include/config/pcie/ecrc.h) \
    $(wildcard include/config/ht/irq.h) \
    $(wildcard include/config/pci/domains.h) \
    $(wildcard include/config/hotplug/pci.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pci_regs.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/mod_devicetable.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/device.h \
    $(wildcard include/config/debug/devres.h) \
    $(wildcard include/config/devtmpfs.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/klist.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/semaphore.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/device.h \
    $(wildcard include/config/dmar.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pm_wakeup.h \
    $(wildcard include/config/pm.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/io.h \
    $(wildcard include/config/has/ioport.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/irqreturn.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/pci_ids.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dmapool.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/scatterlist.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/scatterlist.h \
    $(wildcard include/config/debug/sg.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pci.h \
    $(wildcard include/config/dma/api/debug.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/mm.h \
    $(wildcard include/config/swap.h) \
    $(wildcard include/config/debug/pagealloc.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/debug_locks.h \
    $(wildcard include/config/debug/locking/api/selftests.h) \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable_32.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable_32_types.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/pgtable-2level.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/pgtable.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/page-flags.h \
    $(wildcard include/config/pageflags/extended.h) \
    $(wildcard include/config/have/mlocked/page/bit.h) \
    $(wildcard include/config/arch/uses/pg/uncached.h) \
    $(wildcard include/config/memory/failure.h) \
    $(wildcard include/config/s390.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/vmstat.h \
    $(wildcard include/config/vm/event/counters.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/pci-dma-compat.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dma-mapping.h \
    $(wildcard include/config/has/dma.h) \
    $(wildcard include/config/have/dma/attrs.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dma-attrs.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/bug.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/scatterlist.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/dma-mapping.h \
    $(wildcard include/config/isa.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kmemcheck.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dma-debug.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/swiotlb.h \
    $(wildcard include/config/swiotlb.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/swiotlb.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/dma-coherent.h \
    $(wildcard include/config/have/generic/dma/coherent.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/dma-mapping-common.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/pci.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/fs.h \
    $(wildcard include/config/dnotify.h) \
    $(wildcard include/config/quota.h) \
    $(wildcard include/config/fsnotify.h) \
    $(wildcard include/config/inotify.h) \
    $(wildcard include/config/fs/posix/acl.h) \
    $(wildcard include/config/debug/writecount.h) \
    $(wildcard include/config/file/locking.h) \
    $(wildcard include/config/block.h) \
    $(wildcard include/config/fs/xip.h) \
    $(wildcard include/config/migration.h) \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/limits.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/kdev_t.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dcache.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/radix-tree.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/fiemap.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/quota.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dqblk_xfs.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dqblk_v1.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dqblk_v2.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/dqblk_qtree.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/nfs_fs_i.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/nfs.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/sunrpc/msg_prot.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/inet.h \
  /usr/src/linux-headers-2.6.32-5-common/include/linux/fcntl.h \
  /usr/src/linux-headers-2.6.32-5-common/arch/x86/include/asm/fcntl.h \
  /usr/src/linux-headers-2.6.32-5-common/include/asm-generic/fcntl.h \
  /root/ik220-driver/kdriver-clearsky/ik220.h \
  /root/ik220-driver/kdriver-clearsky/ik220_if.h \
    $(wildcard include/config/en.h) \
    $(wildcard include/config/ssi.h) \

/root/ik220-driver/kdriver-clearsky/ik220.o: $(deps_/root/ik220-driver/kdriver-clearsky/ik220.o)

$(deps_/root/ik220-driver/kdriver-clearsky/ik220.o):
