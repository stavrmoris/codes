ШАГ 1: Создаем "Умный Архив" (Патч)

Мы сейчас запакуем в один архив всё, что мы украли у Убунту, прямо с сохранением структуры папок.

Введи в терминале одну команду (она соберет ядро, модули, прошивки и библиотеки в один файл):
code Bash

tar -czvf cix_alt_bsp_patch.tar.gz \
  /boot/*6.6.89-cix* \
  /lib/modules/6.6.89-cix \
  /lib/firmware \
  /usr/share/cix

У тебя в папке появится файл cix_alt_bsp_patch.tar.gz. Это и есть твоя разработка.
ШАГ 2: Пишем скрипт-установщик

Создадим скрипт, который будет всё это распаковывать и настраивать автоматически.
Введи:
code Bash

nano install_npu_alt.sh

Вставь туда этот роскошный код:
code Bash

#!/bin/bash
# Скрипт интеграции CIX P1 BSP в Alt Linux p11
# Автор: [Твое Имя]

echo "=== [1/4] Распаковка ядра и библиотек CIX ==="
tar -xzvf cix_alt_bsp_patch.tar.gz -C /

echo "=== [2/4] Генерация загрузочного образа (initrd) ==="
make-initrd -k 6.6.89-cix

echo "=== [3/4] Настройка GRUB (отключение графики для ядра 6.6) ==="
# Меняем параметры загрузки на nomodeset
sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT=.*/GRUB_CMDLINE_LINUX_DEFAULT="nomodeset"/' /etc/default/grub
grub-mkconfig -o /boot/grub/grub.cfg

echo "=== [4/4] Автозагрузка драйвера NPU ==="
echo "aipu" > /etc/modules-load.d/aipu.conf
echo "SUBSYSTEM==\"aipu\", MODE=\"0666\"" > /etc/udev/rules.d/99-aipu.rules

echo "=== УСТАНОВКА ЗАВЕРШЕНА! ==="
echo "Пожалуйста, перезагрузите устройство и в BIOS (CIX System Manager) выберите режим ACPI."
echo "В меню GRUB выберите ядро 6.6.89-cix."

Сохрани (Ctrl+O, Enter, Ctrl+X).
Сделай скрипт исполняемым:
code Bash

chmod +x install_npu_alt.sh
