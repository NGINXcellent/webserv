<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Verifica se um arquivo foi enviado corretamente
    if (isset($_FILES["arquivo"]) && $_FILES["arquivo"]["error"] == UPLOAD_ERR_OK) {
        // URL do endpoint onde você deseja enviar a imagem
        $endpoint = "./bin-img/";

        // Caminho temporário do arquivo enviado
        $arquivo_temporario = $_FILES["arquivo"]["tmp_name"];

        // Nome do arquivo original
        $nome_arquivo = $_FILES["arquivo"]["name"];

        // Inicializa uma sessão cURL
        $ch = curl_init();

        // Configura as opções da solicitação cURL
        curl_setopt($ch, CURLOPT_URL, $endpoint);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_POST, 1);

        // Cria um array com os dados do formulário, incluindo o campo de arquivo
        $postData = array(
            'descricao' => 'Esta é uma imagem legal',
            'arquivo' => new CURLFile($arquivo_temporario, 'image/jpeg', $nome_arquivo) // Adiciona o arquivo como um CURLFile
        );

        curl_setopt($ch, CURLOPT_POSTFIELDS, $postData);
				echo "123";
        // Executa a solicitação cURL
        $response = curl_exec($ch);

        // Verifica se ocorreram erros
        if (curl_errno($ch)) {
            echo 'Erro cURL: ' . curl_error($ch);
        }

        // Fecha a sessão cURL
        curl_close($ch);

        // Processa a resposta do servidor
        echo "Resposta do servidor: " . $response;
    } else {
        echo "Erro no upload do arquivo." . $response;
    }
}
?>
