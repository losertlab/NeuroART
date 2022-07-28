% function s=mat2string(m);
%
% converts matrix m (up to 4 dimensions) to an eval'able string
%
% created SVD 10/02
%
function s=mat2string(m,rounddec);

if exist('rounddec','var'),
   m=round(m.*(10^rounddec))./(10^rounddec);
end

msize=size(m);

if length(msize)<=2,
   s=mat2str(m);
   
elseif length(msize)==3,
   s='cat(3,';
   for ii=1:msize(3),
      s=[s,mat2str(m(:,:,ii)),','];
   end
   s(end)=')';
elseif length(msize)==4,
   s='cat(4,';
   for jj=1:msize(4),
      s=[s,'cat(3,'];
      for ii=1:msize(3),
         s=[s,mat2str(m(:,:,ii,jj)),','];
      end
      s(end)=')';
      s=[s,','];
   end
   s(end)=')';
   
end

return

% option to tack on variable name

if ~isempty(inputname(1)),
   s=[inputname(1) ,'=',s];
end
