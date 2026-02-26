

# 🐧 Complete Roadmap: Building a Custom Linux Kernel with a "Hello World" System Call

This is a comprehensive, step-by-step roadmap to help you complete your assignment. I'll break it down into **learning phases** and **execution phases**.

---

## Phase 0: Prerequisites & Foundational Knowledge

Before touching the kernel, make sure you're comfortable with these basics:

### 0.1 — Linux Command Line Basics
- **What to learn:** Navigation (`cd`, `ls`, `pwd`), file editing (`nano`, `vim`), permissions (`chmod`), package management (`apt`), and using `sudo`.
- **Resources:**
  - [LinuxCommand.org](http://linuxcommand.org/lc3_learning_the_shell.php) — Free, beginner-friendly
  - [Ubuntu Command Line Tutorial (Official)](https://ubuntu.com/tutorials/command-line-for-beginners)

### 0.2 — Basic C Programming
- System calls are written in C. You need to understand functions, header files, and return types.
- **Resources:**
  - [W3Schools C Tutorial](https://www.w3schools.com/c/)
  - [Programiz C Tutorial](https://www.programiz.com/c-programming)

### 0.3 — What is a Kernel?
- Understand that the kernel is the core of the OS — it manages hardware, memory, processes, and system calls.
- **Resources:**
  - [OSDev Wiki — What is a Kernel?](https://wiki.osdev.org/What_is_a_kernel%3F)
  - [Linux Kernel Wikipedia](https://en.wikipedia.org/wiki/Linux_kernel)

---

## Phase 1: Understanding System Calls

### 1.1 — What is a System Call?
- A system call is the interface between user-space programs and the kernel. When you call `write()`, `read()`, or `open()` in C, you're making system calls.
- **Resources:**
  - [GeeksforGeeks — System Calls in OS](https://www.geeksforgeeks.org/introduction-of-system-call/)
  - [Linux man page for syscalls](https://man7.org/linux/man-pages/man2/syscalls.2.html)

### 1.2 — How System Calls Work in Linux
- Learn the flow: User program → C library (glibc) → `syscall` instruction → Kernel handler → System call table → Your function
- **Resources:**
  - [Linux Inside — System Calls Chapter](https://0xax.gitbooks.io/linux-insides/content/SysCall/) — **Excellent deep dive**
  - [Anatomy of a System Call (LWN.net)](https://lwn.net/Articles/604287/) — Detailed article

---

## Phase 2: Setting Up Your Environment

### 2.1 — Use a Virtual Machine (STRONGLY Recommended)
> ⚠️ **NEVER compile and install a custom kernel on your main machine during learning.** Use a VM.

- Install **VirtualBox** or **VMware Workstation Player**
- Create an **Ubuntu 22.04 LTS** (or similar) VM with:
  - At least **2 CPU cores**
  - At least **4 GB RAM**
  - At least **50-60 GB disk space** (kernel compilation needs space!)

### 2.2 — Install Required Packages
Once your VM is running, install build dependencies:

```bash name=install-dependencies.sh
sudo apt update && sudo apt upgrade -y
sudo apt install -y build-essential libncurses-dev bison flex libssl-dev \
  libelf-dev dwarves wget fakeroot gcc bc cpio
```

---

## Phase 3: Downloading and Renaming the Kernel (Assignment Step 1)

### 3.1 — Download a Newer Kernel
Go to [kernel.org](https://www.kernel.org/) and pick a stable version.

```bash name=download-kernel.sh
cd ~
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.7.tar.xz
tar -xf linux-6.7.tar.xz
cd linux-6.7
```

### 3.2 — Rename the Kernel to Your Roll Number
This is how `uname -r` will display your roll number. You need to edit the **top-level `Makefile`** of the kernel source.

Open the Makefile:
```bash name=open-makefile.sh
nano Makefile
```

At the very top you'll see something like:
```makefile name=Makefile-before
# SPDX-License-Identifier: GPL-2.0
VERSION = 6
PATCHLEVEL = 7
SUBLEVEL = 0
EXTRAVERSION =
NAME = ...
```

Change `EXTRAVERSION` to your roll number. For example, if your roll number is `22i-1234`:
```makefile name=Makefile-after
# SPDX-License-Identifier: GPL-2.0
VERSION = 6
PATCHLEVEL = 7
SUBLEVEL = 0
EXTRAVERSION = -22i-1234
NAME = ...
```

This will make `uname -r` output: **`6.7.0-22i-1234`**

> 💡 **Tip:** Some instructors want `uname -r` to show *only* the roll number. In that case, you can set something like `VERSION = 22`, `PATCHLEVEL = i`, `SUBLEVEL = 1234`, and `EXTRAVERSION =` — but the above approach is the standard, clean way.

---

## Phase 4: Adding the "Hello World" System Call (Assignment Step 2)

This is the core of your assignment. Follow these steps carefully:

### 4.1 — Create the System Call Directory and Code

```bash name=create-syscall-directory.sh
# From inside the kernel source directory (e.g., ~/linux-6.7)
mkdir hello
```

Create the system call source file:

```c name=hello/hello.c
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(hello)
{
    printk(KERN_INFO "Hello World! This is my custom system call.\n");
    return 0;
}
```

Create the Makefile for this directory:

```makefile name=hello/Makefile
obj-y := hello.o
```

### 4.2 — Add Your Directory to the Core Kernel Makefile

Edit the **top-level Makefile** again:

```bash name=edit-core-makefile.sh
nano Makefile
```

Find the line that says `core-y += kernel/ certs/ mm/ fs/ ipc/ security/ crypto/` (or similar) and add `hello/` to it:

```makefile name=Makefile-core-y
core-y += kernel/ certs/ mm/ fs/ ipc/ security/ crypto/ hello/
```

> ⚠️ **Note:** In newer kernels (6.1+), this line might look different. Search for `core-y` and you might find it in `Kbuild` file instead of `Makefile`. If you see a file called `Kbuild` in the root, check there too.

### 4.3 — Add the System Call to the System Call Table

The system call table location depends on your architecture. For **x86_64**:

```bash name=edit-syscall-table.sh
nano arch/x86/entry/syscalls/syscall_64.tbl
```

Scroll to the end of the numbered entries and add a new entry. Find the last used number (e.g., `450`) and add:

```text name=syscall_64.tbl-entry
451    common    hello    sys_hello
```

> 📌 **Format:** `<number> <abi> <name> <entry_point>`
> Make note of the number `451` (or whatever you use) — you'll need it to test.

### 4.4 — Add the System Call Prototype to the Header

```bash name=edit-syscalls-header.sh
nano include/linux/syscalls.h
```

Before the final `#endif`, add:

```c name=syscalls.h-addition
asmlinkage long sys_hello(void);
```

---

## Phase 5: Configuring and Compiling the Kernel (Assignment Step 3)

### 5.1 — Generate the Configuration

```bash name=configure-kernel.sh
# Copy your current running kernel's config as a starting point
cp /boot/config-$(uname -r) .config

# Update config for the new kernel version (accept defaults for new options)
make olddefconfig
```

Alternatively, your lab manual may ask you to use the menu-based configurator:
```bash name=menuconfig.sh
make menuconfig
```
> In `menuconfig`, you can just **save and exit** if you don't need to change anything specific.

### 5.2 — Handle the Trusted Keys Issue (Common Gotcha!)

You'll likely hit an error about signing keys. Fix it:

```bash name=fix-signing-keys.sh
scripts/config --disable SYSTEM_TRUSTED_KEYS
scripts/config --disable SYSTEM_REVOCATION_KEYS
```

### 5.3 — Compile the Kernel

```bash name=compile-kernel.sh
# Use all available CPU cores for faster compilation
make -j$(nproc)
```

> ⏱️ **This will take 30 minutes to 2+ hours** depending on your hardware. Be patient!

### 5.4 — Install Modules and Kernel

```bash name=install-kernel.sh
sudo make modules_install
sudo make install
```

### 5.5 — Update Bootloader and Reboot

```bash name=reboot.sh
sudo update-grub
sudo reboot
```

After reboot, select your new kernel from the GRUB menu if it doesn't boot automatically.

---

## Phase 6: Testing Everything

### 6.1 — Verify Kernel Version (Roll Number)

```bash name=verify-kernel.sh
uname -r
# Expected output: 6.7.0-22i-1234  (your roll number)
```

### 6.2 — Test the Hello World System Call

Write a test program:

```c name=test_hello.c
#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

int main() {
    // Use the syscall number you assigned (e.g., 451)
    long ret = syscall(451);
    if (ret == 0) {
        printf("System call executed successfully!\n");
    } else {
        printf("System call failed. Return value: %ld\n", ret);
    }
    return 0;
}
```

Compile and run:
```bash name=test-syscall.sh
gcc -o test_hello test_hello.c
sudo ./test_hello
```

Check the kernel log for the output:
```bash name=check-dmesg.sh
dmesg | tail
# You should see: "Hello World! This is my custom system call."
```

---

## 📚 Consolidated Resource List (Best to Worst by Relevance)

| # | Resource | Why It's Useful |
|---|----------|-----------------|
| 1 | [Linux Inside — System Calls](https://0xax.gitbooks.io/linux-insides/content/SysCall/) | Deep understanding of how syscalls work internally |
| 2 | [kernel.org — Official Source](https://www.kernel.org/) | Where to download kernel source |
| 3 | [LWN.net — Anatomy of a System Call](https://lwn.net/Articles/604287/) | Authoritative article on syscall mechanics |
| 4 | [The Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/) | Great for understanding kernel-space programming |
| 5 | [Kernel Newbies](https://kernelnewbies.org/) | Beginner-friendly kernel development community |
| 6 | [Robert Love — Linux Kernel Development (Book)](https://www.oreilly.com/library/view/linux-kernel-development/9780768696974/) | The definitive book on kernel development |
| 7 | [OSDev Wiki](https://wiki.osdev.org/) | Understanding OS concepts at a deep level |
| 8 | Your **Lab Manual** | Follow it closely — your grading depends on it |

---

## ⚠️ Common Pitfalls & Tips

| Pitfall | Solution |
|---------|----------|
| Running out of disk space | Allocate **50-60 GB** to your VM |
| Kernel compilation errors about certs/keys | Run the `scripts/config --disable` commands from Step 5.2 |
| `BTF` errors during compilation | `sudo apt install dwarves` or `scripts/config --disable DEBUG_INFO_BTF` |
| Forgot the syscall number | Check `arch/x86/entry/syscalls/syscall_64.tbl` for your entry |
| VM won't boot after install | Use GRUB to boot back into the old kernel and fix errors |
| `core-y` line not found in Makefile | Check the `Kbuild` file in the root directory instead |

---

## 🗺️ Suggested Learning Timeline

| Day | Task |
|-----|------|
| **Day 1** | Set up VM, install Ubuntu, install dependencies |
| **Day 2** | Read about kernels and system calls (Phase 0 & 1 resources) |
| **Day 3** | Download kernel, rename it, and add the hello world syscall (Phase 3 & 4) |
| **Day 4** | Configure and compile the kernel (Phase 5) — this takes time |
| **Day 5** | Test everything, troubleshoot, and document your work (Phase 6) |

---

This roadmap covers everything from zero knowledge to a working custom kernel. Follow your **lab manual** alongside this guide — if there's ever a conflict, **your lab manual takes priority** since that's what you'll be graded on. Good luck! 🚀
