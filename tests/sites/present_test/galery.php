<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Galeria de Imagens</title>
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
    <h1>Galeria de Imagens</h1>
    <div id="image-gallery">
            <?php
            // Diretório das imagens
            $directory = './bin-img'; // Substitua pelo caminho da sua pasta de imagens

            // Lista todos os arquivos na pasta
            $files = scandir($directory);

            // Exibe as imagens na página
            foreach ($files as $file) {
                if (in_array(strtolower(pathinfo($file, PATHINFO_EXTENSION)), array('jpg', 'jpeg', 'png', 'gif'))) {
                    // Cria uma nova div para cada imagem
                    $div = '<div class="image-item">';

                    // Adiciona a imagem à div
                    $div .= '<img src="' . $directory . '/' . $file . '" alt="' . $file . '">';

                    // Adiciona o formulário de exclusão
                    $div .= '<form action="delete-image.php" method="post">';
                    $div .= '<input type="hidden" name="_method" value="DELETE">';
                    $div .= '<input type="hidden" name="image" value="' . $file . '">';
                    $div .= '<button type="submit" class="styled-button">Excluir</button>';
                    $div .= '</form>';

                    // Fecha a div
                    $div .= '</div>';

                    // Exibe a div na página
                    echo $div;
                }
            }
            ?>
    </div>
</main>
</body>
</html>