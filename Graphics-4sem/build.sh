#!/bin/bash

debug() {
    set -o xtrace
    cmake -B Build -D CMAKE_BUILD_TYPE=Debug &&
        cmake --build ./Build --parallel
}

release() {
    set -o xtrace
    cmake -B Build -D CMAKE_BUILD_TYPE=Release &&
        cmake --build ./Build --parallel
}

clean() {
    set -o xtrace
    rm -rf ./Build
}

zip() {
    git archive --format zip -o task1_zhelezko.zip HEAD
}

run () {
	release
	"./Build/Projects/$1/$1"
}

help() {
    echo "Использование:"
    echo "./build.sh release -> Собрать проект с помощью CMake и make -C ./Build/"
    echo "./build.sh debug -> Собрать проект с сохранением информации для отладки"
    echo "./build.sh clean -> Удалить содержимое директории ./Build/"
    echo "./build.sh run [LAB] -> Собрать и запустить лабораторную"
}

case "$1" in
debug) debug ;;
release) release ;;
clean) clean ;;
zip) zip ;;
run) run "$2" ;;
*) help ;;
esac
