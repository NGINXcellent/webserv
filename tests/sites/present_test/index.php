<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Nginxcellent CGI tester</title>
    <link rel="stylesheet" href="./styles.css">
</head>
<body>
    <main>
    <div class="main-button-container">
        <a href="./galery.php" class="main-button">Galeria</a>
        <a href="./index.php"><img src="./nginxcellent-semfundo.png" style="width: 100%;"></a>
        <a href="./postTest.html" class="main-button">Teste de Post</a>
    </div>
        <h1>Olá 
        <?php
        // Verifique se o parâmetro "avaliador" está presente na querystring
        if (isset($_GET['avaliador'])) {
            // Obtém o valor do parâmetro "avaliador"
            $avaliador = $_GET['avaliador'];
            // Exibe o valor no HTML
            echo $avaliador;
        } else {
            // Caso o parâmetro não esteja presente, exibe uma mensagem padrão
            echo "avaliador";
        }
    ?>
        </h1>
        <p>Seja bem vindo ao melhor servidor web do universo 42</p>
        <p>Este site serve para testarmos as funcionalidades CGI do nosso projeto</p>
        <p>Abaixo, você verá uma imagem aleatória da pasta bin-img</p>
        <div class="image-box">
            <?php
            // Caminho para a pasta que contém as imagens
            $dir = './tests/sites/present_test/bin-img/';

            // Lista de extensões de arquivo de imagem permitidas
            $allowedExtensions = ['jpg', 'jpeg', 'png', 'gif'];

            // Lista de arquivos de imagem na pasta
            $imageFiles = [];

            // Abre a pasta e lê os arquivos
            if (is_dir($dir)) {
                $files = scandir($dir);

                foreach ($files as $file) {
                    $extension = pathinfo($file, PATHINFO_EXTENSION);

                    if (in_array(strtolower($extension), $allowedExtensions)) {
                        $imageFiles[] = $file;
                    }
                }
            }

            // Escolhe uma imagem aleatoriamente
            if (!empty($imageFiles)) {
                $randomImage = $imageFiles[array_rand($imageFiles)];

                // Exibe a imagem
                echo '<img src="' . "./bin-img/" . $randomImage . '" alt="Imagem Aleatória">';
            } else {
                echo 'Nenhuma imagem encontrada na pasta.';
            }
            ?>
        </div>
    </main>
</body>
</html>
