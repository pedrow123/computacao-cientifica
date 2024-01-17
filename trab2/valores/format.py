f_otimizado = open("./fotimizada.txt")
conteudo_otimizado = f_otimizado.read()
valores_otimizado = conteudo_otimizado.split(' ')

f_nao_otimizado = open("./fnao_otimizada.txt")
conteudo_nao_otimizado = f_nao_otimizado.read()
valores_nao_otimizado = conteudo_nao_otimizado.split(' ')


# tamanhoN = [64, 128, 200, 256, 512, 600, 800, 1024, 2000, 3000, 4096, 6000, 7000, 10000, 50000, 10**5, 10**6, 10**7, 10**8]
tamanhoN = [100000000, 10000000, 1000000, 100000, 10000, 1024, 128, 2000, 200, 256, 3000, 4096, 50000, 512, 6000, 600, 64, 7000, 800]
for i, tam in enumerate(tamanhoN):
    print(tam, float(valores_otimizado[i]), float(valores_nao_otimizado[i]))
