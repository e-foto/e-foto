# Sobre o empacotamento do e-foto

Este guia rápido irá descrever quais requisitos devem ser atendidos para criar 
pacotes para distribuição do e-foto em sistemas operacionais diversos.

## Recomendações gerais

Os executáveis destinados a distribuição devem ser configurados como release 
para a compilação. Deve-se revisar a lista de dependencias e versões usadas,
pois o pacote terá de incluir (ou apontar no repositório) todos os recursos 
necessários para a correta execução do e-foto.

### Dependencias comuns

Os recursos de terceiros utilizados atualmente são:

- Módulos de Qt5 para a criação de interfaces gráficas de usuários (GUI)
- A GDAL para abstração de formatos de dados espaciais (vetoriais e raster)
- O programa de linha de comando xsltproc para processamento dos relatórios
- A API OpenGL disponivel nos SOs através de drivers do hardware gráfico

## Pacote Debian

Os pacotes debian se destinam a sistemas Linux e são compativeis compatíveis 
com os repositórios de pacotes baseados em Debian. Nosso arquivo de controle 
foi configurado segundo recomendações do trabalho de Matheus Vargas para que 
possa ser submetido ao repositório do DebianGIS.

### Gerando o pacote Debian 

O executável deve ser colocado na pasta bin e o arquivo de controle atualizado 
para garantir a corretude das dependências, versões e informações do pacote. A 
instrução _strip_ é recomendada para eliminar símbolos de depuração que tenham 
sido mantidos (apesar da opção pela configuração release).

As permissões e autoria do diretório de empacotamento devem ser reconfiguradas

'''
sudo chown -hR root:root <diretório>
sudo chmod -R 755 <diretório>
''' 

O pacote é montado com o comando

'''
sudo dpkg-deb -b <diretório> <nome_do_pacote.deb>
'''

## Pacote MSI

Os pacotes MSI (Microsoft Installer) se destinam a sistemas Windows. Montá-los 
requer uma instalação do WiX toolset v3 e de sua extensão para o Visual Studio
(adotamos a versão Community, tendo em vista a licença de software livre).

### Gerando o pacote MSI 

Os executáveis (e-foto e xsltproc), bem como todas as bibliotecas necessárias, 
devem ser colocados no mesmo diretório do arquivo de projeto _efoto.wixproj_ e 
o arquivo _Product.wxs_ atualizado para garantia da corretude das dependências, 
versões e informações do pacote. O projeto depende de extensões como o WiXUtil 
e o WiXUI, então o caminho para estes recursos deve ser ajustado se necessário. 

O pacote pode ser gerado pela compilação na interface do VS ou através da linha 
de comando usando as instruções light e candle.

