# Building a Linux Kernel from Scratch

This repository documents the journey of building a Linux operating system kernel from scratch, broken down into six fundamental stages. Each stage is represented by a folder named `lab1` through `lab6`. Each `lab` directory contains two subdirectories: `doc` and `src`, representing the experiment report (written in Chinese) and the source code, respectively.

## Labs Overview:

- **Lab 1:** Minimal OS (Booted using Multiboot Protocol)
- **Lab 2:** printf Porting
- **Lab 3:** Memory Detection + Basic Memory Management
- **Lab 4:** Task Management + Context Switching + FCFS Scheduling
- **Lab 5:** Terminal, Clock, Timer
- **Lab 6:** Mutex Locks and Preemptive Scheduling

### Lab 1: Minimal OS

In this experiment, a minimal 'hello world' kernel supporting Multiboot booting protocol is developed. It can be booted using GRUB (+ Virtual Machine) or directly using QEMU. QEMU is an open-source hosted virtual machine monitor that conforms to the protocol requirements. In this experiment, QEMU is used to boot the OS kernel.

**Compile & Run:**

```bash
$ make
$ qemu-system-i386 -kernel multibootHeader.bin -serial stdio
```

### Lab 2 to Lab 5

In these experiments, a script named `source2run.sh` is provided to simplify the compile and run process.

**Compile & Run:**

```bash
$ ./source2run.sh
```

### Lab 6: Mutex Locks and Preemptive Scheduling

In this experiment, two different scheduling algorithms are explored. To compile the source code, use the provided script `source2img.sh` with the respective test argument. The serial port is redirected to a pseudoterminal, whose identifier will be displayed upon running. You can then use the `screen` command to interact with the pseudoterminal and test the functionality.

**Compile:**

```bash
$ ./source2img.sh test1_sjf
$ ./source2img.sh test2_prio0
```

**Run:**

```bash
$ sudo screen /dev/pts/0  # Suppose it's /dev/pts/0
```

## Getting Started:

1. Clone this repository to your local machine.

```bash
$ git clone https://github.com/RunbangZhang/Build-Linux-Kernel-from-Scratch.git
```

1. Navigate to the respective `lab` folder and follow the instructions in the `doc` directory's experiment report.

## Contact:

If you have any questions or run into issues, please open an issue on this repository or contact the repository owner.

Enjoy your journey towards building a Linux kernel from scratch!



