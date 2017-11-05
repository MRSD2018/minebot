function [ cc_x, cc_y ] = circumcenter( pIx,pIy,pJx,pJy,pKx,pKy )
%CIRCUMCENTER Summary of this function goes here
%   Detailed explanation goes here

dIJa =  [pJx - pIx, pJy - pIy];
dIJ.x = dIJa(1);
dIJ.y = dIJa(2);

dJKa = [pKx - pJx, pKy - pJy];
dJK.x = dJKa(1);
dJK.y = dJKa(2);

dKIa = [pIx - pKx, pIy - pKy];
dKI.x = dKIa(1);
dKI.y = dKIa(2);

sqI = pIx * pIx + pIy * pIy;
sqJ = pJx * pJx + pJy * pJy;
sqK = pKx * pKx + pKy * pKy;

det = dJK.x * dIJ.y - dIJ.x * dJK.y;

if (abs(det) < 1.0e-10)
    cc=0;
else
    cc=[(sqI * dJK.y + sqJ * dKI.y + sqK * dIJ.y) / (2 * det),-(sqI * dJK.x + sqJ * dKI.x + sqK * dIJ.x) / (2 * det)];

end    
cc_x = cc(1);
cc_y = cc(2);
end

