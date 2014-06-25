#include "AlgoritmoAprendizaje.h"

/*Constructor por defecto*/	
AlgoritmoAprendizaje::AlgoritmoAprendizaje() {

}    
AlgoritmoAprendizaje::AlgoritmoAprendizaje( RedNeuronal *red, 
                                            int numeroEjemplosEntrenamiento, 
                                            int numeroEjemplosPrueba, 
                                            int numComponentesEntrada,
			 		                        int numComponentesSalida,
			 		                        int tamComponente,
					                        char *inTrainningFile,
					                        char *outTrainningFile,
					                        char *inTestFile,
					                        char *outTestFile
					                       )
{

    this->red = red;
    this->numeroEjemplosEntrenamiento = numeroEjemplosEntrenamiento;
    this->numeroEjemplosPrueba = numeroEjemplosPrueba;
    this->numComponentesEntrada = numComponentesEntrada;
    this->numComponentesSalida = numComponentesSalida;
    this->tamComponente = tamComponente;
    this->inTrainningFile = inTrainningFile;
    this->outTrainningFile = outTrainningFile;
    this->inTestFile = inTestFile;
    this->outTestFile = outTestFile;
}    
/*destructor para liberar memoria*/
AlgoritmoAprendizaje::~AlgoritmoAprendizaje(){
     /*Liberar memoria ejemplos*/
     for( int i= 0; i < numeroEjemplosEntrenamiento; i++){
        
         delete[] ejemplosEntrenamiento[i];
     }
        
     delete[] ejemplosEntrenamiento;

     for( int i= 0; i < numeroEjemplosPrueba; i++){
        
         delete[] ejemplosPrueba[i];
     }
        
     delete[] ejemplosPrueba;
}  	

void AlgoritmoAprendizaje::obtenerEjemplosEntrenamiento() {
    // Asignar memoria a los ejemplos de entrenamiento
    ejemplosEntrenamiento = new double*[numeroEjemplosEntrenamiento];
    for(int i = 0; i < numeroEjemplosEntrenamiento; i++)
       ejemplosEntrenamiento[i] = new double[numComponentesEntrada+numComponentesSalida];   

    /*Abrir archivo y extraer ejemplos*/
    FILE *inTrainningFILE= fopen( inTrainningFile, "rb");
    FILE *outTrainningFILE= fopen( outTrainningFile, "rb");
    
    char c[100000]; 
    char * pch;
    int j;
    /*obtener todos lo arreglos que representan los ejemplos*/
    for( int i= 0; i < numeroEjemplosEntrenamiento; i++) {
        j = 0;
        fgets( c, 100000, inTrainningFILE);
        pch = strtok (c, " ");
        while (pch != NULL)  {            
            ejemplosEntrenamiento[i][j]= strtod( pch, NULL);
            //printf("%lf\n", ejemplosEntrenamiento[i][j]);
            pch = strtok (NULL, " ");
            j++;
        }
        fgets( c, 50, outTrainningFILE);
        pch = strtok (c, " ");
        while (pch != NULL)  {
            ejemplosEntrenamiento[i][j]= strtod( pch, NULL);
            //printf("%lf\n", ejemplosEntrenamiento[i][j]);
            pch = strtok (NULL, " ");
            j++;
        }                
    }

    fclose(inTrainningFILE);/*cerrar el archivo*/ 
    fclose(outTrainningFILE);/*cerrar el archivo*/ 
}
	      
void AlgoritmoAprendizaje::realizarEntrenamiento() {

     red->inicializarMatricesDePesos();
     
     /*variables auxiliares para manejar el ejemplo de entrada y su respectiva salida*/
     double *entrada = new double[numComponentesEntrada];
     double *salida = new double[numComponentesSalida];

     /*ciclo de entrenamiento:*/
     double error= 100.0;/*Iniciar el error en un numero grande de manera
		           para que pueda entrar al ciclo*/
     double alfa= 0.75;  
     int indice = 0;  
     srand(5);/*inicializar los pesos aleatorios con un numero indeterminado*/      
	   
     while( error > 0.00001) {         
         
         indice = 0;
         error = 0.0;
         
         for( int i = 0; i < numeroEjemplosEntrenamiento; i++) {             
            // Preparar el indice a utilizar
            indice = rand()%numeroEjemplosEntrenamiento;
            //formar arreglo de entrada
            for( int j= 0; j < numComponentesEntrada; j++){
            
                entrada[j]= ejemplosEntrenamiento[indice][j];		    		
            }
    		//formar arreglo de salida
    		for( int j=numComponentesEntrada; j < numComponentesEntrada+numComponentesSalida; j++){
    
    		    salida[j-numComponentesEntrada]= ejemplosEntrenamiento[indice][j];	    		    
    		}

            /*printf( "%d", indice);
            printf(" ");*/
    		/*calcular los estados de activacion o reaccion de las neuronas ante el estimulo de entrada*/
    		red->calculoEstadosActivacion( entrada, salida);		
    		/*Calcula de el error que se produjo entre la reaccion que hubo y la esperada*/
            red->calculoErrorSalida();
    		/*Procesamiento hacia atraz*/
    		red->calculoRetroPropagacion();
    		/*ajustar la matriz de pesos*/
    		red->calculoNuevaMatriz();
    		/*Calcular el error total cometido*/
            error= error + red->getError();
         }

	     /*disminuir progresivamente la tasa de aprendizaje*/
         if(alfa > 0.55) {
    		alfa= alfa - 0.001;
    		red->setAlfa(alfa);
    	 }	       	 	    
     }

	 // Eliminar variables temporales
	 delete entrada;
	 delete salida;
}

void AlgoritmoAprendizaje::obtenerEjemplosPrueba() {

    ejemplosPrueba = new double*[numeroEjemplosPrueba];

    for(int i = 0; i < numeroEjemplosPrueba; i++)
        ejemplosPrueba[i] = new double[numComponentesEntrada+numComponentesSalida];     
     
    /*Abrir archivo y extraer ejemplos*/
    FILE *inTestFILE= fopen( inTestFile, "rb");
    FILE *outTestFILE= fopen( outTestFile, "rb");
    
    char c[100000]; 
    char * pch;
    int j;
    /*obtener todos lo arreglos que representan los ejemplos*/
    for( int i= 0; i< numeroEjemplosPrueba; i++){
        j = 0;
        fgets( c, 100000, inTestFILE);
        pch = strtok (c, " ");
        while (pch != NULL)  {
            ejemplosPrueba[i][j] = strtod( pch, NULL);
            pch = strtok (NULL, " ");
            j++;
        }
        fgets( c, 50, outTestFILE);
        pch = strtok (c, " ");
        while (pch != NULL)  {
            ejemplosPrueba[i][j] = strtod( pch, NULL);
            pch = strtok (NULL, " ");
            j++;
        }                
    }      		
     
    fclose(inTestFILE);/*cerrar el archivo*/ 
    fclose(outTestFILE);/*cerrar el archivo*/   
}

float AlgoritmoAprendizaje::realizarPrueba(){

     /*variables auxiliares para manejar el ejemplo de entrada y su respectiva salida*/
     double *entrada = new double[numComponentesEntrada];
     double *salida = new double[numComponentesSalida];

     /*ciclo de prueba*/
     int correctas= 0;
     float porcentajeCorrectas= 0.0;   
     
     for( int i= 0; i < numeroEjemplosPrueba; i++){ 
	
	//formar arreglo de entrada
	for( int j= 0; j < numComponentesEntrada; j++){
	
	    entrada[j]= ejemplosPrueba[i][j];		
	}
	
	//formar arreglo de salida
	for( int j=numComponentesEntrada; j < numComponentesEntrada+numComponentesSalida; j++){

	    salida[j-numComponentesEntrada]= ejemplosPrueba[i][j];
	}	

	/*calcular los estados de activacion o reaccion de las neuronas ante el estimulo de entrada*/
	red->calculoEstadosActivacion( entrada, salida);
	/*Calcula de el error que se produjo entre la reaccion que hubo y la esperada*/
	red->calculoErrorSalida();	
	/*Calcular el error total cometido*/
	if(red->getErrorPrueba() < 0.12005)
	   correctas++;	
     }

     porcentajeCorrectas= (float)correctas/(float)numeroEjemplosPrueba;

	 // Eliminar variables temporales
	 delete entrada;
	 delete salida;

     return porcentajeCorrectas;
}

float AlgoritmoAprendizaje::ejecutarAprendizaje(){

     float desempeno;

     obtenerEjemplosEntrenamiento();
     realizarEntrenamiento();
     obtenerEjemplosPrueba();
     desempeno= realizarPrueba();

     return desempeno;
}   

