import numpy as np
import networkx as nx
from scipy.linalg import solve_triangular

def leer_archivo(input_file_path):
    f = open(input_file_path, 'r')
    n = int(f.readline())
    m = int(f.readline())
    W = np.zeros(shape=(n,n))
    for _ in range(m):
        line = f.readline()
        i = int(line.split()[0]) - 1
        j = int(line.split()[1]) - 1
        W[j,i] = 1.0
    f.close()
    return W

def dibujarGrafo(W, print_ejes=True):
    options = {
        'node_color': 'yellow',
        'node_size': 200,
        'width': 3,
        'arrowstyle': '-|>',
        'arrowsize': 10,
        'with_labels' : True
    }
    N = W.shape[0]
    G = nx.DiGraph(W.T)
    # Renombro nodos de 1 a N
    G = nx.relabel_nodes(G, {i:i+1 for i in range(N)})
    if print_ejes:
        print('Ejes: ', [e for e in G.edges])
    nx.draw(G, pos=nx.spring_layout(G), **options)

def descomposicion3_LU(A):
    n = A.shape[0]
    Ac = A.copy()
    for k in range(n-1):
        if Ac[k, k] == 0:
            # Si el elemento diagonal es cero, no realizamos la división y pasamos a la siguiente iteración
            continue
        Ac[k+1:, k] = Ac[k+1:, k] / Ac[k, k]
        Ac[k+1:, k+1:] -= np.outer(Ac[k+1:, k], Ac[k, k+1:])
    L = np.tril(Ac,-1) + np.eye(A.shape[0])
    U = np.triu(Ac)
    return L, U

def calcularRanking(M, p):
    npages = M.shape[0]
    rnk = np.arange(0, npages) # ind{k] = i, la pagina k tienen el iesimo orden en la lista.
    scr = np.zeros(npages) # scr[k] = alpha, la pagina k tiene un score de alpha
    # CÓDIGO
    I = np.eye(npages)
    D = I.copy()
    for j in range(npages):
        contador = 0
        for i in range(npages):
            contador += M[i][j]
        if contador != 0:
            D[j][j] = 1 / contador
        else:
            D[j][j] = 0  # Maneja el caso en que no hay enlaces salientes
    e = np.ones(npages)
    A = I - p*np.dot(M,D)

    # Verificar si hay valores inf o NaN en la matriz A
    if np.isnan(A).any() or np.isinf(A).any():
        print("La matriz A contiene valores inf o NaN.")
        return None, None

    L, U = descomposicion3_LU(A)

    # Verificar si hay valores inf o NaN en las matrices L y U
    if np.isnan(L).any() or np.isinf(L).any() or np.isnan(U).any() or np.isinf(U).any():
        print("Las matrices L o U contienen valores inf o NaN.")
        return None, None

    y = solve_triangular(L,e, lower = True)
    scr = solve_triangular(U,y,lower=False)

    # Verificar si hay valores inf o NaN en el resultado
    if np.isnan(scr).any() or np.isinf(scr).any():
        print("El resultado contiene valores inf o NaN.")
        return None, None

    # Calcula la normalización L1 de scr
    scr_normalized = scr / np.linalg.norm(scr, 1) 

    # Normaliza aún más los valores normalizados dividiendo por la suma de todos los elementos
    scr_final = scr_normalized / np.sum(scr_normalized) 

    # Ordena los índices de scr_final en orden descendente
    ord = np.argsort(scr_final)[::-1]
    for i in range(npages):
        rnk[i] = ord[i]
    return rnk, scr_final

def obtenerMaximoRankingScore(M, p):
    output = -np.inf
    # Calculo el ranking y los scores
    rnk, scr = calcularRanking(M, p)
    output = np.max(scr)
    return output

