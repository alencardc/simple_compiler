 #!/bin/bash

filesToTest=$(ls | grep .c$) 

#Limpeza de arquivos
rm etapa1
rm etapa1Gold
rm saidas/*

#Compilação gold e nossa implementação
cd ..
make clear
make all
make allGold

cp etapa1 tests
cp etapa1Gold tests

cd tests

for file in $filesToTest 
do
    cat $file | ./etapa1 > saidas/${file}.txt
    cat $file | ./etapa1Gold > saidas/${file}.txt.Gold
done


  