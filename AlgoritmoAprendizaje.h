#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "RedNeuronal.h"

class AlgoritmoAprendizaje {  
      
   private: 
   
    //parametros de entrada
    RedNeuronal *red;//red neuronal que va a ser entrenada
    int numeroEjemplosEntrenamiento;//numero de ejempos de entrenamiento
    int numeroEjemplosPrueba;//numero de ejemplo de prueba
    int numComponentesEntrada;//numero de valores que tiene cada vector de entrada
    int numComponentesSalida;//numero de valores que tiene cada vector de salida
    int tamComponente;//tamaño en bytes de los componentes del vector de entrada
    char *inTrainningFile;
    char *outTrainningFile;
    char *inTestFile;
    char *outTestFile;
    
    //variables auxiliares de uso interno
    double **ejemplosEntrenamiento;
    double **ejemplosPrueba; 
	
   public:
	
    /*Constructor por defecto*/	
    AlgoritmoAprendizaje();    
    AlgoritmoAprendizaje(   RedNeuronal *red, 
                            int numeroEjemplosEntrenamiento, 
                            int numeroEjemplosPrueba, 
                            int numComponentesEntrada,
                            int numComponentesSalida,
                            int tamComponente,
                            char *inTrainningFile,
                            char *outTrainningFile,
                            char *inTestFile,
                            char *outTestFile
                           );    
    /*destructor para liberar memoria*/
    ~AlgoritmoAprendizaje();   	
	
    void obtenerEjemplosEntrenamiento();	      
    void realizarEntrenamiento();
    void obtenerEjemplosPrueba();
    float realizarPrueba();
    float ejecutarAprendizaje();    
 };
