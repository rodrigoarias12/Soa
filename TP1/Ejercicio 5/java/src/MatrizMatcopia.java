package matrizMath;

import static java.lang.Math.pow;
import static java.lang.Math.sqrt;
import java.io.rodrigo;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.Arrays;

import vectorMath.VectorMath;

public class MatrizMath {

	private int fil;
	private int col;
	private double matriz[][];

	public MatrizMath(int i, int j) {
		this.matriz = new double[i][j];
		this.fil = i;
		this.col = j;
	}

	public MatrizMath(int dim) {
		this.matriz = new double[dim][dim];
		this.fil = dim;
		this.col = dim;
		for (int fi = 0; fi < fil; fi++)
			for (int c = 0; c < col; c++)
				this.matriz[fi][c] = 0;

	}

	public MatrizMath(String path) {
		File f = null;
		FileReader fr = null;
		BufferedReader br = null;
		int filas;
		int columnas;
		int i;
		int j;
		double valor;
		try {
			f = new File(path);
			fr = new FileReader(f);
			br = new BufferedReader(fr);
			String linea;

			linea = br.readLine();
			String[] dimensiones = linea.split(" ");
			filas = Integer.parseInt(dimensiones[0]);
			columnas = Integer.parseInt(dimensiones[1]);
			this.fil = filas;
			this.col = columnas;
			matriz = new double[filas][columnas];
			while ((linea = br.readLine()) != null) {
				String[] dato = linea.split(" ");
				i = Integer.parseInt(dato[0]);
				j = Integer.parseInt(dato[1]);
				valor = Double.parseDouble(dato[2]);
				matriz[i][j] = valor;
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

	public MatrizMath producto(MatrizMath m) throws DistDimException {
		if (!(this.col == m.fil))
			throw new DistDimException("Dimensiones incompatibles");
		MatrizMath aux = new MatrizMath(this.fil, m.col);
		for (int k = 0; k < this.fil; k++) {
			for (int j = 0; j < m.col; j++) {
				for (int i = 0; i < this.col; i++)
					aux.matriz[k][j] += this.matriz[k][i] * m.matriz[i][j];
			}
		}
		return aux;
	}

	public MatrizMath producto(double num) {
		MatrizMath aux = new MatrizMath(this.fil, this.col);
		for (int i = 0; i < this.fil; i++)
			for (int j = 0; j < this.col; j++) {
				aux.matriz[i][j] = num * this.matriz[i][j];
			}

		return aux;
	}

	public VectorMath producto(VectorMath vec) throws DistDimException {
		if (!(this.col == vec.getDimension()))
			throw new DistDimException("Dimensiones incompatibles");

		VectorMath aux = new VectorMath(this.fil);
		for (int j = 0; j < this.fil; j++)
			for (int i = 0; i < vec.getDimension(); i++)
				aux.getVec()[j] += this.matriz[j][i] * vec.getVec()[i];
		return aux;
	}

	public double determinante() {

		for (int k = 0; k < this.fil - 1; k++) {
			for (int i = k + 1; i < this.fil; i++) {
				for (int j = k + 1; j < this.fil; j++) {
					this.matriz[i][j] -= this.matriz[i][k] * this.matriz[k][j]
							/ this.matriz[k][k];
				}
			}
		}
		double deter = 1.0;
		for (int i = 0; i < this.fil; i++) {
			deter *= this.matriz[i][i];
		}
		return deter;
	}

	// calcular el error de la matriz invertida (metodo gaus jordan)
	// i - i' a la norma 2 < epcilon por 10a la -12 (este es el error que tiene
	// q dar )
	// llamar a una funcion error

	@Override
	public String toString() {
		String linea = "";
		for (int f = 0; f < fil; f++)
			linea += Arrays.toString(matriz[f]);

		return "MatrizMath [fila=" + fil + ", col=" + col + ", mat=" + linea
				+ "]";

	}

	public MatrizMath suma(MatrizMath m) throws DistDimException {
		if (!((this.fil == m.fil) && (this.col == m.col)))
			throw new DistDimException("Distintas Dimensiones");
		MatrizMath aux = new MatrizMath(this.fil, this.col);
		for (int i = 0; i < this.fil; i++) {
			for (int j = 0; j < this.col; j++) {
				aux.matriz[i][j] = this.matriz[i][j] + m.matriz[i][j];
			}
		}
		return aux;
	}

	public MatrizMath resta(MatrizMath m) throws DistDimException {
		if (!((this.fil == m.fil) && (this.col == m.col)))
			throw new DistDimException("Distintas Dimensiones");
		MatrizMath aux = new MatrizMath(this.fil, this.col);
		for (int i = 0; i < this.fil; i++) {
			for (int j = 0; j < this.col; j++) {
				aux.matriz[i][j] = this.matriz[i][j] - m.matriz[i][j];
			}
		}
		return aux;
	}

	public MatrizMath anadeFila(int i1, int i2, double k) {
		MatrizMath resultado = new MatrizMath(this.fil, this.col);
		for (int i = 0; i < this.fil; i++) {
			for (int j = 0; j < this.col; j++) {
				if (i == i1) {
					// A los elementos de la fila i1 le sumamos k veces el
					// elemento correspondiente de la fila i2
					resultado.matriz[i1][j] = this.matriz[i][j] + k
							* this.matriz[i2][j];
				} else {
					// Los demas elementos los dejamos tal cual
					resultado.matriz[i][j] = this.matriz[i][j];
				}
			}
		}
		return resultado;
	}

	public MatrizMath gaussElim() {
		// Empezamos con la matriz actual como resultado;
		// no hay problema porque las operaciones de fila no modifican
		// la original sino que devuelven matrices nuevas
		MatrizMath resultado = this;
		for (int j = 0; j < this.col; j++) {
			for (int i = j + 1; i < this.fil; i++) {
				resultado = resultado.anadeFila(i, j, -resultado.matriz[i][j]
						/ resultado.matriz[j][j]);
			}
		}
		return resultado;
	}

	public MatrizMath multiplicaFila(int i1, double k) {
		MatrizMath resultado = new MatrizMath(this.fil, this.col);
		for (int i = 0; i < this.fil; i++) {
			for (int j = 0; j < this.col; j++) {
				if (i == i1) {
					// Los elementos de la fila i1 la multiplicamos por k
					resultado.matriz[i1][j] = k * this.matriz[i][j];
				} else {
					// Los demas elementos los dejamos tal cual
					resultado.matriz[i][j] = this.matriz[i][j];
				}
			}
		}
		return resultado;
	}

	public MatrizMath gaussJordanElim() {
		// Empezamos con la matriz triangular superior como resultado
		MatrizMath resultado = this.gaussElim();
		for (int i = 0; i < resultado.fil; i++) {
			// Hacemos los pivotes igual a la unidad
			resultado = resultado.multiplicaFila(i,
					1.0 / resultado.matriz[i][i]);
		}
		for (int j = 0; j < this.fil; j++) {
			for (int i = 0; i < j; i++) {
				// Anulamos los elementos por encima de los pivotes (i<j)
				resultado = resultado.anadeFila(i, j, -resultado.matriz[i][j]
						/ resultado.matriz[j][j]);
			}
		}
		return resultado;
	}

	public MatrizMath inversa() {
		// Primero creamos una nueva matriz que copie 5la actual y la matriz
		// identidad
		MatrizMath tmp = new MatrizMath(this.fil, 2 * this.col);
		for (int i = 0; i < this.fil; i++) {
			// Primera mitad, matriz original
			for (int j = 0; j < this.col; j++) {
				tmp.matriz[i][j] = this.matriz[i][j];
			}
			// Segunda mitad, matriz identidad
			for (int j = this.col; j < 2 * this.col; j++) {
				if (i + this.col == j) {
					// Valen 1 los elementos de la diagonal de la parte derecha
					tmp.matriz[i][j] = 1;
				} else {
					// Valen 0 el resto de los elementos
					tmp.matriz[i][j] = 0;
				}
			}
		}
		// Segundo: aplicamos el metodo de eliminacion de Gauss-Jordan
		tmp = tmp.gaussJordanElim();
		// Tercero: creamos una nueva matriz que equivale a la parte derecha de
		// la matriz actual
		MatrizMath resultado = new MatrizMath(this.fil, this.col);
		for (int i = 0; i < this.fil; i++) {
			for (int j = 0; j < this.col; j++) {
				resultado.matriz[i][j] = tmp.matriz[i][j + this.col];
			}
		}
		return resultado;
	}

	// falta esto del error creo que deberia funcionar . Pero no tuve forma de
	// probarlo
	// por que faltaria esta public VectorMath producto(VectorMath vec)

	public double error(MatrizMath inv) {
		MatrizMath ide = new MatrizMath(this.fil, this.col);
		double sum = 0;
		for (int i = 0; i < this.fil; i++)
			ide.matriz[i][i] = 1;
		MatrizMath merr = ide.resta(this.producto(inv));
		for (int i = 0; i < merr.fil; i++)
			for (int j = 0; j < merr.col; j++)
				sum += merr.matriz[i][j] * merr.matriz[i][j];
		return sqrt(sum);
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		MatrizMath other = (MatrizMath) obj;
		if (col != other.col)
			return false;
		if (fil != other.fil)
			return false;
		if (!Arrays.deepEquals(matriz, other.matriz))
			return false;
		return true;
	}

	public double normaUno() {
		double resultado = 0;
		for (int i = 0; i < fil; i++) {
			for (int j = 0; j < col; j++)
				resultado += matriz[i][j];
		}
		return resultado;
	}

	public double normaDos() {
		double resultado = 0;
		for (int x = 0; x < fil; x++) {
			for(int y = 0; y<col;y++){
			resultado += pow(matriz[x][y], 2);
			}
		}
		resultado = sqrt(resultado);
		return resultado;
	}

	public double normaInfinito() {
		double resultado = matriz[0][0];
		for (int i = 0; i < fil; i++)
			for(int j = 0;j<col;j++)
			if (matriz[i][j] > resultado)
				resultado = matriz[i][j];
		return resultado;
	}

	public double[][] getMatriz() {
		return matriz;
	}

	public void setMatriz(double[][] matriz) {
		this.matriz = matriz;
	}
   public int getFIL()
   {
	   return fil;
	   
   }
   public int getCol()
   {
	   return col;
	   
   }
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		MatrizMath mac1 = new MatrizMath("matriz.in");
		System.out.println(mac1.inversa());
		// MatrizMath mac2 = new MatrizMath(
		// "C:/Users/PABLONOTE/Desktop/matriz1.in");
		// MatrizMath mac3 = new MatrizMath();
		// mac3 = mac1.inversa();
		// System.out.println(mac3.toString());
		//
		// System.out.println(mac1.error(mac3));
	}

}
