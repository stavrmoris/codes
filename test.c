1. Проверь, виден ли SSD
Выполни:

lsblk -f
Найди снова nvme0n1p3 и nvme0n1p1.

2. Пересмонтируй SSD
mkdir -p /mnt/ssd
mkdir -p /mnt/ssd-efi
mount /dev/nvme0n1p3 /mnt/ssd
mount /dev/nvme0n1p1 /mnt/ssd-efi
3. Проверь, что там появилось
ls /mnt/ssd
ls /mnt/ssd/boot
ls /mnt/ssd/boot/grub
4. И сразу сними рабочую конфигурацию
cat /mnt/ssd/boot/extlinux/extlinux.conf 2>/dev/null || echo "no extlinux on ssd"
grep -nA3 -B2 "6.6.89-cix" /mnt/ssd/boot/grub/grub.cfg
