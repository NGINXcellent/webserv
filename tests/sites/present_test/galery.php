<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Galeria de Imagens</title>
    <link rel="stylesheet" href="./styles.css">
</head>
<body>
<main>
    <div class="main-button-container">
        <a href="./galery.php" class="main-button">Galeria</a>
        <a href="./index.php"><img src="./nginxcellent-semfundo.png" style="width: 100%;"></a>
        <a href="./postTest.html" class="main-button">Teste de Post</a>
    </div>
    <h1>Galeria de Imagens</h1>
    <p>Voce pode excluir imagens clicando no botão "Excluir".</p>
    <div id="image-gallery">
            <?php
            // Diretório das imagens
            $directory = './bin-img/'; // Substitua pelo caminho da sua pasta de imagens
            // problema, esta dando request dobrado na imagem: ./tests/sites/present_test/tests/sites/present_test/bin-img/nginxcellent.png
            // Lista todos os arquivos na pasta
            $files = scandir($directory);

            // Exibe as imagens na página
            foreach ($files as $file) {
                if (in_array(strtolower(pathinfo($file, PATHINFO_EXTENSION)), array('jpg', 'jpeg', 'png', 'gif'))) {
                    // Cria uma nova div para cada imagem
                    $div = '<div class="image-item">';

                    // Adiciona a imagem à div
                    $div .= '<img src="' . "./bin-img" . '/' . $file . '" alt="' . $file . '">';

                    // Adiciona o formulário de exclusão
                    $div .= '<form action="delete-image.php" method="post">';
                    $div .= '<input type="hidden" name="_method" value="DELETE">';
                    $div .= '<input type="hidden" name="image" value="' . $file . '">';
                    $div .= '<button type="submit" class="styled-button styled-button-galery">Excluir</button>';
                    $div .= '</form>';

                    // Fecha a div
                    $div .= '</div>';

                    // Exibe a div na página
                    echo $div;
                }
            }
            ?>
    </div>
    <a class="scroll-to-top" href="#">Voltar ao Topo</a>
</main>
</body>
</html>