Вот ключевая штука: ты грузился через Advanced в GRUB, значит все наши правки в extlinux.conf вообще не участвовали. Мы правили не тот загрузочный путь.

Надо править GRUB cmdline для ядра 6.6.89-cix.

Сделай так на рабочем 6.12:

cp -a /etc/default/grub /etc/default/grub.bak
grep -n 'GRUB_CMDLINE' /etc/default/grub
nano /etc/default/grub
Найди/поставь такие строки:

GRUB_CMDLINE_LINUX="panic=30 nomodeset cma=1300M iommu=off noresume"
GRUB_CMDLINE_LINUX_DEFAULT=""
Сохрани, потом:

grub-mkconfig -o /boot/grub/grub.cfg
Проверь, что параметры реально попали в 6.6.89-cix:

grep -nA2 -B1 "vmlinuz-6.6.89-cix" /boot/grub/grub.cfg
Там должна быть строка с чем-то вроде:

linux /boot/vmlinuz-6.6.89-cix root=UUID=... panic=30 nomodeset cma=1300M iommu=off noresume
Потом:

reboot
И снова выбирай в Advanced ядро 6.6.89-cix.

Если опять будет black screen, следующий шаг уже не в параметрах, а в копировании точного рабочего initrd/EFI-запуска со старого SSD. Но сейчас сначала добьём именно GRUB, потому что до этого мы били мимо цели.
