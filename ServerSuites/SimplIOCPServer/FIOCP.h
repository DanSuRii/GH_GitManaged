#pragma once
class FIOCP
{
public:
	FIOCP();
	virtual ~FIOCP();

private:
	void WorkerThread();

	HANDLE hIocp;

	std::vector<std::thread> _workers;
};

