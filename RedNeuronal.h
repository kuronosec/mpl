#define _REDNEURONAL_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>

class RedNeuronal{  
      
   private: 
   
    /*parametros de entrada de la red neuronal*/
    double alfa;  /*tasa o velocidad de aprendizaje*/  
    double umbral;  /*umbral de activacion de las neuronas*/
    double error;  /*error cometido por la red neuronal*/
    int numeroCapas; /*número de capas de la red neuronal*/
    int *neuronasCapa; /*puntero al arreglo que contiene el numero de neurona por capa*/ 
	char *file;
    
    /*variables auxiliares para los calculos*/
    double ***matrizPesos; /*pesos de las conexiones entre las diferentes capas*/
    double *in;  /*vector de entrada para la red neuronal*/
    double *out; /*vector de salida para la red neuronal*/			
    double **net;  /*vectores resultado*/
    double **O;  /*vectores de estados de activacion*/	
    double **delta;  /*vectores de error*/	
	/*finalizacion de la declaracion de las variables*/
	
   public:
	
    /*Constructor por defecto*/
	
    RedNeuronal();    
    RedNeuronal(double alfa, double umbral, double error, int numeroCapas, int neuronasCapa[]);
	RedNeuronal( int numeroCapas, int neuronasCapa[], char *file);
    /*destructor para liberar memoria*/
    ~RedNeuronal();	
	
        void setAlfa(double alfa);
	/*En este método se inicializan las matrices de pesos con númros aleatorios*/        
	void inicializarMatricesDePesos();	
	void calculoEstadosActivacion(double entrada[], double salida[]);
	double *calculoEstadosActivacion(double entrada[]);	
	void calculoErrorSalida();	
	void calculoRetroPropagacion();	
	void calculoNuevaMatriz();
	void saveWeights(char *file);
    double getError();
	double getErrorPrueba();
 };

class RedNeuronalTest {  
      
   private:  
   
	char *file;  
	/*pesos de las conexiones entre las diferentes capas*/
	int entrada;
	int oculto; 
	int salida;
    double **matriz_entrada;
	double **matriz_salida;
	
   public:
	
    /*Constructor por defecto*/
	
    RedNeuronalTest();
	RedNeuronalTest(int entrada, int oculto, int salida, char *file_entrada, char *file_salida);
    /*destructor para liberar memoria*/
    ~RedNeuronalTest();	
	
    double *calculoEstadosActivacion(double entrada[]);	
 };
