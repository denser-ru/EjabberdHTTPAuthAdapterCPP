#!/bin/bash

# # Запуск эхо-сервера с использованием netcat и запись запроса в файл
python ./tests/http_test_server.py > ./tests/request.txt &
hts_pid=$!

sleep 1

# Создание файла с вводными данными
echo -e "\x00\x17auth:User:Server:Password\n\x00\x0Eisuser:User:Server\n\x00\x04exit" > ./tests/input.txt

# Запуск приложения и перенаправление ввода и вывода в фоновом режиме
./auth_http < ./tests/input.txt > ./tests/output.txt &
auth_http_pid=$!

# Дождаться передачи данных из input.txt в pipe
sleep 1

# Вывод ответа
echo "Response:"
cat ./tests/output.txt

# sleep 3
wait $auth_http_pid

# Остановка процессов
kill $hts_pid
