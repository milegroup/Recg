#include <stdlib.h>


/*
	The low-pass suppresses noise
*/
void lowPassFilter(double *vectorLPF, double *initialSamples, int *channelSize)
{
	int contLPF = 12;
	
	while( contLPF < *channelSize )
	{
		vectorLPF[ contLPF ]= 2 * vectorLPF[ contLPF - 1 ] - vectorLPF[ contLPF - 2 ] + initialSamples[ contLPF ] - 2 * initialSamples[ contLPF - 6 ] 
					+ initialSamples[ contLPF - 12 ];		
		contLPF++;
	}

}



/*
	Applies the high pass filter
	The high-pass filter attenuates the P and T waves and the baseline drift
*/
void highPassFilter(double *vectorHPF, double *vectorLPF, int *channelSize)
{
	
	int contHPF = 32;
	
	while( contHPF < *channelSize )
	{
		vectorHPF[ contHPF ]= 32 * vectorLPF[ contHPF - 16 ] - ( 1.0 / 32.0 ) * ( ( vectorHPF[ contHPF - 1 ] + vectorLPF[ contHPF ] - vectorLPF[ contHPF - 32 ] ) );
      		contHPF++;
	}

}

/*
	Calculates the derivate to the result of the filtering steps
*/
void derivative(double *vectorDerivate, double *vectorHPF, int *channelSize)
{	
	int contDerivate = 2;
	
	while( contDerivate < *channelSize )
	{
		vectorDerivate[ contDerivate ] = ( 0.125 ) * ( ( -1 ) * vectorHPF[ contDerivate - 2 ] - 2 * vectorHPF[ contDerivate - 1 ] + 2 * vectorHPF[ contDerivate + 1 ] + vectorHPF[ contDerivate + 2 ]  ) ;
		contDerivate++;
	}
}




/*
	Applies the window integration function to the squared function
*/
void mwi(double *vectorMWI, double *vectorSquare, int *channelSize, double *windowSize)
{
	double suma;
	double n = *windowSize;
	int cont = *windowSize; //tengo que convertir a entero *windowSize porque el vector necesita trabajar con int
	int i = 0;
	while( cont < ( *channelSize -2 ) ) //suma el area de la ventana de integración
	{
		suma = 0;
					
		for( i = ( cont - n + 1 ) ; i < cont; i++ ) 
		{
			suma += vectorSquare[ i ]; //suma las últimas posiciones (el nº de posiciones depende de windowSize) y la actual del vector square
		}

		//printf("Suma: %d ",suma);
		vectorMWI[ cont ] = suma; //va guardando la suma de los anteriores + el actual

		vectorMWI[ cont ] = ( 1.0 / n ) * vectorMWI[ cont ];
		cont++;
	}
}