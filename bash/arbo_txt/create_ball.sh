#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Aucun argument passé au script."
    exit 1
fi

s="my_ball.sh"

# Entête du script my_ball.sh
cat << EOF > "$s"
#!/bin/bash
EOF

# Fonction pour traiter récursivement les fichiers et les dossiers
traiter_fichier() {

local fichier="$1"
if [ -f "$fichier" ];then
uuencode -m "$fichier" < "$fichier" > "$fichier".uu # coder les fichier en base64
cat << TAG >> "$s"                                  # copier le contenu des fichiers encodés
cat << TAGDEFIN > "$fichier".uu                     # créer un fichier identique au fichiers précédents
`cat "$fichier".uu`
TAGDEFIN
uudecode < "$fichier".uu                            # decoder les fichiers avec l'extension .uu  
rm -r "$fichier".uu                                 # supprimer les fichiers avec l'extension .uu
TAG
rm -r "$fichier".uu                                 
elif [ -d "$fichier" ];then
cat << TAG >> "$s"
mkdir -p "$fichier"
TAG
for fichier2 in "$fichier"/*;do
traiter_fichier "$fichier2"
done
fi

}

# traiter tous les argument passés au script
for arg in "$@";do
    traiter_fichier "$arg"
done
chmod u+x "$s"
