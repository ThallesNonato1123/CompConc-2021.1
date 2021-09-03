import java.util.Random;
class tarefa6 extends Thread {
    private int id;
    int[] vetor;
    private final int inicio;
    private final int fim;
    private static int nthreads;
    private static int tam;
    private static int tamBloco;
    private static int contPar = 0;

    public tarefa6(int id, int v[]) {
        this.id = id;
        this.vetor = v;
        inicio = id * tamBloco;
        if (id == nthreads - 1) {
            fim = tam;
        } else {
            fim = inicio + tamBloco;
        }
    }
    
    public void run() {
        for (int i = inicio; i < fim; i++) {
            if (vetor[i] % 2 == 0){
                System.out.print(vetor[i] + " "); contPar++; }
        }
    }

    public static void main(String[] args) {
        

        Random rand = new Random();

        tam = Integer.parseInt(args[0]);
        nthreads = Integer.parseInt(args[1]);

        Thread[] threads = new Thread[nthreads];
        int[] vetor = new int[tam];

        for (int i = 0; i < tam; i++) {
            vetor[i] = i;
        }

        tamBloco = tam / nthreads;

        for (int i = 0; i < nthreads; i++) {
            threads[i] = new tarefa6(i , vetor);
        }

        for (Thread thread : threads) {
            thread.start();
        }

        for (Thread thread : threads) {
            try {
                thread.join();
            } catch (InterruptedException e) {
                return;
            }
        }
        System.out.println();
        System.out.println(contPar);
    }
}
