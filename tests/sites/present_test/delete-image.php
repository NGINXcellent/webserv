<?php
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST["_method"]) && $_POST["_method"] === "DELETE") {
    // Caminho completo da imagem a ser excluída
    $imagePath = $_POST['image'];

    // Envia a solicitação DELETE para o servidor
    $curl = curl_init($imagePath);
    curl_setopt($curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
    $response = curl_exec($curl);
    $httpCode = curl_getinfo($curl, CURLINFO_HTTP_CODE);
    curl_close($curl);

    // Verifica se a solicitação DELETE foi bem-sucedida
    if ($httpCode == 200) {
        echo "A imagem foi excluída com sucesso.";
    } else {
        echo "Erro ao excluir a imagem.", $imagePath;
    }
} else {
    echo "Requisição inválida.";
}
?>