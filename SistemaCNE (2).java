//Trabajo en equipo realizado para la materia Algoritmos y Estructura de Datos 2
//Integrantes: Dante Waisman, Alvaro Petriz Otaño, Tatiana Grinspan y Santiago Dunayevich
package aed;
public class SistemaCNE {
    
    private String[] partidos;
    private String[] distritos;
    private int[] diputadosPorDistrito;
    private int[] ultimasMesasDistritos;
    private int[][][] votosDiputadosHeap; //Acá van los heaps de cada distrito con sus votos de diputados
    private int[][] votosDiputadosPartido; //Acá van los arreglos con los votos ordernados por partido de diputados
    private int[][] bancasDiputados;
    private int[] votosPresidentePartido; //Acá van los arreglos con los votos ordernados por partido de presidente
    private int[] votosTotalesDiputados;
    private int votosTotalesPresidente;
    private int primeroPresidente;
    private int segundoPresidente;
    private boolean[] estaRoto;

    public class VotosPartido{
        private int presidente;
        private int diputados;
        VotosPartido(int presidente, int diputados){this.presidente = presidente; this.diputados = diputados;}
        public int votosPresidente(){return presidente;}
        public int votosDiputados(){return diputados;}
    }

    public SistemaCNE(String[] nombresDistritos, int[] diputadosPorDistrito, String[] nombresPartidos, int[] ultimasMesasDistritos) {
        this.distritos = nombresDistritos;
        this.partidos = nombresPartidos;
        this.diputadosPorDistrito = diputadosPorDistrito;
        this.ultimasMesasDistritos = ultimasMesasDistritos;
        this.votosDiputadosPartido = new int[this.distritos.length][this.partidos.length];
        this.votosDiputadosHeap = new int[this.distritos.length][this.partidos.length-1][2];
        this.votosPresidentePartido = new int[this.partidos.length];
        this.votosTotalesDiputados = new int[this.distritos.length];
        this.votosTotalesPresidente = 0;
        this.bancasDiputados = new int[this.distritos.length][this.partidos.length - 1];
        this.estaRoto = new boolean[this.distritos.length];
        this.primeroPresidente = 0;
        this.segundoPresidente = 1;
    }

    public String nombrePartido(int idPartido) {
        return this.partidos[idPartido]; //O(1) devuelvo una variable
    }

    public String nombreDistrito(int idDistrito) {
        return this.distritos[idDistrito]; //O(1) devuelvo una variable
    }

    public int diputadosEnDisputa(int idDistrito) {
       return this.diputadosPorDistrito[idDistrito]; //O(1) devuelvo una variable
    }

    public String distritoDeMesa(int idMesa) { //Devuelve el nombre del distrito de una mesa
        return this.distritos[busquedaBinaria(this.ultimasMesasDistritos, idMesa)]; //O(log(D)) por ser busqueda binaria en un arreglo de tamaño D 
    }

    public int IdDistritoDeMesa(int idMesa) { //Devuelve el ID del distrito de una mesa
        return busquedaBinaria(this.ultimasMesasDistritos, idMesa); //O(log(D)) por ser busqueda binaria en un arreglo de tamaño D 
    }

    private int busquedaBinaria(int arr[], int x) 
    {
        int izq = 0; 
        int der = arr.length -1;
        while (izq <= der) { //O(log(n)) Este while se ejecuta log(n) veces ya que en cada ciclo la diferencia entre izq y der se reduce a la mitad
            int mid = (izq + der) / 2; //O(1)
            if (izq == der) { //O(1)
                return izq; //O(1)
            } else if (arr[mid] > x) { //O(1)
                der = mid; //O(1)
            } else { //O(1)
              izq = mid + 1; //O(1)
            }  
        }

        return izq; //O(1)
    }

    public void registrarMesa(int idMesa, VotosPartido[] actaMesa) {
        int idDistrito = IdDistritoDeMesa(idMesa); //busco el id del distrito al que pertenece la mesa. O(log(D))

        for(int i = 0; i < this.partidos.length; i++){ // O(P) ya que se ejecuta el ciclo P veces. Recorro votosDistritoPartido[idDistrito] y le sumo los votos del acta a cada partido
            this.votosDiputadosPartido[idDistrito][i] += actaMesa[i].diputados; //O(1)
            this.votosPresidentePartido[i] += actaMesa[i].presidente; //O(1)
            this.votosTotalesDiputados[idDistrito] += actaMesa[i].diputados; //O(1)
            this.votosTotalesPresidente += actaMesa[i].presidente; //O(1)

            if(votosPresidentePartido[i] >= votosPresidentePartido[primeroPresidente] && i != primeroPresidente){ //O(1)
                segundoPresidente = primeroPresidente; //O(1)
                primeroPresidente = i; //O(1)
            }
            else if(votosPresidentePartido[i] >= votosPresidentePartido[segundoPresidente] && i != primeroPresidente){ //O(1)
                    segundoPresidente = i; //O(1)
            }
        }
        
        this.votosDiputadosHeap[idDistrito] = Heap.diputadosArrayAHeap(this.votosDiputadosPartido[idDistrito],this.votosTotalesDiputados[idDistrito]); //O(P) por ser algoritmo de Floyd 
        this.estaRoto[idDistrito] = false; //O(1)
        
    }

    public int votosPresidenciales(int idPartido) { 
        return this.votosPresidentePartido[idPartido]; //O(1)
    }

    public int votosDiputados(int idPartido, int idDistrito) { 
        return this.votosDiputadosPartido[idDistrito][idPartido]; //O(1)
    }

    public int[] resultadosDiputados(int idDistrito){
        int bancas = this.diputadosPorDistrito[idDistrito]; //Bancas a asignar en distrito O(1)

        if(!estaRoto[idDistrito]){ //No se modificó nunca el heap de diputados. O(1)
            for(int i = 0; i < bancas; i++){ //O(Dd * log(P)), ya que el ciclo se ejecuta Dd veces, y cada ejecución tiene commplejidad logarítmica.
                int partidoMax = this.votosDiputadosHeap[idDistrito][0][1]; //Tomo maximo de heap (O(1))
                this.bancasDiputados[idDistrito][partidoMax]+=1; //Sumo una banca en result O(1)
                this.votosDiputadosHeap[idDistrito][0][0] = this.votosDiputadosHeap[idDistrito][0][0] * this.bancasDiputados[idDistrito][partidoMax] / (this.bancasDiputados[idDistrito][partidoMax] + 1); //Divido el máximo =(1)
                this.votosDiputadosHeap[idDistrito] = Heap.rayoHeapificador(this.votosDiputadosHeap[idDistrito], this.partidos.length -1, 0); //Swapeo la raiz del heap donde corresponda (O(log(P))) 
            }
            this.estaRoto[idDistrito] = true; //O(1)
        }        
        return this.bancasDiputados[idDistrito]; //O(1)
    }

    public boolean hayBallotage(){

            if(((float)votosPresidentePartido[primeroPresidente]/this.votosTotalesPresidente)>0.45){ //O(1)
                return false; //O(1)
            }
            else{ //O(1)
                if(((float)votosPresidentePartido[primeroPresidente]/this.votosTotalesPresidente)>0.4 && ((float)(votosPresidentePartido[primeroPresidente]-votosPresidentePartido[segundoPresidente])/this.votosTotalesPresidente>0.1)){ //O(1)
                    return false; //O(1)
                }
                else{ //O(1)
                    return true; //O(1)
                }

            }       

     }
}
