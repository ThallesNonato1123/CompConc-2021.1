/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Codigo: Leitores e escritores usando monitores em Java */
/* -------------------------------------------------------------------*/

// Monitor que implementa a logica do padrao leitores/escritores
class dadoGlobal{
  public static int dado = 0;
}

class Monitor {
  private int leit, escr;  
  
  // Construtor
  Monitor() { 
     this.leit = 0; //leitores lendo (0 ou mais)
     this.escr = 0; //escritor escrevendo (0 ou 1)
  } 
  
  // Entrada para leitores
  public synchronized void EntraLeitor (int id, String tipoThread) {
    try { 
      while (this.escr > 0) {
      //if (this.escr > 0) {
         System.out.println ( tipoThread + ".leitorBloqueado("+id+")" +  " valor atual:" + dadoGlobal.dado);
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      
       this.leit++;  //registra que ha mais um leitor lendo
      System.out.println (tipoThread + ".leitorLendo("+id+")" + " valor atual: " + dadoGlobal.dado);

  } catch (InterruptedException e) { }
  }
  
  // Saida para leitores
  public synchronized void SaiLeitor (int id, String tipoThread) {
     this.leit--; //registra que um leitor saiu
     if (this.leit == 0) 
           this.notify(); //libera escritor (caso exista escritor bloqueado)
     System.out.println (tipoThread + ".leitorSaindo("+id+")" + " valor atual: " + dadoGlobal.dado);
  }
  
  // Entrada para escritores
  public synchronized void EntraEscritor (int id, String tipoThread) {
    try { 
      //while ((this.leit > 0) || (this.escr > 0)) {
      while ((this.leit > 0) || (this.escr > 0)) {
         System.out.println (tipoThread + ".escritorBloqueado("+id+")" +  " valor atual: " + dadoGlobal.dado);
         wait();  //bloqueia pela condicao logica da aplicacao 
      }
      this.escr++; //registra que ha um escritor escrevendo
      System.out.println (tipoThread + ".escritorEscrevendo("+id+")" + " contéudo anterior:" + dadoGlobal.dado);
    } catch (InterruptedException e) { }
  }
  
  // Saida para escritores
  public synchronized void SaiEscritor (int id, String tipoThread) {
     this.escr--; //registra que o escritor saiu
     notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
     System.out.println (tipoThread + ".escritorSaindo("+id+") com novo dado após escrita: " + dadoGlobal.dado);
  }
}



//Aplicacao de exemplo--------------------------------------------------------
// Leitor
class Leitor extends Thread {
  int id; //identificador da thread
  int delay; //atraso bobo
  int dado;
  Monitor monitor;//objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Leitor (int id, int delayTime, Monitor m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {
    int numPrimos = 0;
    try {
      for (;;) {
        this.monitor.EntraLeitor(this.id, "TL");
        
        for (int i=1; i<=dadoGlobal.dado; i++) {
          if(dadoGlobal.dado % i == 0) numPrimos++;
        }
         
        if(numPrimos == 2) System.out.println("valor atual: " + dadoGlobal.dado + " (Primo)");
        numPrimos = 0 ;
        this.monitor.SaiLeitor(this.id, "TL");
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
  Monitor monitor; //objeto monitor para coordenar a lógica de execução das threads

  // Construtor
  Escritor (int id, int delayTime, Monitor m) {
    this.id = id;
    this.delay = delayTime;
    this.monitor = m;
  }

  // Método executado pela thread
  public void run () {

    try {
      for (;;) {
        this.monitor.EntraEscritor(this.id, "TE"); 
        dadoGlobal.dado = id; //...loop bobo para simbolizar o tempo de escrita
        this.monitor.SaiEscritor(this.id,"TE"); 
        sleep(this.delay); //atraso bobo...
      }
    } catch (InterruptedException e) { return; }
  }
}

class LeitEsc extends Thread{
  int id;
  int delay;
  Monitor monitor;

    LeitEsc(int id,int delayTime, Monitor m){
      this.id = id;
      this.delay = delayTime;
      this.monitor = m;
    }
    
    public void run(){
      try{
        for(;;){
          this.monitor.EntraLeitor(this.id, "TLE");
          if(dadoGlobal.dado % 2 == 0) System.out.println("TLE: "+ dadoGlobal.dado + " é par"); else{ System.out.println("TLE" + dadoGlobal.dado + " é impar");}
          this.monitor.SaiLeitor(this.id,"TLE");
          sleep(this.delay);
          this.monitor.EntraEscritor(this.id,"TLE");
          System.out.println("Valor  será dobrado: " + dadoGlobal.dado);
          dadoGlobal.dado*=2;
          System.out.println("Valor dobrado: " + dadoGlobal.dado);
          this.monitor.SaiEscritor(this.id,"TLE");
          sleep(this.delay);
        }
      }catch(InterruptedException e ){ return;}
    }
}
//--------------------------------------------------------
// Classe principal
class principal {
  static final int L = 3;
  static final int E = 2;
  static final int LE = 3;

  public static void main (String[] args) {
    int i;
    Monitor monitor = new Monitor();            // Monitor (objeto compartilhado entre leitores e escritores)
    Leitor[] l    = new Leitor[L];       // Threads leitores
    Escritor[] e = new Escritor[E];   // Threads escritores
    LeitEsc [] le = new LeitEsc[LE];

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
   
   for(i = 0 ; i<LE; i++){
     le[i] = new LeitEsc(i+1, (i+1)*500, monitor);
     le[i].start();
   }
  }
}

