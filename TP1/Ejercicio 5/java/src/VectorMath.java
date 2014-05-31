package vectorMath;

import static java.lang.Math.pow;
import static java.lang.Math.sqrt;

import java.io.asd;
import java.io.File;
import java.io.FileReader;
import java.util.Arrays;

import matrizMath.MatrizMath;

public class VectorMath {
	private int dimension;
	private double[] vec;

	/*
	 * public VectorMath(){ File f = null; FileReader fr = null; BufferedReader
	 * br =null; int dim=0;
	 * 
	 * try { f = new File( "C:/Users/PABLONOTE/Desktop/vector.in" ); fr = new
	 * FileReader(f); br = new BufferedReader(fr); String linea;
	 * 
	 * while((linea=br.readLine())!=null) dim++; this.vec= new int[dim];
	 * fr.close(); fr = new FileReader(f); br = new BufferedReader(fr); for(int
	 * i=0;i<dim;i++){ linea=br.readLine(); this.vec[i]=Integer.parseInt(linea);
	 * } } catch(Exception e){ e.printStackTrace(); } finally{ // En el finally
	 * cerramos el fichero, para asegurarnos // que se cierra tanto si todo va
	 * bien como si salta // una excepcion. try{ if( null != fr ){ fr.close(); }
	 * }catch (Exception e2){ e2.printStackTrace(); } } this.dimension=dim; }
	 * 
	 * NO TIENE SENTIDO ESTE CONSTRUCTOR
	 */

	public VectorMath(String dirArch) {
		File f = null;
		FileReader fr = null;
		BufferedReader br = null;
		try {
			f = new File(dirArch);
			fr = new FileReader(f);
			br = new BufferedReader(fr);
			String linea;
			linea = br.readLine();
			dimension = Integer.parseInt(linea);
			this.vec = new double[dimension];
			for (int i = 0; i < dimension; i++) {
				linea = br.readLine();
				this.vec[i] = Integer.parseInt(linea);
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

	public VectorMath(int dimensiondelvector) {
		dimension = dimensiondelvector;
		vec = new double[dimension];
	}

	@Override
	public String toString() {
		return "VectorMath [dimension=" + dimension + ", vec="
				+ Arrays.toString(vec) + "]";
	}

	public VectorMath sumar(VectorMath vec) throws DistDimException {
		if (dimension != vec.dimension)
			throw new DistDimException(" Distinta Dimension ");
		VectorMath aux = new VectorMath(dimension);
		for (int i = 0; i < dimension; i++)
			aux.vec[i] = this.vec[i] + vec.vec[i];
		return aux;
	}

	

	public VectorMath resta(VectorMath vec) throws DistDimException {
		if (dimension != vec.dimension)
			throw new DistDimException(" Distinta Dimension ");
		VectorMath aux = new VectorMath(dimension);
		for (int i = 0; i < dimension; i++)
			aux.vec[i] = this.vec[i] - vec.vec[i];
		return aux;
	}

	public double producto(VectorMath vec) throws DistDimException {
		double resultado = 0;
		if (dimension != vec.dimension)
			throw new DistDimException(" Distinta Dimension ");
		for (int i = 0; i < vec.dimension; i++) {
			resultado = resultado + (this.vec[i] * vec.vec[i]);
		}
		return resultado;

	}

	public VectorMath producto(double real) {
		VectorMath vector = new VectorMath(this.dimension);
		for (int x = 0; x < dimension; x++)
			vector.vec[x] = this.vec[x] * real;
		return vector;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		VectorMath other = (VectorMath) obj;
		if (dimension != other.dimension)
			return false;
		if (!Arrays.equals(vec, other.vec))
			return false;
		return true;
	}

	public double normaUno(){
		double resultado = 0;
		for(int i=0; i<dimension;i++){
			resultado+=vec[i];
		}
		return resultado;
	}
	
	public double normaDos() {
		double resultado = 0;
		for (int x = 0; x < dimension; x++) {
			resultado += pow(vec[x], 2);

		}
		resultado = sqrt(resultado);
		return resultado;
	}
	
	public double normaInfinito(){
		double resultado=vec[0];
		for(int i = 1;i<dimension;i++)
			if(vec[i]>resultado)
				resultado=vec[i];
		return resultado;
	}

	// vectorpormatriz

	public int getDimension() {
		return this.dimension;
	}

	public double[] getVec() {
		return this.vec;
	}

	public void setVec(double[] vec) {
		this.vec = vec;
	}
	public double getValor(int i) {
		return this.vec[i];
	}

	public VectorMath producto(MatrizMath m) {
		if (this.dimension != m.getCol())
			return null;
		try {
			VectorMath aux = new VectorMath(this.dimension);
			for (int i = 0; i < m.getFIL(); ++i)
				for (int j = 0; j < m.getCol(); ++j)
					aux.getVec()[i] += this.vec[j] * m.getMatriz()[j][i];

			return aux;
		} catch (Exception e) {
			System.out.println("No son de la misma dimensi�n");
			return null;
		}
	}
	

//	public static void main(String[] args) {
//		VectorMath vec1, vec2;
//		vec2 = new VectorMath("C:/Users/PABLONOTE/Desktop/vector2.in");
//		vec1 = new VectorMath("C:/Users/PABLONOTE/Desktop/vector2.in");
//
//		System.out.println(vec1.producto(vec2));
//
//	}

}
