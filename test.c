Делаем так
На рабочей SD-системе под 6.12:

cp -av /boot/initrd-6.6.89-cix.img /boot/initrd-6.6.89-cix.img.sd-bak 2>/dev/null || true
cp -av /mnt/ssd/boot/initrd-6.6.89-cix.img /boot/initrd-6.6.89-cix.img
cp -av /mnt/ssd/lib/firmware/* /lib/firmware/ 2>/dev/null || true
sync
Потом проверь, что файл реально заменился
ls -l /boot/initrd-6.6.89-cix.img
ls -l /mnt/ssd/boot/initrd-6.6.89-cix.img
Размеры должны быть одинаковые или очень близкие.

Дальше не трогаем параметры, просто обновим grub ещё раз
grub-mkconfig -o /boot/grub/grub.cfg
grep -nA2 -B1 "vmlinuz-6.6.89-cix" /boot/grub/grub.cfg
И снова пробуем
reboot
Выбирай тот же 6.6.89-cix в Advanced.
