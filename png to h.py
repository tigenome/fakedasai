import os
from PIL import Image
import re

# Create and save the .h file content
def save_h_file(content, file_path):
    with open(file_path, 'w') as h_file:
        h_file.write(content)

# Получаем путь к текущему каталогу, где находится скрипт
current_directory = os.path.dirname(os.path.realpath(__file__))
os.chdir(current_directory)

# Вывод текущего каталога
print(f"Текущий каталог: {current_directory}")

# Вывод всех файлов в текущем каталоге
print("Файлы в текущем каталоге:")
for file_name in os.listdir("."):
    print(file_name)

# Функция для загрузки и сортировки изображений из текущего каталога
def load_images():
    # Сортируем файлы с учетом чисел в названии
    image_files = sorted([file_name for file_name in os.listdir(".") if file_name.endswith(".png")], key=lambda f: int(re.sub('\D', '', f)))
    images = []
    for file_name in image_files:
        print(f"Загружаем изображение: {file_name}")  # Отладочный вывод
        images.append(Image.open(file_name))
    return images

# Загрузка изображений
images = load_images()

# Проверка, что изображения были загружены
if images:
    # Настройка задержки между кадрами (в миллисекундах)
    duration_ms = 100  # 500 миллисекунд = 0.5 секунды

    # Сохранить как циклический GIF с задержкой между кадрами
    gif_path = "adjusted_speed_animation.gif"
    images[0].save(gif_path, save_all=True, append_images=images[1:], loop=0, duration=duration_ms)
    print(f"GIF сохранен как {gif_path}")

    # Чтение GIF-файла и получение его байтовых данных
    with open(gif_path, 'rb') as gif_file:
        gif_data = gif_file.read()

    # Размер данных и количество кадров
    data_size = len(gif_data)
    num_frames = len(images)

    # Создание содержимого .h файла
    h_file_content = f"""
// 30
// Data size = {data_size} bytes
//
// GIF, Compression=LZW, Size: 128 x 64, 8-Bpp
// {num_frames} frames
//
// for non-Arduino builds...
#ifndef PROGMEM
#define PROGMEM
#endif
const uint8_t _30[] PROGMEM = {{
{','.join(f'0x{byte:02x}' for byte in gif_data)}
}};
"""

    # Путь к .h файлу
    h_file_path = 'animated_slowed_30_final.h'

    # Сохранение .h файла
    with open(h_file_path, 'w') as h_file:
        h_file.write(h_file_content)
    print(f".h файл сохранен как {h_file_path}")

else:
    print("Не удалось найти файлы PNG в текущем каталоге.")
