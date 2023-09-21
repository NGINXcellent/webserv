<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Diretório das imagens
    $directory = './bin-img'; // Substitua pelo caminho da sua pasta de imagens

    // Obtém o nome do arquivo da imagem a ser excluída
    $imageToDelete = $_POST['image'];

    // Verifica se o nome do arquivo é válido (para evitar manipulação)
    if (is_string($imageToDelete) && !empty($imageToDelete) && preg_match('/^[a-zA-Z0-9\s]+\.[a-zA-Z]{3,4}$/', $imageToDelete)) {
        // Caminho completo do arquivo a ser excluído
        $filePath = $directory . '/' . $imageToDelete;

        // Verifica se o arquivo existe antes de excluí-lo
        if (file_exists($filePath)) {
            // Tenta excluir o arquivo
            if (unlink($filePath)) {
                echo "A imagem foi excluída com sucesso.";
            } else {
                echo "Erro ao excluir a imagem.";
            }
        } else {
            echo "A imagem não foi encontrada.";
        }
    } else {
        echo "Nome de arquivo inválido.";
    }
} else {
    echo "Requisição inválida.";
}
?>
