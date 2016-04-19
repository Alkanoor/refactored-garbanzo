#Data challenge


Ceci contient uniquement le code utile utilisé pour le challenge.
Pour convertir les données de manière entièrement contrôlée, j'ai codé moi même un parseur en C++, ce qui après coup n'était pas très utile. J'ai cependant uniquement utilisé les données générées à partir de ce code, puisque ça n'a pas si mal marché. Cela m'a évité de le faire en python, même si c'était finalement bien plus long.
Pour convertir les données initiales (test.csv et train.csv), **il faut les placer dans ce dossier**.

Pour compiler, éxecuter **make**.
Pour convertir les données, éxecuter **make convert** (cela placera le résultat dans le dossier data (non présent au départ)).
Pour éxecuter le code python à partir des données générées, il faut éxecuter **make python**.
Pour éxecuter le code python qui trouve les meilleurs paramètres avec le modèle utilisé, il faut éxecuter **make find_best_parameters**.
Pour rendre le dépôt propre, **make clean** (supprime ce qui est généré lors de la compilation ainsi que les données converties).

L'explication est donnée dans le fichier compte_rendu.pdf.


Il est possible que certaines erreurs surviennent lors de la compilation ou de l'éxecution des scripts python. Si c'est le cas, vous pouvez me le signaler et je ferai ce qui est possible pour le corriger.
