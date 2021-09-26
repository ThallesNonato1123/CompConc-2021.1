package java;
/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

// Monitor que implementa a logica do padrao leitores/escritores
class LE {
  private int leit = 0;      // variavel de leitores
  private int escr = 0 ;     // variavel de escritores
  private int writeRequests = 0; // variavel para controle das requisições dos escritores, para sabermos quando todos os escritores forem atendidos

 // Entrada para leitores
 public synchronized void EntraLeitor (int id) throws InterruptedException {
   //while (this.escr > 0) {
     while(escr > 0 || ( leit>0  && writeRequests > 0)) { // se os escritores estiverem escrevendo, ou houver threads lendo mas com  requisição de escritores, as threads leitoras devem esperar até o fim das requisições
        System.out.println ("le.leitorBloqueado("+id+")" + " this.writeRequests:" + this.writeRequests);
        wait();  //bloqueia pela condicao logica da aplicacao 
     }
     leit++;  //registra que ha mais um leitor lendo
     System.out.println ("le.leitorLendo("+id+")");
  
 }
 
 // Saida para leitores
 public synchronized void SaiLeitor (int id) {
    leit--; //registra que um leitor saiu
    notifyAll(); //libera escritor (caso exista escritor bloqueado)
    System.out.println ("le.leitorSaindo("+id+")");
 }
 
 // Entrada para escritores
 public synchronized void EntraEscritor (int id) throws InterruptedException{
     writeRequests++; // Variável para incrementar quando uma thread escreve
     while ((this.leit > 0) || (this.escr > 0)) {
         System.out.println("le.escritorBloqueado("+id+"), entrou na fila de escritores");
        wait();  //bloqueia pela condicao logica da aplicacao 
     }
     writeRequests--; // variavel para decrementar quando a thread para de escrever
     escr++; //registra que ha um escritor escrevendo
     System.out.println ("le.escritorEscrevendo("+id+")");
 }
 
 // Saida para escritores
 public synchronized void SaiEscritor (int id) {
    this.escr--; //registra que o escritor saiu
    notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
    System.out.println ("le.escritorSaindo("+id+")" );
 }
}
  
  
  
  //Aplicacao de exemplo--------------------------------------------------------
  // Leitor
  class Leitor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo
    LE monitor;//objeto monitor para coordenar a lógica de execução das threads
  
    // Construtor
    Leitor (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    // Método executado pela thread
    public void run () {
      double j=777777777.7, i;
      try {
        for (;;) {
          this.monitor.EntraLeitor(this.id);
          for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de leitura
          this.monitor.SaiLeitor(this.id);
          sleep(this.delay); 
        }
      } catch (InterruptedException e) { return; }
    }
  }
  
  //--------------------------------------------------------
  // Escritor
  class Escritor extends Thread {
    int id; //identificador da thread
    int delay; //atraso bobo...
    LE monitor; //objeto monitor para coordenar a lógica de execução das threads
  
    // Construtor
    Escritor (int id, int delayTime, LE m) {
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
  
    // Método executado pela thread
    public void run () {
      double j=777777777.7, i;
      try {
        for (;;) {
          this.monitor.EntraEscritor(this.id); 
          for (i=0; i<100000000; i++) {j=j/2;} //...loop bobo para simbolizar o tempo de escrita
          this.monitor.SaiEscritor(this.id); 
          sleep(this.delay); //atraso bobo...
        }
      } catch (InterruptedException e) { return; }
    }
  }
  
  //--------------------------------------------------------
  // Classe principal
  class LeitorEscritor {
    static final int L = 4;
    static final int E = 3;
  
    public static void main (String[] args) {
      int i;
      LE monitor = new LE();            // Monitor (objeto compartilhado entre leitores e escritores)
      Leitor[] l = new Leitor[L];       // Threads leitores
      Escritor[] e = new Escritor[E];   // Threads escritores
  
      //inicia o log de saida
      System.out.println ("import verificaLE");
      System.out.println ("le = verificaLE.LE()");
      
      for (i=0; i<E; i++) {
         e[i] = new Escritor(i+1, (i+1)*500, monitor);
         e[i].start(); 
      }
      
      for (i=0; i<L; i++) {
         l[i] = new Leitor(i+1, (i+1)*500, monitor);
         l[i].start(); 
      }
    }
  }
  