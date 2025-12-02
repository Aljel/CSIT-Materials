#!/bin/bash

# Получаем абсолютный путь к текущей директории
current_dir="$(pwd)"

# Запускаем DOSBox с монтированием текущей директории как диска C:
dosbox -c "mount c \"$current_dir\"" -c "c:"

