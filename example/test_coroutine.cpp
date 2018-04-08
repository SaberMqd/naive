/*
#include <iostream>
#include <chrono>
#include "../src/coroutine.h"

//SPSCͨ������������߻������ߣ�Э�̵�����Ϊ���ÿ���
coroutine::Channel<int> channel;

string async_func()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));
	return "21";
}

void routine_func1()
{
	//��ͨ���л�ȡ��Ϣ�����û����Ϣ��yield
	int i = channel.pop();
	std::cout << i << std::endl;

	i = channel.pop();
	std::cout << i << std::endl;
}

void routine_func2(int i)
{
	std::cout << "20" << std::endl;

	//������ǰЭ�̵�ִ�У����ػָ���
	coroutine::yield();

	std::cout << "21" << std::endl;

	//�첽ִ��������������޷�����ִ����ϣ���yield
	string str = coroutine::await(async_func);
	std::cout << str << std::endl;
}

void thread_func()
{
	//����Э�ƣ��ص�������ʽΪ��std::function<void()>
	coroutine::routine_t rt1 = coroutine::create(routine_func1);
	coroutine::routine_t rt2 = coroutine::create(std::bind(routine_func2, 2));

	std::cout << "00" << std::endl;

	//�ָ�rt1
	coroutine::resume(rt1);

	std::cout << "01" << std::endl;

	//�ָ�rt2
	coroutine::resume(rt2);

	std::cout << "02" << std::endl;
	//��ͨ��������Ϣ
	channel.push(10);

	std::cout << "03" << std::endl;
	coroutine::resume(rt2);

	std::cout << "04" << std::endl;
	channel.push(11);

	std::cout << "05" << std::endl;

	//����Э�̡�
	//���飺Э����ִ����Ϻ�ͳһ�ͷţ�����Э��ջ�ռ��еĶ����ܹ���ȫ�ı����ͷš�
	coroutine::destroy(rt1);
	coroutine::destroy(rt2);
}

void TestCoroutine()
{
	std::thread t1(thread_func);
	std::thread t2([]() {
		//��֧�ֿ��̵߳�Э�̵���
	});
	t1.join();
	t2.join();
}

*/