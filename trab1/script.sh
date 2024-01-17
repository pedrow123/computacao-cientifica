#!/bin/bash

# Defina as variáveis do LIKWID_HOME e PATH de acordo com o seu ambiente
export LIKWID_HOME="/home/soft/likwid"
export PATH="$PATH:${LIKWID_HOME}/bin:${LIKWID_HOME}/sbin"

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Compila o programa perfEG se necessário
make all

arquivo=$1
if [[ -d $arquivo ]]; then
    files="${arquivo}/*"
elif [[ -f $arquivo ]]; then
    files=$arquivo
fi	

for f in $files
do
    echo ----------------- EXECUÇÃO DO ARQUIVO $f ------------------
    # Execute o programa perfEG com LIKWID e capture a saída
    likwid-perfctr -C 3 -g FLOPS_DP -m ./ajustePol < $f > saida.txt
    cat saida.txt | head -n 9 | tail -n 4

    echo -n FLOPS_DP em MFLOP/s geraSL =\  
    cat saida.txt | grep MFLOP | head -n 1 | cut -d '|' -f 3 | xargs
    
    echo -n FLOPS_AVX em MFLOP/s geraSL =\   
    cat saida.txt | grep MFLOP | head -n 2 | tail -n 1 | cut -d '|' -f 3 | xargs

    echo -n FLOPS_DP em MFLOP/s solSL =\ 
    cat saida.txt | grep MFLOP | tail -n 2 | head -n 1 | cut -d '|' -f 3 | xargs

    echo -n FLOPS_AVX em MFLOP/s solSL =\ 
    cat saida.txt | grep MFLOP | tail -n 1 | cut -d '|' -f 3 | xargs

    likwid-perfctr -C 3 -g ENERGY -m ./ajustePol < teste.txt > saida.txt

    echo -n ENERGY[J] em geraSL =\ 
    cat saida.txt | grep Energy | head -n 1 | cut -d '|' -f 3 | xargs

    echo -n ENERGY[J] em solSL =\ 
    cat saida.txt | grep Energy | head -n 5 | tail -n 1 | cut -d '|' -f 3 | xargs

    # Verifique se a execução foi bem-sucedida
    if [ $? -eq 0 ]; then
        rm -rf saida.txt
        echo 
	echo "Execução bem-sucedida"
    else
        echo "Erro ao executar o programa ajustePol"
    fi
    echo
done
