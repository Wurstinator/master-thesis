
set terminal pdf size 10,6
set datafile separator ","


set output "analysis/schewe_gendet_ap1_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a random DPA with |Σ|=2"
plot "schewe_gendet_ap1.csv" using 1:2 notitle


set output "analysis/schewe_gendet_ap_compare_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a random DPA with different alphabets"
plot "schewe_gendet_ap1.csv" using 1:2 title "|Σ|=2", \
     "schewe_gendet_ap2.csv" using 1:2 title "|Σ|=4", \
     "schewe_gendet_ap3.csv" using 1:2 title "|Σ|=8"


set output "analysis/schewe_gendet_ap1_statereduction.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Number of removed states"
set title "Schewe10 state reduction on random DPAs with different alphabets"
set ytics 1

stats "schewe_gendet_ap1.csv" using ($1-$9) nooutput
max_ap1 = STATS_max
stats "schewe_gendet_ap2.csv" using ($1-$9) nooutput
max_ap2 = STATS_max
stats "schewe_gendet_ap3.csv" using ($1-$9) nooutput
max_ap3 = STATS_max
max(x, y) = x < y ? y : x
max_ap123 = max(max_ap1, max(max_ap2, max_ap3))
set yrange [-0.1:max_ap123+0.1]

plot "schewe_gendet_ap1.csv" using 1:($1-$9) title "|Σ|=2", \
     "schewe_gendet_ap2.csv" using 1:($1-$9) title "|Σ|=4", \
     "schewe_gendet_ap3.csv" using 1:($1-$9) title "|Σ|=8"

