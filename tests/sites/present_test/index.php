<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Meu Site</title>
    <link rel="stylesheet" href="./styles.css">
</head>
<body>
    <header>
        <nav>
            <ul>
                <li><a href="./index.php"><img src="./nginxcellent-semfundo.png" style="width: 5%;"></a></li>
                <li><a href="./galery.php">Galeria</a></li>
                <li><a href="./postTest.html">Teste de Post</a></li>
                <li><a href="./mimeTest.php">Teste de Delete</a></li>
            </ul>
        </nav>
    </header>

    <main>
        <h1> Ola incrivel avaliador</h1>
        <p>Seja bem vindo ao melhor servidor web do universo 42</p>
        <p>Esse site serve para testarmos as funcionalidades CGI do nosso projeto</p>
        <p>No box abaixo voce vera uma imagem aleatoria da pasta bin-img</p>
        <div class="image-box">
            <?php
            // Caminho para a pasta que contém as imagens
            $dir = './bin-img/';

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
                echo '<img src="' . $dir . $randomImage . '" alt="Imagem Aleatória">';
            } else {
                echo 'Nenhuma imagem encontrada na pasta.';
            }
            ?>
        </div>
        <p>Por favor, explore as possibilidades do nosso projeto</p>
    </main>

</body>
</html>
