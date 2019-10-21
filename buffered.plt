# set the terminal, i.e., the figure format (eps) and font (Helvetica, 20pt)
set term postscript eps enhanced "Helvetica" 20 

# reset all options to default, just for precaution
reset

# set the figure size
set size 0.7,0.7

# set the figure name
set output "buffered.eps"

# set the x axis
set xrange [0:1000]
set xlabel "Simulation time (s)"
set xtics 0,200,1000
set mxtics 2

# set the y axis
set yrange [0:25]
set ylabel "Average buffered packets"
set ytics 0,5,25
set mytics 2

# set the legend (boxed, on the left)
set key box left height 0.5

# set the grid (grid lines start from tics on both x and y axis)
set grid xtics ytics

# set the lambda, mu and K parameters
lambda = 900.0
mu = 1000.0
K = 30

# n is the theoretical number of queued packets at steady state
rho = lambda/mu
n = rho==1.0 ? K/2 : (rho/(1.0-rho))-rho**(K+1)*(K+1)/(1.0-rho**(K+1))
print n

# plot the data from the log file along with the theoretical curve
plot "buffered.log" u 1:5 t "Simulation" w p pt 7 ps 2 lc rgb "#AAAAAA", \
     n t "Theory" w l lt 1 lw 5 lc rgb "#000000"


