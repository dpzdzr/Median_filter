

.data
count dword 0

.code
MyProc1 proc
add RCX, RDX
mov RAX, RCX
ret
MyProc1 endp

MedianFiltering proc

mov rbx, [rdi + 0]

MedianFiltering endp


end

