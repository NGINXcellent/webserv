<?php
// Verifica se o arquivo foi enviado corretamente
$inputData = file_get_contents("php://stdin");

// Faz echo dos dados lidos
echo "Dados recebidos: " . $inputData;

if ($_FILES["fileToUpload"]["error"] == UPLOAD_ERR_OK) {
    // Obtém informações sobre o arquivo enviado
    $fileName = $_FILES["fileToUpload"]["name"];
    $tempFilePath = $_FILES["fileToUpload"]["tmp_name"];
    $targetDirectory = "./cgi-img/";

    // Verifica se o diretório de destino existe e é gravável
    if (!file_exists($targetDirectory)) {
        mkdir($targetDirectory, 0755, true);
    }

    // Define o caminho final para salvar o arquivo
    $targetFilePath = $targetDirectory . $fileName;

    // Move o arquivo temporário para o diretório de destino
    if (move_uploaded_file($tempFilePath, $targetFilePath)) {
        echo "Arquivo enviado com sucesso! Caminho do arquivo: " . $targetFilePath;
    } else {
        echo "Falha ao enviar o arquivo.";
    }
} else {
    echo "Erro no upload do arquivo. Código de erro: " . $_FILES["fileToUpload"]["error"];
}
?>




<!-- <?php

// Diretório de destino para salvar os arquivos
// $targetDirectory = "./tests/sites/present_test/bin-img/";

// Verifica se o diretório de destino existe e é gravável
// if (!file_exists($targetDirectory)) {
//     mkdir($targetDirectory, 0755, true);
// }

// // Verifica se o arquivo foi enviado com sucesso
// if ($_SERVER["REQUEST_METHOD"] == "POST") {
//     // Nome do arquivo de destino (usando o nome original do arquivo)
//     $targetFile = $targetDirectory . basename($_FILES["fileToUpload"]["name"]);

//     // Verifica se o arquivo já existe
//     if (file_exists($targetFile)) {
// 			echo "<script>alert('Ja existe um arquivo com este nome');</script>";

// 			// Retorna para postTest.html
// 			echo "<script>window.location.href = 'postTest.html';</script>";
//     } else {
//         // Tenta mover o arquivo para o diretório de destino
//         if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $targetFile)) {
// 					echo "<script>alert('Arquivo enviado com sucesso!');</script>";

// 					// Retorna para postTest.html
// 					echo "<script>window.location.href = 'postTest.html';</script>";
//         } else {
// 					echo "<script>alert('Falha ao enviar arquivo');</script>";

// 					// Retorna para postTest.html
// 					echo "<script>window.location.href = 'postTest.html';</script>";
//         }
//     }
// }
?> -->