metrica=$1
busca=$2


cat resultados.txt | grep "$busca" | cut -d '=' -f 2 | xargs > otimizado.txt
cat resultados_sem_otimizacao.txt | grep "$busca" | cut -d '=' -f 2 | xargs > nao_otimizado.txt
python3 format.py > ${metrica}.dat
