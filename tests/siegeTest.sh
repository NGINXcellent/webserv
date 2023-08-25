#!/bin/bash

# Definindo a URL alvo
TARGET_URL="192.168.1.35:8080/lfarias/space.jpg"

# Instalação do Siege
if command -v siege &>/dev/null; then
    echo "Siege already installed."
else
    echo "Instaling Siege. . ."
    sudo apt-get update
    sudo apt-get install -y siege
fi

printf "%s\n" "[ROUND 1]"
echo "Siege 10 users sending 1000 requests"
siege -c 10 -r 1000 -R <(echo connection = keep-alive) "$TARGET_URL"

printf "%s\n" "[ROUND 2]"
printf "%s\n" "> press enter to start next round"
read -p name

echo "Siege 100 users sending 1000 requests"
siege -c 100 -r 1000 -R <(echo connection = keep-alive) "$TARGET_URL"

printf "%s\n" "[ROUND 3]"
printf "%s\n" "> press enter to start next round"
read -p name

echo "Siege 200 users sending 100 requests"
siege -c 200 -r 50 -R <(echo connection = keep-alive) "$TARGET_URL"

printf "%s\n" "[ROUND 4]"
printf "%s\n" "> press enter to start next round"
read -p name

echo "Siege 10 users sending requests for 1Minute"
siege -c 10 -t1M -R <(echo connection = keep-alive) "$TARGET_URL"
