

figure(2); hold on
parallel_i = [13.917 9.378 7.281 5.899 5.084 4.387 3.945 3.584 3.239 2.977 2.786 2.607 2.457 2.31 2.236 2.167 2.118 2.115];
parallel_k = [390.84 249.85 229.108 190.58 167.98 150.74 137.65 128.558 117.654 111.36 104.326 98.732 93.127 88.671 84.486 80.88 78.33 78.09];
parallel_j = [53.934 50.751 52.411 53.797 57.75 59.299 64.285 67.55 70.625 75.839 80.166 83.454 87.87 90.644 94.439 98.6 99.749 109.549];
x = [2:1:19];
width = 20;
height = 400;
i = plot(x, parallel_i)
m_i = "Parallel I"
j = plot(x, parallel_j)
m_j = "parallel J"
k = plot(x, parallel_k)
m_k = "parallel K"
% p = plot(x, parallel_i, x, 'g', parallel_j, '--', x, parallel_k, 'b--o');

legend([i, j, k], [m_i, m_j, m_k])
title('$\mid Performance \mid$','Interpreter','latex')
xlabel('Number of Threads. $T$','Interpreter','latex')
ylabel('Elapsed Time $t$','Interpreter','latex')

grid