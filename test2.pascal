const MAX = 1000;
 
var i,j,n,s:longint;
    opt:array[1..MAX]of longint;
 
begin
  readln(n);
  s:=0;
  for i:=1 to n do begin
    opt[i]:=1;
    for j:=1 to i-2 do
      inc(opt[i],opt[j]);
    inc(s,opt[i]);
  end;
 
  writeln(s);
end.