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

    # ---- DP MFLOP/S ----
    # GeraSL
    echo -n FLOPS_DP em MFLOP/s geraSL =\  
    cat saida.txt | grep MFLOP | head -n 1 | cut -d '|' -f 3 | xargs
    
    # SolSL 
    echo -n FLOPS_DP em MFLOP/s solSL =\  
    cat saida.txt | grep MFLOP | head -n 3 | tail -n 1 | cut -d '|' -f 3 | xargs

    # CalcRes
    echo -n FLOPS_DP em MFLOP/s calcRes =\  
    cat saida.txt | grep MFLOP | head -n 5 | tail -n 1 | cut -d '|' -f 3 | xargs
    echo --------------------------------------------------------------------------------
    
    # ---- AVX DP MFLOP/s ----    
    # GeraSL
    echo -n FLOPS_AVX em MFLOP/s geraSL =\   
    cat saida.txt | grep MFLOP | head -n 2 | tail -n 1 | cut -d '|' -f 3 | xargs

    # SolSL
    echo -n FLOPS_AVX em MFLOP/s solSL =\   
    cat saida.txt | grep MFLOP | head -n 4 | tail -n 1 | cut -d '|' -f 3 | xargs

    # CalcRes
    echo -n FLOPS_AVX em MFLOP/s calcRes =\   
    cat saida.txt | grep MFLOP | head -n 6 | tail -n 1 | cut -d '|' -f 3 | xargs    
    echo --------------------------------------------------------------------------------
    
    # ---- Cache miss L2 ----
    likwid-perfctr -C 3 -g L2CACHE -m ./ajustePol < $f > saida.txt
    # GeraSL
    echo -n L2 miss ratio geraSL =\   
    cat saida.txt | grep L2\ miss\ ratio | head -n 1 | tail -n 1 | cut -d '|' -f 3 | xargs
    
    # SolSL
    echo -n L2 miss ratio solSL =\   
    cat saida.txt | grep L2\ miss\ ratio | head -n 2 | tail -n 1 | cut -d '|' -f 3 | xargs

    # CalcRes
    echo -n L2 miss ratio calcRes =\   
    cat saida.txt | grep L2\ miss\ ratio | head -n 3 | tail -n 1 | cut -d '|' -f 3 | xargs
    echo --------------------------------------------------------------------------------

    # ---- Memory bandwidth ----
    likwid-perfctr -C 3 -g L3 -m ./ajustePol < $f > saida.txt
    # GeraSL
    echo -n L3 bandwidth geraSL =\   
    cat saida.txt | grep L3\ bandwidth | head -n 1 | tail -n 1 | cut -d '|' -f 3 | xargs
    
    # SolSL
    echo -n L3 bandwidth solSL =\   
    cat saida.txt | grep L3\ bandwidth | head -n 2 | tail -n 1 | cut -d '|' -f 3 | xargs

    # CalcRes
    echo -n L3 bandwidth calcRes =\   
    cat saida.txt | grep L3\ bandwidth | head -n 3 | tail -n 1 | cut -d '|' -f 3 | xargs
    echo --------------------------------------------------------------------------------

    rm -rf saida.txt
    echo Execução bem sucedida\!
    echo
done
