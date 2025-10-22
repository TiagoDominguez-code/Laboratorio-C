# Laboratorio-C

Maps config

1- Tamaño de mapas 10x10
2- Estados Estaticos[Corte, barro, pav, pozo]
    Corte: obstaculo 
    barro: costoso 
    pozo: costoso
    pav: mejor opcion
3- Usuario cree su mapa
4- 3 config de mapas




Algoritmo A/*


se basa en F(n)= g(n)+ h(n)
donde
n(celda actual)
g(n)  es lo que nos costo llegar desde el inicio hasta la celda donde estamos
h(n)  es una estimacion de lo que nos va a costar llegar desde donde estamos hasta el destino
f(n)  es lo que se analiza para saber que camino cuesta menos

para estimar h(n) usamos la distancia manhattan (solo calles verticales y horizontales)
h(n) = |nx-Fx| + |ny-Fy|            donde F es el destino; nx es la celda actual en x; ny celda actual en y;  

Usa 
1 lista abierta (coleccion de nodos pendientes de ser evaluados)
-pos : (x,y)
-g(n) : entero
-h(n) : entero
-f(n) : entero
nodopadre: nodo desde el cual llego

esta lsita lo que hace es sacar de la lista al nodo con el f(n) mas bajo ya que es el candidato mas prometedor

1 Lista cerrada (nodos ya evaluados, vienen de la lista abierta porque son nodos que ya no son considerados en el algoritmo)

-pos(x,y)
