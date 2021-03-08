 #!/bin/bash

filesToTest=$(ls | grep .c$)
binaryToTest=./etapa3 

#Limpeza de arquivos
rm -f $binaryToTest

#Compilação dos arquivos
cd ..
make clean
make all

cp $binaryToTest tests
cd tests

#Execução do valgrind para os arquivos .c no diretorio e verificação se houve ou não leak.
echo "-------------Reports Valgrind--------------"
for file in $filesToTest 
do
    #Redireciona a saida de erro para a saida padrão, e a padrão manda pra null. Isso pq o valgrind joga a saida pra stderr
    totalLinesMatching=$(valgrind $binaryToTest < $file  2>&1 >/dev/null | grep 'All heap blocks were freed' -c)
    
    if [ $totalLinesMatching -eq 1 ]
    then
        echo "$file: (NO) leak"
    else
        echo "$file: (THERE'S A) Leak"
    fi
done





  