

set terminal pdf size 10,6
set datafile separator ","

# gendet

set output "analysis/schewe_gendet_ap1_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a random DPA with |Σ|=2 and 3 colors"
plot "schewe_gendet_ap1.csv" using 1:2 notitle


set output "analysis/schewe_gendet_ap_compare_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a random DPA with different alphabets and 3 colors"
plot "schewe_gendet_ap1.csv" using 1:2 title "|Σ|=2", \
     "schewe_gendet_ap2.csv" using 1:2 title "|Σ|=4", \
     "schewe_gendet_ap3.csv" using 1:2 title "|Σ|=8"


set output "analysis/schewe_gendet_ap1_statereduction.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Number of removed states"
set title "Schewe10 state reduction on random DPAs with different alphabets and 3 colors"
set ytics 1

stats "schewe_gendet_ap1.csv" using ($1-$9) nooutput
max_ap1 = STATS_max
stats "schewe_gendet_ap2.csv" using ($1-$9) nooutput
max_ap2 = STATS_max
stats "schewe_gendet_ap3.csv" using ($1-$9) nooutput
max_ap3 = STATS_max
max(x, y) = x < y ? y : x
max_ap123 = max(max_ap1, max(max_ap2, max_ap3))
set yrange [-0.1:max_ap123*1.1]

plot "schewe_gendet_ap1.csv" using 1:($1-$9) title "|Σ|=2", \
     "schewe_gendet_ap2.csv" using 1:($1-$9) title "|Σ|=4", \
     "schewe_gendet_ap3.csv" using 1:($1-$9) title "|Σ|=8"



# detnbaut

set ytics auto
set autoscale y
set output "analysis/schewe_detnbaut_ap1_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a DPA with |Σ|=2 that was created by nbautils from an NBA"
plot "schewe_detnbaut_ap1.csv" using 1:2 notitle


set output "analysis/schewe_detnbaut_ap1_statereduction.pdf"
set xlabel "Number of states in the original automaton after Hopcroft reduction"
set ylabel "Number of removed states"
set title "Schewe10 state reduction on DPAs with |Σ|=2 that were created by nbautils from NBAs"
stats "schewe_detnbaut_ap1.csv" using ($5-$9) nooutput
set yrange [-0.2:STATS_max*1.1]
plot "schewe_detnbaut_ap1.csv" using 5:($5-$9) notitle


set output "analysis/schewe_detnbaut_ap1_statereductionrelative.pdf"
set xlabel "Number of states in the original automaton after Hopcroft reduction"
set ylabel "Relative number of removed states"
set title "Schewe10 state reduction on DPAs with |Σ|=2 that were created by nbautils from NBAs"
set autoscale y
plot "schewe_detnbaut_ap1.csv" using 5:(($5-$9)/$5) notitle


set ytics auto
set output "analysis/schewe_detnbaut_ap2_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a DPA with |Σ|=4 that was created by nbautils from an NBA"
plot "schewe_detnbaut_ap2.csv" using 1:2 notitle


set output "analysis/schewe_detnbaut_ap2_statereduction.pdf"
set xlabel "Number of states in the original automaton after Hopcroft reduction"
set ylabel "Number of removed states"
set title "Schewe10 state reduction on DPAs with |Σ|=4 that were created by nbautils from NBAs"
stats "schewe_detnbaut_ap2.csv" using ($5-$9) nooutput
set yrange [-0.2:STATS_max*1.1]
set ytics 1
plot "schewe_detnbaut_ap2.csv" using 5:($5-$9) notitle



# detspot with optimizations

set ytics auto
set autoscale y
set output "analysis/schewe_detspot_ap1_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a DPA with |Σ|=2 that was created by Spot from an NBA"
plot "schewe_detspot_ap1.csv" using 1:2 notitle


set output "analysis/schewe_detspot_ap1_statereduction.pdf"
set xlabel "Number of states in the original automaton after Hopcroft reduction"
set ylabel "Number of removed states"
set title "Schewe10 state reduction on DPAs with |Σ|=2 that were created by Spot from NBAs"
stats "schewe_detspot_ap1.csv" using ($5-$9) nooutput
set yrange [-0.2:STATS_max*1.1]
plot "schewe_detspot_ap1.csv" using 5:($5-$9) notitle


set output "analysis/schewe_detspot_ap1_statereductionrelative.pdf"
set xlabel "Number of states in the original automaton after Hopcroft reduction"
set ylabel "Relative number of removed states"
set title "Schewe10 state reduction on DPAs with |Σ|=2 that were created by Spot from NBAs"
set autoscale y
plot "schewe_detspot_ap1.csv" using 5:(($5-$9)/$5) notitle


set ytics auto
set autoscale y
set output "analysis/schewe_detspot_ap2_time.pdf"
set xlabel "Number of states in the original automaton"
set ylabel "Time taken in milliseconds"
set title "Time for Schewe construction on a DPA with |Σ|=4 that was created by Spot from an NBA"
plot "schewe_detspot_ap2.csv" using 1:2 notitle


set output "analysis/schewe_detspot_ap2_statereduction.pdf"
set xlabel "Number of states in the original automaton after Hopcroft reduction"
set ylabel "Number of removed states"
set title "Schewe10 state reduction on DPAs with |Σ|=4 that were created by Spot from NBAs"
stats "schewe_detspot_ap2.csv" using ($5-$9) nooutput
set yrange [-0.2:STATS_max*1.1]
plot "schewe_detspot_ap2.csv" using 5:($5-$9) notitle







