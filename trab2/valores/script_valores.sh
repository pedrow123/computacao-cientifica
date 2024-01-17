metrica=$1
busca=$2


cat ../otimizada.txt | grep "$busca" | cut -d '=' -f 2 | xargs > fotimizada.txt
cat ../nao_otimizada.txt | grep "$busca" | cut -d '=' -f 2 | xargs > fnao_otimizada.txt
python3 format.py > ${metrica}.dat
