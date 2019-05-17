#include<thread>
#include<algorithm>
using namespace std;
void funkcja()
{
	const int x = 1000, y = 1000, z = 1000;

	//rezerwacja pamięci na macierze

	int p[x][y];
	int r[z][y];
	int s[x][z];
	
	//wypełnienie czynnikow losowymi liczbami
	for (int i = 0; i < x; i++)
		for (int j = 0; j < y; j++)
			p[i][j] = rand() % 31;

	for (int i = 0; i < z; i++)
		for (int j = 0; j < y; j++)
			r[i][j] = rand() % 31;

	//obliczenie iloczynu
	std::thread thread1,thread2;

	thread1=std::thread([&p,&r,&s,x,y,z](){
	
	auto start = std::chrono::system_clock::now();

	int t = 0;
	for (int i = 0; i < x; i++)
		for (int j = 0; j < z; j++)
		{
			for (int k = y/2; k < y; k++)
				t += p[i][k] * r[j][k];
			s[i][j] = t;
			t = 0;
		}
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<double> dur = stop - start;
	
	std::cout<<dur.count()<<std::endl;
	});
	thread2=std::thread([&p,&r,&s,x,y,z]{
	
	auto start = std::chrono::system_clock::now();
	int t = 0;
	for (int i = 0; i < x; i++)
		for (int j = 0; j < z; j++)
		{
			for (int k = 0; k < y/2; k++)
			{
				t += p[i][k] * r[j][k];
			}
			s[i][j] = t;
			t = 0;
		}
	auto stop = std::chrono::system_clock::now();
	std::chrono::duration<double> dur = stop - start;
	
	std::cout<<dur.count()<<std::endl;
	
	});


    cpu_set_t cpuset; //zmienna (maska) przechowująca aktywowane rdzenie
    CPU_ZERO(&cpuset); //wyzerowanie maski
    CPU_SET(0, &cpuset); //tutaj sie dodaje rdzeń, u mnie od 0 do 3, dodaję   //rdzen nr 0
    CPU_SET(2, &cpuset);
    int er = pthread_setaffinity_np(thread1.native_handle(),
                                    1, &cpuset); //aktywuje wątek
    int er2 = pthread_setaffinity_np(thread2.native_handle(),
                                    1, &cpuset);
    if (er != 0) {
      std::cerr << "Error calling pthread_setaffinity_np: " << er << "\n"; //błąd jesli nie aktywowało
    }
    if (er2 != 0) {
      std::cerr << "Error calling pthread_setaffinity_np: " << er2 << "\n";
    }

	thread1.join();
	thread2.join();

}
int main()
{
    srand(time(NULL));

    const rlim_t kStackSize = 110110110000;
    struct rlimit rl;
    int result;

    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0)
    {
        if (rl.rlim_cur < kStackSize)
        {
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);

                cout<<result<<endl;

        }
    }

	funkcja();
}