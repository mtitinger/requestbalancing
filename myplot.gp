# Abbreviations
# u => using
# w => with
# l => lines
# linesp => linespoint
#
# empty filename = reuse last filename

set xlabel "timestamp (s)"
set ylabel "devices (grouped by dutycycle)"

#plot [][0:100] "data.txt" using 1:(y=5) title "VAN" with impulse lw 2, \
#		"" u 2:(y=6) title "PTFl" w impulse lw 2, \
#		"" u 3:(y=60) title "403" w impulse lw 2, \
#		"" u 4:(y=61) title "autretruc" w impulse lw 2

plot [][0:20] "slave1-slots.log" using 1:3 title "REQUESTS IN SLOT" with lp lw 2, \
		"" u 1:4 title "OFFSETS IN SLOT" w lp

set terminal pngcairo
set output "cacherefill.png"
replot
