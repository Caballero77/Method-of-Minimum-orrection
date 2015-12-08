#include <iostream>
#include <iomanip>

const int n = 3;
const float e = 0.00000000000000000000000001;

void SetLU(float **A,float **L, float **U)
{
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			U[0][i] = A[0][i];
			L[i][0] = A[i][0] / U[0][0];
			float Buf = 0;
			for (int k = 0; k < i; k++) Buf += L[i][k] * U[k][j];
			U[i][j] = A[i][j] - Buf;
			if (i > j) L[j][i] = 0;
			else
			{
				Buf = 0;
				for (int k = 0; k < i; k++) Buf += L[j][k] * U[k][i];
				L[j][i] = (A[j][i] - Buf) / U[i][i];
			}
		}
}

void SetY(float *Y,float *B, float **L)
{
	for (int i = 0; i < n; i++)
	{
		float Buf = 0;
		for (int k = 0; k < i; k++) Buf += L[i][k] * Y[k];
		Y[i] = B[i] - Buf;
	}
}

void SetX(float *X,float *Y,float **U)
{
	for (int i = n - 1; i >= 0; i--)
	{
		float Buf = 0;
		for (int k = i + 1; k < n; k++) Buf += U[i][k] * X[k];
		X[i] = (Y[i] - Buf) / U[i][i];
	}
}

void LU(float **A, float *B, float *x)
{
	for (int i = 0; i < n; i++) x[i] = 0;
	float **U, **L,*Y;
	U = new float*[n];
	L = new float*[n];
	Y = new float[n];
	for (int i = 0; i < n; i++)
	{
		L[i] = new float[n];
		U[i] = new float[n];
	}
	SetLU(A, L, U);
	SetY(Y, B, L);
	SetX(x, Y, U);
}

bool ContinueIteration(float *x,float *tempX)
{
	float norm = 0;
	for (int i = 0; i < n; i++)
	{
		norm += (x[i] - tempX[i])*(x[i] - tempX[i]);
	}
	norm = sqrt(norm);
	if (norm > e) return true;
	return false;
}

int main()
{
	//Initialization
	float **A, *x, *b, **B, *r, *w, *tempX, *v;
	A = new float*[n];
	B = new float*[n];
	B[0] = new float[n]{10, 1, 1};
	B[1] = new float[n]{1, 10, 1};
	B[2] = new float[n]{1, 1, 10};
	x = new float[n];
	v = new float[n];
	r = new float[n];
	w = new float[n];
	b = new float[n];
	tempX = new float[n];
	for (int i = 0; i < n; i++)
	{
		A[i] = new float[n];
	}

	//Read A
	std::cout << "Input matrix A : " << std::endl;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cin >> A[i][j];
		}
	}

	//Read B
	std::cout << "Input vector B : " << std::endl;
	for (int i = 0; i < n; i++)
	{
		std::cin >> b[i];
		x[i] = b[i];
	}
	

	//Count of iteration
	unsigned __int64 iterCount = 0;

	//Set Precision
	std::cout << std::setprecision(e);



	do
	{
		//Copy in vector tempX vector x
		for (int i = 0; i < n; i++)
		{
			tempX[i] = x[i];
		}


		//Calculate vector r
		for (int i = 0; i < n; i++)
		{
			r[i] = 0;
			for (int j = 0; j < n; j++)
			{
				r[i] += A[i][j] * x[j];
			}
		}
		for (int i = 0; i < n; i++)
		{
			r[i] -= b[i];
		}

		//Show r
		/*std::cout << "Vector r : ";
		for (int i = 0; i < n; i++)
		{
			std::cout << r[i] << " ";
		}
		std::cout << std::endl;*/

		//Calculate using LU-method system B*w=r
		LU(B, r, w);

		//Show w
		/*std::cout << "Vector w : ";
		for (int i = 0; i < n; i++)
		{
			std::cout << w[i] << " ";
		}
		std::cout << std::endl;*/


		//Calculate A*w int temp
		float *temp;
		temp = new float[n];
		for (int i = 0; i < n; i++)
		{
			temp[i] = 0;
			for (int j = 0; j < n; j++)
			{
				temp[i] += A[i][j] * w[j];
			}
		}


		//Show temp
		/*std::cout << "Vector temp : ";
		for (int i = 0; i < n; i++)
		{
			std::cout << temp[i] << " ";
		}
		std::cout << std::endl;*/

		//Calculate B*v=A*w
		LU(B, temp, v);


		//Show v
		/*std::cout << "Vector v: ";
		for (int i = 0; i < n; i++)
		{
			std::cout << v << " ";
		}
		std::cout << std::endl;*/

		//Calculate R
		float buf1, buf2;
		buf1 = 0;
		buf2 = 0;
		for (int i = 0; i < n; i++)
		{
			buf1 += temp[i] * w[i];
			buf2 += v[i] * temp[i];
		}
		if (buf2 == 0) break;
		float R = buf1 / buf2;

		//Show R
		///std::cout << "R : " << R << std::endl;


		//Calculate X
		for (int i = 0; i < n; i++)
		{
			x[i] = tempX[i] - R*w[i];
		}

		//You now
		iterCount++;

		//Show X
		std::cout << "Iteration " << iterCount <<  " X : ";
		for (int i = 0; i < n; i++)
		{
			std::cout << x[i] << " ";
		}
		std::cout << std::endl;


		//Delete pointers
		delete[]temp;


	} while (ContinueIteration(x,tempX));

	//Write result
	std::cout << std::endl << "Result : ";
	for (int i = 0; i < n; i++)
	{
		std::cout << x[i] << " ";
	}
	std::cout << std::endl << "Count of Iteration : " << iterCount << std::endl;



	system("pause");
	return 0;
}


/*
8 1 1  13
1 5 -1  8
1 -1 5  14

res 1 2 3
*/
/*
1 0 0  a
0 1 0  b
0 0 1  c

res a b c
*/
