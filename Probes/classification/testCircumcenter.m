clear 
clc
close all

pI.x = 1;
pI.y = 1;
pJ.x = 2;
pJ.y = 0;
pK.x = 0;
pK.y = 0;

[ cc_x, cc_y ] = circumcenter( pI,pJ,pK );

figure
hold on
plot(pI.x,pI.y,'.')
plot(pJ.x,pJ.y,'.')
plot(pK.x,pK.y,'.')
plot(cc_x,cc_y,'.')
