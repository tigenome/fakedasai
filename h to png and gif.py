import re
import numpy as np
from PIL import Image, ImageSequence
import os

def extract_data_from_h_file(file_path):
    with open(file_path, 'r') as file:
        content = file.read()

    # Ищем массив данных в формате uint8_t
    match = re.search(r'const uint8_t \w+\[\] PROGMEM = {(.*?)};', content, re.DOTALL)
    if not match:
        print(f"Массив данных не найден в {file_path}")
        return None

    array_str = match.group(1)
    # Удаляем ненужные символы и преобразуем в байты
    array_str = re.sub(r'\s+', '', array_str)
    byte_values = bytes(int(x, 16) for x in array_str.split(','))
    return byte_values

def save_as_gif(byte_data, output_path):
    with open(output_path, 'wb') as file:
        file.write(byte_data)

def convert_gif_to_images(gif_path):
    with Image.open(gif_path) as img:
        frames = [frame.copy() for frame in ImageSequence.Iterator(img)]
        for i, frame in enumerate(frames):
            frame.save(f'{os.path.splitext(gif_path)[0]}_frame_{i}.png')

def process_h_file(filename):
    gif_data = extract_data_from_h_file(filename)
    if gif_data:
        # Сохраняем бинарные данные в файл
        gif_path = os.path.splitext(filename)[0] + '.gif'
        save_as_gif(gif_data, gif_path)
        print(f"GIF сохранен как {gif_path}")

        # Конвертируем GIF в изображения
        convert_gif_to_images(gif_path)
        print(f"Изображения сохранены из {gif_path}")

# Обрабатываем все файлы .h в текущем каталоге
current_directory = os.path.dirname(os.path.realpath(__file__))
for h_file in os.listdir(current_directory):
    if h_file.endswith('.h'):
        process_h_file(os.path.join(current_directory, h_file))