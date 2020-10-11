#! /bin/bash

modprobe zram

if ! ZRAM_DISK="$(zramctl -a lzo -f -s 512M)"; then
    echo "Failed to create zram disk" >&2
    exit 1
fi

if ! mkswap "$ZRAM_DISK"; then
    echo "Failed to create swap" >&2
    exit 1
fi

# Don't use other swap disks.
swapoff -a

swapon "$ZRAM_DISK"

./usemem $(( (1 << 30) + (900 << 20) )) >/dev/kmsg
