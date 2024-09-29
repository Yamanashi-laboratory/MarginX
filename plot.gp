set multiplot layout 7,1
set datafile separator "," 
set xrange[0:999.1]
set xtics nomirror
set ytics nomirror
set xtics scale 2
set ytics scale 2
set xtics 100
set bmargin 0
set tmargin 0
set lmargin 8
unset xlabel
set format x "" 
set format y "%3.0f"

set yrange[-2.22289:22.4782]
set ytics 20
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "NDROCex_adp669_out.CSV" using ($1 * 1e12):($2* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B2 - Phase"

set yrange[-3.80905:41.8996]
set ytics 30
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "NDROCex_adp669_out.CSV" using ($1 * 1e12):($3* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B3 - Phase"

set yrange[-0.748473:7.66068]
set ytics 6
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "NDROCex_adp669_out.CSV" using ($1 * 1e12):($4* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B4 - Phase"

set yrange[-1.22883:7.3885]
set ytics 6
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "NDROCex_adp669_out.CSV" using ($1 * 1e12):($5* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B5 - Phase"

set yrange[-3.7929:41.7219]
set ytics 30
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "NDROCex_adp669_out.CSV" using ($1 * 1e12):($6* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B8 - Phase"

set yrange[-2.08006:22.8806]
set ytics 20
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
set format x
set xlabel "Time [ps]" offset 0,0.1 font "Arial,14"
plot "NDROCex_adp669_out.CSV" using ($1 * 1e12):($7* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B9 - Phase"

unset multiplot
