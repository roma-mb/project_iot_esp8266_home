# Automação Residencial: Sistema elétrico remoto

IOT Project, home automation.

i) Uma breve descrição do funcionamento e uso para quem quiser reproduzir.

ii) O software desenvolvido e a documentação de código.

iii) A descrição do hardware utilizado (plataformas de desenvolvimento, sensores, atuadores, impressão 3D de peças, medidas de peças e caixas etc.)

iv) A documentação das interfaces, protocolos e módulos de comunicação.

v) O projeto deve possuir comunicação/controle via internet (TCP/IP) e uso do Protocolo MQTT.

## Funcionamento:

A implementação será diretamente conectada a luminária do ambiente onde a mesma poderá ser manipulada via celular tendo um controle em qualquer raio geográfico onde há conexão via internet.
A lâmpada será conectada ao meio de alimentação externa e ao módulo relé o qual receberá os comandos da placa ESP8266, para mudar o estado da mesma, acionados pelo dashboard via celular, simulando um botão simples onde apresentará o estado da luminária como ON e OFF e um sensor de temperatura ambiente em grau celsius, DS18B20, o qual informará ao usuário quando solicitado.

#### Uso:

```
> Clonar repositório.
>
```
