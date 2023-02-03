load RT_test1
% this should load variables with structure needed by future mex function

%% assign var names to par1, par2, ... manually and m-function path/name
par1=gpuArray(norm_meanIMG) ;
par2=gpuArray(norm_meanIMG) ;
par3=int16(1);
funcPath='D:\RTapp_v12\utilities\';
funcName='dftregistration_coderGPU.m';
% This only needed if there is test m-script available to substitute mex
% file for testing in the end 
testScript='testScript';

%next specify max size of every dimention
dim1max=1024;
dim2max=dim1max;

% last parameter is 'true' for variable dim sizes
% or use just varaible name for fixed kind of parameter type
inp1=coder.typeof(par1,[dim1max,dim2max],true);
inp2=coder.typeof(par2,[dim1max,dim2max],true);
%inp3=coder.typeof(par3,[dim1max,dim2max],true);
inp3=par3;
%%
cd(funcPath);
coder.screener(funcName);
 dftregistration_coderGPU(par1,par2,par3);
gpucoder dftregistration_coderGPU.prj
% -report dftregistration_coderGPU.m -args { inp1, inp2, inp3 }
%%
% This will substitute funcName with <funcName>_mex in <testScript>.m
coder.runTest(testScript, funcName)