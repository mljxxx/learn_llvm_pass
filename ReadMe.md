
emit readable IR
```
clang -emit-llvm -S -o test.ll test.cpp
```
emit bitcode IR
```
clang -emit-llvm -c -o test.bc test.cpp
```
readable IR to bitcode IR
```
llvm-as test.ll
```
bitcode IR to readable IR
```
llvm-dis test.ll
```
IR to ASM
```
llc test.ll
```
load pass
```
opt --load-pass-plugin ../build/libHelloWorld.dylib --passes=HelloWorld --disable-output test.ll
```
output new IR
```
opt --load-pass-plugin ../build/libHelloWorld.dylib --passes=HelloWorld -S -o test_new.ll test.ll
clang -fpass-plugin=../build/libHelloWorld.dylib -S -emit-llvm -o test_new.ll test.cpp
```