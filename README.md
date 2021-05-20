Para compilar com otimizações, utilize o comando: ./etapa7 -O "nomeArquivo"
Para compilar sem otimizações, utilize o comando: ./etapa7 "nomeArquivo"

Os dois exemplos utilizados na explicação abaixo estão na raiz da entrega, sob o nome de event_duration.c e fatorial_while.c

Nesta etapa, escolhemos otimizações de código que foram aplicadas sobre código assembly e iloc. A seguir listamos as otimizações:

**Código Assembly:**
- Remoção de movs redundantes: Remoção de um mov do tipo src->src. Como o código assembly é de 2 endereços nós implementamos a soma a partir de um mov para um registrador seguido de um add, isso fez com que alguns casos o mov fosse redundante, e por isso removemos tais movs com essa otimização.
- Otimização de constantes literais: Ao invés de mover constantes literais para registradores temporários, usamos diretamente eles nas instruções, substituindo o temporário pelo literal correspondente.
- Substituição de add 1 e sub 1 por inc/dec: Instruções que somam 1/subtraem 1 são substituídas por um inc/dec.
- JMP para a próxima instrução: Eliminamos JMPS que são para um label que vem logo na sequência(próxima instrução).
- Remoção de push/pops pré e pós multiplicação/divisão: Como reservamos os registradores eax/edx para realizar multiplicação/divisão não precisamos mais salvar e restaurar o contexto desses registradores, então removemos nessa etapa tais instruções.
- Otimização de registradores intermediários: Removemos registradores intermediários desnecessários que estavam sendo usados, e suas instruções de inicialização.

**Código Iloc:**
- Otimização de expressões constantes: Ao invés de gerar código para expressões constantes nosso compilador calcula o resultado, assim eliminando a necessidade de gerar código para tais expressões.

Para comparar o desempenho obtido a partir da aplicação das otimizações, reportamos o número de linhas bem como a redução em % do número de linhas, calculada a partir da seguinte fórmula:

`reducao = (nLinhasSemOtimizacao - nLinhasComOtimizacao)/ nLinhasSemOtimizacao`

A partir do uso de tais otimizações, foi possível deixar o código mais curto, o que é exemplificado a partir dos dois códigos a seguir:

**Cálculo de fatorial**
Arquivo `fatorial_while.c` na raiz do projeto.
```c
int fatorial;
 
int main(){
    int i <= 5; 
    fatorial = 1;
    while(i > 1) do{
        fatorial = fatorial * i;
        i = i - 1;
    }
 
    return fatorial;
}
```
Compilando sem otimizações, o assembly gerado tem 60 linhas. Caso usemos a flag -O para ligar as otimizações, o assembly resultante tem 45 linhas. O que representa uma redução de 25% do código gerado.

**Cálculo da duração total em ms de um evento com n palestra**
Arquivo `event_duration.c` na raiz do projeto.
```c
int main() {
  // Duração em ms de um evento com n palestras de duração dada em horas
  // Se a linguagem possuísse vetores, o loop iria percorrer o vetor de duração das palestras que compõem o evento
  int total_duration_ms <= 0;
  int i;
  int n <= 10;
  for (i = 0 : i < n : i = i + 1) {
    total_duration_ms = 60 * 60 * 1000 + total_duration_ms;
  }
  
  return total_duration_ms;
}
```
Compilando sem otimizações, o assembly gerado tem 69 linhas. Caso usemos a flag -O para ligar as otimizações, o assembly resultante tem 47 linhas. O que representa uma redução de 31,88% do código gerado.