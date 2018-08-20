quadrant_x_1 = load(strcat('src_datas_x.txt'));
quadrant_y_1 = load(strcat('src_datas_y.txt'));
quadrant_fit_x_1 = load(strcat('res_x_0821.txt'));
quadrant_fit_y_1 = load(strcat('res_y_0821.txt'));
figure(1);
plot(quadrant_x_1,quadrant_y_1,'r.', quadrant_fit_x_1, quadrant_y_1,'b.', quadrant_x_1, quadrant_fit_y_1, 'g.');
grid on;

figure(2);
A = polyfit(quadrant_x_1,quadrant_y_1,4);
fit_y = polyval(A,quadrant_x_1);

A = polyfit(quadrant_y_1,quadrant_x_1,4);
fit_x = polyval(A,quadrant_y_1);
plot(quadrant_x_1,quadrant_y_1,'r.', fit_x, quadrant_y_1, 'b.', quadrant_x_1,fit_y,'g.')
grid on;