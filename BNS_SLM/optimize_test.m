lb = [0,0];
ub = [1,2];

A = [];
b = [];
Aeq = [];
beq = [];

x0 = (lb + ub)/2;

fun = @(x)errorFun(x,test);

x = fmincon(fun,x0,A,b,Aeq,beq,lb,ub);