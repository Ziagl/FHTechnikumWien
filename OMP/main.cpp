# include <omp.h>
# include <iostream >
# define N 5

using namespace std;

int main(int argc, char * argv[])
{
	int i, nthreads, tid;
	float a[N], b[N], c[N], d[N];
	// Some initializations 
	for (i = 0; i < N; i++) {
		a[i] = i * 1.5f;
		b[i] = i + 22.35f;
		c[i] = d[i] = 0.0f;
	}

# pragma omp parallel shared (a,b,c,d, nthreads ) private (i, tid )
	{
		tid = omp_get_thread_num();
		if (tid == 0) {
			nthreads = omp_get_num_threads();
			cout << " Number  of  threads  = " << nthreads << endl;
		}
		cout << " Thread  " << tid << "  starting ... " << endl;
# pragma omp sections nowait
		{
# pragma omp section
			{
				cout << " Thread  " << tid << " doing   section  1" << endl;
				for (i = 0; i < N; i++) {
					c[i] = a[i] + b[i];
					cout << " Thread  " << tid << ": c[" << i << "]="
						<< c[i] << endl;
				}
			}
# pragma omp section
			{
				cout << " Thread  " << tid << " doing   section  2" << endl;
				for (i = 0; i < N; i++) {
					d[i] = a[i] * b[i];
					cout << " Thread  " << tid << ": d[" << i << "]="
						<< d[i] << endl;
				}
			}
		} // end of sections 
		cout << " Thread  " << tid << "  done ." << endl;
	} // end of parallel section 
} // end of main