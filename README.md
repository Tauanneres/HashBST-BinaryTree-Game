Detective Quest - Nível Mestre
Objetivo

Implementar um jogo em C onde o jogador explora uma mansão, coleta pistas e acusa um suspeito. O sistema integra:

Árvore binária para mapa da mansão

BST para armazenamento de pistas

Tabela hash para associar pistas a suspeitos

Estruturas de Dados Utilizadas

Árvore Binária (Salas da Mansão)
Cada nó representa um cômodo, contendo nome e pista.

BST (Pistas Coletadas)
Permite organizar as pistas coletadas em ordem alfabética.

Tabela Hash (Pista → Suspeito)
Permite buscar rapidamente qual suspeito está relacionado a cada pista.

Funcionalidades

Exploração interativa da mansão (esquerda/direita/sair)

Coleta automática de pistas ao visitar cada sala

Exibição das pistas coletadas em ordem alfabética

Julgamento final baseado nas pistas coletadas
