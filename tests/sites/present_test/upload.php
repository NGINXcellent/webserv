<?php

// echo "<pre>";
// print_r($_SERVER);
// print_r($_FILES);
// print_r($_POST);
// echo "</pre>";

// Diretório de destino para salvar os arquivos
$targetDirectory = "./bin-img/";

// Verifica se o diretório de destino existe e é gravável
if (!file_exists($targetDirectory)) {
    mkdir($targetDirectory, 0755, true);
}

// Verifica se o arquivo foi enviado com sucesso
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Nome do arquivo de destino (usando o nome original do arquivo)
    $targetFile = $targetDirectory . basename($_FILES["fileToUpload"]["name"]);

    // Verifica se o arquivo já existe
    if (file_exists($targetFile)) {
			echo "<script>alert('Ja existe um arquivo com este nome');</script>";

			// Retorna para postTest.html
			echo "<script>window.location.href = 'postTest.html';</script>";
    } else {
        // Tenta mover o arquivo para o diretório de destino
        if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $targetFile)) {
					echo "<script>alert('Arquivo enviado com sucesso!');</script>";

					// Retorna para postTest.html
					echo "<script>window.location.href = 'postTest.html';</script>";
        } else {
					echo "<script>alert('Falha ao enviar arquivo');</script>";

					// Retorna para postTest.html
					echo "<script>window.location.href = 'postTest.html';</script>";
        }
    }
}
?>