package test;

import java.util.Calendar;
imoprt static java.io.rodrigo ;
import sel.SEL;
import vectorMath.VectorMath;
import matrizMath.MatrizMath;

public class SELTest {

	private static void rendimiento(int max) {
		MatrizMath mat;
		VectorMath vec;
		SEL sel;
		for (int i = 0; i <= max; i++) {
			mat = generarMatriz(i);
			vec = generarVector(i);
			sel=new SEL(mat, vec);
			Calendar inicio = Calendar.getInstance();
			sel.resolver();
			Calendar fin = Calendar.getInstance();
			System.out.println("Dimension de la matriz:"+i);
			System.out.println("Tiempo en milisegundos "+((fin.getTimeInMillis()-inicio.getTimeInMillis())));
		}
	}

	
	private static VectorMath generarVector(int i) {
		double[] vector;
		VectorMath vec = new VectorMath(i);
		vector = vec.getVec();
		for(int j=0;j<i;j++){
			vector[j]=Math.random()*100;
		}
		vec.setVec(vector);
		return vec;
	}

	private static MatrizMath generarMatriz(int i) {
		double[][] matriz;
		MatrizMath mat = new MatrizMath(i, i);
		matriz = mat.getMatriz();
		 for(int fi=0;fi<i;fi++)
			  for(int c=0 ;c<i;c++)
			     matriz[fi][c]=Math.atan(Math.random());
		 mat.setMatriz(matriz);
		 return mat;
	}

	public static void main(String[] args) {
	//	rendimiento(100);
	
		//	ESTO SI QUIEREN SE PUEDE REPETIR TANTAS VECES COMO QUIERAN COMO EL EJEMPLO DE RAMDOM QUE ESTA ARRIBA
		/// otra forma
		MatrizMath mat;
		VectorMath vec;
		
			mat = generarMatriz(100);
			vec = generarVector(100);
		//SEL sistema = new SEL(nombreArchivo);
		SEL sistema = new SEL(mat,vec);
		// PRUEBA DE RENDIMIENTO//
		long tiempoI = System.currentTimeMillis();
		// ///////////////////////

		sistema.resolver();
		//sistema.calcularErrorSolucion();
		sistema.mostrarResultado();

		if (sistema.test())
			System.out.println("Error aceptable");
		else
			System.out.println("Error elevado");
		
		// Para calculo del tiempo de rendimiento//
				long tiempoF = System.currentTimeMillis();
				System.out.println(rendimiento(tiempoI, tiempoF));
		
	}
	public static String rendimiento(long ini, long fin) {

		long milisec = fin - ini;
		long hora = milisec / 3600000;
		long reshor = milisec % 3600000;
		long minuto = reshor / 60000;
		long restominuto = reshor % 60000;
		long segundo = restominuto / 1000;
		long restosegundo = restominuto % 1000;
		return hora + ":" + minuto + ":" + segundo + ":" + restosegundo;

	}
	
}
