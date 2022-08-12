# Automação Residencial: Sistema elétrico remoto

IOT Project, home automation.

### Funcionamento:

A implementação será diretamente conectada a luminária do ambiente onde a mesma poderá ser manipulada via celular tendo um controle em qualquer raio geográfico onde há conexão via internet.
A lâmpada será conectada ao meio de alimentação externa e ao módulo relé o qual receberá os comandos da placa ESP8266, para mudar o estado da mesma, acionados pelo dashboard via celular, simulando um botão simples onde apresentará o estado da luminária como ON e OFF e um sensor de temperatura ambiente em grau celsius, DS18B20, o qual informará ao usuário quando solicitado.

![image](https://user-images.githubusercontent.com/48102306/169675362-049a47de-bdc0-4449-ac35-e7c8bbdf8140.png)

### Uso:

* Clonar repositório.
* Baixar bibliotecas necessárias
* Criar conta [Adafruit IO](https://accounts.adafruit.com), criar Feeds com os seguintes nomes ``room-1-lamp`` e ``room-1-temperature``

![image](https://user-images.githubusercontent.com/48102306/169675233-39c34406-d8a7-49ea-9a15-e0e40e493f3f.png)

* Atualizar o arquivo config.h com os dados do Adafruit IO MQTT, após criação da conta { IO_USERNAME, IO_KEY } e senha do roteador wifi { WIFI_SSID, WIFI_PASS }.
* Conectar o NodeMcu ESP8266 e importar o código

**Pinagem:**

![image](https://user-images.githubusercontent.com/48102306/169675857-dd53525e-c27c-4df5-8438-f469abc7280a.png)








