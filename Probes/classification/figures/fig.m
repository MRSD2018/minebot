% FALSE
meanCircleFitError, meanGroundTruthError
4.508529428378694, 5.021521213571978
stdCircleFitError, stdGroundTruthError
3.780896593971208, 1.777659913889879

% TRUE
meanGroundTruthError, meanCircleFitError
1.706867872477738, 1.057415715429267
stdCircleFitError, stdGroundTruthError
0.977576391271889, 0.468710633654602

figure
names = {'TRUE'; 'FALSE'};
hold on; grid on;
errorbar(1, 1.057415715429267, 0.977576391271889, 'b','LineWidth',2)
plot(1, 1.057415715429267,'.b','MarkerSize',25);

errorbar(2, 5.021521213571978, 3.780896593971208, 'r','LineWidth',2)
plot(2, 5.021521213571978,'.r','MarkerSize',25);
set(gca,'xtick',[1:5],'xticklabel',names)
xlim([0.5 2.5])
ylabel('Fit Error')
