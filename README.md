# SDK-1.1M
![SDK-1.1M](https://cdn.bitrix24.ru/b7462885/landing/2ca/2ca398edf04bc44f40c235946ee1b2f0/sdk11_2x.jpg)

### Описание
SDK-1.1M - комплексная платформа для технического творчества и изучения электроники:
стенд-конструктор, платы расширения, аксессуары, документация и методическое обеспечение.

Стенд SDK-1.1M состоит из платы ввода-вывода и заменяемого процессорного модуля. В настоящий момент стенд поставляется с микроконтроллерами STM32F407 и STM32F427. На плате ввода-вывода располагаются разъемы для подключения модулей расширения SDK-Х и модулей, совместимых с Arduino.

[Дополнительные статьи про SDK-1.1M](https://sdk11.ru/tag/sdk-1-1m/)

### Основные характеристики SDK-1.1MC.407
- Микроконтроллер STM32F407VGT6;
- Внешняя EEPROM объемом 1 Кбит;
- Часы реального времени MCP79411;
- Графический OLED дисплей WEO012864DL фирмы Winstar;
- Интерфейс ввода/вывода общего назначения (GPIO) PCA9538PW;
- Инерционный модуль iNEMO LSM9DS1;
- Электромагнитный излучатель звука HC0903A;
- Набор сигнальных светодиодов (зеленый, желтый, красный);
- Клавиатура из 12 кнопок;
- RS-485;
- Ethernet 10/100;
- USB.

![Структурная схема SDK-1.1M](https://cdn.bitrix24.ru/b7462885/landing/e81/e81fd718179eaa9734f0d0913b2b0d46/SDK-1.1MC.407.png)

### Рекомендуемый инструментарий для разработки
- STM32CubeMX + SystemWorkbench for STM32
- STM32CubeIDE

### Конфигурационные файлы для OpenOCD

#### Для STM32F407VG/STM32F427VI
```
interface ftdi
ftdi_vid_pid 0x0403 0x6010
ftdi_layout_init 0x0408 0x0ffb
ftdi_layout_signal nSRST -oe 0x0800 

set WORKAREASIZE 0x8000

transport select jtag

set CHIPNAME STM32F4x7
set BOARDNAME SDK11M

reset_config srst_only

set CONNECT_UNDER_RESET 1
source [find target/stm32f4x.cfg]
```

### Установка драйвера

#### Windows 7/8/10:
Для программирования стенда необходим драйвер WinUSB, установить который можно при помощи программы [Zadig](https://zadig.akeo.ie/).

##### Порядок установки:
- Подключить SDK-1.1M к компьютеру через разъем <b>USB DBG</b>
- Нажать <b>Options</b>
- Установить галочку <b>List All Devices</b>
- Выбрать устройство <b>Dual RS232 (Interface 0)</b> или <b>RS232 (Interface 0)</b> или <b>SDK 1.1M Debugger (Interface 0)</b>. Не выбирать устройства с <b>Interface 1</b>!
- Выбрать из списка доступных драйверов <b>WinUSB</b>
- Нажать кнопку <b>Replace/Install</b>

#### Debian/Ubuntu:

##### Порядок установки:
- Подключить SDK-1.1M к компьютеру через разъем <b>USB DBG</b>
- Установить драйвер libusb:
`sudo apt-get install libusb-1.0-0`
- Открыть файл:
`sudo nano /etc/udev/rules.d/50-myusb.rules` или `sudo vi/etc/udev/rules.d/50-myusb.rules`
- Добавить строку:
`SUBSYSTEMS=="usb", ATTRS{idVendor}=="0403",ATTRS{idProduct}=="6010",TAG+="uaccess"`
- Сохранить и закрыть файл
- Ввести команду:
`sudo udevadm control —reload`
- Отключить и снова подключить SDK-1.1M к компьютеру

#### macOS:
Установка драйвера <b>не требуется</b>.

### Электронные ресурсы
- [Учебное пособие](https://books.ifmo.ru/file/pdf/3031.pdf) – техническая документация, лабораторные работы
- [SDK-1.1M](https://sdk11.ru) – сайт продукта
- [ВКонтакте](https://vk.com/lmtspb) – группа ВКонтакте
- [Дизайн-центр ЛМТ](http://lmt.spb.ru) – доступные конфигурации стенда, дополнительное оборудование, цены, техническая поддержка
