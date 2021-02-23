 #!/bin/bash

cd /home/mwcamargo/compiler/simple_compiler/tests

filesToTest=$(ls | grep .c$) 

#Limpeza de arquivos
rm -f etapa1
rm -f etapa1Gold
rm -f saidas/*.txt

#Compilação gold e nossa implementação
cd ..
make clear
make all

cp etapa1 tests

cd tests
mkdir -p saidas
touch saidas/log_test.txt
for file in $filesToTest 
do
    cat $file | ./etapa1 > /dev/null
    if [ $? -eq 0 ]
    then
        echo "$file: Success" >> saidas/log_test.txt
    else
        echo "$file: Error" >> saidas/log_test.txt
    fi
done

diff saidas/log_test.txt saidas/log_test.gold



  