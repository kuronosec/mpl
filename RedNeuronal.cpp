#include "RedNeuronal.h"    

RedNeuronal::RedNeuronal(){    
}   

RedNeuronal::RedNeuronal(double alfa, double umbral, double error, int numeroCapas, int neuronasCapa[]) {

	this->alfa= alfa;
	this->umbral= umbral;
	this->error= error;
	this->numeroCapas= numeroCapas;
	this->neuronasCapa= neuronasCapa;

	/*Crear en memoria matrices de pesos*/
	matrizPesos= new double**[numeroCapas-1];

	for( int i= 0; i < numeroCapas-1; i++) {
	     
	     matrizPesos[i]= new double*[neuronasCapa[i]+1];
	     for( int j= 0; j< neuronasCapa[i]+1; j++){
	     
		  matrizPesos[i][j]= new double[neuronasCapa[i+1]];
	     }       
	}
	/*Crear en memoria vectores resultado*/        
	net= new double*[numeroCapas-1];

	for( int i= 0; i < numeroCapas-1; i++){

	     net[i]= new double[neuronasCapa[i+1]];
	}

	/*Crear en memoria vectores de estados de activacion*/
	O= new double*[numeroCapas];

	for( int i= 0; i < numeroCapas; i++){

	     O[i]= new double[neuronasCapa[i]];
	} 

	/*Crear en memoria vectores de error*/
	delta= new double*[numeroCapas-1];

	for( int i= 0; i < numeroCapas-1; i++){

	     delta[i]= new double[neuronasCapa[i+1]];
	}              
}

RedNeuronal::RedNeuronal(int numeroCapas, int neuronasCapa[], char *file) {	
	this->numeroCapas= numeroCapas;
	this->neuronasCapa= neuronasCapa;
	this->file = file;

	/*Crear en memoria matrices de pesos*/
	matrizPesos= new double**[numeroCapas-1];

	for( int i= 0; i < numeroCapas-1; i++) {
	     
	     matrizPesos[i]= new double*[neuronasCapa[i]+1];
	     for( int j= 0; j< neuronasCapa[i]+1; j++){
	     
		  matrizPesos[i][j]= new double[neuronasCapa[i+1]];
	     }       
	}
	/*Crear en memoria vectores resultado*/        
	net= new double*[numeroCapas-1];

	for( int i= 0; i < numeroCapas-1; i++){

	     net[i]= new double[neuronasCapa[i+1]];
	}

	/*Crear en memoria vectores de estados de activacion*/
	O= new double*[numeroCapas];

	for( int i= 0; i < numeroCapas; i++){

	     O[i]= new double[neuronasCapa[i]];
	} 

	/*Crear en memoria vectores de error*/
	delta= new double*[numeroCapas-1];

	for( int i= 0; i < numeroCapas-1; i++){

	     delta[i]= new double[neuronasCapa[i+1]];
	}

	/*Abrir archivo y extraer pesos*/
    FILE *inWeightsFile = fopen( this->file, "r");    
    char c[100000]; 
    char * pch;

	if( inWeightsFile != NULL) {
		for( int i= 0; i< numeroCapas-1; i++) {
			for( int j= 0; j< neuronasCapa[i]+1; j++) {
				fgets( c, 100000, inWeightsFile);
				pch = strtok (c, " ");
				for( int k= 0; k < neuronasCapa[i+1]; k++) {
					matrizPesos[i][j][k]= strtod( pch, NULL); 
					pch = strtok (NULL, " ");
				}
			}
		}
		
		/*cerrar el archivo*/
		fclose(inWeightsFile);
	} else {
		printf("Archivo no Encontrado");	
	}
}

/*destructor para liberar memoria*/
RedNeuronal::~RedNeuronal() {

	/*Liberar la memoria de las matrices de pesos*/
	for( int i= 0; i < numeroCapas-1; i++){             
	     
	     for( int j= 0; j< neuronasCapa[i]+1; j++){
	     
		  delete[] matrizPesos[i][j];
	     }
	     
	     delete[] matrizPesos[i];      
	}

	delete[] matrizPesos;

	/*Liberar memoria vectores resultado*/       
	for( int i= 0; i < numeroCapas-1; i++){

	     delete[] net[i];
	}

	delete net;

	/*Liberar memoria vectores de estados de activacion*/
	for( int i= 0; i < numeroCapas; i++){

	     delete[] O[i];
	} 

	delete[] O;
	/*Liberar memoria vectores de error*/
	for( int i= 0; i < numeroCapas-1; i++){

	     delete[] delta[i];
	}

	delete[] delta;
}	

//asignar alfa para su aumento progresivo
void RedNeuronal::setAlfa(double alfa){

	this->alfa= alfa;
}

/*En este método se inicializan las matrices de pesos con númros aleatorios*/
void RedNeuronal::inicializarMatricesDePesos() {	 
        srand(37);/*inicializar los pesos aleatorios con un numero indeterminado*/
	for( int i= 0; i< numeroCapas-1; i++){

		for( int j= 0; j< neuronasCapa[i]+1; j++){

		   for( int k= 0; k < neuronasCapa[i+1]; k++){
		   
			matrizPesos[i][j][k]= -1 + 2*((double)rand()/RAND_MAX); 
            //printf("%lf\n", matrizPesos[i][j][k]);                       
		   }
		}
	}	  
	/*fin del procedimiento*/  
}	

void RedNeuronal::calculoEstadosActivacion(double entrada[], double salida[]) {

	/* Asignar la entrada a la primera capa*/
	for( int i= 0; i< neuronasCapa[0]; i++) {
	   O[0][i]= entrada[i];           
	}	

	this->out= salida;//se asigna el vector de salida        

	/* Se calcula el estado de activación para todas las capas*/
	for( int i= 0; i< numeroCapas-1; i++) {	  
	  
		for( int j= 0; j< neuronasCapa[i+1]; j++) {
		  	
		     // Apply bias		  	 
		     net[i][j]= matrizPesos[i][0][j];		     
		     for( int k= 0; k< neuronasCapa[i]; k++) {
			      net[i][j]= net[i][j] + matrizPesos[i][k+1][j]*O[i][k];                  					
		     }    
			 //printf("net: %lf  \n", net[i][j]);	               	
		     O[i+1][j]= (1/(1 + exp(-1*net[i][j])));/*funcion sigmoidal*/
		     //printf("o: %lf  \n", O[i+1][j]);	 
		}		
	}

	/*fin del metodo*/	
} 

double* RedNeuronal::calculoEstadosActivacion(double entrada[]) {

	double *salida = new double[neuronasCapa[numeroCapas-1]];
	/* Asignar la entrada a la primera capa*/
	for( int i= 0; i< neuronasCapa[0]; i++) {
	   O[0][i]= entrada[i];           
	}        

	/* Se calcula el estado de activación para todas las capas*/
	for( int i= 0; i< numeroCapas-1; i++) {	  
		for( int j= 0; j< neuronasCapa[i+1]; j++) {		  	
		     // Apply bias		  	 
		     net[i][j]= matrizPesos[i][0][j];		     
		     for( int k= 0; k< neuronasCapa[i]; k++) {
			      net[i][j]= net[i][j] + matrizPesos[i][k+1][j]*O[i][k];                  					
		     }    
			 //printf("net: %lf  \n", net[i][j]);	               	
		     O[i+1][j]= (1/(1 + exp(-1*net[i][j])));/*funcion sigmoidal*/
		     //printf("o: %lf  \n", O[i+1][j]);	 
		}		
	}

	for( int i= 0; i < neuronasCapa[numeroCapas-1]; i++) {   
		salida[i] = O[numeroCapas-1][i]; 
		//printf("%f\n", salida[i]);
	}

	return salida;
	/*fin del metodo*/	
}

void RedNeuronal::calculoErrorSalida() {

    /*vector deltak correspondiente a la salida*/
    for( int i= 0; i < neuronasCapa[numeroCapas-1]; i++) {
    
       delta[numeroCapas-2][i]= (out[i] - O[numeroCapas-1][i])*((O[numeroCapas-1][i]*(1- O[numeroCapas-1][i]))+0.01);
       /*printf("%lf", out[i]);
       printf(" ");
       printf("%lf", O[numeroCapas-1][i]); 
       printf(" ");*/ 
    } 
    //printf("\n");     
    /*fin del método*/
}

void RedNeuronal::calculoRetroPropagacion() {
	
	double aux= 0; /*variables temporal para la suma*/
	   
	for(int i= numeroCapas-2; i > 0; i--){
	       
	    for( int j= 0; j < neuronasCapa[i]; j++){
	    	    
		aux= 0.0;
		
		for( int k= 0; k < neuronasCapa[i+1]; k++){
		
		        aux= aux + delta[i][k]*matrizPesos[i][j+1][k];					        
		}		

		delta[i-1][j]= aux*O[i][j]*(1-O[i][j]);			
	    }                 
	}
		    
	/*fin del procedimiento*/
}

void RedNeuronal::calculoNuevaMatriz() {

    /*Calculo de las nuevas matices de pesos*/	    
    for( int i= 0; i < numeroCapas-1; i++){     
        // Calculate new weiths values to bias
        for( int k= 0; k < neuronasCapa[i+1]; k++){    		
    		    matrizPesos[i][0][k]= matrizPesos[i][0][k] + alfa*delta[i][k];                    
   		}
   		// Calculate new weiths values
	    for( int j= 0; j < neuronasCapa[i]; j++) {	    
    		for( int k= 0; k < neuronasCapa[i+1]; k++){    		
    		    matrizPesos[i][j+1][k]= matrizPesos[i][j+1][k] + alfa*delta[i][k]*O[i][j];                    
    		}
	    }    
   }
   /*fin del procedimiento*/
 }

 double RedNeuronal::getError(){
	 
   double aux= 0.0;/*variable auxiliar para acumular la suma*/
   
   for( int i= 0; i < neuronasCapa[numeroCapas-1]; i++){
   
      aux= aux + pow(delta[numeroCapas-2][i], 2.0);/*regresion lineal*/      
   }   
   error= 0.5*aux;   
   return error;	   
   /*fin del procedimiento*/	
}

double RedNeuronal::getErrorPrueba() {
	 
   double aux= 0.0;/*variable auxiliar para acumular la suma*/
   
   for( int i= 0; i < neuronasCapa[numeroCapas-1]; i++){
   
      aux= aux + pow(out[i] - O[numeroCapas-1][i], 2.0);/*regresion lineal*/      
   }   
   error= 0.5*aux; 
   //printf( "%lf\n", error);   
   return error;	   
   /*fin del procedimiento*/	
}

void RedNeuronal::saveWeights(char *file) {
    /*Abrir archivo y guardar pesos*/
    FILE *outWeightsFile = fopen( file, "w");    
    char line[100000];
    char weight[50];    

	for( int i= 0; i< numeroCapas-1; i++) {
		for( int j= 0; j< neuronasCapa[i]+1; j++) {	
            sprintf( line, "%s", "");		
			for( int k= 0; k < neuronasCapa[i+1]; k++) {
                if( k == neuronasCapa[i+1]-1)		   
				    sprintf( weight, "%f\n", matrizPesos[i][j][k]);
                else
                    sprintf( weight, "%f ", matrizPesos[i][j][k]);
                strcat( line, weight );
			}
            fputs( line, outWeightsFile );			
		}
	}	
	/*cerrar el archivo*/
    fclose(outWeightsFile); 
}
	
 
RedNeuronalTest::RedNeuronalTest(int entrada, int oculto, int salida, char *file_entrada, char *file_salida) {	
	this->file = file;
	this->entrada = entrada;
	this->oculto = oculto;
	this->salida = salida;

	/*Crear en memoria matrices de pesos*/
	matriz_entrada = new double*[entrada];
	matriz_salida = new double*[oculto];

	for(int i = 0; i < entrada; i++) {
		matriz_entrada[i] = new double[oculto];
	}

	for(int i = 0; i < oculto; i++) {
		matriz_salida[i] = new double[salida];
	}
	
	/*Abrir archivo y extraer pesos*/
    FILE *inWeightsFile = fopen( file_entrada, "r");    
    char c[2000]; 
    char *pch;

	if( inWeightsFile != NULL) {
		for( int i = 0; i < entrada; i++) {
			fgets( c, 2000, inWeightsFile);
			pch = strtok (c, " ");
			for( int j = 0; j < oculto; j++) {
				matriz_entrada[i][j]= strtod( pch, NULL); 
				pch = strtok (NULL, " ");
			}
		}		
		/*cerrar el archivo*/
		fclose(inWeightsFile);
	} else {
		printf("Archivo no Encontrado");	
	}

	inWeightsFile = fopen( file_salida, "r");

	if( inWeightsFile != NULL) {
		for( int i = 0; i < oculto; i++) {
			fgets( c, 2000, inWeightsFile);
			pch = strtok (c, " ");
			for( int j = 0; j < salida; j++) {
				matriz_salida[i][j]= strtod( pch, NULL); 
				pch = strtok (NULL, " ");
			}
		}		
		/*cerrar el archivo*/
		fclose(inWeightsFile);
	} else {
		printf("Archivo no Encontrado");	
	}
}

double* RedNeuronalTest::calculoEstadosActivacion(double capa_entrada[]) {

	double *capa_oculta = new double[oculto];
	double *capa_salida = new double[salida];    

	for( int j = 0; j < oculto; j++) {
		capa_oculta[j] = 0;
		for( int i = 0; i < entrada; i++) {		  	
			capa_oculta[j] = capa_oculta[j] + capa_entrada[i]*matriz_entrada[i][j];
		}		
	}

	for( int j = 0; j < oculto; j++) {		  	
		capa_oculta[j] = (1/(1 + exp(-1*0.1*capa_oculta[j])));
	}	

	for( int k = 0; k < salida; k++) {	
		capa_salida[k] = 0;
		for( int j = 0; j < oculto; j++) {		  	
			capa_salida[k] = capa_salida[k] + capa_oculta[j]*matriz_salida[j][k];
		}		
	}

	delete capa_oculta;

	for ( int i = 0; i < 10; i++) {
		printf("%f\n", capa_salida[i]);
	}

	return capa_salida;
	/*fin del metodo*/	
}

/*destructor para liberar memoria*/
RedNeuronalTest::~RedNeuronalTest() {

	for(int i = 0; i < entrada; i++) {
		delete matriz_entrada[i];
	}

	for(int i = 0; i < oculto; i++) {
		delete matriz_salida[i];
	}

	delete[] matriz_entrada;
	delete[] matriz_salida;	
}