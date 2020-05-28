`usemem.c` is a program to make high memory utilization and its memory
is swapped out to swap devices.

`setup.sh` is a bash script that creates zram device, sets up swap
area in the created zram disk, activates the swap device and run
usemem with 1GB as an argument.

Note that please change the 1GB argument according to your test
environment. The 1GB is simply becuase I used a guest machine with 1GB
memory for testing.

You can check utilization of swap device by swapon command:

```
# swapon
NAME       TYPE      SIZE  USED PRIO
/dev/zram0 partition 512M  82.3M  -2
```

If you recognize the swap device is actually used, create a memory dump
in your favorite way.

After you get a memory dump, you can test crash's zram feature as
follows. Note that the address range of the buffer of `usemem` is
logged into the kernel message so you can see it later by log command.

```
crash> ps | grep usemem
  36125    969   0  ffff891d317a0000  IN  74.9 1870108 1571584  usemem
crash> set 36125
    PID: 36125
COMMAND: "usemem"
   TASK: ffff891d317a0000  [THREAD_INFO: ffff891d317a0000]
    CPU: 0
  STATE: TASK_INTERRUPTIBLE
crash> log | tail -n 1
[67215.497485] [00007f07398c5000-00007f07ab8c5001]
crash> vtop -u 0x00007f07398c5000
VIRTUAL     PHYSICAL
7f07398c5000  (not mapped)
                                                                                                                                                                               
   PGD: 6da707f0 => 800000000232b067
   PUD: 232b0e0 => 7ccbe067
   PMD: 7ccbee60 => 6f807067
   PTE: 6f807628 => 7fffffffd0b6002

      PTE           SWAP     OFFSET
7fffffffd0b6002  /dev/zram0   96847

      VMA           START       END     FLAGS FILE
ffff891d0e5d5320 7f07398c4000 7f07ab8c6000 8100073

SWAP: /dev/zram0  OFFSET: 96847

crash> mod -s zram
     MODULE       NAME                      SIZE  OBJECT FILE
ffffffffc0894400  zram                     28672  /usr/lib/debug/usr/lib/modules/5.6.6-200.fc31.x86_64/kernel/drivers/block/zram/zram.ko.debug
crash> rd -u 0x00007f07398c5000
    7f07398c5000:  0000000000000000                    ........
crash> rd -u 0x00007f07398c6000
    7f07398c6000:  0000000000001000                    ........
crash> rd -u 0x00007f07398c7000
    7f07398c7000:  0000000000002000                    . ......
crash> rd -u 0x00007f07398c8000
    7f07398c8000:  0000000000003000                    .0......
crash> quit
```
