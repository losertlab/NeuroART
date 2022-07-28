Hx = [51, 38, 13, -2, 38, -14];
Hy = [114, 74, 69, 111, 94, 103];

Bx = [161, 187, 192, 157, 191, 228];
By = [270, 254, 319, 243, 302, 270];

symbols = ['x', '+', '*', '.', 'o', 's'];
figure;

for itr = 1:6
    scatter(Hx(itr), Hy(itr), 100, symbols(itr),'r');
    hold on;
end

for itr = 1:6
    scatter(Bx(itr), By(itr), 100, symbols(itr),'b');
    hold on;
end

HBx = Bx - Hx;
HBy = By - Hy;
figure; plot([0,HBx(1)],[0,HBy(1)]); hold on;
plot([0,HBx(2)],[0,HBy(2)]); hold on;
plot([0,HBx(3)],[0,HBy(3)]); hold on;
plot([0,HBx(4)],[0,HBy(4)]); hold on;
plot([0,HBx(5)],[0,HBy(5)]); hold on;
plot([0,HBx(6)],[0,HBy(6)]); hold on;