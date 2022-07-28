% function sout=strsep(s,delim,keepstr)
%
% created SVD 3/24/02
%
% 
function sout=strsep(s,delim,keepstr)

if iscell(s),
   sout=s;
   return
end

if ~exist('keepstr','var'),
   keepstr=0;
end

sout={};
if length(s)==0,
   return
end
if ~exist('delim','var'),
   delim=' ';
end

x1=[0 find(s==delim)];
x2=[x1(2:end) length(s)+1];

for ii=1:length(x1),
   ts=s((x1(ii)+1):(x2(ii)-1));
   if keepstr | ismember(exist(ts),[1 2 3 4 5 6]),
      sout{ii}=ts;
   elseif isempty(ts),
      sout{ii}=[];
   else
      try
         sout{ii}=evalin('caller',ts);
      catch
         sout{ii}=ts;
      end
   end
   
   %f strcmp(num2str(str2num(sout{ii})),sout{ii}),
   %  sout{ii}=str2num(sout{ii});
   %nd
end



