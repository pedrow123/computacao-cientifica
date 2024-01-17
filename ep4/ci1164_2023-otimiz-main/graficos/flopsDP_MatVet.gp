#!/usr/bin/gnuplot -c

## set encoding iso_8859_15
set encoding utf

# set terminal qt persist

set grid

set style data point
set style function line
set style line 1 lc 3 pt 7 ps 0.3
set boxwidth 1
set xtics
# set xrange [0:]
set xlabel  "N (tamanho matriz)"

#
# ALTERNATIVA 2: Tabela com 3 colunas 
#
set key outside right # posição da legenda: outside {left | right}
unset logscale x
set datafile separator whitespace
set ylabel  "Flops DP [MFLOP/s]"
set title   "Flops DP função multMatVet"
set terminal qt 1 title "Flops DP função multMatVet"
plot '../valores/flopsDPMatVet.dat' using 1:2 title "<com otimização>" with linespoints, \
     '' using 1:3 title "<sem otimização>" with linespoints

pause -1


# Gerando figura PNG
set terminal png
set output "flopsDP_MatVet.png"
plot '../valores/flopsDPMatVet.dat' using 1:2 title "<sem otimização>" with linespoints, \
     '' using 1:3 title "<com otimização>" with linespoints
replot
unset output