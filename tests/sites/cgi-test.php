<!DOCTYPE html>
<meta charset="UTF-8">
<html>
<head>
    <title>Exemplo PHP</title>
</head>
<body>
    <h1>Olá, mundo!</h1>
    <p>Este é um exemplo simples de um script PHP.</p>
    <?php
    // Verifica se a variável QUERY_STRING está definida
    if (isset($_SERVER['QUERY_STRING'])) {
        // Divide a Query String em parâmetros separados
        parse_str($_SERVER['QUERY_STRING'], $queryParameters);

        // Verifica se os parâmetros 'nome' e 'idade' existem
        if (isset($queryParameters['nome']) && isset($queryParameters['idade'])) {
            $nome = $queryParameters['nome'];
            $idade = $queryParameters['idade'];
            echo "Olá $nome, você tem $idade anos.";
        } else {
            echo "Parâmetros 'nome' e 'idade' não foram fornecidos na Query String.";
        }
    } else {
        echo "Query String não definida.";
    }

    $data = date('d/m/Y H:i:s');
    echo "<p>A hora atual é: $data</p>";
    sleep(9999);
    ?>
</body>
</html>