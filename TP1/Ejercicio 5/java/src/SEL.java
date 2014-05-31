package sel;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.PrintWriter;
import matrizMath.MatrizMath;
import vectorMath.VectorMath;

public class SEL {

	private int dimension;
	private MatrizMath mat;
	private VectorMath vec;
	private VectorMath resultado;
	private double error =-1;
   //constructores
	public SEL(MatrizMath mat, VectorMath vec) {
		this.mat = mat;
		this.vec = vec;
	}
	public SEL() {
		this.mat = new MatrizMath(0, 0);
		this.vec = new VectorMath(0);
	}

	public SEL(int n) {
		this.dimension = n;
		this.vec = new VectorMath(n);
		this.resultado = new VectorMath(n);
		this.mat = new MatrizMath(n, n);
	}

	public SEL(int fil, int col) {
		this.mat = new MatrizMath(fil, col);
		this.vec = new VectorMath(fil);
	}
	
	public SEL(String path) {
		File f = null;
		FileReader fr = null;
		BufferedReader br = null;
		int i;
		int j;
		double valor;
		try {
			f = new File(path);
			fr = new FileReader(f);
			br = new BufferedReader(fr);
			String linea;
			linea = br.readLine();

			this.dimension = Integer.parseInt(linea);
			this.mat = new MatrizMath(this.dimension);

			for (int k = 0; k < this.dimension * this.dimension; k++) {
				linea = br.readLine();
				String[] dato = linea.split(" ");
				i = Integer.parseInt(dato[0]);
				j = Integer.parseInt(dato[1]);
				valor = Double.parseDouble(dato[2]);
				this.mat.getMatriz()[i][j] = valor;
			}
			this.vec = new VectorMath(this.dimension);

			for (int k = 0; k < this.dimension; k++) {
				linea = br.readLine();
				this.vec.getVec()[k] = Double.parseDouble(linea);
			}

		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			// En el finally cerramos el fichero, para asegurarnos
			// que se cierra tanto si todo va bien como si salta
			// una excepcion.
			try {
				if (null != fr) {
					fr.close();
				}
			} catch (Exception e2) {
				e2.printStackTrace();
			}
		}

	}

	//metodos soluciones
	public void resolver() {
		this.resultado = this.mat.inversa().producto(this.vec);
		calcularErrorSolucion();
	}

	public void mostrarResultado() {
		for (int k = 0; k < this.dimension; k++) {
			System.out.println(this.resultado.getVec()[k]);
		}

	}
	// Este metodo calcula el errro que tenemos con el error de verdad .
	public boolean test() {
		return this.error <= 0.0001;
	}
   // otra forma 
/*	public void calcularErrorSolucion() {
		VectorMath b = this.matA.producto(this.vecSolucion);
		this.error = (this.vecIndep.restar(b)).normaDos();
	}*/
	private void calcularErrorSolucion() throws DistDimException {
		error = mat.producto(resultado).resta(vec).normaDos();
	}


	@Override
	public String toString() {
		return "SEL [dimension=" + dimension + ", mat=" + mat + ", vec=" + vec
				+ (resultado == null ? "" : ", resultado=" + resultado) + "]";
	}

	public static void main(String[] args) {
		SEL sel1 = new SEL("sel.in");
		sel1.resolver();
		sel1.calcularErrorSolucion();
		System.out.println(sel1.toString());
		sel1.escribirArchivo("resultado.out");
	}

	public double getError() {
		return error;
	}

	public void setError(double error) {
		this.error = error;
	}

	public void escribirArchivo(String path) {

		FileWriter fichero = null;
		PrintWriter pw = null;
		try {
			fichero = new FileWriter("resultado.out");
			pw = new PrintWriter(fichero);

		for (int i = 0; i < this.dimension; i++)
			pw.println(this.resultado.getVec()[i]);		
			pw.println("");
		pw.println(error);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				// Nuevamente aprovechamos el finally para
				// asegurarnos que se cierra el fichero.
				if (null != fichero)
					fichero.close();
			} catch (Exception e2) {
				e2.printStackTrace();
			}
		}

	}
}

