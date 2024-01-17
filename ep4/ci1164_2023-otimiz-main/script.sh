#!/bin/bash

# Defina as variáveis do LIKWID_HOME e PATH de acordo com o seu ambiente
export LIKWID_HOME="/home/soft/likwid"
export PATH="$PATH:${LIKWID_HOME}/bin:${LIKWID_HOME}/sbin"

echo "performance" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor

# Compila o programa perfEG se necessário
make all

arquivo=$1

cat $arquivo | while read line
do
    echo ----------------- EXECUÇÃO PARA N = $line ------------------
    # Execute o programa perfEG com LIKWID e capture a saída
    likwid-perfctr -C 3 -g L3 -m ./matmult $line > aux.txt
    echo -n Tempo MatVet =\ 
    cat aux.txt | grep "Tempo matriz" | cut -d ':' -f 2 | head -n 1

    echo -n Tempo MatMat =\ 
    cat aux.txt | grep "Tempo matriz" | cut -d ':' -f 2 | tail -n 1

    echo -n L3 bandwidth MatVet =\ 
    cat aux.txt | grep "L3 bandwidth" | cut -d '|' -f 3 | head -n 1 | xargs
    echo -n L3 bandwidth MatMat =\ 
    cat aux.txt | grep "L3 bandwidth" | cut -d '|' -f 3 | tail -n 1 | xargs
    
    likwid-perfctr -C 3 -g L2CACHE -m ./matmult $line > aux.txt
    echo -n L2 miss ratio MatVet =\ 
    cat aux.txt | grep "L2 miss ratio" | cut -d '|' -f 3 | head -n 1 | xargs
    echo -n L2 miss ratio MatMat =\ 
    cat aux.txt | grep "L2 miss ratio" | cut -d '|' -f 3 | tail -n 1 | xargs
    
    likwid-perfctr -C 3 -g ENERGY -m ./matmult $line > aux.txt
    echo -n Energy \[J\] MatVet =\ 
    cat aux.txt | grep "Energy \[J\]" | cut -d '|' -f 3 | head -n 1 | xargs
    echo -n Energy \[J\] MatMat =\ 
    cat aux.txt | grep "Energy \[J\]" | cut -d '|' -f 3 | tail -n 1 | xargs
    
    likwid-perfctr -C 3 -g FLOPS_DP -m ./matmult $line > aux.txt
    echo -n FLOPS_DP MatVet =\ 
    cat aux.txt | grep "DP MFLOP/s" | cut -d '|' -f 3 | head -n 1 | xargs
    echo -n FLOPS_DP MatMat =\ 
    cat aux.txt | grep "DP MFLOP/s" | cut -d '|' -f 3 | head -n 3 | tail -n 1 | xargs
    
    echo -n FLOPS_AVX MatVet =\ 
    cat aux.txt | grep "DP MFLOP/s" | cut -d '|' -f 3 | head -n 2 | tail -n 1 | xargs
    echo -n FLOPS_AVX MatMat =\ 
    cat aux.txt | grep "DP MFLOP/s" | cut -d '|' -f 3 | tail -n 1 | xargs

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

echo "powersave" > /sys/devices/system/cpu/cpufreq/policy3/scaling_governor
