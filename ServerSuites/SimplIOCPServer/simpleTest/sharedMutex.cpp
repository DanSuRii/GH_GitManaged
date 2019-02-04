#include <iostream>
#include <shared_mutex>
#include <vector>

#include <memory>
#include <thread>
#include <atomic>

#include <chrono>

#include <random>

const int MAX_BUF_LEN = 8192;

template< class T,class ...Args >
auto New( Args&& ...args )
{
    return std::make_shared<T>(args ...);
}

class Buffer
{
public:
    Buffer( int nIdx ) : _nIdx(nIdx)
    {}

    char* GetBufferPtr(){ return buf; }
private:
    int _nIdx;
    char buf[MAX_BUF_LEN]{0};
};
using PBUFFER = std::shared_ptr< Buffer >;

class BufferPool
{
public:
    BufferPool()
    : _cont(16)
    , generator(seed)
    {

    }

    PBUFFER RandAccess(  )
    {
			static char const * pStrs[]{
				"	8Q77AIFCqjdD1Ly5e6aq	",
				"	ccZD3iqWiKMaGGfFjzc3	",
				"	mMux9toqKtDSmlhLqx8k	",
				"	lKOXNm4VzYs3O48KFVoP	",
				"	fOLix3P6TaUjqanyIVtQ	",
				"	DZ4ho30NLaLaYpANvXBb	",
				"	btxHhkoYdfPVn2eONXsl	",
				"	5NppFIBdYsospPnVqTRr	",
				"	1QCQi7LhkphZMocOWjSH	",
				"	ia3YAB3KURYceLPMUj2v	",
				"	LSNWiyGzdFRi4i2JTz23	",
				"	d7BjAvqNHAk5qJDys3UZ	",
				"	LtblMFMfSIk0wdiR3VpR	",
				"	mndSbaqFQS3bWtBm2yjr	",
				"	tLdRsfBJ79Owb6XRzE6i	",
				"	lhe5oxDwkDmPKFFRyWv7	",
				"	22qbcUHDoaAQHpBxMj1f	",
				"	3VfzKi4h1dT1y4KDnbhY	",
				"	MtRe6SHencJZNu1azuKm	",
				"	2GBdYamRVMs6NYNf6zug	",
				"	FNIqs8Wn2AiHiTNCT4Fx	",
				"	uZ1BasFBjA4h45Er4Nv9	",
				"	L1WtnLbXWon0aIb402cK	",
				"	mrlFsCprKhKXZJiPNYlO	",
				"	5PRoq1seCUNdGxzGY9WM	",
				"	7eeoH3PaVblD2AXvTL58	",
				"	EMVZF9GnBxACzVflxgpI	",
				"	B8PEBFFeqv8kGRwEcJqL	",
				"	5QNsjOqttJdaFJYjTBeM	",
				"	2TtaxDPK9OCXPRYdN04E	",
			};


        int toAccess = generator() % _cont.size();
        if( nullptr == _cont[toAccess])
        {
            std::shared_lock<decltype(mtxCont)> shrd( mtxCont );
            _cont[toAccess] = New<Buffer>(toAccess);
        }

        {
            std::shared_lock<decltype(mtxCont)> shrd( mtxCont );
            std::strcpy( _cont[toAccess]->GetBufferPtr(), pStrs[ generator() % _countof(pStrs) ] );
        }

        return _cont[toAccess];
    }

    void GrowUp()
    {
        using namespace std::chrono_literals;

        std::unique_lock<decltype(mtxCont)> uqgrd( mtxCont );
        _cont.resize( _cont.size() * 2);
        std::this_thread::sleep_for( 2s ); //if there are longterm work
    }
    
private:
    std::vector< PBUFFER > _cont;
    std::shared_mutex mtxCont;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937_64 generator;

};

int main(void)
{
    using namespace std::chrono_literals;
    BufferPool bufPool;
    const int MAX_GROWUP_CNT = 50;
    
    std::atomic_int GrowUpcnt( 0 );

	std::thread thAccessor[6];
	for( auto& cur : thAccessor)
	{
		cur =
        std::thread([&] {
            while (GrowUpcnt < MAX_GROWUP_CNT)
            {
                std::cout << std::this_thread::get_id() << ">> Try to RandAccess" << std::endl;
                bufPool.RandAccess();
                std::cout << std::this_thread::get_id() << ">> RandAccessSucceess, leave" << std::endl;
				std::this_thread::sleep_for(1s);
			}
		});
    };
	std::thread thGrowupper[2];
	for(auto& cur : thGrowupper)
	{
		cur =
        std::thread([&] {
            while (GrowUpcnt++ < MAX_GROWUP_CNT)
            {
                std::cout << std::this_thread::get_id() << ">> Try to Growup :" << GrowUpcnt << "th" << std::endl;
                bufPool.GrowUp();
                std::cout << std::this_thread::get_id() << ">> Growup Success :" << GrowUpcnt << "th" << std::endl;
                std::this_thread::sleep_for(2s);
            }
		});
    };

    for( auto& cur: thAccessor )
        cur.join();

    for( auto& cur: thGrowupper)
        cur.join();

    system("pause");
    
    return 0;
}