% Load Cell Calibration
clear all; close all;

files = dir('lab-10-26-17');
files = files(3:length(files));

for i = 1:length(files)
    filename = strcat ( strcat(files(i).folder, "\"), files(i).name );
    
    C = strsplit(files(i).name,".");
    name = str2double(C{1});
    truth(i) = name;
    
    M = csvread(filename);
    avg = mean(M);
    measured(i) = avg;

end

truth = truth/1000; % convert to kg

format long g
f = polyfit(measured,truth,1); % linear fit

scale = 1/f(1)
offset = f(2)

x1 = linspace(min(measured),max(measured));
y1 = polyval(f,x1);

figure;
hold on;
title('Load Cell Calibration')
xlabel('Measured');
ylabel('Actual (kg)')
plot (measured,truth,'x');
plot (x1,y1);
hold off;
grid on;
