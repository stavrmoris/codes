#!/bin/bash
# Скрипт интеграции CIX P1 BSP в Alt Linux p11
# Автор: Ставр Марискин

echo "Разработано @stavrmoris"

# Получаем текущую директорию, где лежит скрипт (чтобы точно найти архив)
DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=== [1/4] Распаковка ядра и библиотек CIX ==="
if[ ! -f "$DIR/cix_alt_bsp_patch.tar.gz" ]; then
    echo "ОШИБКА: Архив cix_alt_bsp_patch.tar.gz не найден в папке $DIR!"
    exit 1
fi
tar -xzvf "$DIR/cix_alt_bsp_patch.tar.gz" -C /

echo "=== [2/4] Генерация загрузочного образа (initrd) ==="
make-initrd -k 6.6.89-cix

echo "=== [3/4] Настройка GRUB (отключение графики для ядра 6.6) ==="
# Бронебойный способ: комментируем старую строку (если есть) и пишем свою
sed -i '/^GRUB_CMDLINE_LINUX_DEFAULT/ s/^/#/' /etc/default/grub
echo 'GRUB_CMDLINE_LINUX_DEFAULT="nomodeset"' >> /etc/default/grub

# Обновляем загрузчик
grub-mkconfig -o /boot/grub/grub.cfg

echo "=== [4/4] Автозагрузка драйвера NPU ==="
# Заставляем систему загружать aipu.ko при старте
echo "aipu" > /etc/modules-load.d/aipu.conf
# Даем права 666 файлу /dev/aipu автоматически
echo 'SUBSYSTEM=="aipu", MODE="0666"' > /etc/udev/rules.d/99-aipu.rules

echo "=== УСТАНОВКА ЗАВЕРШЕНА! ==="
echo "Пожалуйста, перезагрузите устройство и в BIOS (CIX System Manager) выберите режим ACPI."
echo "В меню GRUB выберите ядро 6.6.89-cix."


    echo "=== [3/4] Настройка GRUB (отключение графики для ядра 6.6) ==="
# 1. Пусть система генерирует свой стандартный конфиг
grub-mkconfig -o /boot/grub/grub.cfg

# 2. МЕТОД КУВАЛДЫ: Находим в сгенерированном файле все строки запуска ядра 6.6 
# и насильно приписываем 'nomodeset' в самый конец строки!
sed -i '/linux[[:space:]]*\/boot\/vmlinuz-6.6.89-cix/ s/$/ nomodeset/' /boot/grub/grub.cfg
