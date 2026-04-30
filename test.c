ШАГ 1: Проверяем пульс (Самое приятное)

Введи:
code Bash

uname -r

(Если видишь 6.6.89-cix — кайфуй).

Теперь «будим» наш NPU. Так как ядро родное, никаких патчей и insmod не нужно, оно всё знает само:
code Bash

modprobe aipu
chmod 666 /dev/aipu
ls -l /dev/aipu

(Если увидел crw-rw-rw- ... /dev/aipu — всё, аппаратная часть идеальна. Ядро подало ток на чип и создало канал связи).
ШАГ 2: Возвращаем "Мозги" (UMD библиотеки)

Нам нужна та самая папка /usr/share/cix с драйверами для Питона. Втыкай флешку с Убунту (если вытаскивал), смотри её имя через lsblk (например, sda2) и копируй:
code Bash

mkdir -p /mnt/ubuntu
mount /dev/sda2 /mnt/ubuntu   # ЗАМЕНИ sda2 на свой раздел с флешки!

cp -r /mnt/ubuntu/usr/share/cix /usr/share/

umount /mnt/ubuntu

ШАГ 3: Ставим среду (Conda и Python)

Так как Альт на флешке чистый, быстро накатываем Конду и зависимости (всё под рутом):
code Bash

apt-get update
apt-get install -y wget curl git git-lfs patch gcc gcc-c++ make glibc-devel-static

# Качаем и ставим Конду
wget https://github.com/conda-forge/miniforge/releases/latest/download/Miniforge3-Linux-aarch64.sh
bash Miniforge3-Linux-aarch64.sh -b -p /root/miniforge3
source /root/miniforge3/bin/activate

# Создаем ИИ-окружение
conda create -y -n cix_npu python=3.11
conda activate cix_npu

# ВАЖНО: Ставим китайские либы
pip install /usr/share/cix/pypi/libnoe*.whl --force-reinstall
pip install /usr/share/cix/pypi/ZhouyiOperators*.whl --force-reinstall
pip install /usr/share/cix/pypi/onnxruntime_zhouyi*.whl --force-reinstall
pip install "numpy<2.0" opencv-python tqdm

ШАГ 4: Качаем модельку (Точечно!)

Качаем только ResNet50, чтобы не ждать полчаса:
code Bash

cd /root
git lfs install
git clone https://www.modelscope.cn/cix/ai_model_hub_25_Q3.git
cd ai_model_hub_25_Q3
git lfs pull --include="models/ComputeVision/Image_Classification/onnx_resnet_v1_50/*"ШАГ 1: Грузимся в рабочую систему

    Перезагрузи плату (отключи-включи питание).

    В меню GRUB НЕ ВЫБИРАЙ 6.6! Выбери стандартное альтовское ядро 6.12....

    Система должна нормально загрузиться с SD-карты.

    Заходи в терминал, делай su - (ты уже знаешь пароль).

ШАГ 2: Пересаживаем ядро (Крадем с SSD на SD)

Проверь, что SSD примонтирован (если нет — примонтируй):
code Bash

mkdir -p /mnt/ssd
mount /dev/nvme0n1p3 /mnt/ssd

Теперь копируем ядро 6.6 и его модули прямо в твою новую систему на SD-карте:
code Bash

cp /mnt/ssd/boot/vmlinuz-6.6.89-cix /boot/
cp /mnt/ssd/boot/System.map-6.6.89-cix /boot/
cp /mnt/ssd/boot/config-6.6.89-cix /boot/

cp -r /mnt/ssd/lib/modules/6.6.89-cix /lib/modules/

(Прошивки firmware и китайскую папку /usr/share/cix скопируешь позже, сейчас главное — ядро).
ШАГ 3: Собираем "свой" Initrd

Это то, на чем споткнулся GPT. Альту нужен свой загрузочный образ, чтобы он понял, что грузиться надо с SD-карты (sdb2), а не с SSD.
Вводи:
code Bash

make-initrd -k 6.6.89-cix

(Дождись завершения, он соберет файл initrd-6.6.89-cix.img в папке /boot).
ШАГ 4: Лечим черный экран (nomodeset)

Открывай конфиг загрузчика новой системы:
code Bash

nano /etc/default/grub

Найди строку GRUB_CMDLINE_LINUX_DEFAULT.
Удали оттуда слова quiet и splash.
Впиши туда nomodeset.
Должно получиться вот так:
GRUB_CMDLINE_LINUX_DEFAULT="nomodeset"
(Сохраняй: Ctrl+O, Enter, Ctrl+X).
ШАГ 5: Обновляем GRUB (чтобы он забыл про SSD)

Теперь мы заставим GRUB перечитать все диски и прописать правильные UUID (именно от SD-карты).
code Bash

grub-mkconfig -o /boot/grub/grub.cfg

ШАГ 6: БИОС И ПЕРЕЗАГРУЗКА

    Вводи reboot.

    При загрузке быстро жми ESC / F2, чтобы зайти в BIOS.

    Зайди в CIX System Manager -> System Table Selection и ОБЯЗАТЕЛЬНО проверь, что там стоит ACPI! (Вендорное ядро 6.6 работает только через ACPI, если там стоит Device Tree — будет черный экран или зависание). Сохрани (F10).

    В меню GRUB выбирай наше пересаженное ядро 6.6.89-cix.
