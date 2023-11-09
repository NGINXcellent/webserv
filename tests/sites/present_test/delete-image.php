<?php
if ($_SERVER["REQUEST_METHOD"] === "POST" && isset($_POST["_method"]) && $_POST["_method"] === "DELETE" && isset($_POST["image"])) {
    $imageFileName = $_POST["image"];
    $imagePath = "./bin-img/" . $imageFileName;

    // Verifica se o arquivo existe antes de tentar excluí-lo
    if (file_exists($imagePath)) {
        // Tenta excluir o arquivo
        if (unlink($imagePath)) {
            echo "Imagem '$imageFileName' foi excluída com sucesso!";
        } else {
            echo "Erro ao excluir a imagem '$imageFileName'.";
        }
    } else {
        echo "Arquivo '$imageFileName' não encontrado no caminho: $imagePath";
    }
} else {
    echo "Acesso inválido.";
}
?>