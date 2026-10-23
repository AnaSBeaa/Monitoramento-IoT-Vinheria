
# 🍷 Vinheria Agnello — Monitoramento IoT com FIWARE

## Integrantes

| Nome completo | RM |
|---|---:|
| Ana Beatriz Santos | 569085 |
| Paloma Cordeiro | 571460 |
| Yasmin Gomes | 572363 |

---

## Vídeo de demonstração prática
[Vídeo Monitoramento](https://youtu.be/u1aAduivg9o?feature=shared)

## 1. Sobre o projeto

O projeto Vinheria Agnello tem como objetivo desenvolver uma solução de Internet das Coisas (IoT) para monitorar as condições ambientais relevantes para o armazenamento de vinhos.

A solução utiliza um ESP32 simulado na plataforma Wokwi, um sensor DHT22 para medir temperatura e umidade e um sensor LDR para monitorar a luminosidade. Os dados são enviados por meio do protocolo MQTT e consultados no FIWARE Orion utilizando a API NGSIv2.

Também foram utilizados o Postman para consultar os atributos da entidade e o MyMQTT para acompanhar as mensagens e testar a atualização manual de dados.

## 2. Objetivos

- Monitorar temperatura, umidade e luminosidade.
- Coletar dados utilizando o ESP32 e os sensores DHT22 e LDR.
- Simular o circuito eletrônico na plataforma Wokwi.
- Transmitir informações utilizando o protocolo MQTT.
- Incluir data e hora nas mensagens enviadas pelo dispositivo.
- Integrar os dados ao FIWARE Orion.
- Consultar os atributos utilizando o Postman.
- Testar a publicação manual de mensagens por meio do MyMQTT.
- Verificar a comunicação entre os componentes da solução IoT.

## 3. Tecnologias utilizadas

| Tecnologia | Finalidade |
|---|---|
| ESP32 | Coleta e transmissão dos dados |
| Wokwi | Simulação do circuito e do código |
| DHT22 | Medição de temperatura e umidade |
| LDR | Leitura da luminosidade |
| Wi-Fi | Conexão do ESP32 |
| MQTT | Protocolo de comunicação IoT |
| FIWARE Orion Context Broker | Gerenciamento e consulta da entidade |
| IoT Agent | Integração das mensagens MQTT com o FIWARE |
| Docker | Execução dos serviços em containers |
| Ubuntu | Ambiente utilizado para configurar os serviços |
| Postman | Consulta dos dados pela API NGSIv2 |
| MyMQTT | Acompanhamento e publicação manual de mensagens |

## 4. Arquitetura da solução

O fluxo de comunicação do projeto é:

**Sensores DHT22 e LDR → ESP32 no Wokwi → Wi-Fi → MQTT → IoT Agent → FIWARE Orion → Postman**

O ESP32 coleta os dados dos sensores e publica as mensagens em um tópico MQTT. O IoT Agent interpreta as mensagens de acordo com a configuração do dispositivo e encaminha os valores para a entidade correspondente no FIWARE Orion.

O Postman é utilizado para consultar os atributos da entidade, enquanto o MyMQTT permite acompanhar as mensagens e realizar testes de publicação manual.

## 5. Configuração do ambiente

### 5.1. Ubuntu e Docker

O ambiente foi preparado em uma máquina virtual com Ubuntu. O Docker foi utilizado para executar os serviços necessários à aplicação.

Para verificar os containers em execução, utilize:

```bash
sudo docker ps
```

Esse comando lista os containers ativos no ambiente.

### 5.2. Bibliotecas do Wokwi

O projeto utiliza bibliotecas para leitura do sensor DHT22 e comunicação MQTT.

No arquivo `libraries.txt`, devem constar as seguintes bibliotecas:

```text
DHT sensor library
Adafruit Unified Sensor
PubSubClient
```

A biblioteca `DHT sensor library` permite realizar as leituras de temperatura e umidade. A `Adafruit Unified Sensor` é uma dependência utilizada pela biblioteca do sensor, enquanto a `PubSubClient` possibilita a comunicação com o broker MQTT.

## 6. Configuração do ESP32 no Wokwi

O circuito é composto por um ESP32, um sensor DHT22 e um sensor LDR.

O DHT22 fornece as leituras de temperatura e umidade. O LDR é conectado a uma entrada analógica do ESP32, permitindo obter uma leitura relacionada à luminosidade.

### 6.1. Configuração MQTT

O código utiliza as seguintes configurações:

```cpp
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

const char* MQTT_SERVER = "3.135.234.101";
const int MQTT_PORT = 1883;

const char* MQTT_CLIENT_ID = "ESP32-device042";
const char* MQTT_TOPIC = "/TEF/device042/attrs";
```

O ESP32 utiliza a rede de simulação do Wokwi e se conecta ao broker MQTT disponibilizado para a atividade.

O tópico utilizado para a publicação das mensagens é:

```text
/TEF/device042/attrs
```

### 6.2. Coleta e envio dos dados

O programa realiza as seguintes etapas:

1. Inicializa o sensor DHT22.
2. Configura a leitura analógica do LDR.
3. Conecta o ESP32 à rede Wi-Fi.
4. Sincroniza o horário por meio de um servidor NTP.
5. Estabelece a conexão MQTT.
6. Realiza as leituras de temperatura, umidade e luminosidade.
7. Inclui o timestamp na mensagem.
8. Publica os dados no tópico configurado.
9. Repete o processo aproximadamente a cada cinco segundos.

A luminosidade é representada em uma escala percentual de 0 a 100, calculada a partir da leitura analógica do LDR. Esse valor representa uma escala relativa e não uma medição calibrada em lux.

### 6.3. Formato da mensagem MQTT

O payload utiliza o formato configurado para o dispositivo:

```text
|temperatura|24.0
|umidade|40.0
|luminosidade|65
|hora|2026-10-23T21:47:02Z
```

Os valores são apenas um exemplo ilustrativo. Durante a simulação, eles variam de acordo com as leituras dos sensores e o horário obtido.

O formato compacto é utilizado na mensagem MQTT para que o IoT Agent possa interpretar os atributos conforme a configuração do dispositivo.

### 6.4. Monitor Serial

No Monitor Serial, os dados são apresentados em linhas separadas para facilitar a leitura:

```text
Dados coletados:
Temperatura: 24.0 °C
Umidade: 40.0 %
Luminosidade: 65 %
Horario: 2026-10-23T21:47:02Z
Mensagem MQTT enviada!
Topico: /TEF/device042/attrs
----------------------------
```

Os valores acima são ilustrativos. A exibição separada no Monitor Serial não altera o formato compacto publicado no MQTT.

## 7. Consulta dos dados no FIWARE Orion

### 7.1. Identificação da entidade

A entidade consultada no projeto é:

```text
urn:ngsi-ld:IndoorEnvironmentObserved:042
```

### 7.2. Requisição pelo Postman

No Postman, configure uma requisição do tipo **GET** para o seguinte endereço:

```text
http://3.135.234.101:1026/v2/entities/urn:ngsi-ld:IndoorEnvironmentObserved:042
```

Em **Headers**, configure:

| Key | Value |
|---|---|
| `fiware-service` | `smart` |
| `fiware-servicepath` | `/` |

Depois, clique em **Send** para consultar a entidade.

### 7.3. Atributos consultados

A resposta JSON contém os atributos disponibilizados pela entidade. Os principais atributos utilizados no projeto são:

| Atributo | Descrição |
|---|---|
| `temperature` | Temperatura medida pelo DHT22 |
| `relativeHumidity` | Umidade relativa medida pelo DHT22 |
| `luminosity` | Valor relacionado à luminosidade |
| `dateObserved` | Data e hora associadas à observação, quando configuradas |
| `TimeInstant` | Instante registrado no contexto, quando presente |

Os nomes e metadados retornados dependem do cadastro da entidade e do mapeamento configurado no FIWARE.

### 7.4. Verificação da atualização automática

Para testar a atualização dos dados:

1. Inicie a simulação no Wokwi.
2. Confira no Monitor Serial se o ESP32 está conectado ao MQTT.
3. Verifique se as mensagens estão sendo publicadas.
4. Faça uma requisição GET no Postman.
5. Altere os valores dos sensores no Wokwi.
6. Aguarde uma nova publicação.
7. Execute novamente a requisição GET e compare os valores retornados.

O recebimento de uma publicação pelo broker MQTT não garante, isoladamente, que o Orion tenha atualizado a entidade. A mensagem precisa ser interpretada pelo IoT Agent e encaminhada à entidade correta.

## 8. Comunicação pelo MyMQTT

O aplicativo MyMQTT foi utilizado para acompanhar as mensagens MQTT e testar a escrita manual de um atributo.

### 8.1. Configuração da conexão

- **Broker:** `3.135.234.101`
- **Porta:** `1883`
- **Tópico do dispositivo:** `/TEF/device042/attrs`

A conexão deve utilizar as configurações de autenticação e segurança disponibilizadas para o ambiente da atividade.

### 8.2. Leitura das mensagens

Para acompanhar as mensagens enviadas pelo ESP32, inscreva-se no tópico:

```text
/TEF/device042/attrs
```

Com a simulação em execução, o aplicativo poderá exibir as mensagens publicadas pelo dispositivo.

### 8.3. Escrita manual de um atributo

Também foi testada a publicação manual de uma mensagem no mesmo tópico.

Para atualizar a temperatura, publique o seguinte payload:

```text
t|13.0
```

A publicação deve ser realizada no tópico:

```text
/TEF/device042/attrs
```

Em seguida, execute novamente a requisição GET no Postman:

```text
http://3.135.234.101:1026/v2/entities/urn:ngsi-ld:IndoorEnvironmentObserved:042
```

No teste realizado, o atributo `temperature` foi atualizado para o valor `13`.

Esse resultado demonstra que a publicação manual, utilizando o formato esperado pelo dispositivo, conseguiu atualizar o atributo consultado no FIWARE Orion.

## 9. Testes e validação

Os testes realizados durante o desenvolvimento estão resumidos a seguir.

| Teste | Resultado |
|---|---|
| Inicialização do Docker | Docker instalado e utilizado no ambiente |
| Conexão Wi-Fi do ESP32 | Conexão realizada no Wokwi |
| Conexão MQTT do ESP32 | Conexão estabelecida com o broker |
| Publicação periódica | Monitor Serial confirmou o envio das mensagens |
| Consulta da entidade no Postman | Entidade retornada pela API NGSIv2 |
| Escrita manual pelo MyMQTT | `t\|13.0` atualizou a temperatura no Orion |
| Organização do Monitor Serial | Atributos exibidos em linhas separadas |

### 9.1. Histórico e exportação

O projeto previa consulta histórica e exportação dos dados. Entretanto, a consulta atual da entidade pelo Orion não comprova, por si só, que o armazenamento histórico esteja funcionando.

Esses recursos devem ser considerados validados somente depois de testar o serviço histórico, confirmar que os registros estão sendo armazenados e verificar o procedimento de exportação exigido pela atividade.

## 10. Dificuldades encontradas e soluções

### 10.1. Configuração do Ubuntu e do Docker

Uma das dificuldades foi preparar o ambiente na máquina virtual Ubuntu e instalar o Docker para executar os serviços necessários.

Foi preciso verificar a instalação e confirmar que o Docker estava funcionando antes de avançar para a configuração do FIWARE.

### 10.2. Erros de sintaxe no Docker Compose

Durante a criação do arquivo `docker-compose.yml`, ocorreram erros de sintaxe YAML relacionados à indentação e à escrita de propriedades.

Foi necessário revisar o arquivo para que os serviços pudessem ser configurados corretamente. Essa etapa demonstrou a importância de respeitar a estrutura exigida pelo YAML.

### 10.3. Publicação MQTT sem atualização da entidade

Uma dificuldade ocorreu quando a publicação MQTT era confirmada no Monitor Serial, mas os valores consultados no Postman não eram atualizados em todos os testes.

A confirmação de publicação indica que o broker aceitou a mensagem, mas não garante que o FIWARE Orion tenha recebido a atualização. Por isso, foi necessário conferir o tópico, o formato da mensagem e a integração com o IoT Agent.

### 10.4. Teste de escrita pelo MyMQTT

A publicação manual pelo MyMQTT foi utilizada para testar a atualização de um atributo de forma isolada.

Ao publicar `t|13.0` no tópico `/TEF/device042/attrs` e realizar novamente o GET no Postman, foi possível observar a atualização da temperatura para `13`.

Esse teste ajudou a validar o formato de mensagem utilizado para a escrita manual.

### 10.5. Atributos duplicados em português e inglês

Durante as consultas ao Orion, foram observados atributos equivalentes com nomes diferentes, como:

- `temperature` e `temperatura`;
- `relativeHumidity` e `umidade`;
- `luminosity` e `luminosidade`.

O FIWARE trata nomes diferentes como atributos distintos. Por isso, a solução deve manter uma convenção consistente de nomenclatura e respeitar o mapeamento esperado pelo IoT Agent.

Antes de remover qualquer atributo, é necessário verificar quais nomes são utilizados pela aplicação e pela configuração do dispositivo, evitando excluir dados necessários.

### 10.6. Aprendizados

As dificuldades enfrentadas mostraram a importância de testar cada parte do sistema separadamente: sensores, conexão Wi-Fi, broker MQTT, IoT Agent e FIWARE Orion.

A verificação do resultado pelo Postman após cada teste de publicação ajudou a identificar se o atributo esperado estava sendo atualizado.

## 11. Evidências do funcionamento

As evidências do projeto podem ser organizadas em uma pasta chamada `evidencias`.

- **Evidência 1 — Wokwi:** circuito com ESP32, DHT22 e LDR.
- **Evidência 2 — Monitor Serial:** conexão MQTT, leituras e timestamp.
- **Evidência 3 — MyMQTT:** conexão com o broker e tópico `/TEF/device042/attrs`.
- **Evidência 4 — Escrita MQTT:** publicação manual de `t|13.0`.
- **Evidência 5 — Postman:** resposta da entidade antes da atualização.
- **Evidência 6 — Postman atualizado:** resposta mostrando a temperatura alterada para `13`.
- **Evidência 7 — Docker:** serviços em execução, caso seja solicitado na entrega.

Não inclua senhas, tokens ou outras credenciais nas capturas de tela.

## 12. Vídeo demonstrativo

O vídeo deve apresentar o funcionamento da solução na seguinte ordem:

1. Introdução ao projeto e seu objetivo.
2. Circuito simulado no Wokwi.
3. Leitura dos sensores e apresentação dos atributos no Monitor Serial.
4. Conexão e publicação de mensagens MQTT.
5. Consulta da entidade no FIWARE Orion pelo Postman.
6. Publicação manual de `t|13.0` pelo MyMQTT.
7. Nova consulta no Postman, demonstrando a atualização da temperatura.
8. Breve explicação das dificuldades encontradas e das soluções aplicadas.

**Link do vídeo:** [Inserir link]

**Link do projeto Wokwi:** [Inserir link]

## 13. Conclusão

O projeto da Vinheria Agnello demonstrou a utilização de um ESP32 simulado no Wokwi para coletar dados ambientais por meio dos sensores DHT22 e LDR.

As leituras são transmitidas por MQTT e consultadas em uma entidade do FIWARE Orion utilizando a API NGSIv2. O uso do Postman permitiu verificar os atributos da entidade, enquanto o MyMQTT foi utilizado para testar uma atualização manual de temperatura.

Durante o desenvolvimento, foram enfrentadas dificuldades relacionadas à configuração do ambiente Docker, à sintaxe do Docker Compose, ao formato das mensagens MQTT e à padronização dos nomes dos atributos.

Os testes realizados ajudaram a compreender a comunicação entre os componentes e a importância de validar os dados em cada etapa da integração.

Funcionalidades adicionais, como histórico persistido e exportação, devem ser descritas como concluídas somente após sua configuração e validação.

---