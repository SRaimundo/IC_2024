import math

nomeArquivoDados = "dadosEntrada.txt"
nomeArquivoAcomodacoes = "acomodacoes.txt"
nomeArquivoVoos = "voos.txt"

cidadeId = {}  # dicionário para converter o nome da cidade em uma representação numérica
idCidade = {}  # dicionário para converter a representação numérica no nome da cidade
numCidade = 0
origem = ""
quantDias = 0
quantCidades = 0
limiteDias = []  # quantidade mínima de dias, quantidade máxima de dias
cidade = ""
id = ""
chegada = 0
saida = 0
valor = 0.0

with open(nomeArquivoDados, 'r') as arquivoDados:
    linhas = arquivoDados.readlines()
    origem = linhas[0].strip()
    idCidade[numCidade] = origem
    cidadeId[origem] = numCidade
    numCidade += 1

    quantDias = int(linhas[1].strip()) + 1
    quantCidades = int(linhas[2].strip()) + 1

    limiteDias = [0] * quantCidades

    for i in range(3, quantCidades + 3):
        dados_cidade = linhas[i].split()
        cidade, minimoDias, maximoDias = dados_cidade
        limiteDias[numCidade] = (int(minimoDias), int(maximoDias))
        idCidade[numCidade] = cidade
        cidadeId[cidade] = numCidade
        numCidade += 1

with open(nomeArquivoAcomodacoes, 'r') as arquivoAcomodacoes:
    linhas = arquivoAcomodacoes.readlines()
    quantAcomodacoes = int(linhas[0].strip())

    acomodacoes = [[[math.inf] * quantDias for _ in range(quantDias)] for _ in range(numCidade)]

    for i in range(1, quantAcomodacoes + 1):
        dados_acomodacao = linhas[i].split()
        id, chegada, saida, cidade, valor = dados_acomodacao
        if cidade in cidadeId:
            acomodacoes[cidadeId[cidade]][int(chegada)][int(saida)] = min(acomodacoes[cidadeId[cidade]][int(chegada)][int(saida)], float(valor))

with open(nomeArquivoVoos, 'r') as arquivoVoos:
    linhas = arquivoVoos.readlines()
    quantVoos = int(linhas[0].strip())

    voos = [[[math.inf, math.inf] for _ in range(numCidade)] for _ in range(numCidade)]

    for i in range(1, quantVoos + 1):
        dados_voo = linhas[i].split()
        id, saida, chegada, cidadeOrigem, cidadeDestino, valor, tempoViagem = dados_voo
        if cidadeOrigem in cidadeId and cidadeDestino in cidadeId and saida == chegada:
            voos[cidadeId[cidadeOrigem]][cidadeId[cidadeDestino]][int(saida)] = min(voos[cidadeId[cidadeOrigem]][cidadeId[cidadeDestino]][int(saida)], [float(valor), float(tempoViagem)])
