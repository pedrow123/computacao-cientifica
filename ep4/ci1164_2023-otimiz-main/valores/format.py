f_otimizado = open("./otimizado.txt")
conteudo_otimizado = f_otimizado.read()
valores_otimizado = conteudo_otimizado.split(' ')

f_nao_otimizado = open("./nao_otimizado.txt")
conteudo_nao_otimizado = f_nao_otimizado.read()
valores_nao_otimizado = conteudo_nao_otimizado.split(' ')


tamanhoN = [64, 100, 128, 200, 256, 512, 600, 900, 1024, 2000, 2048, 3000, 4000]

for i, tam in enumerate(tamanhoN):
    print(tam, float(valores_otimizado[i]), float(valores_nao_otimizado[i]))
