#!/bin/bash

# URL alvo
TARGET_URL="http://localhost:8080/lfarias/space.jpg"
POST_TARGET_URL="http://localhost:8080/lfarias/"
URL_ENCODED="http://localhost:8080/lfarias/tobemodified.txt"

# # Requisição GET
# echo "Enviando requisição GET para $TARGET_URL"
# response=$(curl -s -o /dev/null -w "%{http_code}" -X GET "$TARGET_URL")
# if [ $response -ge 200 ] && [ $response -lt 300 ]; then
#     echo "GET request bem-sucedida (Status: $response)"
# else
#     echo "GET request falhou (Status: $response)"
# fi

# # Requisição DELETE
# echo "Enviando requisição DELETE para $TARGET_URL"
# response=$(curl -s -o /dev/null -w "%{http_code}" -X DELETE "$TARGET_URL")
# if [ $response -ge 200 ] && [ $response -lt 300 ]; then
#     echo "DELETE request bem-sucedida (Status: $response)"
# else
#     echo "DELETE request falhou (Status: $response)"
# fi

# # Requisição POST urlencoded
# echo "Enviando requisição POST com dados codificados em URL para $URL_ENCODED"
# response=$(curl -s -o /dev/null -w "%{http_code}" -X POST --data-urlencode "key1=value1&key2=value2" -H "Content-Type: application/x-www-form-urlencoded" "$URL_ENCODED")

# if [ $response -ge 200 ] && [ $response -lt 300 ]; then
#     echo "POST request bem-sucedida (Status: $response)"
# else
#     echo "POST request falhou (Status: $response)"
# fi

# # Requisição POST com dados em formato multipart
# echo "Enviando requisição POST com dados em formato multipart para $TARGET_URL"
# response=$(curl -s -o /dev/null -w "%{http_code}" -X POST -F "key1=value1" -F "key2=value2" "$POST_TARGET_URL")

# if [ $response -ge 200 ] && [ $response -lt 300 ]; then
#     echo "POST request bem-sucedida (Status: $response)"
# else
#     echo "POST request falhou (Status: $response)"
# fi

# # Requisição POST com dados em formato multipart
# echo "Enviando requisição PUT que não deve rodar para $TARGET_URL"
# response=$(curl -s -o /dev/null -w "%{http_code}" -X PUT -F "key1=value1" "$POST_TARGET_URL")

# if [ $response -ge 200 ] && [ $response -lt 300 ]; then
#     echo "POST request bem-sucedida (Status: $response)"
# else
#     echo "POST request falhou (Status: $response)"
# fi