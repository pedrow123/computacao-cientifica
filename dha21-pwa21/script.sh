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
    likwid-perfctr -C 3 -g FLOPS_DP -m ./perfEG < $f > saida.txt
    cat saida.txt | head -n 9 | tail -n 4
    echo -n MFLOPS/s = 
    cat saida.txt | grep MFLOP/s | head -n 1 | cut -d"|" -f3 | xargs

    cat saida.txt | head -n 14 | tail -n 5
    echo -n MFLOPS/s = 
    cat saida.txt | grep MFLOP/s | head -n 4 | tail -n -1 | cut -d"|" -f3 | xargs

    cat saida.txt | head -n 19 | tail -n 5
    echo -n MFLOPS/s =  
    cat saida.txt | grep MFLOP/s | head -n 7 | tail -n -1 | cut -d"|" -f3 | xargs


    # Verifique se a execução foi bem-sucedida
    if [ $? -eq 0 ]; then
        rm -rf saida.txt
        echo 
	echo "Execução bem-sucedida"
    else
        echo "Erro ao executar o programa perfEG"
    fi
    echo
done
