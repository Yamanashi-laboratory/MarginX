set multiplot layout 12,1
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

set yrange[-0.752527:8.2778]
set ytics 7
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($2* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B1.XI19 - Phase"

set yrange[-1.21515:8.07463]
set ytics 7
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($3* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B2.XI19 - Phase"

set yrange[-21.1246:2.57329]
set ytics 10
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($4* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B3.XI19 - Phase"

set yrange[-3.81822:41.9823]
set ytics 30
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($5* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B5.XI19 - Phase"

set yrange[-2.05012:22.5514]
set ytics 20
set ylabel "Phase [rad]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($6* 1e0) smooth unique lw 2 lc rgb "royalblue" title "B6.XI19 - Phase"

set yrange[-0.1034:1.1374]
set ytics 1
set ylabel "Voltage [mV]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($7* 1e3) smooth unique lw 2 lc rgb "royalblue" title "Vinclk - Voltage"

set yrange[-157.554:964.441]
set ytics 800
set ylabel "Voltage [uV]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($8* 1e6) smooth unique lw 2 lc rgb "royalblue" title "B4.XI19 - Voltage"

set yrange[-343.48:493.799]
set ytics 400
set ylabel "Voltage [uV]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($9* 1e6) smooth unique lw 2 lc rgb "royalblue" title "B6.XI19 - Voltage"

set yrange[-0.2:2.2]
set ytics 2
set ylabel "Current [mA]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($10* 1e3) smooth unique lw 2 lc rgb "royalblue" title "IINx - Current"

set yrange[-0.2:2.2]
set ytics 2
set ylabel "Current [mA]" offset -0.5,0 font "Arial,10"
plot "a.csv" using ($1 * 1e12):($11* 1e3) smooth unique lw 2 lc rgb "royalblue" title "IINy - Current"

set yrange[-0.2:2.2]
set ytics 2
set ylabel "Current [mA]" offset -0.5,0 font "Arial,10"
set format x
set xlabel "Time [ps]" offset 0,0.1 font "Arial,14"
plot "a.csv" using ($1 * 1e12):($12* 1e3) smooth unique lw 2 lc rgb "royalblue" title "IINRST - Current"

unset multiplot
